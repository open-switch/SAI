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
* @file  sai_vxlan_tunnel_unit_test_utils.h
*
* @brief This file contains utility functions definitions for VXLAN
*        Tunnel test cases.
*
*************************************************************************/
#ifndef __SAI_VXLAN_TUNNEL_UNIT_TEST_UTILS_H__
#define __SAI_VXLAN_TUNNEL_UNIT_TEST_UTILS_H__

extern "C" {
#include "sai.h"
#include "saitypes.h"
#include "saistatus.h"
#include "saiport.h"
#include "saitunnel.h"
#include "saibridge.h"
#include <string.h>
}

#define SAI_MAX_PORTS  256
#define SAI_PORT_FOUR_LANES 4
#define SAI_PORT_TWO_LANES 2
#define SAI_PORT_ONE_LANE 1
#define SAI_PORT_SPEED_HUNDRED_GIG 100000
#define SAI_PORT_SPEED_FIFTY_GIG 50000
#define SAI_PORT_SPEED_FORTY_GIG 40000
#define SAI_PORT_SPEED_TWENTY_FIVE_GIG 25000
#define SAI_PORT_SPEED_TEN_GIG 10000

void sai_test_vxlan_assert_success(sai_status_t status, bool assert_success);

void sai_test_vxlan_expect_success(sai_status_t status, bool expect_success);

void sai_test_vxlan_1d_bridge_create(sai_object_id_t *bridge_id, bool assert_success);

void sai_test_vxlan_1d_bridge_remove(sai_object_id_t bridge_id, bool expect_success);

void sai_test_vxlan_1d_bridge_l2mc_groups_set(sai_object_id_t bridge_id,
                                              bool unknown_unicast_l2mc_enable,
                                              sai_object_id_t unknown_unicast_l2mc_group,
                                              bool unknown_mcast_l2mc_enable,
                                              sai_object_id_t unknown_mcast_l2mc_group,
                                              bool broadcast_l2mc_enable,
                                              sai_object_id_t broadcast_l2mc_group,
                                              bool expect_success);

void sai_test_vxlan_encap_tunnel_map_create(sai_object_id_t *encap_map_id,
                                            bool assert_success);

void sai_test_vxlan_decap_tunnel_map_create(sai_object_id_t *decap_map_id,
                                            bool assert_success);

void sai_test_vxlan_tunnel_map_remove(sai_object_id_t tunnel_map_id,
                                      bool expect_success);

void sai_test_vxlan_encap_map_entry_create(sai_object_id_t *encap_map_entry_id,
                                           sai_object_id_t encap_map_id,
                                           sai_object_id_t bridge_id,
                                           sai_uint32_t vnid,
                                           bool assert_success);

void sai_test_vxlan_decap_map_entry_create(sai_object_id_t *decap_map_entry_id,
                                           sai_object_id_t decap_map_id,
                                           sai_uint32_t vnid,
                                           sai_object_id_t bridge_id,
                                           bool assert_success);

void sai_test_vxlan_tunnel_map_entry_remove(sai_object_id_t tunnel_map_entry_id,
                                            bool expect_success);

void sai_test_vxlan_encap_map_entry_set(sai_object_id_t tunnel_map_entry_id,
                                        sai_uint32_t vnid, bool expect_success);

void sai_test_vxlan_decap_map_entry_set(sai_object_id_t tunnel_map_entry_id,
                                        sai_object_id_t bridge_id, bool expect_success);

void sai_test_vxlan_encap_map_entry_get(sai_object_id_t tunnel_map_entry_id,
                                        unsigned int attr_count,
                                        sai_attribute_t *p_attr_list,
                                        bool expect_success);

void sai_test_vxlan_encap_map_entry_verify_bridge(unsigned int attr_count,
                                                  sai_attribute_t *p_attr_list,
                                                  sai_object_id_t bridge_id);

void sai_test_vxlan_encap_map_entry_verify_vnid(unsigned int attr_count,
                                                sai_attribute_t *p_attr_list,
                                                sai_uint32_t vnid);

void sai_test_vxlan_decap_map_entry_get(sai_object_id_t tunnel_map_entry_id,
                                        unsigned int attr_count,
                                        sai_attribute_t *p_attr_list,
                                        bool expect_success);

void sai_test_vxlan_decap_map_entry_verify_bridge(unsigned int attr_count,
                                                  sai_attribute_t *p_attr_list,
                                                  sai_object_id_t bridge_id);

void sai_test_vxlan_decap_map_entry_verify_vnid(unsigned int attr_count,
                                                sai_attribute_t *p_attr_list,
                                                sai_uint32_t vnid);

void sai_test_vxlan_tunnel_create(sai_object_id_t *tunnel_id,
                                  sai_object_id_t underlay_rif,
                                  sai_object_id_t overlay_rif,
                                  const char *tunnel_sip,
                                  sai_object_id_t encap_map_id,
                                  sai_object_id_t decap_map_id,
                                  bool assert_success);

void sai_test_vxlan_tunnel_remove(sai_object_id_t tunnel_id,
                                  bool expect_success);

void sai_test_vxlan_access_port_create(sai_object_id_t *bridge_sub_port_id,
                                       sai_object_id_t bridge_id,
                                       sai_object_id_t port_id,
                                       sai_uint32_t vlan_id,
                                       bool assert_success);

void sai_test_vxlan_tunnel_port_create(sai_object_id_t *bridge_tunnel_port_id,
                                       sai_object_id_t bridge_id,
                                       sai_object_id_t tunnel_id,
                                       bool assert_success);

void sai_test_vxlan_1q_bridge_port_create(sai_object_id_t *bridge_port_id,
                                          sai_object_id_t lag_id,
                                          bool assert_success);

void sai_test_vxlan_default_1q_port_create(sai_object_id_t port_id,
                                           bool expect_success);

void sai_test_vxlan_default_1q_port_remove(sai_object_id_t port_id,
                                           bool expect_success);

void sai_test_vxlan_bridge_port_remove(sai_object_id_t bridge_port_id,
                                       bool expect_success);

void sai_test_vxlan_bridge_port_admin_state_set(sai_object_id_t bridge_port_id,
                                                bool enable, bool expect_success);

void sai_test_vxlan_bridge_port_learn_mode_set(sai_object_id_t bridge_port_id,
                                               sai_bridge_port_fdb_learning_mode_t mode,
                                               bool expect_success);

void sai_test_vxlan_tunnel_term_create(sai_object_id_t *tunnel_term_id,
                                       sai_object_id_t vr_id,
                                       const char *term_sip,
                                       const char *term_dip,
                                       sai_object_id_t tunnel_id,
                                       bool assert_success);

void sai_test_vxlan_tunnel_term_remove(sai_object_id_t tunnel_term_id,
                                       bool expect_success);

void sai_test_vxlan_fdb_entry_create(sai_object_id_t bv_id,
                                     const char *p_mac_str,
                                     sai_object_id_t bridge_port_id,
                                     bool is_tunnel_port,
                                     const char *p_remote_ip,
                                     bool assert_success);

void sai_test_vxlan_fdb_entry_remove(sai_object_id_t bv_id,
                                     const char *p_mac_str,
                                     bool expect_success);

void sai_test_vxlan_l2mc_group_create(sai_object_id_t *l2mc_group_id,
                                      bool assert_success);

void sai_test_vxlan_l2mc_group_remove(sai_object_id_t l2mc_group_id,
                                      bool expect_success);

void sai_test_vxlan_l2mc_group_member_create(sai_object_id_t *l2mc_group_member_id,
                                             sai_object_id_t l2mc_group_id,
                                             sai_object_id_t bridge_port_id,
                                             bool is_tunnel_bridge_port,
                                             const char *p_remote_ip,
                                             bool assert_success);

void sai_test_vxlan_l2mc_group_member_remove(sai_object_id_t l2mc_group_member_id,
                                             bool expect_success);

void sai_test_vxlan_lag_create(sai_object_id_t *lag_id, bool assert_success);

void sai_test_vxlan_lag_remove(sai_object_id_t lag_id, bool expect_success);

void sai_test_vxlan_lag_member_create(sai_object_id_t *lag_member_id,
                                      sai_object_id_t lag_id, sai_object_id_t port_id,
                                      bool assert_success);

void sai_test_vxlan_lag_member_remove(sai_object_id_t lag_member_id,
                                      bool expect_success);

void sai_test_vxlan_port_lag_rif_create(sai_object_id_t *port_lag_rif_id, sai_object_id_t vr_id,
                                        sai_object_id_t port_lag_id, bool assert_success);

void sai_test_vxlan_vlan_rif_create(sai_object_id_t *vlan_rif_id, sai_object_id_t vr_id,
                                    unsigned int vlan_id, bool assert_success);

void sai_test_vxlan_rif_remove(sai_object_id_t port_lag_rif_id, bool expect_success);

void sai_test_vxlan_vlan_create(sai_object_id_t *vlan_obj_id, const unsigned int vlan_id,
                                bool assert_success);

void sai_test_vxlan_vlan_remove(sai_object_id_t vlan_obj_id, bool expect_success);

void sai_test_vxlan_nexthop_create(sai_object_id_t *next_hop_id, sai_object_id_t rif_id,
                                   sai_ip_addr_family_t ip_af, const char *p_nh_ip_addr,
                                   bool assert_success);

void sai_test_vxlan_nexthop_remove(sai_object_id_t next_hop_id, bool expect_success);

void sai_test_vxlan_neighbor_create(sai_object_id_t rif_id, sai_ip_addr_family_t ip_af,
                                    const char *ip_str, const char *mac_str, bool assert_success);

void sai_test_vxlan_neighbor_remove(sai_object_id_t rif_id, sai_ip_addr_family_t ip_af,
                                    const char *ip_str, bool expect_success);

void sai_test_vxlan_route_create(sai_object_id_t vr_id, sai_ip_addr_family_t ip_af,
                                 const char *prefix_str, unsigned int prefix_len,
                                 sai_object_id_t nh_id, bool assert_success);

void sai_test_vxlan_route_remove(sai_object_id_t vr_id, sai_ip_addr_family_t ip_af,
                                 const char *prefix_str, unsigned int prefix_len,
                                 bool expect_success);

void sai_test_vxlan_nh_group_create(sai_object_id_t *nh_group_id, bool assert_success);

void sai_test_vxlan_nh_group_remove(sai_object_id_t nh_group_id, bool expect_success);

void sai_test_vxlan_add_nh_to_group (sai_object_id_t nh_group_id,
                                     unsigned int nh_count,
                                     sai_object_id_t *p_nh_list,
                                     bool assert_success);

void sai_test_vxlan_remove_nh_from_group (sai_object_id_t nh_group_id,
                                          unsigned int nh_count,
                                          sai_object_id_t *p_nh_list,
                                          bool expect_success);

sai_object_id_t sai_get_port_id_from_port_info(unsigned port_idx);

sai_status_t sai_set_flex_port_configuration(unsigned int port,
                                             sai_port_breakout_mode_type_t breakout_mode,
                                             uint32_t speed);
void sai_test_vxlan_flood_packet_on_vxlan_l3mc_index(sai_object_id_t bridgeId, sai_object_id_t l2mcIndex);
#endif
