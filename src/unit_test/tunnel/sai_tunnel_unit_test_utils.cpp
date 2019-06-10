/************************************************************************
* LEGALESE:   "Copyright (c) 2016, Dell Inc. All rights reserved."
*
* This source code is confidential, proprietary, and contains trade
* secrets that are the sole property of Dell Inc.
* Copy and/or distribution of this source code or disassembly or reverse
* engineering of the resultant object code are strictly forbidden without
* the written consent of Dell Inc.
*
************************************************************************/
/**
* @file  sai_tunnel_unit_test_utils.cpp
*
* @brief This file contains utility and helper function definitions for
*        testing the SAI Tunnel functionalities.
*
*************************************************************************/

#include "gtest/gtest.h"

#include "sai_tunnel_unit_test.h"
#include "sai_l3_unit_test_utils.h"
#include <map>

extern "C" {
#include "sai.h"
#include "saitypes.h"
#include "saistatus.h"
#include "saiswitch.h"
#include "saitunnel.h"
#include "saibridge.h"
#include "saihostifextensions.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdarg.h>
#include <arpa/inet.h>
#include <inttypes.h>
}


/* Definition for the data members */
sai_switch_api_t* saiTunnelTest::p_sai_switch_api_tbl = NULL;
sai_port_api_t*   saiTunnelTest::p_sai_port_api_tbl = NULL;
sai_tunnel_api_t* saiTunnelTest::p_sai_tunnel_api_tbl = NULL;
sai_bridge_api_t* saiTunnelTest::p_sai_bridge_api_tbl = NULL;
sai_fdb_api_t*    saiTunnelTest::p_sai_fdb_api_tbl = NULL;
sai_l2mc_group_api_t* saiTunnelTest::p_sai_l2mc_group_api_tbl = NULL;
sai_hostif_api_t*     saiTunnelTest::p_sai_hostif_api_tbl = NULL;
sai_lag_api_t*    saiTunnelTest::p_sai_lag_api_tbl = NULL;
sai_object_id_t saiTunnelTest::test_vlan_obj_id = SAI_NULL_OBJECT_ID;
sai_object_id_t saiTunnelTest::dflt_vr_id = SAI_NULL_OBJECT_ID;
sai_object_id_t saiTunnelTest::dflt_overlay_vr_id = SAI_NULL_OBJECT_ID;
sai_object_id_t saiTunnelTest::dflt_port_rif_id = SAI_NULL_OBJECT_ID;
sai_object_id_t saiTunnelTest::dflt_vlan_rif_id = SAI_NULL_OBJECT_ID;
sai_object_id_t saiTunnelTest::dflt_overlay_rif_id = SAI_NULL_OBJECT_ID;
sai_object_id_t saiTunnelTest::dflt_underlay_rif_id = SAI_NULL_OBJECT_ID;
sai_object_id_t saiTunnelTest::dflt_underlay_port_nh_id = SAI_NULL_OBJECT_ID;
sai_object_id_t saiTunnelTest::dflt_underlay_vlan_nh_id = SAI_NULL_OBJECT_ID;
sai_object_id_t saiTunnelTest::dflt_underlay_nhg_id = SAI_NULL_OBJECT_ID;
const char *  saiTunnelTest::nh_ip_str_1 = "50.1.1.1";
const char *  saiTunnelTest::nh_ip_str_2 = "60.1.1.1";
unsigned int saiTunnelTest::port_count = 0;
sai_object_id_t saiTunnelTest::port_list[SAI_TEST_MAX_PORTS] = {0};
unsigned int saiTunnelTest::bridge_port_count = 0;
sai_object_id_t saiTunnelTest::bridge_port_list[SAI_TEST_MAX_PORTS] = {0};
sai_object_id_t saiTunnelTest::default_bridge_id = 0;
static sai_object_id_t switch_id = 0;
static sai_kv_pair_t kvpair;

#define TUNNEL_PRINT(msg, ...) \
    printf(msg"\n", ##__VA_ARGS__)

/*
 * Stubs for Callback functions to be passed from adapter host/application.
 */
#ifdef UNREFERENCED_PARAMETER
#elif defined(__GNUC__)
#define UNREFERENCED_PARAMETER(P)   (void)(P)
#else
#define UNREFERENCED_PARAMETER(P)   (P)
#endif

static inline void sai_port_state_evt_callback (
                                     uint32_t count,
                                     sai_port_oper_status_notification_t *data)
{
    UNREFERENCED_PARAMETER(count);
    UNREFERENCED_PARAMETER(data);
}

static inline void sai_fdb_evt_callback (uint32_t count,
                                         sai_fdb_event_notification_data_t *data)
{
    UNREFERENCED_PARAMETER(count);
    UNREFERENCED_PARAMETER(data);
}

static inline void sai_switch_operstate_callback (sai_switch_oper_status_t
                                                  switchstate)
{
    UNREFERENCED_PARAMETER(switchstate);
}

static inline void sai_packet_event_callback (const void *buffer,
                                              sai_size_t buffer_size,
                                              uint32_t attr_count,
                                              const sai_attribute_t *attr_list)
{
    UNREFERENCED_PARAMETER(buffer);
    UNREFERENCED_PARAMETER(buffer_size);
    UNREFERENCED_PARAMETER(attr_count);
    UNREFERENCED_PARAMETER(attr_list);
}

static inline void sai_switch_shutdown_callback (void)
{
}

const char* profile_get_value(sai_switch_profile_id_t profile_id,
                              const char* variable)
{
    kv_iter kviter;
    std::string key;

    if (variable ==  NULL)
        return NULL;

    key = variable;

    kviter = kvpair.find(key);
    if (kviter == kvpair.end()) {
        return NULL;
    }
    return kviter->second.c_str();
}

int profile_get_next_value(sai_switch_profile_id_t profile_id,
                           const char** variable,
                           const char** value)
{
    kv_iter kviter;
    std::string key;

    if (variable == NULL || value == NULL) {
        return -1;
    }
    if (*variable == NULL) {
            if (kvpair.size() < 1) {
            return -1;
        }
        kviter = kvpair.begin();
    } else {
        key = *variable;
        kviter = kvpair.find(key);
        if (kviter == kvpair.end()) {
            return -1;
        }
        kviter++;
        if (kviter == kvpair.end()) {
            return -1;
        }
    }
    *variable = (char *)kviter->first.c_str();
    *value = (char *)kviter->second.c_str();
    return 0;
}

void kv_populate(void)
{
    kvpair["SAI_KEY_VXLAN_RIOT_ENABLE"] = "1";
    kvpair["SAI_KEY_VXLAN_NUM_OVERLAY_NEXT_HOPS"] = "8192";
    kvpair["SAI_KEY_VXLAN_NUM_OVERLAY_RIFS"] = "4096";
    kvpair["SAI_KEY_VXLAN_OVERLAY_ECMP_ENABLE"] = "1";
}

sai_kv_pair_t kvpair_get(void)
{
    return kvpair;
}

/* SAI switch initialization */
void saiTunnelTest::SetUpTestCase (void)
{
    sai_status_t              status;
    sai_attribute_t attr;
    uint8_t  sai_mac [6] = {0x00,0x00,0x00,0xaa,0xbb,0xcc};

    sai_service_method_table_t  sai_service_method_table;
    kv_populate();

    sai_service_method_table.profile_get_value = profile_get_value;
    sai_service_method_table.profile_get_next_value = profile_get_next_value;

    ASSERT_EQ(SAI_STATUS_SUCCESS,sai_api_initialize(0, &sai_service_method_table));


    /*
     * Query and populate the SAI Switch API Table.
     */
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_api_query
               (SAI_API_SWITCH, (static_cast<void**>
                                 (static_cast<void*>(&p_sai_switch_api_tbl)))));

    ASSERT_TRUE (p_sai_switch_api_tbl != NULL);

    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_api_query
            (SAI_API_BRIDGE, (static_cast<void**>
                              (static_cast<void*>(&p_sai_bridge_api_tbl)))));

    ASSERT_TRUE (p_sai_bridge_api_tbl != NULL);

    ASSERT_EQ(NULL,sai_api_query(SAI_API_HOSTIF,
           (static_cast<void**>(static_cast<void*>(&p_sai_hostif_api_tbl)))));
    ASSERT_TRUE(p_sai_hostif_api_tbl != NULL);

    EXPECT_TRUE(p_sai_hostif_api_tbl->send_hostif_packet !=NULL);

    /*
     * Switch Initialization.
     */

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

    ASSERT_TRUE(p_sai_switch_api_tbl->create_switch != NULL);

    EXPECT_EQ (SAI_STATUS_SUCCESS,
               (p_sai_switch_api_tbl->create_switch (&switch_id , attr_count,
                                                         sai_attr_set)));

    sai_test_port_api_table_get ();

    /* Query the Tunnel API method tables */
    sai_test_tunnel_api_table_get ();

    sai_test_bridge_api_table_get ();

    sai_test_fdb_api_table_get ();

    sai_test_l2mc_group_api_table_get();

    sai_test_lag_api_table_get();

    /* Query the L3 API method table */
    saiL3Test::SetUpL3ApiQuery ();

    /* Setup router mac*/
    attr.id = SAI_SWITCH_ATTR_SRC_MAC_ADDRESS;

    memcpy (attr.value.mac, sai_mac, sizeof (sai_mac_t));
    status = p_sai_switch_api_tbl->set_switch_attribute
            (switch_id,(const sai_attribute_t *)&attr);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    /* Set up the default VLAN OBJ ID for L3 API */
    {
        sai_attribute_t attr;
        sai_switch_api_t* lp_sai_switch_api_tbl = NULL;

        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_api_query
                (SAI_API_SWITCH, (static_cast<void**>
                                  (static_cast<void*>(&lp_sai_switch_api_tbl)))));

        ASSERT_TRUE (lp_sai_switch_api_tbl != NULL);
        memset (&attr, 0, sizeof (attr));

        attr.id = SAI_SWITCH_ATTR_DEFAULT_VLAN_ID;
        ASSERT_EQ (SAI_STATUS_SUCCESS,
                lp_sai_switch_api_tbl->get_switch_attribute (switch_id, 1, &attr));

        printf ("Default VLAN obj ID is %lu.\r\n", attr.value.oid);
        saiL3Test ::sai_l3_default_vlan_obj_id_set(attr.value.oid);
    }

    /* Get the switch port count and port list */
    memset (&attr, 0, sizeof (attr));

    attr.id = SAI_SWITCH_ATTR_PORT_LIST;
    attr.value.objlist.count = SAI_TEST_MAX_PORTS;
    attr.value.objlist.list  = port_list;

    status = p_sai_switch_api_tbl->get_switch_attribute (switch_id,1, &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    port_count = attr.value.objlist.count;

    uint32_t port_idx = 0;
    for(port_idx = 0; port_idx < port_count; port_idx++) {

        attr.id = SAI_PORT_ATTR_ADMIN_STATE;
        attr.value.booldata = true;
        status = p_sai_port_api_tbl->set_port_attribute(port_list[port_idx],
                                                        &attr);
        ASSERT_EQ (SAI_STATUS_SUCCESS, status);
    }

    attr.id = SAI_SWITCH_ATTR_DEFAULT_1Q_BRIDGE_ID;

    status = p_sai_switch_api_tbl->get_switch_attribute(switch_id,1,&attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, status);
    default_bridge_id = attr.value.oid;

    attr.id = SAI_BRIDGE_ATTR_PORT_LIST;
    attr.value.objlist.count = SAI_TEST_MAX_PORTS;
    attr.value.objlist.list = bridge_port_list;

    status = p_sai_bridge_api_tbl->get_bridge_attribute(default_bridge_id, 1, &attr);
    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    bridge_port_count = attr.value.objlist.count;

    TUNNEL_PRINT ("Switch port count is %u.", port_count);
    TUNNEL_PRINT ("Bridge port count is %u.", bridge_port_count);

    ASSERT_TRUE (port_count != 0);

    uint32_t bridge_port_idx = 0;

    for(bridge_port_idx = 0; bridge_port_idx < bridge_port_count; bridge_port_idx++) {

        attr.id =  SAI_BRIDGE_PORT_ATTR_INGRESS_FILTERING;
        attr.value.booldata = true;

        status = p_sai_bridge_api_tbl->set_bridge_port_attribute(
                                          bridge_port_list[bridge_port_idx],
                                          &attr);
        ASSERT_EQ (SAI_STATUS_SUCCESS, status);

        attr.id =  SAI_BRIDGE_PORT_ATTR_EGRESS_FILTERING;
        attr.value.booldata = true;

        status = p_sai_bridge_api_tbl->set_bridge_port_attribute(
                                          bridge_port_list[bridge_port_idx],
                                          &attr);
        ASSERT_EQ (SAI_STATUS_SUCCESS, status);

        attr.id =  SAI_BRIDGE_PORT_ATTR_ADMIN_STATE;
        attr.value.booldata = true;

        status = p_sai_bridge_api_tbl->set_bridge_port_attribute(
                                          bridge_port_list[bridge_port_idx],
                                          &attr);
        ASSERT_EQ (SAI_STATUS_SUCCESS, status);
    }


    /* Setup Default Underlay Router objects */
    sai_test_tunnel_underlay_router_setup ();

    /* Use the default VRF for Overlay Virtual Router instance */
    dflt_overlay_vr_id = dflt_vr_id;

    /* Create a default Overlay RIF */
    /* TODO Change this to loopback type */
    status = saiL3Test::sai_test_rif_create (&dflt_overlay_rif_id,
                                             saiL3Test::default_rif_attr_count,
                                             SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                             dflt_overlay_vr_id,
                                             SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                             SAI_ROUTER_INTERFACE_TYPE_PORT,
                                             SAI_ROUTER_INTERFACE_ATTR_PORT_ID,
                                             sai_test_tunnel_port_id_get (1));
    ASSERT_EQ (SAI_STATUS_SUCCESS, status);
}

void saiTunnelTest::TearDownTestCase (void)
{
    sai_status_t      status;

    /* Remove Overlay RIF */
    status = saiL3Test::sai_test_rif_remove (dflt_overlay_rif_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    sai_test_tunnel_underlay_router_tear_down ();
}

void saiTunnelTest::sai_test_tunnel_underlay_router_setup (void)
{
    sai_status_t      status;
    sai_object_id_t   nh_list [4];
    unsigned int      ecmp_count = 0;
    unsigned int      test_port_index_1 = 0;
    unsigned int      test_port_index_2 = 1;
    sai_object_id_t   test_port_id_1;
    sai_object_id_t   test_bridge_port_id_1;
    const char *      mac_str_1 = "00:22:22:22:22:22";
    const char *      mac_str_2 = "00:44:44:44:44:44";
    const unsigned int vr_attr_count = 1;
    const unsigned int underlay_rif_attr_count = 2;

    test_port_id_1 = sai_test_tunnel_port_id_get (test_port_index_1);
    test_bridge_port_id_1 = sai_test_tunnel_bridge_port_id_get (test_port_index_2);

    /* Create Vlan */
    status = saiL3Test::sai_test_vlan_create (&test_vlan_obj_id, test_vlan);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    /* Create a default Virtual Router instance */
    status = saiL3Test::sai_test_vrf_create (&dflt_vr_id, vr_attr_count,
                                             SAI_VIRTUAL_ROUTER_ATTR_SRC_MAC_ADDRESS,
                                             "00:00:00:00:00:b4:b5");

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    /* Create a Port RIF */
    status = saiL3Test::sai_test_rif_create (&dflt_port_rif_id,
                                             saiL3Test::default_rif_attr_count,
                                             SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                             dflt_vr_id,
                                             SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                             SAI_ROUTER_INTERFACE_TYPE_PORT,
                                             SAI_ROUTER_INTERFACE_ATTR_PORT_ID,
                                             test_port_id_1);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    /* Create a VLAN RIF */
    status = saiL3Test::sai_test_rif_create (&dflt_vlan_rif_id,
                                             saiL3Test::default_rif_attr_count,
                                             SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                             dflt_vr_id,
                                             SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                             SAI_ROUTER_INTERFACE_TYPE_VLAN,
                                             SAI_ROUTER_INTERFACE_ATTR_VLAN_ID,
                                             test_vlan);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    /* Create a default Underlay RIF */
    status = saiL3Test::sai_test_rif_create (&dflt_underlay_rif_id, underlay_rif_attr_count,
                                             SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                             dflt_vr_id,
                                             SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                             SAI_ROUTER_INTERFACE_TYPE_LOOPBACK);
    ASSERT_EQ (SAI_STATUS_SUCCESS, status);


    /* Neighbor creation */
    status = saiL3Test::sai_test_neighbor_create (dflt_port_rif_id,
                                                  SAI_IP_ADDR_FAMILY_IPV4,
                                                  nh_ip_str_1,
                                                  saiL3Test::default_neighbor_attr_count,
                                                  SAI_NEIGHBOR_ENTRY_ATTR_DST_MAC_ADDRESS,
                                                  mac_str_1);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    status = saiL3Test::sai_test_neighbor_fdb_entry_create (mac_str_2,
                                                            test_vlan_obj_id, test_bridge_port_id_1);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    status = saiL3Test::sai_test_neighbor_create (dflt_vlan_rif_id,
                                                  SAI_IP_ADDR_FAMILY_IPV4,
                                                  nh_ip_str_2,
                                                  saiL3Test::default_neighbor_attr_count,
                                                  SAI_NEIGHBOR_ENTRY_ATTR_DST_MAC_ADDRESS,
                                                  mac_str_2);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    /* Next-hop Creation */
    status = saiL3Test::sai_test_nexthop_create (&dflt_underlay_port_nh_id,
                                                 saiL3Test::default_nh_attr_count,
                                                 SAI_NEXT_HOP_ATTR_TYPE,
                                                 SAI_NEXT_HOP_TYPE_IP,
                                                 SAI_NEXT_HOP_ATTR_IP,
                                                 SAI_IP_ADDR_FAMILY_IPV4,
                                                 nh_ip_str_1,
                                                 SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
                                                 dflt_port_rif_id);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    status = saiL3Test::sai_test_nexthop_create (&dflt_underlay_vlan_nh_id,
                                                 saiL3Test::default_nh_attr_count,
                                                 SAI_NEXT_HOP_ATTR_TYPE,
                                                 SAI_NEXT_HOP_TYPE_IP,
                                                 SAI_NEXT_HOP_ATTR_IP,
                                                 SAI_IP_ADDR_FAMILY_IPV4,
                                                 nh_ip_str_2,
                                                 SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
                                                 dflt_vlan_rif_id);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    /* Next-hop Group Creation */
    nh_list [ecmp_count] = dflt_underlay_port_nh_id;
    ecmp_count++;
    nh_list [ecmp_count] = dflt_underlay_vlan_nh_id;
    ecmp_count++;

    status = saiL3Test::sai_test_nh_group_create (&dflt_underlay_nhg_id, nh_list,
                                                  saiL3Test::default_nh_group_attr_count,
                                                  SAI_NEXT_HOP_GROUP_ATTR_TYPE,
                                                  SAI_NEXT_HOP_GROUP_TYPE_ECMP,
                                                  SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_MEMBER_LIST,
                                                  ecmp_count);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);
}

void saiTunnelTest::sai_test_tunnel_underlay_router_tear_down (void)
{
    sai_status_t  status;

    /* Remove all Next-hop Groups */
    status = saiL3Test::sai_test_nh_group_remove (dflt_underlay_nhg_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    /* Remove all Next-hops */
    status = saiL3Test::sai_test_nexthop_remove (dflt_underlay_vlan_nh_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    status = saiL3Test::sai_test_nexthop_remove (dflt_underlay_port_nh_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    /* Remove all Neighbors */
    status = saiL3Test::sai_test_neighbor_remove (dflt_port_rif_id,
                                                  SAI_IP_ADDR_FAMILY_IPV4,
                                                  nh_ip_str_1);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    status = saiL3Test::sai_test_neighbor_remove (dflt_vlan_rif_id,
                                                  SAI_IP_ADDR_FAMILY_IPV4,
                                                  nh_ip_str_2);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    /* Remove all RIF */
    status = saiL3Test::sai_test_rif_remove (dflt_port_rif_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    status = saiL3Test::sai_test_rif_remove (dflt_vlan_rif_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    status = saiL3Test::sai_test_rif_remove (dflt_underlay_rif_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    /* Remove VRF */
    status = saiL3Test::sai_test_vrf_remove (dflt_vr_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    /* Remove Vlan */
    status = saiL3Test::sai_test_vlan_remove (test_vlan_obj_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);
}

sai_object_id_t saiTunnelTest::sai_test_tunnel_port_id_get (uint32_t port_index)
{
    if(port_index >= port_count) {
        return 0;
    }

    return port_list [port_index];
}
/*Returns the bridge port id when the port index is passed*/
sai_object_id_t saiTunnelTest::sai_test_tunnel_bridge_port_id_get (uint32_t port_index)
{
    if(port_index >= bridge_port_count) {
        return 0;
    }

    return bridge_port_list [port_index];
}

/* SAI Tunnel API Query */
void saiTunnelTest::sai_test_port_api_table_get (void)
{
    /*
     * Query and populate the SAI Tunnel API Table.
     */
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_api_query
               (SAI_API_PORT, (static_cast<void**>
                                 (static_cast<void*>(&p_sai_port_api_tbl)))));

    ASSERT_TRUE (p_sai_port_api_tbl != NULL);
}

/* SAI Tunnel API Query */
void saiTunnelTest::sai_test_tunnel_api_table_get (void)
{
    /*
     * Query and populate the SAI Tunnel API Table.
     */
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_api_query
               (SAI_API_TUNNEL, (static_cast<void**>
                                (static_cast<void*>(&p_sai_tunnel_api_tbl)))));

    ASSERT_TRUE (p_sai_tunnel_api_tbl != NULL);
}

/* SAI Bridge API Query */
void saiTunnelTest::sai_test_bridge_api_table_get (void)
{
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_api_query
               (SAI_API_BRIDGE, (static_cast<void**>
                                 (static_cast<void*>(&p_sai_bridge_api_tbl)))));

    ASSERT_TRUE (p_sai_bridge_api_tbl != NULL);
}

/* SAI FDB API Query */
void saiTunnelTest::sai_test_fdb_api_table_get (void)
{
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_api_query
               (SAI_API_FDB, (static_cast<void**>
                                 (static_cast<void*>(&p_sai_fdb_api_tbl)))));

    ASSERT_TRUE (p_sai_fdb_api_tbl != NULL);
}

/* SAI L2MC Group API Query */
void saiTunnelTest::sai_test_l2mc_group_api_table_get (void)
{
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_api_query
               (SAI_API_L2MC_GROUP, (static_cast<void**>
                                     (static_cast<void*>(&p_sai_l2mc_group_api_tbl)))));

     ASSERT_TRUE (p_sai_l2mc_group_api_tbl != NULL);
}

/* SAI LAG API Query */
void saiTunnelTest::sai_test_lag_api_table_get (void)
{
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_api_query
               (SAI_API_LAG, (static_cast<void**>
                                     (static_cast<void*>(&p_sai_lag_api_tbl)))));

    ASSERT_TRUE (p_sai_lag_api_tbl != NULL);
}

void saiTunnelTest::sai_test_tunnel_attr_value_cleanup(unsigned int attr_count,
                                                       sai_attribute_t *p_attr_list)
{
    unsigned int     index = 0;
    sai_attribute_t *p_attr = NULL;

    for (index = 0; index < attr_count; index++) {

        p_attr = &p_attr_list [index];

        if((p_attr->id == SAI_TUNNEL_ATTR_ENCAP_MAPPERS) ||
           (p_attr->id == SAI_TUNNEL_ATTR_DECAP_MAPPERS)) {
            free(p_attr->value.objlist.list);
        }
    }
}

void saiTunnelTest::sai_test_tunnel_attr_value_fill (unsigned int attr_count,
                                                     va_list *p_varg_list,
                                                     sai_attribute_t *p_attr_list)
{
    unsigned int     af;
    unsigned int     index;
    unsigned int     object_count;
    const char      *p_ip_str;
    sai_attribute_t *p_attr;

    for (index = 0; index < attr_count; index++) {

        p_attr = &p_attr_list [index];

        p_attr->id = va_arg ((*p_varg_list), unsigned int);

        switch (p_attr->id) {

            case SAI_TUNNEL_ATTR_ENCAP_SRC_IP:
                af       = va_arg ((*p_varg_list), unsigned int);
                p_ip_str = va_arg ((*p_varg_list), const char *);

                if (af == SAI_IP_ADDR_FAMILY_IPV4) {

                    inet_pton (AF_INET, p_ip_str,
                               (void *)&p_attr->value.ipaddr.addr.ip4);
                } else {

                    inet_pton (AF_INET6, p_ip_str,
                               (void *)&p_attr->value.ipaddr.addr.ip6);
                }

                p_attr->value.ipaddr.addr_family = (sai_ip_addr_family_t) af;
                TUNNEL_PRINT ("Attr Index: %d, Set Encap SIP Addr family: %d, "
                              "IP Addr: %s.", index, af, p_ip_str);
                break;

            case SAI_TUNNEL_ATTR_UNDERLAY_INTERFACE:
            case SAI_TUNNEL_ATTR_OVERLAY_INTERFACE:
                p_attr->value.oid = va_arg ((*p_varg_list), unsigned long);
                TUNNEL_PRINT ("Attr Index: %d, Set RIF Id value: 0x%" PRIx64 ".",
                              index, p_attr->value.oid);
                break;

            case SAI_TUNNEL_ATTR_TYPE:
            case SAI_TUNNEL_ATTR_ENCAP_TTL_MODE:
            case SAI_TUNNEL_ATTR_ENCAP_DSCP_MODE:
            case SAI_TUNNEL_ATTR_ENCAP_ECN_MODE:
            case SAI_TUNNEL_ATTR_DECAP_TTL_MODE:
            case SAI_TUNNEL_ATTR_DECAP_DSCP_MODE:
            case SAI_TUNNEL_ATTR_DECAP_ECN_MODE:
                p_attr->value.s32 = va_arg ((*p_varg_list), unsigned int);
                break;

            case SAI_TUNNEL_ATTR_ENCAP_TTL_VAL:
            case SAI_TUNNEL_ATTR_ENCAP_DSCP_VAL:
                p_attr->value.u8 = va_arg ((*p_varg_list), unsigned int);
                break;

            case SAI_TUNNEL_ATTR_ENCAP_MAPPERS:
            case SAI_TUNNEL_ATTR_DECAP_MAPPERS:
                object_count = va_arg ((*p_varg_list), unsigned int);
                p_attr->value.objlist.list = (sai_object_id_t*)calloc(
                                             object_count, sizeof(sai_object_id_t));

                if (!p_attr->value.objlist.list) {
                    TUNNEL_PRINT("Failed to allocate memory for mappers");
                    p_attr->value.objlist.count = 0;
                    return;
                }
                p_attr->value.objlist.count = object_count;

                for (object_count = 0; object_count <
                     p_attr->value.objlist.count; object_count++) {
                     p_attr->value.objlist.list[object_count] =
                                    va_arg ((*p_varg_list), sai_object_id_t);
                }
                break;

            default:
                p_attr->value.u64 = va_arg ((*p_varg_list), unsigned int);
                TUNNEL_PRINT ("Attr Index: %d, Set Attr Id: %d to value: %ld.",
                              index, p_attr->id, p_attr->value.u64);
                break;
        }
    }
}

void saiTunnelTest::sai_test_tunnel_map_attr_value_fill (unsigned int attr_count,
                                                         va_list *p_varg_list,
                                                         sai_attribute_t *p_attr_list)
{
    unsigned int     index = 0;
    sai_attribute_t *p_attr = NULL;

    for (index = 0; index < attr_count; index++) {

        p_attr = &p_attr_list [index];

        p_attr->id = va_arg ((*p_varg_list), unsigned int);

        switch (p_attr->id) {

            case SAI_TUNNEL_MAP_ATTR_TYPE:
                p_attr->value.s32 = va_arg ((*p_varg_list), int);
                break;

            case SAI_TUNNEL_MAP_ATTR_ENTRY_LIST:
                TUNNEL_PRINT("Read only attribute is provided");
                break;

            default:
                TUNNEL_PRINT("Unknown tunnel map attribute id %d",p_attr->id);
        }
    }
}

void saiTunnelTest::sai_test_tunnel_map_entry_attr_value_fill (unsigned int attr_count,
                                                               va_list *p_varg_list,
                                                               sai_attribute_t *p_attr_list)
{
    unsigned int     index = 0;
    sai_attribute_t *p_attr = NULL;

    for (index = 0; index < attr_count; index++) {

        p_attr = &p_attr_list [index];

        p_attr->id = va_arg ((*p_varg_list), unsigned int);

        switch (p_attr->id) {

            case SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE:
                p_attr->value.s32 = va_arg ((*p_varg_list), int);
                break;

            case SAI_TUNNEL_MAP_ENTRY_ATTR_OECN_KEY:
            case SAI_TUNNEL_MAP_ENTRY_ATTR_OECN_VALUE:
            case SAI_TUNNEL_MAP_ENTRY_ATTR_UECN_KEY:
            case SAI_TUNNEL_MAP_ENTRY_ATTR_UECN_VALUE:
                p_attr->value.u8 = va_arg ((*p_varg_list), unsigned int);;
                break;

            case SAI_TUNNEL_MAP_ENTRY_ATTR_VLAN_ID_KEY:
            case SAI_TUNNEL_MAP_ENTRY_ATTR_VLAN_ID_VALUE:
                p_attr->value.u16 = va_arg ((*p_varg_list), unsigned int);
                break;

            case SAI_TUNNEL_MAP_ENTRY_ATTR_VNI_ID_KEY:
            case SAI_TUNNEL_MAP_ENTRY_ATTR_VNI_ID_VALUE:
                p_attr->value.u32 = va_arg ((*p_varg_list), unsigned int);
                break;

            case SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP:
            case SAI_TUNNEL_MAP_ENTRY_ATTR_BRIDGE_ID_KEY:
            case SAI_TUNNEL_MAP_ENTRY_ATTR_BRIDGE_ID_VALUE:
                p_attr->value.oid = va_arg ((*p_varg_list), sai_object_id_t);
                break;

            default:
                TUNNEL_PRINT("Unknown tunnel map entry attribute id %d",p_attr->id);

        }
    }
}

void saiTunnelTest::sai_test_bridge_attr_value_fill (unsigned int attr_count,
                                                     va_list *p_varg_list,
                                                     sai_attribute_t *p_attr_list)
{
    unsigned int     index;
    sai_attribute_t *p_attr;

    for (index = 0; index < attr_count; index++) {

        p_attr = &p_attr_list [index];

        p_attr->id = va_arg ((*p_varg_list), unsigned int);

        switch (p_attr->id) {

            case SAI_BRIDGE_ATTR_TYPE:
                p_attr->value.s32 = va_arg ((*p_varg_list), int);
                break;

            case SAI_BRIDGE_ATTR_PORT_LIST:
                break;

            case SAI_BRIDGE_ATTR_MAX_LEARNED_ADDRESSES:
                p_attr->value.u32 = va_arg ((*p_varg_list), unsigned int);
                break;

            case SAI_BRIDGE_ATTR_LEARN_DISABLE:
                p_attr->value.booldata = va_arg ((*p_varg_list), unsigned int);
                break;

            case SAI_BRIDGE_ATTR_UNKNOWN_UNICAST_FLOOD_GROUP:
            case SAI_BRIDGE_ATTR_UNKNOWN_MULTICAST_FLOOD_GROUP:
            case SAI_BRIDGE_ATTR_BROADCAST_FLOOD_GROUP:
                p_attr->value.oid = va_arg ((*p_varg_list), unsigned long);
                break;

            case SAI_BRIDGE_ATTR_UNKNOWN_UNICAST_FLOOD_CONTROL_TYPE:
            case SAI_BRIDGE_ATTR_UNKNOWN_MULTICAST_FLOOD_CONTROL_TYPE:
            case SAI_BRIDGE_ATTR_BROADCAST_FLOOD_CONTROL_TYPE:
                p_attr->value.s32 = va_arg ((*p_varg_list), int);
                break;

            default:
                TUNNEL_PRINT("Unknown bridge attribute id %d",p_attr->id);
        }
    }
}

void saiTunnelTest::sai_test_bridge_port_attr_value_fill (unsigned int attr_count,
                                                          va_list *p_varg_list,
                                                          sai_attribute_t *p_attr_list)
{
    unsigned int     index;
    sai_attribute_t *p_attr;

    for (index = 0; index < attr_count; index++) {

        p_attr = &p_attr_list [index];

        p_attr->id = va_arg ((*p_varg_list), unsigned int);

        switch (p_attr->id) {

            case SAI_BRIDGE_PORT_ATTR_TYPE:
                p_attr->value.s32 = va_arg ((*p_varg_list), int);
                break;

            case SAI_BRIDGE_PORT_ATTR_PORT_ID:
                p_attr->value.oid = va_arg ((*p_varg_list), unsigned long);
                break;

            case SAI_BRIDGE_PORT_ATTR_VLAN_ID:
                p_attr->value.u16 = va_arg ((*p_varg_list), unsigned int);
                break;

            case SAI_BRIDGE_PORT_ATTR_RIF_ID:
                p_attr->value.oid = va_arg ((*p_varg_list), unsigned long);
                break;

            case SAI_BRIDGE_PORT_ATTR_TUNNEL_ID:
                p_attr->value.oid = va_arg ((*p_varg_list), unsigned long);
                break;

            case SAI_BRIDGE_PORT_ATTR_BRIDGE_ID:
                p_attr->value.oid = va_arg ((*p_varg_list), unsigned long);
                break;

            case SAI_BRIDGE_PORT_ATTR_FDB_LEARNING_MODE:
                p_attr->value.s32 = va_arg ((*p_varg_list), int);
                break;

            case SAI_BRIDGE_PORT_ATTR_MAX_LEARNED_ADDRESSES:
                p_attr->value.s32 = va_arg ((*p_varg_list), unsigned int);
                break;

            case SAI_BRIDGE_PORT_ATTR_FDB_LEARNING_LIMIT_VIOLATION_PACKET_ACTION:
                p_attr->value.s32 = va_arg ((*p_varg_list), unsigned int);
                break;

            case SAI_BRIDGE_PORT_ATTR_ADMIN_STATE:
                p_attr->value.booldata = va_arg ((*p_varg_list), unsigned int);
                break;

            case SAI_BRIDGE_PORT_ATTR_INGRESS_FILTERING:
                p_attr->value.booldata = va_arg ((*p_varg_list), unsigned int);
                break;

            default:
                TUNNEL_PRINT("Unknown bridge port attribute id %d",p_attr->id);
        }
    }
}

sai_status_t saiTunnelTest::sai_test_bridge_create (sai_object_id_t *bridge_id,
                                                    unsigned int attr_count, ...)
{
    sai_status_t     status = SAI_STATUS_FAILURE;
    va_list          ap;
    sai_attribute_t  attr_list [max_bridge_obj_attr_count];

    if (attr_count > max_bridge_obj_attr_count) {

        TUNNEL_PRINT ("%s(): Attr count %u is greater than Bridge attr count "
                      "%u.", __FUNCTION__, attr_count, max_bridge_obj_attr_count);

        return status;
    }

    memset (attr_list, 0, sizeof (attr_list));

    TUNNEL_PRINT ("Testing Bridge object Creation with attribute count: %u.",
                  attr_count);

    va_start (ap, attr_count);

    sai_test_bridge_attr_value_fill (attr_count, &ap, attr_list);

    va_end (ap);

    status = p_sai_bridge_api_tbl->create_bridge (bridge_id, switch_id, attr_count,
                                                  attr_list);

    if (status != SAI_STATUS_SUCCESS) {
        TUNNEL_PRINT ("SAI Bridge Creation API failed with error: %d.", status);
    } else {
        TUNNEL_PRINT ("SAI Bridge Creation API success, Bridge object Id: "
                      "0x%" PRIx64 ".", *bridge_id);
    }

    return status;
}

sai_status_t saiTunnelTest::sai_test_bridge_remove (sai_object_id_t bridge_id)
{
    sai_status_t       status = SAI_STATUS_FAILURE;

    TUNNEL_PRINT ("Testing Bridge object Id: 0x%" PRIx64 " remove.", bridge_id);

    status = p_sai_bridge_api_tbl->remove_bridge (bridge_id);

    if (status != SAI_STATUS_SUCCESS) {
        TUNNEL_PRINT ("SAI Bridge remove API failed with error: %d.", status);
    } else {
        TUNNEL_PRINT ("SAI Bridge remove API success.");
    }

    return status;
}

sai_status_t saiTunnelTest::sai_test_bridge_set (sai_object_id_t bridge_id,
                                                 ...)
{
    sai_status_t     status = SAI_STATUS_FAILURE;
    unsigned int     attr_count = 1;
    va_list          ap;
    sai_attribute_t  attr;

    TUNNEL_PRINT ("Testing Bridge object set");

    va_start (ap, bridge_id);

    sai_test_bridge_attr_value_fill (attr_count, &ap, &attr);

    va_end (ap);

    status = p_sai_bridge_api_tbl->set_bridge_attribute (bridge_id, &attr);

    if (status != SAI_STATUS_SUCCESS) {
        TUNNEL_PRINT ("SAI Bridge set API failed with error: %d.", status);
    } else {
        TUNNEL_PRINT ("SAI Bridge set API success, Bridge object Id: "
                      "0x%" PRIx64 ".", bridge_id);
    }

    return status;
}

sai_status_t saiTunnelTest::sai_test_bridge_port_create (sai_object_id_t *bridge_port_id,
                                                         unsigned int attr_count, ...)
{
    sai_status_t     status = SAI_STATUS_FAILURE;
    va_list          ap;
    sai_attribute_t  attr_list [max_bridge_port_obj_attr_count];

    if (attr_count > max_bridge_port_obj_attr_count) {

        TUNNEL_PRINT ("%s(): Attr count %u is greater than Bridge port attr count "
                      "%u.", __FUNCTION__, attr_count, max_bridge_port_obj_attr_count);

        return status;
    }

    memset (attr_list, 0, sizeof (attr_list));

    TUNNEL_PRINT ("Testing Bridge port object creation with attribute count: %u.",
                  attr_count);

    va_start (ap, attr_count);

    sai_test_bridge_port_attr_value_fill (attr_count, &ap, attr_list);

    va_end (ap);

    status = p_sai_bridge_api_tbl->create_bridge_port (bridge_port_id, switch_id, attr_count,
                                                       attr_list);

    if (status != SAI_STATUS_SUCCESS) {
        TUNNEL_PRINT ("SAI Bridge port creation API failed with error: %d.", status);
    } else {
        TUNNEL_PRINT ("SAI Bridge port creation successful, Bridge port object Id: "
                      "0x%" PRIx64 ".", *bridge_port_id);
    }

    return status;
}

sai_status_t saiTunnelTest::sai_test_bridge_port_remove (sai_object_id_t bridge_port_id)
{
    sai_status_t       status;

    TUNNEL_PRINT ("Testing Bridge port object Id: 0x%" PRIx64 " remove.", bridge_port_id);

    status = p_sai_bridge_api_tbl->remove_bridge_port (bridge_port_id);

    if (status != SAI_STATUS_SUCCESS) {
        TUNNEL_PRINT ("SAI Bridge port remove API failed with error: %d.", status);
    } else {
        TUNNEL_PRINT ("SAI Bridge port remove API success.");
    }

    return status;
}

sai_status_t saiTunnelTest::sai_test_bridge_port_set (sai_object_id_t bridge_port_id, ...)
{
    sai_status_t     status = SAI_STATUS_FAILURE;
    va_list          ap;
    sai_attribute_t  attr;
    unsigned int     attr_count = 1;

    memset (&attr, 0, sizeof (attr));

    TUNNEL_PRINT ("Testing Bridge port object set");

    va_start (ap, bridge_port_id);

    sai_test_bridge_port_attr_value_fill (attr_count, &ap, &attr);

    va_end (ap);

    status = p_sai_bridge_api_tbl->set_bridge_port_attribute (bridge_port_id, &attr);

    if (status != SAI_STATUS_SUCCESS) {
        TUNNEL_PRINT ("SAI Bridge port set API failed for Bridge Port 0x%" PRIx64
                      "with error: %d.",bridge_port_id, status);
    } else {
        TUNNEL_PRINT ("SAI Bridge port set API success for Bridge Port object Id: "
                      "0x%" PRIx64 ".", bridge_port_id);
    }

    return status;
}

void saiTunnelTest::sai_test_bridge_port_stats_type_fill (unsigned int stats_count,
                                                     va_list *p_varg_list,
                                                     sai_bridge_port_stat_t *p_tunnel_stats_list)
{
    unsigned int counter_idx = 0;

    for(counter_idx = 0; counter_idx < stats_count; counter_idx++) {
        p_tunnel_stats_list[counter_idx] = (sai_bridge_port_stat_t) va_arg ((*p_varg_list),
                                                                       unsigned int);

    }
}

sai_status_t saiTunnelTest::sai_test_bridge_port_stats_get (sai_object_id_t bridge_port_obj_id,
                                                       unsigned int stats_count, ...)
{
    sai_status_t      status = SAI_STATUS_FAILURE;
    va_list           ap;
    sai_bridge_port_stat_t bridge_stats_list [max_tunnel_stats_count];
    uint32_t          num_counters = 0;
    uint32_t          counter_id = 0;
    uint64_t          stats_val [max_tunnel_stats_count];

    if (stats_count > max_tunnel_stats_count) {

        TUNNEL_PRINT ("%s(): Stats count %u is greater than Max tunnel stats count "
                      "%u.", __FUNCTION__, stats_count, max_tunnel_stats_count);

        return status;
    }

    memset (bridge_stats_list, 0, sizeof (bridge_stats_list));
    memset (stats_val, 0, sizeof (stats_val));

    TUNNEL_PRINT ("Testing bridge port stats get for bridge id 0x%" PRIx64 " "
                  "with stats count: %u.", bridge_port_obj_id, stats_count);

    va_start (ap, stats_count);

    sai_test_bridge_port_stats_type_fill (stats_count, &ap, bridge_stats_list);

    va_end (ap);

    num_counters = stats_count;

    status = p_sai_bridge_api_tbl->get_bridge_port_stats (bridge_port_obj_id, num_counters,
                                                     &bridge_stats_list[0],
                                                     &stats_val[0]);

    if (status != SAI_STATUS_SUCCESS) {
        TUNNEL_PRINT ("SAI Bridge port stats get failed for 0x%" PRIx64 " "
                      "with error: %d.", bridge_port_obj_id, status);
    } else {

        for(counter_id = 0; counter_id < num_counters; counter_id++) {
            TUNNEL_PRINT ("Stat id = %d stat value %" PRIu64 "",bridge_stats_list[counter_id],
                          stats_val[counter_id]);
        }
    }

    return status;
}

sai_status_t saiTunnelTest::sai_test_bridge_port_stats_clear (sai_object_id_t bridge_port_obj_id,
                                                       unsigned int stats_count, ...)
{
    sai_status_t      status = SAI_STATUS_FAILURE;
    va_list           ap;
    sai_bridge_port_stat_t bridge_stats_list [max_tunnel_stats_count];
    uint32_t          num_counters = 0;

    if (stats_count > max_tunnel_stats_count) {

        TUNNEL_PRINT ("%s(): Stats count %u is greater than Max tunnel stats count "
                      "%u.", __FUNCTION__, stats_count, max_tunnel_stats_count);

        return status;
    }

    memset (bridge_stats_list, 0, sizeof (bridge_stats_list));

    TUNNEL_PRINT ("Testing bridge port stats clear for bridge id 0x%" PRIx64 " "
                  "with stats count: %u.", bridge_port_obj_id, stats_count);

    va_start (ap, stats_count);

    sai_test_bridge_port_stats_type_fill (stats_count, &ap, bridge_stats_list);

    va_end (ap);

    num_counters = stats_count;

    status = p_sai_bridge_api_tbl->clear_bridge_port_stats (bridge_port_obj_id, num_counters,
                                                     &bridge_stats_list[0]);

    if (status != SAI_STATUS_SUCCESS) {
        TUNNEL_PRINT ("SAI Bridge port stats clear failed for bridge id 0x%" PRIx64 " "
                      "with error: %d.", bridge_port_obj_id, status);
    } else {
        TUNNEL_PRINT ("SAI Bridge port stats clear passed for bridge id 0x%" PRIx64 " "
                      "with error: %d.", bridge_port_obj_id, status);

        }

    return status;
}
sai_status_t saiTunnelTest::sai_test_tunnel_map_create (
                                               sai_object_id_t *tunnel_map_id,
                                               unsigned int attr_count, ...)
{
    sai_status_t     status = SAI_STATUS_FAILURE;
    va_list          ap;
    sai_attribute_t  attr_list [max_tunnel_map_obj_attr_count];

    if (attr_count > max_tunnel_map_obj_attr_count) {

        TUNNEL_PRINT ("%s(): Attr count %u is greater than Tunnel map attr count "
                      "%u.", __FUNCTION__, attr_count, max_tunnel_map_obj_attr_count);

        return status;
    }

    memset (attr_list, 0, sizeof (attr_list));

    TUNNEL_PRINT ("Testing Tunnel map object Creation with attribute count: %u.",
                  attr_count);

    va_start (ap, attr_count);

    sai_test_tunnel_map_attr_value_fill (attr_count, &ap, attr_list);

    va_end (ap);

    status = p_sai_tunnel_api_tbl->create_tunnel_map (tunnel_map_id, switch_id, attr_count,
                                                      attr_list);

    if (status != SAI_STATUS_SUCCESS) {
        TUNNEL_PRINT ("SAI Tunnel map creation API failed with error: %d.", status);
    } else {
        TUNNEL_PRINT ("SAI Tunnel map creation API success, Tunnel map object Id: "
                      "0x%" PRIx64 ".", *tunnel_map_id);
    }

    return status;
}

sai_status_t saiTunnelTest::sai_test_tunnel_map_remove (sai_object_id_t tunnel_map_id)
{
    sai_status_t       status = SAI_STATUS_FAILURE;

    TUNNEL_PRINT ("Testing Tunnel map object Id: 0x%" PRIx64 " remove.", tunnel_map_id);

    status = p_sai_tunnel_api_tbl->remove_tunnel_map (tunnel_map_id);

    if (status != SAI_STATUS_SUCCESS) {
        TUNNEL_PRINT ("SAI Tunnel map remove API failed with error: %d.", status);
    } else {
        TUNNEL_PRINT ("SAI Tunnel map remove API success.");
    }

    return status;
}

sai_status_t saiTunnelTest::sai_test_tunnel_map_entry_create (
                                               sai_object_id_t *tunnel_map_entry_id,
                                               unsigned int attr_count, ...)
{
    sai_status_t     status = SAI_STATUS_FAILURE;
    va_list          ap;
    sai_attribute_t  attr_list [max_tunnel_map_entry_obj_attr_count];

    if (attr_count > max_tunnel_map_entry_obj_attr_count) {

        TUNNEL_PRINT ("%s(): Attr count %u is greater than Tunnel map entry attr count "
                      "%u.", __FUNCTION__, attr_count, max_tunnel_map_entry_obj_attr_count);

        return status;
    }

    memset (attr_list, 0, sizeof (attr_list));

    TUNNEL_PRINT ("Testing Tunnel map entry object Creation with attribute count: %u.",
                  attr_count);

    va_start (ap, attr_count);

    sai_test_tunnel_map_entry_attr_value_fill (attr_count, &ap, attr_list);

    va_end (ap);

    status = p_sai_tunnel_api_tbl->create_tunnel_map_entry (tunnel_map_entry_id, switch_id,
                                                            attr_count, attr_list);
    if (status != SAI_STATUS_SUCCESS) {
        TUNNEL_PRINT ("SAI Tunnel map entry creation API failed with error: %d.", status);
    } else {
        TUNNEL_PRINT ("SAI Tunnel map entry creation API success, Tunnel map entry object Id: "
                      "0x%" PRIx64 ".", *tunnel_map_entry_id);
    }

    return status;
}

sai_status_t saiTunnelTest::sai_test_tunnel_map_entry_remove (sai_object_id_t tunnel_map_entry_id)
{
    sai_status_t       status = SAI_STATUS_FAILURE;

    TUNNEL_PRINT ("Testing Tunnel map entry object Id: 0x%" PRIx64 " remove.",
                  tunnel_map_entry_id);

    status = p_sai_tunnel_api_tbl->remove_tunnel_map_entry (tunnel_map_entry_id);

    if (status != SAI_STATUS_SUCCESS) {
        TUNNEL_PRINT ("SAI Tunnel map entry remove API failed with error: %d.", status);
    } else {
        TUNNEL_PRINT ("SAI Tunnel map entry remove API success.");
    }

    return status;
}

sai_status_t saiTunnelTest::sai_test_tunnel_map_entry_set (sai_object_id_t tunnel_map_entry_id,
                                                           ...)
{
    sai_status_t     status = SAI_STATUS_FAILURE;
    va_list          ap;
    sai_attribute_t  attr;
    unsigned int     attr_count = 1;

    memset (&attr, 0, sizeof (attr));

    TUNNEL_PRINT ("Testing Tunnel map entry object set");

    va_start (ap, tunnel_map_entry_id);

    sai_test_tunnel_map_entry_attr_value_fill (attr_count, &ap, &attr);

    va_end (ap);

    status = p_sai_tunnel_api_tbl->set_tunnel_map_entry_attribute(tunnel_map_entry_id,
                                                                  &attr);

    if (status != SAI_STATUS_SUCCESS) {
        TUNNEL_PRINT ("SAI Tunnel map entry set API failed for tunnel map entry object "
                      "id 0x%" PRIx64 "with error: %d.",tunnel_map_entry_id, status);
    } else {
        TUNNEL_PRINT ("SAI Tunnel map entry set API success for tunnnel map entry object Id: "
                      "0x%" PRIx64 ".", tunnel_map_entry_id);
    }

    return status;
}

sai_status_t saiTunnelTest::sai_test_tunnel_map_entry_get (
                                            sai_object_id_t tunnel_map_entry_id,
                                            sai_attribute_t *p_attr_list,
                                            unsigned int attr_count, ...)
{
    unsigned int     index = 0;
    sai_status_t     status = SAI_STATUS_FAILURE;
    va_list          ap;

    if (attr_count > max_tunnel_map_entry_obj_attr_count) {

        TUNNEL_PRINT ("%s(): Attr count %u is greater than Tunnel map entry attr count "
                      "%u.", __FUNCTION__, attr_count, max_tunnel_map_entry_obj_attr_count);

        return status;
    }

    TUNNEL_PRINT ("Testing Tunnel map entry object get with attribute count: %u.",
                  attr_count);

    va_start (ap, attr_count);
    for (index = 0; index < attr_count; index++) {
        p_attr_list[index].id = va_arg (ap , unsigned int);
    }
    va_end (ap);

    status = p_sai_tunnel_api_tbl->get_tunnel_map_entry_attribute(tunnel_map_entry_id,
                                                                  attr_count, p_attr_list);
    if (status != SAI_STATUS_SUCCESS) {
        TUNNEL_PRINT ("SAI Tunnel map entry get API failed with error: %d.", status);
    } else {
        TUNNEL_PRINT ("SAI Tunnel map entry get API success for tunnnel map entry object Id: "
                      "0x%" PRIx64 ".", tunnel_map_entry_id);
    }

    return status;
}

sai_status_t saiTunnelTest::sai_test_tunnel_create (
                                               sai_object_id_t *tunnel_id,
                                               unsigned int attr_count, ...)
{
    sai_status_t     status = SAI_STATUS_FAILURE;
    va_list          ap;
    sai_attribute_t  attr_list [max_tunnel_obj_attr_count];

    if (attr_count > max_tunnel_obj_attr_count) {

        TUNNEL_PRINT ("%s(): Attr count %u is greater than Tunnel attr count "
                      "%u.", __FUNCTION__, attr_count, max_tunnel_obj_attr_count);

        return status;
    }

    memset (attr_list, 0, sizeof (attr_list));

    TUNNEL_PRINT ("Testing Tunnel object creation with attribute count: %u.",
                  attr_count);

    va_start (ap, attr_count);

    sai_test_tunnel_attr_value_fill (attr_count, &ap, attr_list);

    va_end (ap);

    status = p_sai_tunnel_api_tbl->create_tunnel (tunnel_id, switch_id, attr_count,
                                                  attr_list);

    if (status != SAI_STATUS_SUCCESS) {
        TUNNEL_PRINT ("SAI Tunnel creation failed with error: %d.", status);
    } else {
        TUNNEL_PRINT ("SAI Tunnel creation successful, Tunnel object Id: "
                      "0x%" PRIx64 ".", *tunnel_id);
    }

    sai_test_tunnel_attr_value_cleanup(attr_count, attr_list);

    return status;
}

sai_status_t saiTunnelTest::sai_test_tunnel_remove (sai_object_id_t tunnel_id)
{
    sai_status_t       status;

    TUNNEL_PRINT ("Testing Tunnel object Id: 0x%" PRIx64 " remove.", tunnel_id);

    status = p_sai_tunnel_api_tbl->remove_tunnel (tunnel_id);

    if (status != SAI_STATUS_SUCCESS) {
        TUNNEL_PRINT ("SAI Tunnel remove API failed with error: %d.", status);
    } else {
        TUNNEL_PRINT ("SAI Tunnel remove API success.");
    }

    return status;
}

void saiTunnelTest::sai_test_tunnel_term_attr_value_fill (unsigned int attr_count,
                                                          va_list *p_varg_list,
                                                          sai_attribute_t *p_attr_list)
{
    unsigned int     af;
    unsigned int     index;
    const char      *p_ip_str;
    sai_attribute_t *p_attr;

    for (index = 0; index < attr_count; index++) {

        p_attr = &p_attr_list [index];

        p_attr->id = va_arg ((*p_varg_list), unsigned int);

        switch (p_attr->id) {

            case SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_DST_IP:
            case SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_SRC_IP:
                af       = va_arg ((*p_varg_list), unsigned int);
                p_ip_str = va_arg ((*p_varg_list), const char *);

                if (af == SAI_IP_ADDR_FAMILY_IPV4) {

                    inet_pton (AF_INET, p_ip_str,
                               (void *)&p_attr->value.ipaddr.addr.ip4);
                } else {

                    inet_pton (AF_INET6, p_ip_str,
                               (void *)&p_attr->value.ipaddr.addr.ip6);
                }

                p_attr->value.ipaddr.addr_family = (sai_ip_addr_family_t) af;
                TUNNEL_PRINT ("Attr Index: %d, Set Termination IP Addr family: %d, "
                              "IP Addr: %s.", index, af, p_ip_str);
                break;

            case SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_ACTION_TUNNEL_ID:
            case SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_VR_ID:
                p_attr->value.oid = va_arg ((*p_varg_list), unsigned long);
                TUNNEL_PRINT ("Attr Index: %d, Set OId value: 0x%" PRIx64 ".",
                              index, p_attr->value.oid);
                break;

            case SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_TYPE:
            case SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_TUNNEL_TYPE:
                p_attr->value.s32 = va_arg ((*p_varg_list), unsigned int);
                break;

            default:
                p_attr->value.u64 = va_arg ((*p_varg_list), unsigned int);
                TUNNEL_PRINT ("Attr Index: %d, Set Attr Id: %d to value: %ld.",
                              index, p_attr->id, p_attr->value.u64);
                break;
        }
    }
}

sai_status_t saiTunnelTest::sai_test_tunnel_term_entry_create (
                                               sai_object_id_t *tunnel_term_id,
                                               unsigned int attr_count, ...)
{
    sai_status_t     status = SAI_STATUS_FAILURE;
    va_list          ap;
    sai_attribute_t  attr_list [max_tunnel_term_attr_count];

    if (attr_count > max_tunnel_term_attr_count) {

        TUNNEL_PRINT ("%s(): Attr count %u is greater than Max attr count "
                      "%u.", __FUNCTION__, attr_count, max_tunnel_term_attr_count);

        return status;
    }

    memset (attr_list, 0, sizeof (attr_list));

    TUNNEL_PRINT ("Testing Tunnel Termination table entry object creation "
                  "with attribute count: %u.", attr_count);

    va_start (ap, attr_count);

    sai_test_tunnel_term_attr_value_fill (attr_count, &ap, attr_list);

    va_end (ap);

    status = p_sai_tunnel_api_tbl->create_tunnel_term_table_entry (
                             tunnel_term_id, switch_id, attr_count, attr_list);

    if (status != SAI_STATUS_SUCCESS) {
        TUNNEL_PRINT ("SAI Tunnel Termination table entry creation failed "
                      "with error: %d.", status);
    } else {
        TUNNEL_PRINT ("SAI Tunnel Termination table entry creation successful"
                      ", Tunnel term object Id: 0x%" PRIx64 ".", *tunnel_term_id);
    }

    return status;
}

sai_status_t saiTunnelTest::sai_test_tunnel_term_entry_remove (
                                             sai_object_id_t tunnel_term_id)
{
    sai_status_t       status;

    TUNNEL_PRINT ("Testing Tunnel Termination table entry object Id: "
                  "0x%" PRIx64 " remove.", tunnel_term_id);

    status = p_sai_tunnel_api_tbl->remove_tunnel_term_table_entry (tunnel_term_id);

    if (status != SAI_STATUS_SUCCESS) {
        TUNNEL_PRINT ("SAI Tunnel Termination table entry remove API failed "
                      "with error: %d.", status);
    } else {
        TUNNEL_PRINT ("SAI Tunnel Termination table entry remove API success.");
    }

    return status;
}

void saiTunnelTest::sai_test_tunnel_stats_type_fill (unsigned int stats_count,
                                                     va_list *p_varg_list,
                                                     sai_tunnel_stat_t *p_tunnel_stats_list)
{
    unsigned int counter_idx = 0;

    for(counter_idx = 0; counter_idx < stats_count; counter_idx++) {
        p_tunnel_stats_list[counter_idx] = (sai_tunnel_stat_t) va_arg ((*p_varg_list),
                                                                       unsigned int);

    }
}

sai_status_t saiTunnelTest::sai_test_tunnel_stats_get (sai_object_id_t tunnel_id,
                                                       unsigned int stats_count, ...)
{
    sai_status_t      status = SAI_STATUS_FAILURE;
    va_list           ap;
    sai_tunnel_stat_t tunnel_stats_list [max_tunnel_stats_count];
    uint32_t          num_counters = 0;
    uint32_t          counter_id = 0;
    uint64_t          stats_val [max_tunnel_stats_count];

    if (stats_count > max_tunnel_stats_count) {

        TUNNEL_PRINT ("%s(): Stats count %u is greater than Max tunnel stats count "
                      "%u.", __FUNCTION__, stats_count, max_tunnel_stats_count);

        return status;
    }

    memset (tunnel_stats_list, 0, sizeof (tunnel_stats_list));
    memset (stats_val, 0, sizeof (stats_val));

    TUNNEL_PRINT ("Testing Tunnel stats get for tunnel id 0x%" PRIx64 " "
                  "with stats count: %u.", tunnel_id, stats_count);

    va_start (ap, stats_count);

    sai_test_tunnel_stats_type_fill (stats_count, &ap, tunnel_stats_list);

    va_end (ap);

    num_counters = stats_count;

    status = p_sai_tunnel_api_tbl->get_tunnel_stats (tunnel_id, num_counters,
                                                     &tunnel_stats_list[0],
                                                     &stats_val[0]);

    if (status != SAI_STATUS_SUCCESS) {
        TUNNEL_PRINT ("SAI Tunnel stats get failed for tunnel id 0x%" PRIx64 " "
                      "with error: %d.", tunnel_id, status);
    } else {

        for(counter_id = 0; counter_id < num_counters; counter_id++) {
            TUNNEL_PRINT ("Stat id = %d stat value %" PRIu64 "",tunnel_stats_list[counter_id],
                          stats_val[counter_id]);
        }
    }

    return status;
}

sai_status_t saiTunnelTest::sai_test_tunnel_stats_clear (sai_object_id_t tunnel_id,
                                                         unsigned int stats_count, ...)
{
    sai_status_t      status = SAI_STATUS_FAILURE;
    va_list           ap;
    sai_tunnel_stat_t tunnel_stats_list [max_tunnel_stats_count];
    uint32_t          num_counters = 0;

    if (stats_count > max_tunnel_stats_count) {

        TUNNEL_PRINT ("%s(): Stats count %u is greater than Max tunnel stats count "
                      "%u.", __FUNCTION__, stats_count, max_tunnel_stats_count);

        return status;
    }

    memset (tunnel_stats_list, 0, sizeof (tunnel_stats_list));

    TUNNEL_PRINT ("Testing Tunnel stats clear for tunnel id 0x%" PRIx64 " "
                  "with stats count: %u.", tunnel_id, stats_count);

    va_start (ap, stats_count);

    sai_test_tunnel_stats_type_fill (stats_count, &ap, tunnel_stats_list);

    va_end (ap);

    num_counters = stats_count;

    status = p_sai_tunnel_api_tbl->clear_tunnel_stats (tunnel_id, num_counters,
                                                       &tunnel_stats_list[0]);

    if (status != SAI_STATUS_SUCCESS) {
        TUNNEL_PRINT ("SAI Tunnel stats clear failed for tunnel id 0x%" PRIx64 " "
                      "with error: %d.", tunnel_id, status);
    }

    return status;
}

void saiTunnelTest::sai_test_mac_str_to_bytes_get (const char *mac_str, uint8_t *mac)
{
    unsigned int mac_in [6];
    unsigned int byte_idx = 0;

    sscanf (mac_str, "%x:%x:%x:%x:%x:%x", &mac_in [0], &mac_in [1],
            &mac_in [2], &mac_in [3], &mac_in [4], &mac_in [5]);

    for (byte_idx = 0; byte_idx < 6; byte_idx++) {
        mac [byte_idx] = mac_in [byte_idx];
    }
}

sai_status_t saiTunnelTest::sai_test_tunnel_fdb_entry_create(sai_fdb_entry_type_t type,
                                                             sai_object_id_t bv_id,
                                                             const char *p_mac_str,
                                                             sai_packet_action_t packet_action,
                                                             sai_object_id_t bridge_port_id,
                                                             bool is_tunnel_port,
                                                             const char *p_remote_ip)
{
    sai_fdb_entry_t    fdb_entry;
    sai_status_t       status;
    const unsigned int default_fdb_attr_count = 4;
    unsigned int       attr_count = 0;
    sai_attribute_t    attr_list [default_fdb_attr_count];

    memset (&fdb_entry, 0, sizeof (sai_fdb_entry_t));

    sai_test_mac_str_to_bytes_get (p_mac_str, fdb_entry.mac_address);

    fdb_entry.bv_id = bv_id;

    attr_list[attr_count].id = SAI_FDB_ENTRY_ATTR_TYPE;
    attr_list[attr_count].value.s32 = type;
    attr_count++;

    if(bridge_port_id != SAI_NULL_OBJECT_ID) {
        attr_list[attr_count].id = SAI_FDB_ENTRY_ATTR_BRIDGE_PORT_ID;
        attr_list[attr_count].value.oid = bridge_port_id;
        attr_count++;
    }

    attr_list[attr_count].id = SAI_FDB_ENTRY_ATTR_PACKET_ACTION;
    attr_list[attr_count].value.s32 = packet_action;
    attr_count++;

    if(is_tunnel_port) {
        attr_list[attr_count].id = SAI_FDB_ENTRY_ATTR_ENDPOINT_IP;
        attr_list[attr_count].value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
        inet_pton (AF_INET, p_remote_ip, (void *)&attr_list[attr_count].value.ipaddr.addr.ip4);
        attr_count++;
    }

    TUNNEL_PRINT("Testing fdb entry create for vlan/bridge 0x%" PRIx64 " mac %s "
                 "bridge port 0x%" PRIx64 "",bv_id, p_mac_str, bridge_port_id);

    status = p_sai_fdb_api_tbl->create_fdb_entry (&fdb_entry, attr_count, attr_list);

    if(status != SAI_STATUS_SUCCESS) {

        TUNNEL_PRINT("FDB entry creation for vlan/bridge 0x%" PRIx64 " mac %s "
                     "bridge port 0x%" PRIx64 " failed with error %d", bv_id,
                     p_mac_str, bridge_port_id, status);

    } else {

        TUNNEL_PRINT("FDB entry creation for vlan/bridge 0x%" PRIx64 " mac %s "
                     "bridge port 0x%" PRIx64 " successful",bv_id, p_mac_str,
                     bridge_port_id);
    }

    return status;
}

sai_status_t saiTunnelTest::sai_test_tunnel_fdb_entry_remove(sai_object_id_t bv_id,
                                                             const char *p_mac_str)
{
    sai_fdb_entry_t    fdb_entry;
    sai_status_t       status;

    memset (&fdb_entry, 0, sizeof (sai_fdb_entry_t));

    sai_test_mac_str_to_bytes_get (p_mac_str, fdb_entry.mac_address);
    fdb_entry.bv_id = bv_id;

    TUNNEL_PRINT("Testing fdb entry remove for vlan/bridge 0x%" PRIx64 " mac %s ",
                 bv_id, p_mac_str);

    status = p_sai_fdb_api_tbl->remove_fdb_entry (&fdb_entry);

    if(status != SAI_STATUS_SUCCESS) {

        TUNNEL_PRINT("FDB entry removal for vlan/bridge 0x%" PRIx64 " mac %s "
                     "failed with error %d", bv_id, p_mac_str, status);

    } else {

        TUNNEL_PRINT("FDB entry removal for vlan/bridge 0x%" PRIx64 " mac %s "
                     " successful",bv_id, p_mac_str);
    }


    return status;
}

sai_status_t saiTunnelTest::sai_test_tunnel_l2mc_group_create(
                                                   sai_object_id_t *l2mc_group_id)
{
    sai_status_t status = SAI_STATUS_FAILURE;

    TUNNEL_PRINT ("Testing L2MC Group create");

    status = p_sai_l2mc_group_api_tbl->create_l2mc_group(l2mc_group_id,switch_id,
                                                         0, NULL);

    if (status != SAI_STATUS_SUCCESS) {
        TUNNEL_PRINT ("SAI L2MC Group create API failed with error: %d.", status);
    } else {
        TUNNEL_PRINT ("SAI L2MC Group 0x%" PRIx64 " created successfully", *l2mc_group_id);
    }

    return status;
}

sai_status_t saiTunnelTest::sai_test_tunnel_l2mc_group_remove(
                                                      sai_object_id_t l2mc_group_id)
{
    sai_status_t status = SAI_STATUS_FAILURE;

    TUNNEL_PRINT ("Testing L2MC Group remove");

    status = p_sai_l2mc_group_api_tbl->remove_l2mc_group(l2mc_group_id);

    if (status != SAI_STATUS_SUCCESS) {
        TUNNEL_PRINT ("SAI L2MC Group 0x%" PRIx64 " remove failed with error: %d.",
                      l2mc_group_id, status);
    } else {
        TUNNEL_PRINT ("SAI L2MC Group 0x%" PRIx64 " removed successfully", l2mc_group_id);
    }

    return status;
}

void saiTunnelTest::sai_test_l2mc_group_member_attr_value_fill (unsigned int attr_count,
                                                                va_list *p_varg_list,
                                                                sai_attribute_t *p_attr_list)
{
    unsigned int     af;
    unsigned int     index;
    const char      *p_ip_str;
    sai_attribute_t *p_attr;

    for (index = 0; index < attr_count; index++) {

        p_attr = &p_attr_list [index];

        p_attr->id = va_arg ((*p_varg_list), unsigned int);

        switch (p_attr->id) {

            case SAI_L2MC_GROUP_MEMBER_ATTR_L2MC_GROUP_ID:
                p_attr->value.oid = va_arg ((*p_varg_list), unsigned long);
                TUNNEL_PRINT ("Attr Index: %d, L2MC group id value: 0x%" PRIx64 ".",
                              index, p_attr->value.oid);

                break;

            case SAI_L2MC_GROUP_MEMBER_ATTR_L2MC_OUTPUT_ID:
                p_attr->value.oid = va_arg ((*p_varg_list), unsigned long);
                TUNNEL_PRINT ("Attr Index: %d, Bridge port id value: 0x%" PRIx64 ".",
                              index, p_attr->value.oid);
                break;

            case SAI_L2MC_GROUP_MEMBER_ATTR_L2MC_ENDPOINT_IP:
                af = SAI_IP_ADDR_FAMILY_IPV4;
                p_ip_str = va_arg ((*p_varg_list), const char *);
                inet_pton (AF_INET, p_ip_str, (void *)&p_attr->value.ipaddr.addr.ip4);

                p_attr->value.ipaddr.addr_family = (sai_ip_addr_family_t) af;
                TUNNEL_PRINT ("Attr Index: %d, L2MC IP Addr family: %d, "
                              "IP Addr: %s.", index, af, p_ip_str);
                break;

            default:
                p_attr->value.u64 = va_arg ((*p_varg_list), unsigned int);
                TUNNEL_PRINT ("Attr Index: %d, Set Attr Id: %d to value: %ld.",
                              index, p_attr->id, p_attr->value.u64);
                break;
        }
    }
}

sai_status_t saiTunnelTest::sai_test_tunnel_l2mc_group_member_create (
                                               sai_object_id_t *l2mc_group_member_id,
                                               unsigned int attr_count, ...)
{
    sai_status_t     status = SAI_STATUS_FAILURE;
    va_list          ap;
    sai_attribute_t  attr_list [max_l2mc_group_member_attr_count];

    if (attr_count > max_l2mc_group_member_attr_count) {

        TUNNEL_PRINT ("%s(): Attr count %u is greater than Max attr count "
                      "%u.", __FUNCTION__, attr_count, max_l2mc_group_member_attr_count);

        return status;
    }

    memset (attr_list, 0, sizeof (attr_list));

    TUNNEL_PRINT ("Testing L2MC Group member object creation "
                  "with attribute count: %u.", attr_count);

    va_start (ap, attr_count);

    sai_test_l2mc_group_member_attr_value_fill (attr_count, &ap, attr_list);

    va_end (ap);

    status = p_sai_l2mc_group_api_tbl->create_l2mc_group_member (l2mc_group_member_id,
                                                          switch_id, attr_count, attr_list);

    if (status != SAI_STATUS_SUCCESS) {
        TUNNEL_PRINT ("SAI Tunnel L2MC group member creation failed with error: %d.", status);
    } else {
        TUNNEL_PRINT ("SAI L2MC group member creation successful, L2MC group member "
                      "object Id: 0x%" PRIx64 ".", *l2mc_group_member_id);
    }

    return status;
}

sai_status_t saiTunnelTest::sai_test_tunnel_l2mc_group_member_remove(
                                                      sai_object_id_t l2mc_group_member_id)
{
    sai_status_t status = SAI_STATUS_FAILURE;

    TUNNEL_PRINT ("Testing L2MC Group member object remove");

    status = p_sai_l2mc_group_api_tbl->remove_l2mc_group_member (l2mc_group_member_id);

    if (status != SAI_STATUS_SUCCESS) {
        TUNNEL_PRINT ("SAI Tunnel L2MC group member 0x%" PRIx64 " remove failed "
                      "with error: %d.", l2mc_group_member_id, status);
    } else {
        TUNNEL_PRINT ("SAI Tunnel L2MC group member 0x%" PRIx64 " removed successfully",
                       l2mc_group_member_id);
    }

    return status;
}

sai_status_t saiTunnelTest::sai_test_tunnel_lag_create(sai_object_id_t *lag_id)
{
    sai_status_t status = SAI_STATUS_FAILURE;

    TUNNEL_PRINT ("Testing LAG create");

    status = p_sai_lag_api_tbl->create_lag(lag_id, switch_id, 0, NULL);

    if (status != SAI_STATUS_SUCCESS) {
        TUNNEL_PRINT ("SAI LAG create API failed with error: %d.", status);
    } else {
        TUNNEL_PRINT ("SAI LAG 0x%" PRIx64 " created successfully", *lag_id);
    }

    return status;
}

sai_status_t saiTunnelTest::sai_test_tunnel_lag_remove(sai_object_id_t lag_id)
{
    sai_status_t status = SAI_STATUS_FAILURE;

    TUNNEL_PRINT ("Testing LAG remove");

    status = p_sai_lag_api_tbl->remove_lag(lag_id);

    if (status != SAI_STATUS_SUCCESS) {
        TUNNEL_PRINT ("SAI LAG 0x%" PRIx64 " remove failed with error: %d.",
                      lag_id, status);
    } else {
        TUNNEL_PRINT ("SAI LAG 0x%" PRIx64 " removed successfully", lag_id);
    }

    return status;
}

sai_status_t saiTunnelTest::sai_test_tunnel_lag_member_create(sai_object_id_t *lag_member,
                                                              sai_object_id_t lag_id,
                                                              sai_object_id_t port_id)
{
    sai_status_t status = SAI_STATUS_FAILURE;
    sai_attribute_t attr_list [2];
    uint32_t        count = 0;

    TUNNEL_PRINT ("Testing LAG member add");

    attr_list [count].id = SAI_LAG_MEMBER_ATTR_LAG_ID;
    attr_list [count].value.oid = lag_id;
    count++;

    attr_list [count].id = SAI_LAG_MEMBER_ATTR_PORT_ID;
    attr_list [count].value.oid = port_id;
    count++;

    status = p_sai_lag_api_tbl->create_lag_member(lag_member, switch_id, count, attr_list);

    if (status != SAI_STATUS_SUCCESS) {
        TUNNEL_PRINT ("SAI LAG member create failed with error: %d. Lag id 0x%"
                       PRIx64 " Port id 0x%" PRIx64 ,status, lag_id, port_id);
    } else {
        TUNNEL_PRINT ("SAI LAG member 0x%" PRIx64 " created successfully. Lag id 0x%"
                       PRIx64 " Port id 0x%" PRIx64 ,*lag_member, lag_id, port_id);
    }

    return status;
}

sai_status_t saiTunnelTest::sai_test_tunnel_lag_member_remove(sai_object_id_t lag_member)
{
    sai_status_t status;

    status = p_sai_lag_api_tbl->remove_lag_member(lag_member);

    if (status != SAI_STATUS_SUCCESS) {

        TUNNEL_PRINT ("SAI LAG member 0x%" PRIx64 " remove failed with error: %d", lag_member,
                      status);
    } else {

        TUNNEL_PRINT ("SAI LAG member 0x%" PRIx64 " removed successfully", lag_member);
    }

    return status;
}

sai_status_t saiTunnelTest::sai_test_flood_packet_on_vxlan_l3mc_index(sai_object_id_t bridgeId, sai_object_id_t l2mcIndex)
{
    sai_attribute_t sai_attr[3];
    sai_status_t status;

    unsigned char buffer[] =
       {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x01, 0xe8, 0x03, 0x04, 0x07,
        0x08, 0x06, 0x00, 0x01, 0x08, 0x00, 0x06, 0x04, 0x00, 0x01, 0x00, 0x01,
        0x02, 0x03, 0x04, 0x07, 0x14, 0x00, 0x00, 0x2e, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x14, 0x00, 0x00, 0x01, 0x10, 0x02, 0x17, 0x01, 0x7f, 0x0a,
        0x0a, 0x0a, 0x00, 0x00, 0x00, 0x85, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00 };

    sai_attr[0].id = SAI_HOSTIF_PACKET_ATTR_HOSTIF_TX_TYPE;
    sai_attr[0].value.s32 = SAI_HOSTIF_TX_TYPE_HYBRID;

    sai_attr[1].id = SAI_HOSTIF_PACKET_ATTR_EGR_BRIDGE_ID;
    sai_attr[1].value.s64 = bridgeId;

    sai_attr[2].id = SAI_HOSTIF_PACKET_ATTR_EGR_L2MC_GROUP;
    sai_attr[2].value.s64 = l2mcIndex;

    status = p_sai_hostif_api_tbl->send_hostif_packet(SAI_NULL_OBJECT_ID, buffer,
                            sizeof(buffer), 3, sai_attr);
    return status;
}

