/************************************************************************
* LEGALESE:   "Copyright (c) 2017, Dell Inc. All rights reserved."
*
* This source code is confidential, proprietary, and contains trade
* secrets that are the sole property of Dell Inc.
* Copy and/or distribution of this source code or disassembly or reverse
* engineering of the resultant object code are strictly forbidden without
* the written consent of Dell Inc.
*
************************************************************************/
/**
* @file  sai_qos_buffer_tile_unit_test.cpp
*
* @brief This file contains tests for qos buffer having tile
*        based concept in NPU.
*************************************************************************/

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "gtest/gtest.h"

extern "C" {
#include "sai_qos_unit_test_utils.h"
#include "sai.h"
#include "saibuffer.h"
#include "saiport.h"
#include "saiqueue.h"
#include <inttypes.h>
}

static sai_object_id_t switch_id =0;

#define LOG_PRINT(msg, ...) \
    printf(msg, ##__VA_ARGS__)
#define PG_ALL 255
#define QUEUE_ALL 255

#define MAX_SHADOW_LIST 4
unsigned int sai_total_buffer_size = 0;

/*
 * API query is done while running the first test case and
 * the method table is stored in sai_switch_api_table so
 * that its available for the rest of the test cases which
 * use the method table
 */
class qos_buffer : public ::testing::Test
{
    protected:
        static void SetUpTestCase()
        {
            sai_attribute_t get_attr;
            ASSERT_EQ(SAI_STATUS_SUCCESS,sai_api_query(SAI_API_SWITCH,
                        (static_cast<void**>(static_cast<void*>(&sai_switch_api_table)))));

            ASSERT_TRUE(sai_switch_api_table != NULL);

            EXPECT_TRUE(sai_switch_api_table->remove_switch != NULL);
            EXPECT_TRUE(sai_switch_api_table->set_switch_attribute != NULL);
            EXPECT_TRUE(sai_switch_api_table->get_switch_attribute != NULL);


            sai_attribute_t sai_attr_set[7];
            uint32_t attr_count = 7;

            memset(sai_attr_set,0, sizeof(sai_attr_set));

            sai_attr_set[0].id = SAI_SWITCH_ATTR_INIT_SWITCH;
            sai_attr_set[0].value.booldata = 1;

            sai_attr_set[1].id = SAI_SWITCH_ATTR_SWITCH_PROFILE_ID;
            sai_attr_set[1].value.u32 = 0;

            sai_attr_set[2].id = SAI_SWITCH_ATTR_FDB_EVENT_NOTIFY;
            sai_attr_set[2].value.ptr = (void *)sai_fdb_evt_callback;

            sai_attr_set[3].id = SAI_SWITCH_ATTR_PORT_STATE_CHANGE_NOTIFY;
            sai_attr_set[3].value.ptr = (void *)sai_port_state_evt_callback;

            sai_attr_set[4].id = SAI_SWITCH_ATTR_PACKET_EVENT_NOTIFY;
            sai_attr_set[4].value.ptr = (void *)sai_packet_event_callback;

            sai_attr_set[5].id = SAI_SWITCH_ATTR_SWITCH_STATE_CHANGE_NOTIFY;
            sai_attr_set[5].value.ptr = (void *)sai_switch_operstate_callback;

            sai_attr_set[6].id = SAI_SWITCH_ATTR_SHUTDOWN_REQUEST_NOTIFY;
            sai_attr_set[6].value.ptr = (void *)sai_switch_shutdown_callback;

            ASSERT_TRUE(sai_switch_api_table->create_switch != NULL);

            EXPECT_EQ (SAI_STATUS_SUCCESS,
                    (sai_switch_api_table->create_switch (&switch_id , attr_count,
                                                          sai_attr_set)));


            ASSERT_EQ(SAI_STATUS_SUCCESS,sai_api_query(SAI_API_BUFFER,
                        (static_cast<void**>(static_cast<void*>(&sai_buffer_api_table)))));

            ASSERT_TRUE(sai_buffer_api_table != NULL);

            EXPECT_TRUE(sai_buffer_api_table->create_buffer_pool != NULL);
            EXPECT_TRUE(sai_buffer_api_table->remove_buffer_pool != NULL);
            EXPECT_TRUE(sai_buffer_api_table->set_buffer_pool_attribute != NULL);
            EXPECT_TRUE(sai_buffer_api_table->get_buffer_pool_attribute != NULL);
            EXPECT_TRUE(sai_buffer_api_table->set_ingress_priority_group_attribute != NULL);
            EXPECT_TRUE(sai_buffer_api_table->get_ingress_priority_group_attribute != NULL);
            EXPECT_TRUE(sai_buffer_api_table->create_buffer_profile != NULL);
            EXPECT_TRUE(sai_buffer_api_table->remove_buffer_profile != NULL);
            EXPECT_TRUE(sai_buffer_api_table->set_buffer_profile_attribute != NULL);
            EXPECT_TRUE(sai_buffer_api_table->get_buffer_profile_attribute != NULL);

            ASSERT_EQ(NULL,sai_api_query(SAI_API_PORT,
                        (static_cast<void**>(static_cast<void*>(&sai_port_api_table)))));

            ASSERT_TRUE(sai_port_api_table != NULL);

            EXPECT_TRUE(sai_port_api_table->set_port_attribute != NULL);
            EXPECT_TRUE(sai_port_api_table->get_port_attribute != NULL);
            EXPECT_TRUE(sai_port_api_table->get_port_stats != NULL);

            ASSERT_EQ(NULL,sai_api_query(SAI_API_QUEUE,
                        (static_cast<void**>(static_cast<void*>(&sai_queue_api_table)))));

            ASSERT_TRUE(sai_queue_api_table != NULL);
            p_sai_qos_queue_api_table = sai_queue_api_table;

            EXPECT_TRUE(sai_queue_api_table->set_queue_attribute != NULL);
            EXPECT_TRUE(sai_queue_api_table->get_queue_attribute != NULL);
            EXPECT_TRUE(sai_queue_api_table->get_queue_stats != NULL);
            EXPECT_TRUE(sai_queue_api_table->create_queue != NULL);
            EXPECT_TRUE(sai_queue_api_table->remove_queue != NULL);
            sai_attribute_t sai_port_attr;
            uint32_t * port_count = sai_qos_update_port_count();
            sai_status_t ret = SAI_STATUS_SUCCESS;

            memset (&sai_port_attr, 0, sizeof (sai_port_attr));

            sai_port_attr.id = SAI_SWITCH_ATTR_PORT_LIST;
            sai_port_attr.value.objlist.count = 256;
            sai_port_attr.value.objlist.list  = sai_qos_update_port_list();
            ret = sai_switch_api_table->get_switch_attribute(0,1,&sai_port_attr);
            *port_count = sai_port_attr.value.objlist.count;

            ASSERT_EQ(SAI_STATUS_SUCCESS,ret);

            memset(&get_attr, 0, sizeof(get_attr));
            get_attr.id = SAI_SWITCH_ATTR_TOTAL_BUFFER_SIZE;
            ASSERT_EQ(SAI_STATUS_SUCCESS,
                    sai_switch_api_table->get_switch_attribute(switch_id,1, &get_attr));
            sai_total_buffer_size = (get_attr.value.u32)*1024;

            printf("Total buffer size - %d bytes\r\n", sai_total_buffer_size);
            for (uint32_t i = 0; i < sai_qos_max_ports_get(); i++) {
                printf("port idx %d port id 0x%lx \r\n", i, sai_qos_port_id_get(i));
            }

        }

        void sai_qos_xpe_buffer_verify (sai_object_id_t pool_id,
                                        sai_object_id_t *shadow_pool_list,
                                        unsigned int pool_size,
                                        unsigned int pool_shared_size,
                                        unsigned int xpe_0_reserved_size,
                                        unsigned int xpe_1_reserved_size,
                                        unsigned int xpe_2_reserved_size,
                                        unsigned int xpe_3_reserved_size);

        void sai_qos_get_pg_shadow_list (sai_object_id_t pg_id, sai_object_id_t *shadow_list);
        void sai_qos_get_buffer_pool_shadow_list (sai_object_id_t pool_id, sai_object_id_t *shadow_list);
        void sai_qos_get_q_shadow_list (sai_object_id_t q_id, sai_object_id_t *shadow_list);

        static sai_switch_api_t* sai_switch_api_table;
        static sai_buffer_api_t* sai_buffer_api_table;
        static sai_port_api_t* sai_port_api_table;
        static sai_queue_api_t* sai_queue_api_table;

        /* MX TH+ tile0- 0,1  tile1- 8 tile2-22  tile3-30 */
        static const unsigned int test_tile0_port_index = 0;
        static const unsigned int test_tile0_port_index_1 = 1;
        static const unsigned int test_tile1_port_index = 8;
        static const unsigned int test_tile1_port_index_1 = 9;
        static const unsigned int test_tile2_port_index = 22;
        static const unsigned int test_tile3_port_index = 30;
};

sai_switch_api_t* qos_buffer ::sai_switch_api_table = NULL;
sai_buffer_api_t* qos_buffer ::sai_buffer_api_table = NULL;
sai_port_api_t* qos_buffer ::sai_port_api_table = NULL;
sai_queue_api_t* qos_buffer ::sai_queue_api_table = NULL;

void qos_buffer :: sai_qos_get_buffer_pool_shadow_list (sai_object_id_t pool_id, sai_object_id_t *shadow_list)
{
    sai_attribute_t              get_attr;
    unsigned int                 i = 0;

    get_attr.id = SAI_BUFFER_POOL_ATTR_SHADOW_POOL_LIST;
    get_attr.value.objlist.count = MAX_SHADOW_LIST;
    get_attr.value.objlist.list  = shadow_list;
    ASSERT_EQ (SAI_STATUS_SUCCESS,
               sai_buffer_api_table->get_buffer_pool_attribute(pool_id, 1,  &get_attr));

    printf (" Pool ID : 0x%" PRIx64 " \r\n", pool_id);

    for (i = 0; i < get_attr.value.objlist.count; i++) {
        printf (" Shadow Pool ID : 0x%" PRIx64 " \r\n", shadow_list[i]);
    }
}

void qos_buffer :: sai_qos_get_pg_shadow_list (sai_object_id_t pg_id, sai_object_id_t *shadow_list)
{
    sai_attribute_t              get_attr;
    unsigned int                 i = 0;

    get_attr.id = SAI_INGRESS_PRIORITY_GROUP_ATTR_SHADOW_PG_LIST;
    get_attr.value.objlist.count = MAX_SHADOW_LIST;
    get_attr.value.objlist.list  = shadow_list;
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_buffer_api_table->get_ingress_priority_group_attribute
                                               (pg_id, 1, &get_attr));

    printf (" PG ID : 0x%" PRIx64 " \r\n", pg_id);

    for (i = 0; i < get_attr.value.objlist.count; i++) {
        printf (" Shadow PG ID : 0x%" PRIx64 " \r\n", shadow_list[i]);
    }
}

void qos_buffer :: sai_qos_get_q_shadow_list (sai_object_id_t q_id, sai_object_id_t *shadow_list)
{
    sai_attribute_t              get_attr;
    unsigned int                 i = 0;

    get_attr.id = SAI_QUEUE_ATTR_SHADOW_QUEUE_LIST;
    get_attr.value.objlist.count = MAX_SHADOW_LIST;
    get_attr.value.objlist.list  = shadow_list;
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_queue_api_table->get_queue_attribute
                                  (q_id, 1, &get_attr));

    printf (" q ID : 0x%" PRIx64 " \r\n", q_id);

    for (i = 0; i < get_attr.value.objlist.count; i++) {
        printf (" Shadow q ID : 0x%" PRIx64 " \r\n", shadow_list[i]);
    }
}

void qos_buffer :: sai_qos_xpe_buffer_verify (sai_object_id_t pool_id, sai_object_id_t *shadow_pool_list,
                                unsigned int pool_size, unsigned int pool_shared_size,
                                unsigned int xpe_0_reserved_size, unsigned int xpe_1_reserved_size,
                                unsigned int xpe_2_reserved_size, unsigned int xpe_3_reserved_size)
{
    sai_attribute_t              get_attr;
    unsigned int                 xpe_reserved_size[MAX_SHADOW_LIST] = {0};
    unsigned int                 shadow_pool_size = 0;
    unsigned int                 shadow_shared_size = 0;
    int                          i = 0;

    get_attr.id = SAI_BUFFER_POOL_ATTR_SIZE;
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_buffer_api_table->get_buffer_pool_attribute(pool_id,
                                                                             1, &get_attr));
    printf(" pool_size %d \r\n", pool_size);
    ASSERT_EQ (get_attr.value.u32, pool_size);

    get_attr.id = SAI_BUFFER_POOL_ATTR_SHARED_SIZE;
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_buffer_api_table->get_buffer_pool_attribute(pool_id,
                                                                             1, &get_attr));
    printf(" pool_shared_size %d \r\n", pool_shared_size);
    ASSERT_EQ (get_attr.value.u32, pool_shared_size);

    for (i = 0; i < MAX_SHADOW_LIST; i++) {
        get_attr.id = SAI_BUFFER_POOL_ATTR_SIZE;
        ASSERT_EQ(SAI_STATUS_SUCCESS, sai_buffer_api_table->get_buffer_pool_attribute(shadow_pool_list[i],
                                                                                 1, &get_attr));
        shadow_pool_size = get_attr.value.u32;
        printf(" shadow_pool_size %d \r\n", shadow_pool_size);

        get_attr.id = SAI_BUFFER_POOL_ATTR_SHARED_SIZE;
        ASSERT_EQ(SAI_STATUS_SUCCESS, sai_buffer_api_table->get_buffer_pool_attribute(shadow_pool_list[i],
                                                                                 1, &get_attr));
        shadow_shared_size = get_attr.value.u32;
        printf(" shadow_shared_size %d \r\n", shadow_shared_size);

        xpe_reserved_size[i] = shadow_pool_size - shadow_shared_size;
        printf(" xpe_reserved_size [%d] %d \r\n", i, xpe_reserved_size[i]);
    }

    ASSERT_EQ (xpe_0_reserved_size, xpe_reserved_size[0]);
    ASSERT_EQ (xpe_1_reserved_size, xpe_reserved_size[1]);
    ASSERT_EQ (xpe_2_reserved_size, xpe_reserved_size[2]);
    ASSERT_EQ (xpe_3_reserved_size, xpe_reserved_size[3]);
}

TEST_F(qos_buffer, sai_qos_shadow_list_get_test)
{
    sai_object_id_t pool_id = 0;
    sai_object_id_t pool_id_1 = 0;
    sai_object_id_t shadow_id_list[MAX_SHADOW_LIST] = {0};
    sai_object_id_t shadow_id_list_1[MAX_SHADOW_LIST] = {0};
    sai_object_id_t port_id = 0;
    sai_object_id_t pg_obj[1] = {0};
    sai_object_id_t q_obj[1] = {0};
    sai_attribute_t set_attr;

    unsigned int sai_buffer_pool_test_size_1 = 10485760; // 10MB

    printf("sai_buffer_pool_test_size_1 - %d bytes\r\n", sai_buffer_pool_test_size_1);

    // pool 10MB in dynamic mode
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_create_buffer_pool(sai_buffer_api_table, &pool_id,
                                  sai_buffer_pool_test_size_1, SAI_BUFFER_POOL_TYPE_INGRESS,
                                  SAI_BUFFER_POOL_THRESHOLD_MODE_DYNAMIC));

    sai_qos_get_buffer_pool_shadow_list (pool_id, shadow_id_list);
    /*  Pool Shared Size =  4 * MIN(XPE's shared size)
     *                        XPE 0     XPE 1     XPE 2   XPE 3
     * Reserved               0         0         0       0
     * shared size            2.5MB     2.5MB     2.5MB   2.5MB
     * Pool Shared Size  =  4 * 2.5MB
     */
    sai_qos_xpe_buffer_verify(pool_id, shadow_id_list, sai_buffer_pool_test_size_1,
                              sai_buffer_pool_test_size_1, 0, 0, 0, 0);

    // pool 0MB in dynamic mode
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_create_buffer_pool(sai_buffer_api_table, &pool_id_1,
                                  0, SAI_BUFFER_POOL_TYPE_INGRESS,
                                  SAI_BUFFER_POOL_THRESHOLD_MODE_DYNAMIC));

    sai_qos_get_buffer_pool_shadow_list (pool_id_1, shadow_id_list_1);
    /*  Pool Shared Size =  4 * MIN(XPE's shared size)
     *                        XPE 0     XPE 1     XPE 2   XPE 3
     * Reserved               0         0         0       0
     * shared size            0MB       0MB       0MB     0MB
     * Pool Shared Size  =  4 * 0MB
     */
    sai_qos_xpe_buffer_verify(pool_id_1, shadow_id_list_1, 0, 0, 0, 0, 0, 0);

    set_attr.id = SAI_BUFFER_POOL_ATTR_SIZE;
    set_attr.value.u32 = 8388608;
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_buffer_api_table->set_buffer_pool_attribute(pool_id,
                                                       (const sai_attribute_t *)&set_attr));
    /*  Pool Shared Size =  4 * MIN(XPE's shared size)
     *                        XPE 0     XPE 1     XPE 2   XPE 3
     * Reserved               0         0         0       0
     * shared size            2MB       2MB       2MB     2MB
     * Pool Shared Size  =  4 * 2MB
     */
    sai_qos_xpe_buffer_verify(pool_id, shadow_id_list, 8388608,
                              8388608, 0, 0, 0, 0);

    set_attr.id = SAI_BUFFER_POOL_ATTR_SIZE;
    set_attr.value.u32 = 2097152;
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_buffer_api_table->set_buffer_pool_attribute(pool_id_1,
                                                       (const sai_attribute_t *)&set_attr));
    /*  Pool Shared Size =  4 * MIN(XPE's shared size)
     *                        XPE 0     XPE 1     XPE 2   XPE 3
     * Reserved               0         0         0       0
     * shared size            0.5MB     0.5MB     0.5MB   0.5MB
     * Pool Shared Size  =  4 * 0.5MB
     */
    sai_qos_xpe_buffer_verify(pool_id_1, shadow_id_list_1, 2097152,
                              2097152, 0, 0, 0, 0);

    set_attr.id = SAI_BUFFER_POOL_ATTR_SIZE;
    set_attr.value.u32 = sai_buffer_pool_test_size_1;
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_buffer_api_table->set_buffer_pool_attribute(pool_id,
                                                                             (const sai_attribute_t *)&set_attr));
    /*  Pool Shared Size =  4 * MIN(XPE's shared size)
     *                        XPE 0     XPE 1     XPE 2   XPE 3
     * Reserved               0         0         0       0
     * shared size            2.5MB     2.5MB     2.5MB   2.5MB
     * Pool Shared Size  =  4 * 2.5MB
     */
    sai_qos_xpe_buffer_verify(pool_id, shadow_id_list, sai_buffer_pool_test_size_1,
                              sai_buffer_pool_test_size_1, 0, 0, 0, 0);

    // tile 0
    port_id = sai_qos_port_id_get (test_tile0_port_index);
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_qos_buffer_get_first_pg(sai_port_api_table, port_id, &pg_obj[0]));
    sai_qos_get_pg_shadow_list(pg_obj[0], shadow_id_list);

    // tile 1
    port_id = sai_qos_port_id_get (test_tile1_port_index);
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_qos_buffer_get_first_pg(sai_port_api_table, port_id, &pg_obj[0]));
    sai_qos_get_pg_shadow_list(pg_obj[0], shadow_id_list);

    // tile 0
    port_id = sai_qos_port_id_get (test_tile0_port_index);
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_qos_buffer_get_first_queue(sai_port_api_table, port_id, &q_obj[0]));
    sai_qos_get_q_shadow_list(q_obj[0], shadow_id_list);

    // tile 2
    port_id = sai_qos_port_id_get (test_tile2_port_index);
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_qos_buffer_get_first_queue(sai_port_api_table, port_id, &q_obj[0]));
    sai_qos_get_q_shadow_list(q_obj[0], shadow_id_list);
}

TEST_F(qos_buffer, sai_qos_buffer_profile_pg_tile_test)
{
    sai_attribute_t set_attr;
    sai_attribute_t get_attr[6];
    sai_object_id_t profile_id = 0;
    sai_object_id_t profile_id_1 = 0;
    sai_object_id_t pool_id = 0;
    sai_object_id_t pool_shadow_id_list[MAX_SHADOW_LIST] = {0};
    sai_object_id_t port_id = 0;
    sai_object_id_t pg_obj[10] = {0} /*SAI_NULL_OBJECT_ID */;

    unsigned int sai_buffer_pool_test_size_1 = 10485760; // 10MB
    unsigned int sai_buffer_profile_test_size_1 = 131072;// 0.125MB
    unsigned int sai_buffer_profile_test_size_2 = 524288;// 0.5MB
    unsigned int sai_buffer_profile_test_size_3 = 1048576;//1MB
    unsigned int sai_buffer_profile_test_size_4 = 262144;//0.25MB

    printf("Total buffer size - %d bytes\r\n", sai_total_buffer_size);
    printf("sai_buffer_pool_test_size_1 - %d bytes\r\n", sai_buffer_pool_test_size_1);
    printf("sai_buffer_profile_test_size_1 - %d bytes\r\n", sai_buffer_profile_test_size_1);
    printf("sai_buffer_profile_test_size_2 - %d bytes\r\n", sai_buffer_profile_test_size_2);

    // pool 10MB in dynamic mode
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_create_buffer_pool(sai_buffer_api_table, &pool_id,
                                  sai_buffer_pool_test_size_1, SAI_BUFFER_POOL_TYPE_INGRESS,
                                  SAI_BUFFER_POOL_THRESHOLD_MODE_DYNAMIC));

    // profile_id = reserved size 0.5MB, static - 0 hdrm - 0.125MB
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_create_buffer_profile(sai_buffer_api_table, &profile_id,
                                  0x6b ,pool_id, sai_buffer_profile_test_size_2, 0, 1, 0,
                                  sai_buffer_profile_test_size_1, sai_buffer_profile_test_size_1));

    // profile_id_1 = reserved size 1MB, static - 0 hdrm - 0.25MB
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_create_buffer_profile(sai_buffer_api_table, &profile_id_1,
                                  0x6b ,pool_id, sai_buffer_profile_test_size_3, 0, 1, 0,
                                  sai_buffer_profile_test_size_4, sai_buffer_profile_test_size_4));

    sai_qos_get_buffer_pool_shadow_list (pool_id, pool_shadow_id_list);

    /*  Pool Shared Size =  4 * MIN(XPE's shared size)
     *                        XPE 0     XPE 1     XPE 2   XPE 3
     * Reserved               0         0         0       0
     * shared size            2.5MB     2.5MB     2.5MB   2.5MB
     * Pool Shared Size  =  4 * 2.5MB
     */
    sai_qos_xpe_buffer_verify (pool_id, pool_shadow_id_list, sai_buffer_pool_test_size_1,
                               sai_buffer_pool_test_size_1, 0, 0, 0, 0);

    // tile 0
    port_id = sai_qos_port_id_get (test_tile0_port_index);
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_qos_buffer_get_first_pg(sai_port_api_table, port_id, &pg_obj[0]));
    set_attr.id = SAI_INGRESS_PRIORITY_GROUP_ATTR_BUFFER_PROFILE;
    set_attr.value.oid = profile_id;
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_buffer_api_table->set_ingress_priority_group_attribute
                                               (pg_obj[0], (const sai_attribute_t *)&set_attr));

    memset(get_attr, 0, sizeof(get_attr));
    get_attr[0].id = SAI_INGRESS_PRIORITY_GROUP_ATTR_BUFFER_PROFILE;
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_buffer_api_table->
               get_ingress_priority_group_attribute (pg_obj[0], 1, get_attr));

    EXPECT_EQ (get_attr[0].value.oid, profile_id);

    ASSERT_EQ (SAI_STATUS_OBJECT_IN_USE,
               sai_buffer_api_table->remove_buffer_profile (profile_id));
    /*  Pool Shared Size =  4 * MIN(XPE's shared size)
     *                        XPE 0     XPE 1     XPE 2   XPE 3
     * Reserved               0.625     0.625         0       0
     * shared size            1.875MB   1.875MB     2.5MB   2.5MB
     * Pool Shared Size  =  4 * 1.875MB
     */

    sai_qos_xpe_buffer_verify(pool_id, pool_shadow_id_list,
                              sai_buffer_pool_test_size_1, 7864320,
                              655360, 655360, 0, 0);
    printf("Apply profile on tile 1 \r\n");
    // tile 1
    port_id = sai_qos_port_id_get (test_tile1_port_index);
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_qos_buffer_get_first_pg(sai_port_api_table, port_id, &pg_obj[1]));

    set_attr.id = SAI_INGRESS_PRIORITY_GROUP_ATTR_BUFFER_PROFILE;
    set_attr.value.oid = profile_id;
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_buffer_api_table->set_ingress_priority_group_attribute
                                               (pg_obj[1], (const sai_attribute_t *)&set_attr));

    /*  Pool Shared Size =  4 * MIN(XPE's shared size)
     *                        XPE 0     XPE 1     XPE 2   XPE 3
     * Reserved               0.625     0.625     0.625   0.625
     * shared size            1.875MB   1.875MB   1.875MB 1.875MB
     * Pool Shared Size  =  4 * 1.875 */

    sai_qos_xpe_buffer_verify(pool_id, pool_shadow_id_list,
                              sai_buffer_pool_test_size_1, 7864320,
                              655360, 655360, 655360, 655360);
    // tile 2
    port_id = sai_qos_port_id_get (test_tile2_port_index);
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_qos_buffer_get_first_pg(sai_port_api_table, port_id, &pg_obj[2]));

    set_attr.id = SAI_INGRESS_PRIORITY_GROUP_ATTR_BUFFER_PROFILE;
    set_attr.value.oid = profile_id;
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_buffer_api_table->set_ingress_priority_group_attribute
                                               (pg_obj[2], (const sai_attribute_t *)&set_attr));

    /*  Pool Shared Size =  4 * MIN(XPE's shared size)
     *                        XPE 0     XPE 1     XPE 2   XPE 3
     * Reserved               0.625     0.625     1.25    1.25
     * shared size            1.875MB   1.875MB   1.25    1.25MB
     * Pool Shared Size  =  4 * 1.25MB
     */
    sai_qos_xpe_buffer_verify(pool_id, pool_shadow_id_list,
                              sai_buffer_pool_test_size_1, 5242880,
                              655360, 655360, 1310720, 1310720);

    // tile 3
    port_id = sai_qos_port_id_get (test_tile3_port_index);
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_qos_buffer_get_first_pg(sai_port_api_table, port_id, &pg_obj[3]));

    set_attr.id = SAI_INGRESS_PRIORITY_GROUP_ATTR_BUFFER_PROFILE;
    set_attr.value.oid = profile_id;
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_buffer_api_table->set_ingress_priority_group_attribute
                                               (pg_obj[3], (const sai_attribute_t *)&set_attr));

    /*  Pool Shared Size =  4 * MIN(XPE's shared size)
     *                        XPE 0     XPE 1     XPE 2   XPE 3
     * Reserved               1.25      1.25      1.25    1.25
     * shared size            1.25MB    1.25MB    1.25    1.25MB
     * Pool Shared Size  =  4 * 1.25MB
     */
    sai_qos_xpe_buffer_verify(pool_id, pool_shadow_id_list,
                              sai_buffer_pool_test_size_1, 5242880,
                              1310720, 1310720, 1310720, 1310720);
    // update tile 0 with 1.25MB reserved size
    port_id = sai_qos_port_id_get (test_tile0_port_index_1);
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_qos_buffer_get_first_pg(sai_port_api_table, port_id, &pg_obj[4]));

    set_attr.id = SAI_INGRESS_PRIORITY_GROUP_ATTR_BUFFER_PROFILE;
    set_attr.value.oid = profile_id_1;
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_buffer_api_table->set_ingress_priority_group_attribute
                                               (pg_obj[4], (const sai_attribute_t *)&set_attr));

    /* Pool Shared Size =  4 * MIN(XPE's shared size)
     *                        XPE 0     XPE 1     XPE 2   XPE 3
     * Reserved               2.5       2.5      1.25    1.25
     * shared size            0MB       0MB      1.25    1.25MB
     * Pool Shared Size  =  4 * 0MB
     */
    sai_qos_xpe_buffer_verify(pool_id, pool_shadow_id_list,
                              sai_buffer_pool_test_size_1, 0,
                              2621440, 2621440, 1310720, 1310720);

    printf("After updating tile 0 with 2.5MB\r\n");

   // update tile 1 with 1.25MB reserved size
    port_id = sai_qos_port_id_get (test_tile1_port_index_1);
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_qos_buffer_get_first_pg(sai_port_api_table, port_id, &pg_obj[5]));

    set_attr.id = SAI_INGRESS_PRIORITY_GROUP_ATTR_BUFFER_PROFILE;
    set_attr.value.oid = profile_id_1;
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_buffer_api_table->set_ingress_priority_group_attribute
                                               (pg_obj[5], (const sai_attribute_t *)&set_attr));

    /* Pool Shared Size =  4 * MIN(XPE's shared size)
     *                        XPE 0     XPE 1     XPE 2   XPE 3
     * Reserved               2.5       2.5      2.5      2.5
     * shared size            0MB       0MB      0        0MB
     * Pool Shared Size  =  4 * 0MB
     */
    sai_qos_xpe_buffer_verify(pool_id, pool_shadow_id_list,
                              sai_buffer_pool_test_size_1, 0,
                              2621440, 2621440, 2621440, 2621440);

    printf("After updating tile 1 with 2.5MB\r\n");
    printf("Modify the buffer profile \r\n");
    printf("Reduce Reserved size from 0.5MB to 0MB\r\n");
    set_attr.id = SAI_BUFFER_PROFILE_ATTR_BUFFER_SIZE;
    set_attr.value.u32 = 0;
    ASSERT_EQ (SAI_STATUS_SUCCESS,
               sai_buffer_api_table->set_buffer_profile_attribute (profile_id,
                                              (const sai_attribute_t *)&set_attr));
    /* Pool Shared Size =  4 * MIN(XPE's shared size)
     *                        XPE 0     XPE 1     XPE 2   XPE 3
     * Reserved               1.5       1.5       1.5     1.5
     * shared size            1MB       1MB       1       1
     * Pool Shared Size  =  4 * 1MB
     */
    sai_qos_xpe_buffer_verify(pool_id, pool_shadow_id_list,
                              sai_buffer_pool_test_size_1, 4194304,
                              1572864, 1572864, 1572864, 1572864);
    printf("Increase Reserved size to 0MB to 0.5MB,  xoff 0.125MB\r\n");
    set_attr.id = SAI_BUFFER_PROFILE_ATTR_BUFFER_SIZE;
    set_attr.value.u32 = sai_buffer_profile_test_size_4 * 2; // 0.5 MB
    ASSERT_EQ (SAI_STATUS_SUCCESS,
               sai_buffer_api_table->set_buffer_profile_attribute (profile_id,
                                              (const sai_attribute_t *)&set_attr));
    /* Pool Shared Size =  4 * MIN(XPE's shared size)
     *                        XPE 0     XPE 1     XPE 2   XPE 3
     * Reserved               2.5       2.5      2.5      2.5
     * shared size            0MB       0MB      0        0MB
     * Pool Shared Size  =  4 * 0MB
     */
    sai_qos_xpe_buffer_verify(pool_id, pool_shadow_id_list,
                              sai_buffer_pool_test_size_1, 0,
                              2621440, 2621440, 2621440, 2621440);

    printf("Xoff size set 0.125MB to 0MB, reserved size 0.5MB\r\n");

    set_attr.id = SAI_BUFFER_PROFILE_ATTR_XOFF_TH;
    set_attr.value.u32 = 0;
    ASSERT_EQ (SAI_STATUS_SUCCESS,
               sai_buffer_api_table->set_buffer_profile_attribute (profile_id,
                                              (const sai_attribute_t *)&set_attr));

     /* Pool Shared Size =  4 * MIN(XPE's shared size)
     *                        XPE 0     XPE 1     XPE 2   XPE 3
     * Reserved               2.25       2.25      2.25      2.25
     * shared size            0.25       0.25MB    0.25      0.25MB
     * Pool Shared Size  =  4 * 0.25MB
     */
    sai_qos_xpe_buffer_verify(pool_id, pool_shadow_id_list,
                              sai_buffer_pool_test_size_1, 1048576,
                              2359296, 2359296, 2359296, 2359296);

    printf("Xoff size set 0MB to 0.25MB, reserved size 0.5MB\r\n");

    set_attr.id = SAI_BUFFER_PROFILE_ATTR_XOFF_TH;
    set_attr.value.u32 = sai_buffer_profile_test_size_1 * 2;
    ASSERT_EQ (SAI_STATUS_INSUFFICIENT_RESOURCES,
               sai_buffer_api_table->set_buffer_profile_attribute (profile_id,
                                              (const sai_attribute_t *)&set_attr));

     /* Pool Shared Size =  4 * MIN(XPE's shared size)
     *                        XPE 0     XPE 1     XPE 2   XPE 3
     * Reserved               2.25       2.25      2.25      2.25
     * shared size            0.25       0.25MB    0.25      0.25MB
     * Pool Shared Size  =  4 * 0.25MB
     */
    sai_qos_xpe_buffer_verify(pool_id, pool_shadow_id_list,
                              sai_buffer_pool_test_size_1, 1048576,
                              2359296, 2359296, 2359296, 2359296);
    printf("Removeing profile from PG\r\n");
    for (int idx = 0; idx < 10 ; idx++)
    {
        set_attr.id = SAI_INGRESS_PRIORITY_GROUP_ATTR_BUFFER_PROFILE;
        set_attr.value.oid = SAI_NULL_OBJECT_ID;
        if (pg_obj[idx]) {
            ASSERT_EQ (SAI_STATUS_SUCCESS, sai_buffer_api_table->set_ingress_priority_group_attribute
                       (pg_obj[idx], (const sai_attribute_t *)&set_attr));
        }
    }

    /*  Max resevred size in pool = 4 * sum(tiles reserved size )
     *  Shared Size per tile = ( Pool Size - (Max resevred size in pool)/max tiles.
     *                        Tile 0    Tile 1    Tile 2  Tile 3
     * Reserved               0MB       0MB       0MB     0MB
     * shared size            2.5MB     2.5MB     2.5MB   2.5MB
     *
     */

    sai_qos_xpe_buffer_verify(pool_id, pool_shadow_id_list, 10485760, 10485760, 0, 0, 0, 0);


    printf("After Removeing profiles from PG\r\n");
    ASSERT_EQ (SAI_STATUS_SUCCESS,
               sai_buffer_api_table->remove_buffer_profile (profile_id));

    ASSERT_EQ (SAI_STATUS_SUCCESS,
               sai_buffer_api_table->remove_buffer_profile (profile_id_1));

    sai_buffer_api_table->remove_buffer_pool (pool_id);
}

TEST_F(qos_buffer, sai_qos_buffer_profile_xoff_size_tile_test)
{
    sai_attribute_t set_attr;
    sai_attribute_t get_attr[6];
    sai_object_id_t profile_id = 0;
    sai_object_id_t pool_id = 0;
    sai_object_id_t port_id =  0;
    sai_object_id_t pg_obj[10] = {0} /*SAI_NULL_OBJECT_ID */;
    sai_object_id_t pool_shadow_id_list[MAX_SHADOW_LIST] = {0};

    unsigned int sai_buffer_pool_test_size_1 = 10485760; // 10MB
    unsigned int sai_buffer_profile_test_size_1 = 131072;// 0.125MB
    unsigned int sai_buffer_profile_test_size_2 = 524288;// 0.5MB
    unsigned int sai_buffer_pool_xoff_size = 2097152;// 2MB

    printf("Total buffer size - %d bytes\r\n", sai_total_buffer_size);
    printf("sai_buffer_pool_test_size_1 - %d bytes\r\n", sai_buffer_pool_test_size_1);
    printf("sai_buffer_pool_xoff_size   - %d bytes\r\n", sai_buffer_pool_xoff_size);
    printf("sai_buffer_profile_test_size_1 - %d bytes\r\n", sai_buffer_profile_test_size_1);
    printf("sai_buffer_profile_test_size_2 - %d bytes\r\n", sai_buffer_profile_test_size_2);

    // pool 10MB in dynamic mode
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_create_buffer_pool(sai_buffer_api_table, &pool_id,
                                  sai_buffer_pool_test_size_1, SAI_BUFFER_POOL_TYPE_INGRESS,
                                  SAI_BUFFER_POOL_THRESHOLD_MODE_DYNAMIC));

    sai_qos_get_buffer_pool_shadow_list (pool_id, pool_shadow_id_list);
    /*  Pool Shared Size =  4 * MIN(XPE's shared size)
     *                        XPE 0     XPE 1     XPE 2   XPE 3
     * Reserved               0         0         0       0
     * shared size            2.5MB     2.5MB     2.5MB   2.5MB
     * Pool Shared Size  =  4 * 2.5MB
     */
    sai_qos_xpe_buffer_verify (pool_id, pool_shadow_id_list, sai_buffer_pool_test_size_1,
                               sai_buffer_pool_test_size_1, 0, 0, 0, 0);


    set_attr.id = SAI_BUFFER_POOL_ATTR_XOFF_SIZE;
    set_attr.value.u32 = sai_buffer_pool_xoff_size;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_buffer_api_table->set_buffer_pool_attribute(pool_id,
                                                  (const sai_attribute_t *)&set_attr));

    /*  Pool Shared Size =  4 * MIN(XPE's shared size)
     *                        XPE 0     XPE 1     XPE 2   XPE 3
     * Reserved               2MB       2MB       2MB     2MB
     * shared size            0.5MB     0.5MB     0.5MB   0.5MB
     * Pool Shared Size  =  4 * 0.5MB
     */
    sai_qos_xpe_buffer_verify (pool_id, pool_shadow_id_list, sai_buffer_pool_test_size_1,
                               2097152, 2097152, 2097152, 2097152, 2097152);

    // profile_id = reserved size 0.5MB, static - 0 hdrm - 0MB
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_create_buffer_profile(sai_buffer_api_table, &profile_id,
                                  0x6b ,pool_id, sai_buffer_profile_test_size_2, 0, 1, 0,
                                  0, 0));
    // tile 0
    port_id = sai_qos_port_id_get (test_tile0_port_index);
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_qos_buffer_get_first_pg(sai_port_api_table, port_id, &pg_obj[0]));
    set_attr.id = SAI_INGRESS_PRIORITY_GROUP_ATTR_BUFFER_PROFILE;
    set_attr.value.oid = profile_id;
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_buffer_api_table->set_ingress_priority_group_attribute
                                               (pg_obj[0], (const sai_attribute_t *)&set_attr));

    memset(get_attr, 0, sizeof(get_attr));
    get_attr[0].id = SAI_INGRESS_PRIORITY_GROUP_ATTR_BUFFER_PROFILE;
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_buffer_api_table->
               get_ingress_priority_group_attribute (pg_obj[0], 1, get_attr));

    EXPECT_EQ (get_attr[0].value.oid, profile_id);

    ASSERT_EQ (SAI_STATUS_OBJECT_IN_USE,
               sai_buffer_api_table->remove_buffer_profile (profile_id));
    /*  Pool Shared Size =  4 * MIN(XPE's shared size)
     *                        XPE 0     XPE 1     XPE 2   XPE 3
     * Reserved               2.5MB     2.5       2MB     2MB
     * shared size            0MB       0MB       0.5MB   0.5MB
     * Pool Shared Size  =  4 * 0MB
     */
    sai_qos_xpe_buffer_verify (pool_id, pool_shadow_id_list, sai_buffer_pool_test_size_1,
                               0, 2621440, 2621440, 2097152, 2097152);

    printf("Reduce XOFF_SIZE 2MB to 1MB\r\n");

    set_attr.id = SAI_BUFFER_POOL_ATTR_XOFF_SIZE;
    set_attr.value.u32 = sai_buffer_pool_xoff_size/2;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_buffer_api_table->set_buffer_pool_attribute(pool_id,
                                                  (const sai_attribute_t *)&set_attr));

    /*  Max resevred size in pool = 4 * sum(tiles reserved size )
     *  Shared Size per tile = ( Pool Size - (Max resevred size in pool)/max tiles.
     *                        Tile 0    Tile 1    Tile 2  Tile 3
     * Reserved               1.5MB      1.5MB    1MB     1MB
     * shared size            1MB        1MB      1.5MB   1.5MB
     * Pool Shared Size  =  4 * 1MB
     */
    sai_qos_xpe_buffer_verify (pool_id, pool_shadow_id_list, sai_buffer_pool_test_size_1,
                               4194304, 1572864, 1572864, 1048576, 1048576);

    printf("Increase XOFF_SIZE 1MB to 4MB(i.e 0.75MB increases for each tile)\r\n");

    set_attr.id = SAI_BUFFER_POOL_ATTR_XOFF_SIZE;
    set_attr.value.u32 = (sai_buffer_pool_xoff_size * 2);

    ASSERT_EQ(SAI_STATUS_INSUFFICIENT_RESOURCES, sai_buffer_api_table->set_buffer_pool_attribute(pool_id,
                                                  (const sai_attribute_t *)&set_attr));
    /*  Max resevred size in pool = 4 * sum(tiles reserved size )
     *  Shared Size per tile = ( Pool Size - (Max resevred size in pool)/max tiles.
     *                        Tile 0    Tile 1    Tile 2  Tile 3
     * Reserved               1.5MB      1.5MB    1MB     1MB
     * shared size            1MB        1MB      1.5MB   1.5MB
     * Pool Shared Size  =  4 * 1MB
     */
    sai_qos_xpe_buffer_verify (pool_id, pool_shadow_id_list, sai_buffer_pool_test_size_1,
                               4194304, 1572864, 1572864, 1048576, 1048576);

    printf("Increase XOFF_SIZE 1MB to 2MB\r\n");

    set_attr.id = SAI_BUFFER_POOL_ATTR_XOFF_SIZE;
    set_attr.value.u32 = sai_buffer_pool_xoff_size;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_buffer_api_table->set_buffer_pool_attribute(pool_id,
                                                  (const sai_attribute_t *)&set_attr));
    /*  Pool Shared Size =  4 * MIN(XPE's shared size)
     *                        XPE 0     XPE 1     XPE 2   XPE 3
     * Reserved               2.5MB     2.5       2MB     2MB
     * shared size            0MB       0MB       0.5MB   0.5MB
     * Pool Shared Size  =  4 * 0MB
     */
    sai_qos_xpe_buffer_verify (pool_id, pool_shadow_id_list, sai_buffer_pool_test_size_1,
                               0, 2621440, 2621440, 2097152, 2097152);


    printf("Increase XOFF_SIZE 2MB to 0(i.e 0.5MB release to each tile and hdrm 1.25MB )\r\n");

    set_attr.id = SAI_BUFFER_POOL_ATTR_XOFF_SIZE;
    set_attr.value.u32 = 0;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_buffer_api_table->set_buffer_pool_attribute(pool_id,
                                                  (const sai_attribute_t *)&set_attr));

    /*  Max resevred size in pool = 4 * sum(tiles reserved size )
     *  Shared Size per tile = ( Pool Size - (Max resevred size in pool)/max tiles.
     *                        Tile 0    Tile 1    Tile 2  Tile 3
     * Reserved               0.5MB     0.5MB     0MB     0MB
     * shared size            2MB       2MB       2.5MB   2.5MB
     * Pool Shared Size  =  4 * 2MB
     */
    sai_qos_xpe_buffer_verify (pool_id, pool_shadow_id_list, sai_buffer_pool_test_size_1,
                               8388608, 524288, 524288, 0, 0);

    printf("Increase xoff_th to 0.5MB\r\n");

    // profile size update - increase xooff_th size to 0.5MB
    set_attr.id = SAI_BUFFER_PROFILE_ATTR_XOFF_TH;
    set_attr.value.u32 = sai_buffer_profile_test_size_2;
    ASSERT_EQ (SAI_STATUS_SUCCESS,
               sai_buffer_api_table->set_buffer_profile_attribute(profile_id,
                                              (const sai_attribute_t *)&set_attr));
    /*  Max resevred size in pool = 4 * sum(tiles reserved size )
     *  Shared Size per tile = ( Pool Size - (Max resevred size in pool)/max tiles.
     *                        Tile 0    Tile 1    Tile 2  Tile 3
     * Reserved               1MB       1MB       0MB     0MB
     * shared size            1.5MB     1.5MB     2.5MB   2.5MB
     * Pool Shared Size  =  4 * 1.5MB
     */
    sai_qos_xpe_buffer_verify (pool_id, pool_shadow_id_list, sai_buffer_pool_test_size_1,
                               6291456, 1048576, 1048576, 0, 0);

    printf("Increase XOFF_SIZE 0MB to 2MB\r\n");

    set_attr.id = SAI_BUFFER_POOL_ATTR_XOFF_SIZE;
    set_attr.value.u32 = sai_buffer_pool_xoff_size;

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_buffer_api_table->set_buffer_pool_attribute(pool_id,
                                                  (const sai_attribute_t *)&set_attr));

    /*  Max resevred size in pool = 4 * sum(tiles reserved size )
     *  Shared Size per tile = ( Pool Size - (Max resevred size in pool)/max tiles.
     *                        Tile 0    Tile 1    Tile 2  Tile 3
     * Reserved               2.5MB     2.5MB       2MB     2MB
     * shared size            0MB      0MB     0.5MB   0.5MB
     * Pool Shared Size  =  4 * 0MB
     */
    sai_qos_xpe_buffer_verify (pool_id, pool_shadow_id_list, sai_buffer_pool_test_size_1,
                               0, 2621440, 2621440, 2097152, 2097152);

    printf("Removeing profile from PG\r\n");
    for (int idx = 0; idx < 10 ; idx++)
    {
        set_attr.id = SAI_INGRESS_PRIORITY_GROUP_ATTR_BUFFER_PROFILE;
        set_attr.value.oid = SAI_NULL_OBJECT_ID;
        if (pg_obj[idx]) {
            ASSERT_EQ (SAI_STATUS_SUCCESS, sai_buffer_api_table->set_ingress_priority_group_attribute
                       (pg_obj[idx], (const sai_attribute_t *)&set_attr));
        }
    }
    /*  Max resevred size in pool = 4 * sum(tiles reserved size )
     *  Shared Size per tile = ( Pool Size - (Max resevred size in pool)/max tiles.
     *                        Tile 0    Tile 1    Tile 2  Tile 3
     * Reserved               2MB       2MB       2MB     2MB
     * shared size            0.5MB     0.5MB     0.5MB   0.5MB
     * Pool Shared Size  =  4 * 0.5MB
     */
    sai_qos_xpe_buffer_verify (pool_id, pool_shadow_id_list, sai_buffer_pool_test_size_1,
                               2097152, 2097152, 2097152, 2097152, 2097152);

    printf("After Removeing profiles from PG\r\n");

    ASSERT_EQ (SAI_STATUS_SUCCESS,
               sai_buffer_api_table->remove_buffer_profile (profile_id));

    printf("After Removeing profiles\r\n");

    ASSERT_EQ (SAI_STATUS_SUCCESS,
               sai_buffer_api_table->remove_buffer_pool (pool_id));
}

TEST_F(qos_buffer, sai_qos_buffer_profile_queue_tile_test)
{

    sai_attribute_t set_attr;
    sai_attribute_t get_attr[6];
    sai_object_id_t profile_id = 0;
    sai_object_id_t profile_id_1 = 0;
    sai_object_id_t pool_id = 0;
    sai_object_id_t pool_shadow_id_list[MAX_SHADOW_LIST] = {0};
    sai_object_id_t port_id = 0;
    sai_object_id_t q_obj[10] = {0} /*SAI_NULL_OBJECT_ID */;

    unsigned int sai_buffer_pool_test_size_1 = 10485760; // 10MB
    unsigned int sai_buffer_profile_test_size_1 = 1048576;// 1MB
    unsigned int sai_buffer_profile_test_size_2 = 524288;// 0.5MB

    printf("Total buffer size - %d bytes\r\n", sai_total_buffer_size);
    printf("sai_buffer_pool_test_size_1 - %d bytes\r\n", sai_buffer_pool_test_size_1);
    printf("sai_buffer_profile_test_size_1 - %d bytes\r\n", sai_buffer_profile_test_size_1);
    printf("sai_buffer_profile_test_size_2 - %d bytes\r\n", sai_buffer_profile_test_size_2);

    // pool 10MB in dynamic mode
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_create_buffer_pool(sai_buffer_api_table, &pool_id,
                                  sai_buffer_pool_test_size_1, SAI_BUFFER_POOL_TYPE_EGRESS,
                                  SAI_BUFFER_POOL_THRESHOLD_MODE_DYNAMIC));

    // profile_id = reserved size 0.5MB, static - 0
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_create_buffer_profile(sai_buffer_api_table, &profile_id,
                                  0xb ,pool_id, sai_buffer_profile_test_size_2, 0, 1, 0, 0, 0));

    // profile_id_1 = reserved size 1MB, static - 0
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_create_buffer_profile(sai_buffer_api_table, &profile_id_1,
                                  0xb ,pool_id, sai_buffer_profile_test_size_1, 0, 1, 0, 0, 0));

    sai_qos_get_buffer_pool_shadow_list (pool_id, pool_shadow_id_list);

    /*  Pool Shared Size =  4 * MIN(XPE's shared size)
     *                        XPE 0     XPE 1     XPE 2   XPE 3
     * Reserved               0         0         0       0
     * shared size            2.5MB     2.5MB     2.5MB   2.5MB
     * Pool Shared Size  =  4 * 2.5MB
     */
    sai_qos_xpe_buffer_verify (pool_id, pool_shadow_id_list, sai_buffer_pool_test_size_1,
                               sai_buffer_pool_test_size_1, 0, 0, 0, 0);

    // tile 0
    port_id = sai_qos_port_id_get (test_tile0_port_index);
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_qos_buffer_get_first_queue(sai_port_api_table, port_id, &q_obj[0]));
    set_attr.id = SAI_QUEUE_ATTR_BUFFER_PROFILE_ID;
    set_attr.value.oid = profile_id;
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_queue_api_table->set_queue_attribute
               (q_obj[0], (const sai_attribute_t *)&set_attr));

    memset(get_attr, 0, sizeof(get_attr));
    get_attr[0].id = SAI_QUEUE_ATTR_BUFFER_PROFILE_ID;
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_queue_api_table->get_queue_attribute (
                                                     q_obj[0], 1,  get_attr));
    EXPECT_EQ (get_attr[0].value.oid, profile_id);

    ASSERT_EQ (SAI_STATUS_OBJECT_IN_USE,
               sai_buffer_api_table->remove_buffer_profile (profile_id));

    /*  Pool Shared Size =  4 * MIN(XPE's shared size)
     *                        XPE 0     XPE 1       XPE 2     XPE 3
     * Reserved               0.5       0           0.5       0
     * shared size            2         2.5         2MB      2.5MB
     * Pool Shared Size  =  4 * 2MB
     */

    sai_qos_xpe_buffer_verify(pool_id, pool_shadow_id_list,
                              sai_buffer_pool_test_size_1, 8388608,
                              524288, 0, 524288, 0);
    printf("Apply profile on tile 1 \r\n");
    // tile 1
    port_id = sai_qos_port_id_get (test_tile1_port_index);
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_qos_buffer_get_first_queue(sai_port_api_table, port_id, &q_obj[1]));
    set_attr.id = SAI_QUEUE_ATTR_BUFFER_PROFILE_ID;
    set_attr.value.oid = profile_id;
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_queue_api_table->set_queue_attribute
               (q_obj[1], (const sai_attribute_t *)&set_attr));

    /*  Pool Shared Size =  4 * MIN(XPE's shared size)
     *                        XPE 0     XPE 1       XPE 2     XPE 3
     * Reserved               1         0             1        0
     * shared size            1.5       2.5         1.5MB      2.5MB
     * Pool Shared Size  =  4 * 1.5MB
     */

    sai_qos_xpe_buffer_verify(pool_id, pool_shadow_id_list,
                              sai_buffer_pool_test_size_1, 6291456,
                              1048576, 0, 1048576, 0);
    // tile 2

    port_id = sai_qos_port_id_get (test_tile2_port_index);
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_qos_buffer_get_first_queue(sai_port_api_table, port_id, &q_obj[2]));
    set_attr.id = SAI_QUEUE_ATTR_BUFFER_PROFILE_ID;
    set_attr.value.oid = profile_id;
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_queue_api_table->set_queue_attribute
               (q_obj[2], (const sai_attribute_t *)&set_attr));
    /*  Pool Shared Size =  4 * MIN(XPE's shared size)
     *                        XPE 0     XPE 1       XPE 2     XPE 3
     * Reserved               1         0.5             1      0.5
     * shared size            1.5       2            1.5MB     2MB
     * Pool Shared Size  =  4 * 1.5MB
     */

    sai_qos_xpe_buffer_verify(pool_id, pool_shadow_id_list,
                              sai_buffer_pool_test_size_1, 6291456,
                              1048576, 524288, 1048576, 524288);

    // tile 3
    port_id = sai_qos_port_id_get (test_tile3_port_index);
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_qos_buffer_get_first_queue(sai_port_api_table, port_id, &q_obj[3]));
    set_attr.id = SAI_QUEUE_ATTR_BUFFER_PROFILE_ID;
    set_attr.value.oid = profile_id;
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_queue_api_table->set_queue_attribute
               (q_obj[3], (const sai_attribute_t *)&set_attr));

    /*  Pool Shared Size =  4 * MIN(XPE's shared size)
     *                        XPE 0     XPE 1       XPE 2     XPE 3
     * Reserved               1         1             1        1
     * shared size            1.5       1.5           1.5MB    1.5MB
     * Pool Shared Size  =  4 * 1.5MB
     */

    sai_qos_xpe_buffer_verify(pool_id, pool_shadow_id_list,
                              sai_buffer_pool_test_size_1, 6291456,
                              1048576, 1048576, 1048576, 1048576);
  // update tile 0 with 1MB reserved size
    port_id = sai_qos_port_id_get (test_tile0_port_index_1);
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_qos_buffer_get_first_queue(sai_port_api_table, port_id,
                                                                 &q_obj[4]));
    set_attr.id = SAI_QUEUE_ATTR_BUFFER_PROFILE_ID;
    set_attr.value.oid = profile_id;
     ASSERT_EQ (SAI_STATUS_SUCCESS, sai_queue_api_table->set_queue_attribute
                (q_obj[4], (const sai_attribute_t *)&set_attr));

    /*  Pool Shared Size =  4 * MIN(XPE's shared size)
     *                        XPE 0     XPE 1       XPE 2     XPE 3
     * Reserved               1.5         1         1.5        1
     * shared size            1           1.5       1MB      1.5MB
     * Pool Shared Size  =  4 * 1MB
     */

    sai_qos_xpe_buffer_verify(pool_id, pool_shadow_id_list,
                              sai_buffer_pool_test_size_1, 4194304,
                              1572864, 1048576, 1572864, 1048576);

    printf("After updating tile 0 with 1MB\r\n");

    printf("Reduce Reserved size from 0.5MB to 0.25MB\r\n");
    set_attr.id = SAI_BUFFER_PROFILE_ATTR_BUFFER_SIZE;
    set_attr.value.u32 = sai_buffer_profile_test_size_2/2;
    ASSERT_EQ (SAI_STATUS_SUCCESS,
               sai_buffer_api_table->set_buffer_profile_attribute (profile_id,
                                              (const sai_attribute_t *)&set_attr));
    /*  Pool Shared Size =  4 * MIN(XPE's shared size)
     *                        XPE 0     XPE 1       XPE 2     XPE 3
     * Reserved               0.75       0.5         0.75      0.5
     * shared size            1.75         2         1.75      2MB
     * Pool Shared Size  =  4 * 1.75MB
     */
    sai_qos_xpe_buffer_verify(pool_id, pool_shadow_id_list,
                              sai_buffer_pool_test_size_1, 7340032,
                              786432, 524288, 786432, 524288);
    printf("Increase Reserved size to 0.75MB \r\n");
    // profile update - increase reserved size to 0.75MB
    set_attr.id = SAI_BUFFER_PROFILE_ATTR_BUFFER_SIZE;
    set_attr.value.u32 = ((sai_buffer_profile_test_size_2/2) * 3); // 0.75 MB
    ASSERT_EQ (SAI_STATUS_SUCCESS,
               sai_buffer_api_table->set_buffer_profile_attribute (profile_id,
                                              (const sai_attribute_t *)&set_attr));
    /*  Pool Shared Size =  4 * MIN(XPE's shared size)
     *                        XPE 0     XPE 1       XPE 2     XPE 3
     * Reserved               2.25       1.5         2.25      1.5
     * shared size            0.25       1           0.25      1MB
     * Pool Shared Size  =  4 * 0.25MB
     */
    sai_qos_xpe_buffer_verify(pool_id, pool_shadow_id_list,
                              sai_buffer_pool_test_size_1, 1048576,
                              2359296, 1572864, 2359296, 1572864);

    printf("Apply profile id 1 to queue on tile 0 \r\n");
    // update tile 0 with new profile id of size 1MB reserved size
     set_attr.id = SAI_QUEUE_ATTR_BUFFER_PROFILE_ID;
     set_attr.value.oid = profile_id_1;
     ASSERT_EQ (SAI_STATUS_SUCCESS, sai_queue_api_table->set_queue_attribute
                (q_obj[4], (const sai_attribute_t *)&set_attr));

    /*  Pool Shared Size =  4 * MIN(XPE's shared size)
     *                        XPE 0     XPE 1       XPE 2     XPE 3
     * Reserved               2.5       1.5         2.5       1.5
     * shared size            0         1           0         1MB
     * Pool Shared Size  =  4 * 0MB
     */
    sai_qos_xpe_buffer_verify(pool_id, pool_shadow_id_list,
                              sai_buffer_pool_test_size_1, 0,
                              2621440, 1572864, 2621440, 1572864);

    // remove profiles
    printf("Removeing profile from Q\r\n");
    for (int idx = 0; idx < 10 ; idx++)
    {
        set_attr.id = SAI_QUEUE_ATTR_BUFFER_PROFILE_ID;
        set_attr.value.oid = SAI_NULL_OBJECT_ID;
        if (q_obj[idx]) {
            ASSERT_EQ (SAI_STATUS_SUCCESS, sai_queue_api_table->set_queue_attribute
                       (q_obj[idx], (const sai_attribute_t *)&set_attr));
        }
    }

    /*  Max resevred size in pool = 4 * sum(tiles reserved size )
     *  Shared Size per tile = ( Pool Size - (Max resevred size in pool)/max tiles.
     *                        Tile 0    Tile 1    Tile 2  Tile 3
     * Reserved               0MB       0MB       0MB     0MB
     * shared size            2.5MB     2.5MB     2.5MB   2.5MB
     *
     */

    sai_qos_xpe_buffer_verify (pool_id, pool_shadow_id_list, 10485760, 10485760, 0, 0, 0, 0);
    printf("After Removeing profiles from Q\r\n");

    ASSERT_EQ (SAI_STATUS_SUCCESS,
               sai_buffer_api_table->remove_buffer_profile (profile_id));

    ASSERT_EQ (SAI_STATUS_SUCCESS,
               sai_buffer_api_table->remove_buffer_profile (profile_id_1));

    printf("After Removeing profiles\r\n");
    sai_buffer_api_table->remove_buffer_pool (pool_id);
}
