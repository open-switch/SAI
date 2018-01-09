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
* @file  sai_vxlan_tunnel_unit_test_utils.cpp
*
* @brief This file contains utilities for VXLAN Tunnel test cases.
*
*************************************************************************/

#include "gtest/gtest.h"

#include "sai_tunnel_unit_test.h"
#include "sai_l3_unit_test_utils.h"
#include "sai_bridge_unit_test_utils.h"
#include "sai_vxlan_tunnel_unit_test_utils.h"

extern "C" {
#include "saitypes.h"
#include "saistatus.h"
#include "saiport.h"
#include "saitunnel.h"
#include "saibridge.h"
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
}

void sai_test_vxlan_assert_success(sai_status_t status,
                                   bool assert_success)
{
    if(assert_success) {
        ASSERT_EQ (SAI_STATUS_SUCCESS, status);
    } else {
        ASSERT_NE (SAI_STATUS_SUCCESS, status);
    }
}

void sai_test_vxlan_expect_success(sai_status_t status,
                                   bool expect_success)
{
    if(expect_success) {
        EXPECT_EQ (SAI_STATUS_SUCCESS, status);
    } else {
        EXPECT_NE (SAI_STATUS_SUCCESS, status);
    }
}

void sai_test_vxlan_1d_bridge_create(sai_object_id_t *bridge_id,
                                     bool assert_success)
{
    sai_status_t status;

    status = saiTunnelTest::sai_test_bridge_create (bridge_id, 1, SAI_BRIDGE_ATTR_TYPE,
                                     SAI_BRIDGE_TYPE_1D);

    sai_test_vxlan_assert_success(status, assert_success);
}

void sai_test_vxlan_1d_bridge_remove(sai_object_id_t bridge_id,
                                     bool expect_success)
{
    sai_status_t status;

    status = saiTunnelTest::sai_test_bridge_remove(bridge_id);

    sai_test_vxlan_expect_success(status, expect_success);
}

void sai_test_vxlan_1d_bridge_l2mc_groups_set(sai_object_id_t bridge_id,
                                              bool unknown_unicast_l2mc_enable,
                                              sai_object_id_t unknown_unicast_l2mc_group,
                                              bool unknown_mcast_l2mc_enable,
                                              sai_object_id_t unknown_mcast_l2mc_group,
                                              bool broadcast_l2mc_enable,
                                              sai_object_id_t broadcast_l2mc_group,
                                              bool expect_success)
{
    sai_status_t status;
    sai_bridge_flood_control_type_t uuc_flood_type = SAI_BRIDGE_FLOOD_CONTROL_TYPE_SUB_PORTS;
    sai_bridge_flood_control_type_t umc_flood_type = SAI_BRIDGE_FLOOD_CONTROL_TYPE_SUB_PORTS;
    sai_bridge_flood_control_type_t bc_flood_type = SAI_BRIDGE_FLOOD_CONTROL_TYPE_SUB_PORTS;

    status = saiTunnelTest::sai_test_bridge_set(bridge_id,
                                                SAI_BRIDGE_ATTR_UNKNOWN_UNICAST_FLOOD_GROUP,
                                                unknown_unicast_l2mc_group);

    sai_test_vxlan_expect_success(status, expect_success);

    if(unknown_unicast_l2mc_enable) {
        uuc_flood_type = SAI_BRIDGE_FLOOD_CONTROL_TYPE_L2MC_GROUP;
    }

    status = saiTunnelTest::sai_test_bridge_set(bridge_id,
                                                SAI_BRIDGE_ATTR_UNKNOWN_UNICAST_FLOOD_CONTROL_TYPE,
                                                uuc_flood_type);

    sai_test_vxlan_expect_success(status, expect_success);

    status = saiTunnelTest::sai_test_bridge_set(bridge_id,
                                                SAI_BRIDGE_ATTR_UNKNOWN_MULTICAST_FLOOD_GROUP,
                                                unknown_mcast_l2mc_group);

    sai_test_vxlan_expect_success(status, expect_success);

    if(unknown_mcast_l2mc_enable) {
        umc_flood_type = SAI_BRIDGE_FLOOD_CONTROL_TYPE_L2MC_GROUP;
    }

    status = saiTunnelTest::sai_test_bridge_set(bridge_id,
                                                SAI_BRIDGE_ATTR_UNKNOWN_MULTICAST_FLOOD_CONTROL_TYPE,
                                                umc_flood_type);

    sai_test_vxlan_expect_success(status, expect_success);

    status = saiTunnelTest::sai_test_bridge_set(bridge_id,
                                                SAI_BRIDGE_ATTR_BROADCAST_FLOOD_GROUP,
                                                broadcast_l2mc_group);

    sai_test_vxlan_expect_success(status, expect_success);

    if(broadcast_l2mc_enable) {
        bc_flood_type = SAI_BRIDGE_FLOOD_CONTROL_TYPE_L2MC_GROUP;
    }

    status = saiTunnelTest::sai_test_bridge_set(bridge_id,
                                                SAI_BRIDGE_ATTR_BROADCAST_FLOOD_CONTROL_TYPE,
                                                bc_flood_type);

    sai_test_vxlan_expect_success(status, expect_success);
}

void sai_test_vxlan_encap_tunnel_map_create(sai_object_id_t *encap_map_id,
                                            bool assert_success)
{
    sai_status_t status;

    status = saiTunnelTest::sai_test_tunnel_map_create(encap_map_id, 1,
                                                       SAI_TUNNEL_MAP_ATTR_TYPE,
                                                       SAI_TUNNEL_MAP_TYPE_BRIDGE_IF_TO_VNI);

    sai_test_vxlan_assert_success(status, assert_success);
}

void sai_test_vxlan_decap_tunnel_map_create(sai_object_id_t *decap_map_id,
                                            bool assert_success)
{
    sai_status_t status;

    status = saiTunnelTest::sai_test_tunnel_map_create(decap_map_id, 1,
                                                       SAI_TUNNEL_MAP_ATTR_TYPE,
                                                       SAI_TUNNEL_MAP_TYPE_VNI_TO_BRIDGE_IF);

    sai_test_vxlan_assert_success(status, assert_success);
}

void sai_test_vxlan_tunnel_map_remove(sai_object_id_t tunnel_map_id,
                                      bool expect_success)
{
    sai_status_t status;

    status = saiTunnelTest::sai_test_tunnel_map_remove(tunnel_map_id);

    sai_test_vxlan_expect_success(status, expect_success);
}

void sai_test_vxlan_encap_map_entry_create(sai_object_id_t *encap_map_entry_id,
                                           sai_object_id_t encap_map_id,
                                           sai_object_id_t bridge_id,
                                           sai_uint32_t vnid,
                                           bool assert_success)
{
    sai_status_t status;

    status = saiTunnelTest::sai_test_tunnel_map_entry_create(encap_map_entry_id, 4,
                                              SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE,
                                              SAI_TUNNEL_MAP_TYPE_BRIDGE_IF_TO_VNI,
                                              SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP,
                                              encap_map_id,
                                              SAI_TUNNEL_MAP_ENTRY_ATTR_BRIDGE_ID_KEY,
                                              bridge_id,
                                              SAI_TUNNEL_MAP_ENTRY_ATTR_VNI_ID_VALUE,
                                              vnid);

    sai_test_vxlan_assert_success(status, assert_success);
}

void sai_test_vxlan_decap_map_entry_create(sai_object_id_t *decap_map_entry_id,
                                           sai_object_id_t decap_map_id,
                                           sai_uint32_t vnid,
                                           sai_object_id_t bridge_id,
                                           bool assert_success)
{
    sai_status_t status;

    status = saiTunnelTest::sai_test_tunnel_map_entry_create(decap_map_entry_id, 4,
                                              SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE,
                                              SAI_TUNNEL_MAP_TYPE_VNI_TO_BRIDGE_IF,
                                              SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP,
                                              decap_map_id,
                                              SAI_TUNNEL_MAP_ENTRY_ATTR_VNI_ID_KEY,
                                              vnid,
                                              SAI_TUNNEL_MAP_ENTRY_ATTR_BRIDGE_ID_VALUE,
                                              bridge_id);

    sai_test_vxlan_assert_success(status, assert_success);
}

void sai_test_vxlan_tunnel_map_entry_remove(sai_object_id_t tunnel_map_entry_id,
                                            bool expect_success)
{
    sai_status_t status;

    status = saiTunnelTest::sai_test_tunnel_map_entry_remove(tunnel_map_entry_id);

    sai_test_vxlan_expect_success(status, expect_success);
}

void sai_test_vxlan_encap_map_entry_set(sai_object_id_t tunnel_map_entry_id,
                                        sai_uint32_t vnid,
                                        bool expect_success)
{
    sai_status_t status;

    status = saiTunnelTest::sai_test_tunnel_map_entry_set(tunnel_map_entry_id,
                                                         SAI_TUNNEL_MAP_ENTRY_ATTR_VNI_ID_VALUE,
                                                         vnid);

    sai_test_vxlan_expect_success(status, expect_success);
}

void sai_test_vxlan_decap_map_entry_set(sai_object_id_t tunnel_map_entry_id,
                                        sai_object_id_t bridge_id,
                                        bool expect_success)
{
    sai_status_t status;

    status = saiTunnelTest::sai_test_tunnel_map_entry_set(tunnel_map_entry_id,
                                                          SAI_TUNNEL_MAP_ENTRY_ATTR_BRIDGE_ID_VALUE,
                                                          bridge_id);

    sai_test_vxlan_expect_success(status, expect_success);
}

void sai_test_vxlan_encap_map_entry_get(sai_object_id_t tunnel_map_entry_id,
                                        unsigned int attr_count,
                                        sai_attribute_t *p_attr_list,
                                        bool expect_success)
{
    sai_status_t status;
    unsigned int encap_attr_count = 4;
    sai_attribute_t encap_attr_list[encap_attr_count];

    if(attr_count < encap_attr_count) {
        sai_test_vxlan_expect_success(SAI_STATUS_FAILURE, expect_success);
    }

    status = saiTunnelTest::sai_test_tunnel_map_entry_get (tunnel_map_entry_id,
                                                           encap_attr_list,
                                                           encap_attr_count,
                                                           SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE,
                                                           SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP,
                                                           SAI_TUNNEL_MAP_ENTRY_ATTR_BRIDGE_ID_KEY,
                                                           SAI_TUNNEL_MAP_ENTRY_ATTR_VNI_ID_VALUE);

    sai_test_vxlan_expect_success(status, expect_success);
    memcpy(p_attr_list, &encap_attr_list, sizeof(encap_attr_list));
}

void sai_test_vxlan_encap_map_entry_verify_bridge(unsigned int attr_count,
                                                  sai_attribute_t *p_attr_list,
                                                  sai_object_id_t bridge_id)
{
    unsigned int index = 0;
    sai_status_t status = SAI_STATUS_FAILURE;

    for(index = 0; index < attr_count; index++) {
        if(p_attr_list[index].id == SAI_TUNNEL_MAP_ENTRY_ATTR_BRIDGE_ID_KEY) {
            if(p_attr_list[index].value.oid == bridge_id) {
                status = SAI_STATUS_SUCCESS;
                break;
            }
        }
    }

    EXPECT_EQ(SAI_STATUS_SUCCESS, status);
}

void sai_test_vxlan_encap_map_entry_verify_vnid(unsigned int attr_count,
                                                sai_attribute_t *p_attr_list,
                                                sai_uint32_t vnid)
{
    unsigned int index = 0;
    sai_status_t status = SAI_STATUS_FAILURE;

    for(index = 0; index < attr_count; index++) {
        if(p_attr_list[index].id == SAI_TUNNEL_MAP_ENTRY_ATTR_VNI_ID_VALUE) {
            if(p_attr_list[index].value.u32 == vnid) {
                status = SAI_STATUS_SUCCESS;
                break;
            }
        }
    }

    EXPECT_EQ(SAI_STATUS_SUCCESS, status);
}

void sai_test_vxlan_decap_map_entry_get(sai_object_id_t tunnel_map_entry_id,
                                        unsigned int attr_count,
                                        sai_attribute_t *p_attr_list,
                                        bool expect_success)
{
    sai_status_t status;
    unsigned int decap_attr_count = 4;
    sai_attribute_t decap_attr_list[decap_attr_count];

    if(attr_count < decap_attr_count) {
        sai_test_vxlan_expect_success(SAI_STATUS_FAILURE, expect_success);
    }

    status = saiTunnelTest::sai_test_tunnel_map_entry_get (tunnel_map_entry_id,
                                                           decap_attr_list,
                                                           decap_attr_count,
                                                           SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE,
                                                           SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP,
                                                           SAI_TUNNEL_MAP_ENTRY_ATTR_BRIDGE_ID_VALUE,
                                                           SAI_TUNNEL_MAP_ENTRY_ATTR_VNI_ID_KEY);

    sai_test_vxlan_expect_success(status, expect_success);
    memcpy(p_attr_list, &decap_attr_list, sizeof(decap_attr_list));
}

void sai_test_vxlan_decap_map_entry_verify_bridge(unsigned int attr_count,
                                                  sai_attribute_t *p_attr_list,
                                                  sai_object_id_t bridge_id)
{
    unsigned int index = 0;
    sai_status_t status = SAI_STATUS_FAILURE;

    for(index = 0; index < attr_count; index++) {
        if(p_attr_list[index].id == SAI_TUNNEL_MAP_ENTRY_ATTR_BRIDGE_ID_VALUE) {
            if(p_attr_list[index].value.oid == bridge_id) {
                status = SAI_STATUS_SUCCESS;
                break;
            }
        }
    }

    EXPECT_EQ(SAI_STATUS_SUCCESS, status);
}

void sai_test_vxlan_decap_map_entry_verify_vnid(unsigned int attr_count,
                                                sai_attribute_t *p_attr_list,
                                                sai_uint32_t vnid)
{
    unsigned int index = 0;
    sai_status_t status = SAI_STATUS_FAILURE;

    for(index = 0; index < attr_count; index++) {
        if(p_attr_list[index].id == SAI_TUNNEL_MAP_ENTRY_ATTR_VNI_ID_KEY) {
            if(p_attr_list[index].value.u32 == vnid) {
                status = SAI_STATUS_SUCCESS;
                break;
            }
        }
    }

    EXPECT_EQ(SAI_STATUS_SUCCESS, status);
}

void sai_test_vxlan_tunnel_create(sai_object_id_t *tunnel_id,
                                  sai_object_id_t underlay_rif,
                                  sai_object_id_t overlay_rif,
                                  const char *tunnel_sip,
                                  sai_object_id_t encap_map_id,
                                  sai_object_id_t decap_map_id,
                                  bool assert_success)
{
    sai_status_t status;

    status = saiTunnelTest::sai_test_tunnel_create (tunnel_id, 6,
                                     SAI_TUNNEL_ATTR_TYPE,
                                     SAI_TUNNEL_TYPE_VXLAN,
                                     SAI_TUNNEL_ATTR_UNDERLAY_INTERFACE,
                                     underlay_rif,
                                     SAI_TUNNEL_ATTR_OVERLAY_INTERFACE,
                                     overlay_rif,
                                     SAI_TUNNEL_ATTR_ENCAP_SRC_IP,
                                     SAI_IP_ADDR_FAMILY_IPV4,
                                     tunnel_sip,
                                     SAI_TUNNEL_ATTR_ENCAP_MAPPERS,
                                     1, encap_map_id,
                                     SAI_TUNNEL_ATTR_DECAP_MAPPERS,
                                     1, decap_map_id);

    sai_test_vxlan_assert_success(status, assert_success);
}

void sai_test_vxlan_tunnel_remove(sai_object_id_t tunnel_id,
                                  bool expect_success)
{
    sai_status_t status;

    status = saiTunnelTest::sai_test_tunnel_remove (tunnel_id);

    sai_test_vxlan_expect_success(status, expect_success);
}

void sai_test_vxlan_access_port_create(sai_object_id_t *bridge_sub_port_id,
                                       sai_object_id_t bridge_id,
                                       sai_object_id_t port_id,
                                       sai_uint32_t vlan_id,
                                       bool assert_success)

{
    sai_status_t status;

    status = saiTunnelTest::sai_test_bridge_port_create(bridge_sub_port_id, 5,
                                          SAI_BRIDGE_PORT_ATTR_TYPE,
                                          SAI_BRIDGE_PORT_TYPE_SUB_PORT,
                                          SAI_BRIDGE_PORT_ATTR_BRIDGE_ID,
                                          bridge_id,
                                          SAI_BRIDGE_PORT_ATTR_PORT_ID,
                                          port_id,
                                          SAI_BRIDGE_PORT_ATTR_VLAN_ID,
                                          vlan_id,
                                          SAI_BRIDGE_PORT_ATTR_ADMIN_STATE,
                                          true);

    sai_test_vxlan_assert_success(status, assert_success);
}

void sai_test_vxlan_1q_bridge_port_create(sai_object_id_t *bridge_port_id,
                                          sai_object_id_t lag_id,
                                          bool assert_success)
{
    sai_status_t status;

    status = saiTunnelTest::sai_test_bridge_port_create (bridge_port_id, 3,
                                                         SAI_BRIDGE_PORT_ATTR_TYPE,
                                                         SAI_BRIDGE_PORT_TYPE_PORT,
                                                         SAI_BRIDGE_PORT_ATTR_PORT_ID,
                                                         lag_id,
                                                         SAI_BRIDGE_PORT_ATTR_ADMIN_STATE,
                                                         true);

    sai_test_vxlan_assert_success(status, assert_success);

}

void sai_test_vxlan_tunnel_port_create(sai_object_id_t *bridge_tunnel_port_id,
                                       sai_object_id_t bridge_id,
                                       sai_object_id_t tunnel_id,
                                       bool assert_success)
{
    sai_status_t status;

    status = saiTunnelTest::sai_test_bridge_port_create (bridge_tunnel_port_id, 4,
                                          SAI_BRIDGE_PORT_ATTR_TYPE,
                                          SAI_BRIDGE_PORT_TYPE_TUNNEL,
                                          SAI_BRIDGE_PORT_ATTR_BRIDGE_ID,
                                          bridge_id,
                                          SAI_BRIDGE_PORT_ATTR_TUNNEL_ID,
                                          tunnel_id,
                                          SAI_BRIDGE_PORT_ATTR_ADMIN_STATE,
                                          true);

    sai_test_vxlan_assert_success(status, assert_success);
}

void sai_test_vxlan_bridge_port_remove(sai_object_id_t bridge_port_id,
                                       bool expect_success)
{
    sai_status_t status;

    status = saiTunnelTest::sai_test_bridge_port_remove (bridge_port_id);

    sai_test_vxlan_expect_success(status, expect_success);
}

void sai_test_vxlan_default_1q_port_create(sai_object_id_t port_id,
                                           bool expect_success)
{
    sai_status_t status;

    status = sai_bridge_ut_init_bridge_port_configs(saiL3Test::switch_id, port_id);

    sai_test_vxlan_expect_success(status, expect_success);
}

void sai_test_vxlan_default_1q_port_remove(sai_object_id_t port_id,
                                          bool expect_success)
{
    sai_status_t status;

    status = sai_bridge_ut_clean_bridge_port_configs(saiL3Test::switch_id, port_id);

    sai_test_vxlan_expect_success(status, expect_success);
}

void sai_test_vxlan_bridge_port_admin_state_set(sai_object_id_t bridge_port_id,
                                                bool enable, bool expect_success)
{
    sai_status_t status;

    status = saiTunnelTest::sai_test_bridge_port_set (bridge_port_id,
                                                      SAI_BRIDGE_PORT_ATTR_ADMIN_STATE,
                                                      enable);

    sai_test_vxlan_expect_success(status, expect_success);
}

void sai_test_vxlan_bridge_port_learn_mode_set(sai_object_id_t bridge_port_id,
                                               sai_bridge_port_fdb_learning_mode_t mode,
                                               bool expect_success)
{
    sai_status_t status;

    status = saiTunnelTest::sai_test_bridge_port_set (bridge_port_id,
                                                      SAI_BRIDGE_PORT_ATTR_FDB_LEARNING_MODE,
                                                      mode);

    sai_test_vxlan_expect_success(status, expect_success);
}

void sai_test_vxlan_tunnel_term_create(sai_object_id_t *tunnel_term_id,
                                       sai_object_id_t vr_id,
                                       const char *term_sip,
                                       const char *term_dip,
                                       sai_object_id_t tunnel_id,
                                       bool assert_success)
{
    sai_status_t status;

    status = saiTunnelTest::sai_test_tunnel_term_entry_create (tunnel_term_id, 6,
                                                SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_VR_ID,
                                                vr_id,
                                                SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_TYPE,
                                                SAI_TUNNEL_TERM_TABLE_ENTRY_TYPE_P2P,
                                                SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_DST_IP,
                                                SAI_IP_ADDR_FAMILY_IPV4, term_dip,
                                                SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_SRC_IP,
                                                SAI_IP_ADDR_FAMILY_IPV4, term_sip,
                                                SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_TUNNEL_TYPE,
                                                SAI_TUNNEL_TYPE_VXLAN,
                                                SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_ACTION_TUNNEL_ID,
                                                tunnel_id);

    sai_test_vxlan_assert_success(status, assert_success);
}

void sai_test_vxlan_tunnel_term_remove(sai_object_id_t tunnel_term_id,
                                       bool expect_success)
{
    sai_status_t status;

    status = saiTunnelTest::sai_test_tunnel_term_entry_remove(tunnel_term_id);

    sai_test_vxlan_expect_success(status, expect_success);
}

void sai_test_vxlan_fdb_entry_create(sai_object_id_t bv_id,
                                     const char *p_mac_str,
                                     sai_object_id_t bridge_port_id,
                                     bool is_tunnel_port,
                                     const char *p_remote_ip,
                                     bool assert_success)
{
    sai_status_t status;

    status = saiTunnelTest::sai_test_tunnel_fdb_entry_create(SAI_FDB_ENTRY_TYPE_DYNAMIC,
                                                             bv_id, p_mac_str,
                                                             SAI_PACKET_ACTION_FORWARD,
                                                             bridge_port_id,
                                                             is_tunnel_port,
                                                             p_remote_ip);
    sai_test_vxlan_assert_success(status, assert_success);
}

void sai_test_vxlan_fdb_entry_remove(sai_object_id_t bv_id,
                                     const char *p_mac_str,
                                     bool expect_success)
{
    sai_status_t status;

    status = saiTunnelTest::sai_test_tunnel_fdb_entry_remove(bv_id, p_mac_str);

    sai_test_vxlan_expect_success(status, expect_success);
}

void sai_test_vxlan_l2mc_group_create(sai_object_id_t *l2mc_group_id,
                                      bool assert_success)
{
    sai_status_t status;

    status = saiTunnelTest::sai_test_tunnel_l2mc_group_create(l2mc_group_id);

    sai_test_vxlan_assert_success(status, assert_success);
}

void sai_test_vxlan_l2mc_group_remove(sai_object_id_t l2mc_group_id,
                                      bool expect_success)
{
    sai_status_t status;

    status = saiTunnelTest::sai_test_tunnel_l2mc_group_remove(l2mc_group_id);

    sai_test_vxlan_expect_success(status, expect_success);
}

void sai_test_vxlan_l2mc_group_member_create(sai_object_id_t *l2mc_group_member_id,
                                             sai_object_id_t l2mc_group_id,
                                             sai_object_id_t bridge_port_id,
                                             bool is_tunnel_bridge_port,
                                             const char *p_remote_ip,
                                             bool assert_success)
{
    sai_status_t status;
    unsigned int attr_count = 2;

    if(is_tunnel_bridge_port) {
        attr_count ++;
    }

    if(is_tunnel_bridge_port) {
        status = saiTunnelTest::sai_test_tunnel_l2mc_group_member_create(l2mc_group_member_id,
                                                       attr_count,
                                                       SAI_L2MC_GROUP_MEMBER_ATTR_L2MC_GROUP_ID,
                                                       l2mc_group_id,
                                                       SAI_L2MC_GROUP_MEMBER_ATTR_L2MC_OUTPUT_ID,
                                                       bridge_port_id,
                                                       SAI_L2MC_GROUP_MEMBER_ATTR_L2MC_ENDPOINT_IP,
                                                       p_remote_ip);
    } else {
        status = saiTunnelTest::sai_test_tunnel_l2mc_group_member_create(l2mc_group_member_id,
                                                       attr_count,
                                                       SAI_L2MC_GROUP_MEMBER_ATTR_L2MC_GROUP_ID,
                                                       l2mc_group_id,
                                                       SAI_L2MC_GROUP_MEMBER_ATTR_L2MC_OUTPUT_ID,
                                                       bridge_port_id);
    }

    sai_test_vxlan_assert_success(status, assert_success);
}

void sai_test_vxlan_l2mc_group_member_remove(sai_object_id_t l2mc_group_member_id,
                                             bool expect_success)
{
    sai_status_t status;

    status = saiTunnelTest::sai_test_tunnel_l2mc_group_member_remove(l2mc_group_member_id);

    sai_test_vxlan_expect_success(status, expect_success);
}

void sai_test_vxlan_lag_create(sai_object_id_t *lag_id, bool assert_success)
{
    sai_status_t status;

    status = saiTunnelTest::sai_test_tunnel_lag_create(lag_id);

    sai_test_vxlan_assert_success(status, assert_success);
}

void sai_test_vxlan_lag_remove(sai_object_id_t lag_id, bool expect_success)
{
    sai_status_t status;

    status = saiTunnelTest::sai_test_tunnel_lag_remove(lag_id);

    sai_test_vxlan_expect_success(status, expect_success);
}

void sai_test_vxlan_lag_member_create(sai_object_id_t *lag_member_id,
                                      sai_object_id_t lag_id,
                                      sai_object_id_t port_id,
                                      bool assert_success)
{
    sai_status_t status;

    status = saiTunnelTest::sai_test_tunnel_lag_member_create(lag_member_id,
                                                              lag_id, port_id);

    sai_test_vxlan_assert_success(status, assert_success);
}

void sai_test_vxlan_lag_member_remove(sai_object_id_t lag_member_id,
                                      bool expect_success)
{
    sai_status_t status;

    status = saiTunnelTest::sai_test_tunnel_lag_member_remove(lag_member_id);

    sai_test_vxlan_expect_success(status, expect_success);
}

void sai_test_vxlan_port_lag_rif_create(sai_object_id_t *port_lag_rif_id, sai_object_id_t vr_id,
                                        sai_object_id_t port_lag_id, bool assert_success)
{
     sai_status_t status;

     status = saiL3Test::sai_test_rif_create (port_lag_rif_id,
                                              3,
                                              SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                              vr_id,
                                              SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                              SAI_ROUTER_INTERFACE_TYPE_PORT,
                                              SAI_ROUTER_INTERFACE_ATTR_PORT_ID,
                                              port_lag_id);

     sai_test_vxlan_assert_success(status, assert_success);
}

void sai_test_vxlan_rif_remove(sai_object_id_t rif_id, bool expect_success)
{
    sai_status_t status;

    status = saiL3Test::sai_test_rif_remove(rif_id);

    sai_test_vxlan_expect_success(status, expect_success);
}

void sai_test_vxlan_vlan_rif_create(sai_object_id_t *vlan_rif_id, sai_object_id_t vr_id,
                                    const unsigned int vlan_id, bool assert_success)
{
    sai_status_t status;

    status = saiL3Test::sai_test_rif_create (vlan_rif_id,
                                             3,
                                             SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                             vr_id,
                                             SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                             SAI_ROUTER_INTERFACE_TYPE_VLAN,
                                             SAI_ROUTER_INTERFACE_ATTR_VLAN_ID,
                                             vlan_id);

    sai_test_vxlan_assert_success(status, assert_success);
}

void sai_test_vxlan_vlan_create(sai_object_id_t *vlan_obj_id,
                                const unsigned int vlan_id, bool assert_success)
{
    sai_status_t status;

    status = saiL3Test::sai_test_vlan_create(vlan_obj_id, vlan_id);

    sai_test_vxlan_assert_success(status, assert_success);
}

void sai_test_vxlan_vlan_remove(sai_object_id_t vlan_obj_id, bool expect_success)
{
    sai_status_t status;

    status = saiL3Test::sai_test_vlan_remove(vlan_obj_id);

    sai_test_vxlan_expect_success(status, expect_success);
}

void sai_test_vxlan_nexthop_create(sai_object_id_t *next_hop_id, sai_object_id_t rif_id,
                                   sai_ip_addr_family_t ip_af, const char *p_nh_ip_addr,
                                   bool assert_success)
{
    sai_status_t status;

    status = saiL3Test::sai_test_nexthop_create (next_hop_id, 3,
                                      SAI_NEXT_HOP_ATTR_TYPE,
                                      SAI_NEXT_HOP_TYPE_IP,
                                      SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
                                      rif_id,
                                      SAI_NEXT_HOP_ATTR_IP,
                                      ip_af, p_nh_ip_addr);

    sai_test_vxlan_assert_success(status, assert_success);
}

void sai_test_vxlan_nexthop_remove(sai_object_id_t next_hop_id, bool expect_success)
{
    sai_status_t status;

    status = saiL3Test::sai_test_nexthop_remove(next_hop_id);

    sai_test_vxlan_expect_success(status, expect_success);
}

void sai_test_vxlan_neighbor_create(sai_object_id_t rif_id, sai_ip_addr_family_t ip_af,
                                    const char *ip_str, const char *mac_str, bool assert_success)
{
    sai_status_t status;

    status = saiL3Test::sai_test_neighbor_create (rif_id, ip_af, ip_str,
                                                  1, SAI_NEIGHBOR_ENTRY_ATTR_DST_MAC_ADDRESS,
                                                  mac_str);

    sai_test_vxlan_assert_success(status, assert_success);
}

void sai_test_vxlan_neighbor_remove(sai_object_id_t rif_id, sai_ip_addr_family_t ip_af,
                                    const char *ip_str, bool expect_success)
{
    sai_status_t status;

    status = saiL3Test::sai_test_neighbor_remove (rif_id, ip_af, ip_str);

    sai_test_vxlan_expect_success(status, expect_success);
}

void sai_test_vxlan_route_create(sai_object_id_t vr_id, sai_ip_addr_family_t ip_af,
                                 const char *prefix_str, unsigned int prefix_len,
                                 sai_object_id_t nh_id, bool assert_success)
{
    sai_status_t status;

    status = saiL3Test::sai_test_route_create (vr_id, ip_af, prefix_str, prefix_len,
                                               1, SAI_ROUTE_ENTRY_ATTR_NEXT_HOP_ID,
                                               nh_id);

    sai_test_vxlan_assert_success(status, assert_success);
}

void sai_test_vxlan_route_remove(sai_object_id_t vr_id, sai_ip_addr_family_t ip_af,
                                 const char *prefix_str, unsigned int prefix_len,
                                 bool expect_success)
{
    sai_status_t status;

    status = saiL3Test::sai_test_route_remove (vr_id, ip_af, prefix_str,
                                               prefix_len);

    sai_test_vxlan_expect_success(status, expect_success);
}

void sai_test_vxlan_nh_group_create(sai_object_id_t *nh_group_id, bool assert_success)
{
    sai_status_t status;

    status = saiL3Test::sai_test_nh_group_create_no_nh_list(nh_group_id,
                                                     SAI_NEXT_HOP_GROUP_TYPE_ECMP);

    sai_test_vxlan_assert_success(status, assert_success);
}

void sai_test_vxlan_nh_group_remove(sai_object_id_t nh_group_id, bool expect_success)
{
    sai_status_t status;

    status = saiL3Test::sai_test_nh_group_remove(nh_group_id);

    sai_test_vxlan_expect_success(status, expect_success);
}

void sai_test_vxlan_add_nh_to_group (sai_object_id_t nh_group_id,
                                     unsigned int nh_count,
                                     sai_object_id_t *p_nh_list,
                                     bool assert_success)
{
    sai_status_t status;

    status = saiL3Test::sai_test_add_nh_to_group(nh_group_id, nh_count,
                                                 p_nh_list);

    sai_test_vxlan_assert_success(status, assert_success);
}

void sai_test_vxlan_remove_nh_from_group (sai_object_id_t nh_group_id,
                                          unsigned int nh_count,
                                          sai_object_id_t *p_nh_list,
                                          bool expect_success)
{
    sai_status_t status;

    status = saiL3Test::sai_test_remove_nh_from_group(nh_group_id, nh_count,
                                                      p_nh_list);

    sai_test_vxlan_expect_success(status, expect_success);
}

typedef struct sai_hw_port_info_t {
    uint32_t hw_lane;
    sai_object_id_t port_id;
    bool port_valid;
}sai_hw_port_info;

static sai_hw_port_info port_info[SAI_MAX_PORTS] = {0};

static void sai_get_port_hw_lane_map_table()
{
    sai_attribute_t sai_port_attr,sai_lane_attr;
    uint8_t count = 0, lane = 0;
    sai_status_t ret = SAI_STATUS_SUCCESS;
    sai_object_id_t port_list[SAI_MAX_PORTS] = {0};
    uint8_t lane_count = 0;
    uint32_t port_idx = 0;
    uint32_t port_count = 0;
    sai_object_id_t switch_id = 0;
    sai_switch_api_t *sai_switch_api_table = saiTunnelTest::switch_api_tbl_get();
    sai_port_api_t *sai_port_api_table = saiTunnelTest::port_api_tbl_get();

    memset (&sai_port_attr, 0, sizeof (sai_port_attr));
    memset (&sai_lane_attr, 0, sizeof (sai_lane_attr));

    sai_port_attr.id = SAI_SWITCH_ATTR_PORT_LIST;
    sai_port_attr.value.objlist.count = SAI_MAX_PORTS;
    sai_port_attr.value.objlist.list  = port_list;

    sai_switch_api_table->get_switch_attribute(switch_id,1,&sai_port_attr);
    port_count = sai_port_attr.value.objlist.count;

    sai_lane_attr.id = SAI_PORT_ATTR_HW_LANE_LIST;

    sai_lane_attr.value.u32list.count = 1;
    sai_lane_attr.value.u32list.list = (uint32_t *)calloc (1, sizeof(uint32_t));

    ASSERT_TRUE(sai_lane_attr.value.u32list.list != NULL);

    for (count = 0;count < port_count;count ++) {
        ret = sai_port_api_table->get_port_attribute(port_list[count], 1, &sai_lane_attr);

        /* Handles buffer overflow case, by reallocating required memory */
        if(ret == SAI_STATUS_BUFFER_OVERFLOW ) {
            free(sai_lane_attr.value.u32list.list);
            sai_lane_attr.value.u32list.list = (uint32_t *)calloc (sai_lane_attr.value.u32list.count,
                                                                   sizeof(uint32_t));
            ret = sai_port_api_table->get_port_attribute(port_list[count], 1, &sai_lane_attr);
        }

        if(ret != SAI_STATUS_SUCCESS) {
            EXPECT_EQ(SAI_STATUS_SUCCESS, ret);
            continue;
        }

        for(lane = 0; lane < sai_lane_attr.value.u32list.count; lane++ ) {
            port_info[lane_count].hw_lane = sai_lane_attr.value.u32list.list[lane];
            port_info[lane_count].port_id = port_list[count];
            if(lane == 0) {
                port_info[lane_count].port_valid = true;
            }
            lane_count ++;
        }
    }

    /*print port dump*/
    printf("Port dump list :\n");
    for(port_idx = 0; port_idx < port_count; port_idx++) {
        printf("port_idx:%d port_id:0x%" PRIx64 " hw_lane:%d isvalid:%s \n",
               port_idx,port_info[port_idx].port_id,port_info[port_idx].hw_lane,
               port_info[port_idx].port_valid ? "True":"False");
    }
    free(sai_lane_attr.value.u32list.list);
}

sai_object_id_t sai_get_port_id_from_port_info(unsigned port_idx)
{
    return port_info[port_idx].port_id;
}

sai_status_t sai_set_flex_port_configuration(unsigned int port,
                                         sai_port_breakout_mode_type_t breakout_mode,
                                         uint32_t speed)
{
    sai_attribute_t sai_attr_set[2];
    sai_attribute_t sai_attr_get;
    sai_status_t ret = SAI_STATUS_SUCCESS;
    int32_t current_mode = 0;
    uint32_t current_speed = 0;
    unsigned int count = 0;
    static bool port_info_init = false;
    uint32_t port_lane_list[SAI_PORT_FOUR_LANES] = {0};
    sai_object_id_t switch_id = 0;
    sai_port_api_t *p_sai_port_api_tbl = saiTunnelTest::port_api_tbl_get();

    if(!port_info_init) {
        sai_get_port_hw_lane_map_table();
        port_info_init = true;
    }

    memset(&sai_attr_get, 0, sizeof(sai_attribute_t));
    memset(sai_attr_set, 0, sizeof(sai_attr_set));

    sai_attr_get.id = SAI_PORT_ATTR_CURRENT_BREAKOUT_MODE_TYPE;
    ret = p_sai_port_api_tbl->get_port_attribute(port_info[port].port_id, 1, &sai_attr_get);
    EXPECT_EQ(SAI_STATUS_SUCCESS, ret);

    current_mode = sai_attr_get.value.s32;
    if (current_mode == breakout_mode){
        memset(&sai_attr_get, 0, sizeof(sai_attribute_t));
        sai_attr_get.id = SAI_PORT_ATTR_SPEED;
        ret = p_sai_port_api_tbl->get_port_attribute(
                                   port_info[port].port_id, 1, &sai_attr_get);
        EXPECT_EQ(SAI_STATUS_SUCCESS, ret);
        current_speed = sai_attr_get.value.u32;
        if (current_speed == speed) {
            return SAI_STATUS_SUCCESS;
        }
    }

    if(current_mode == SAI_PORT_BREAKOUT_MODE_TYPE_1_LANE) {
        EXPECT_EQ(SAI_STATUS_SUCCESS,
                  sai_bridge_ut_clean_bridge_port_configs(switch_id, port_info[port].port_id));
        ret = p_sai_port_api_tbl->remove_port(port_info[port].port_id);
    } else if (current_mode == SAI_PORT_BREAKOUT_MODE_TYPE_2_LANE) {
        for (count = 0; count < SAI_PORT_TWO_LANES; count ++) {
            EXPECT_EQ(SAI_STATUS_SUCCESS,
                      sai_bridge_ut_clean_bridge_port_configs(switch_id,
                      port_info[port+(count*SAI_PORT_FOUR_LANES)].port_id));
            ret = p_sai_port_api_tbl->remove_port(
                               port_info[port+(count*SAI_PORT_FOUR_LANES)].port_id);
            if (ret != SAI_STATUS_SUCCESS){
                break;
            }
        }
    } else {
        for (count = 0; count < SAI_PORT_FOUR_LANES; count++) {
            EXPECT_EQ(SAI_STATUS_SUCCESS,
                  sai_bridge_ut_clean_bridge_port_configs(switch_id,
                  port_info[port+count].port_id));
            ret = p_sai_port_api_tbl->remove_port(port_info[port+count].port_id);
            if (ret != SAI_STATUS_SUCCESS){
                break;
            }
        }
    }
    EXPECT_EQ(SAI_STATUS_SUCCESS, ret);

    if (SAI_PORT_BREAKOUT_MODE_TYPE_4_LANE == breakout_mode) {
        for (count = 0; count < SAI_PORT_FOUR_LANES; count++) {
            sai_attr_set[0].id = SAI_PORT_ATTR_HW_LANE_LIST;
            sai_attr_set[0].value.u32list.count = 1;
            sai_attr_set[0].value.u32list.list = &port_info[port+count].hw_lane;

            sai_attr_set[1].id = SAI_PORT_ATTR_SPEED;
            sai_attr_set[1].value.u32 = speed;
            ret = p_sai_port_api_tbl->create_port(
                                  &port_info[port + count].port_id,switch_id,
                                  2, sai_attr_set);
            EXPECT_EQ(SAI_STATUS_SUCCESS, ret);
            EXPECT_EQ(SAI_STATUS_SUCCESS,
                      sai_bridge_ut_init_bridge_port_configs(switch_id,
                                          port_info[port + count].port_id));
        }
    } else if (SAI_PORT_BREAKOUT_MODE_TYPE_1_LANE == breakout_mode) {
        sai_attr_set[0].id = SAI_PORT_ATTR_HW_LANE_LIST;
        sai_attr_set[0].value.u32list.count = 4;
        for (count = 0; count < SAI_PORT_FOUR_LANES; count++) {
            port_lane_list[count] = port_info[port+count].hw_lane;
        }
        sai_attr_set[0].value.u32list.list = port_lane_list;

        sai_attr_set[1].id = SAI_PORT_ATTR_SPEED;
        sai_attr_set[1].value.u32 = speed;
        ret = p_sai_port_api_tbl->create_port(
                                       &port_info[port].port_id, switch_id,2,sai_attr_set);
        EXPECT_EQ(SAI_STATUS_SUCCESS, ret);
        EXPECT_EQ(SAI_STATUS_SUCCESS,
                sai_bridge_ut_init_bridge_port_configs(switch_id,
                    port_info[port].port_id));
    } else if (SAI_PORT_BREAKOUT_MODE_TYPE_2_LANE == breakout_mode) {
        for (count = 0; count < SAI_PORT_TWO_LANES; count++) {
            if (count == 1) {
                port = port + 1;
            }
            sai_attr_set[0].id = SAI_PORT_ATTR_HW_LANE_LIST;
            sai_attr_set[0].value.u32list.count = 2;
            sai_attr_set[0].value.u32list.list = &port_info[port + count].hw_lane;
            sai_attr_set[0].value.u32list.list = &port_info[port + count + 1].hw_lane;

            sai_attr_set[1].id = SAI_PORT_ATTR_SPEED;
            sai_attr_set[1].value.u32 = speed;
            ret = p_sai_port_api_tbl->create_port(
                                     &port_info[port + count * SAI_PORT_TWO_LANES].port_id,
                                     switch_id,2,sai_attr_set);
            EXPECT_EQ(SAI_STATUS_SUCCESS, ret);
            EXPECT_EQ(SAI_STATUS_SUCCESS,
                    sai_bridge_ut_init_bridge_port_configs(switch_id,
                        port_info[port + count*SAI_PORT_TWO_LANES].port_id));
        }
    }

    sai_get_port_hw_lane_map_table();

    return ret;
}
