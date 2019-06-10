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
* @file  sai_vxlan_tunnel_unit_test.cpp
*
* @brief This file contains test cases for VXLAN Tunnel functionality.
*
*************************************************************************/

#include "gtest/gtest.h"

#include "sai_tunnel_unit_test.h"
#include "sai_l3_unit_test_utils.h"
#include "sai_vxlan_tunnel_unit_test_utils.h"
#include <inttypes.h>
extern "C" {
#include "saitypes.h"
#include "saistatus.h"
#include "saibridge.h"
#include "sainexthopgroupextensions.h"
#include <string.h>
}

TEST_F (saiTunnelTest, create_and_remove_vxlan_tunnel_map_object)
{
    sai_object_id_t    encap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_id          = SAI_NULL_OBJECT_ID;

    sai_test_vxlan_encap_tunnel_map_create(&encap_map_id, true);
    sai_test_vxlan_decap_tunnel_map_create(&decap_map_id, true);

    sai_test_vxlan_tunnel_map_remove (decap_map_id, true);
    sai_test_vxlan_tunnel_map_remove (encap_map_id, true);
}

TEST_F (saiTunnelTest, create_and_remove_vxlan_tunnel_map_entry_object)
{
    sai_object_id_t    bridge_id             = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_id_1        = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_id_1        = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_entry_id_1  = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_entry_id_1  = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_id_2        = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_id_2        = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_entry_id_2  = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_entry_id_2  = SAI_NULL_OBJECT_ID;
    sai_uint32_t       vnid                  = 100;
    /*vnid range is from 0 - 16777215*/
    sai_uint32_t       invalid_vnid          = 16777216;

    sai_test_vxlan_1d_bridge_create(&bridge_id, true);

    sai_test_vxlan_encap_tunnel_map_create(&encap_map_id_1, true);
    sai_test_vxlan_decap_tunnel_map_create(&decap_map_id_1, true);
    sai_test_vxlan_encap_tunnel_map_create(&encap_map_id_2, true);
    sai_test_vxlan_decap_tunnel_map_create(&decap_map_id_2, true);


    sai_test_vxlan_encap_map_entry_create(&encap_map_entry_id_1, encap_map_id_1,
                                          bridge_id, vnid, true);

    sai_test_vxlan_decap_map_entry_create(&decap_map_entry_id_1, decap_map_id_1,
                                          vnid, bridge_id, true);

    /* Negative cases start*/

    /* Try creating encap/decap entry with invalid bridge id */
    sai_test_vxlan_encap_map_entry_create(&encap_map_entry_id_2, encap_map_id_1,
                                          bridge_id + 1, vnid, false);

    sai_test_vxlan_decap_map_entry_create(&decap_map_entry_id_2, decap_map_id_1,
                                          vnid, bridge_id + 1, false);

    /* Try creating encap/decap entry with invalid vnid */
    sai_test_vxlan_encap_map_entry_create(&encap_map_entry_id_2, encap_map_id_2,
                                          bridge_id, invalid_vnid , false);

    sai_test_vxlan_decap_map_entry_create(&decap_map_entry_id_2, decap_map_id_2,
                                          invalid_vnid, bridge_id + 1, false);

    /* Try creating duplicate encap entry for same bridge id in the same tunnel
     * map.*/
    sai_test_vxlan_encap_map_entry_create(&encap_map_entry_id_2, encap_map_id_1,
                                          bridge_id, vnid, false);

    /* Try creating encap map entry with decap map object.*/
    sai_test_vxlan_encap_map_entry_create(&encap_map_entry_id_2, decap_map_id_1,
                                          bridge_id, vnid, false);

    /* Try creating duplicate decap entry for same vnid in the same tunnel
     * map.*/
    sai_test_vxlan_decap_map_entry_create(&decap_map_entry_id_2, decap_map_id_1,
                                          vnid, bridge_id, false);

    /* Try creating decap map entry with encap map object.*/
    sai_test_vxlan_decap_map_entry_create(&decap_map_entry_id_2, encap_map_id_1,
                                          vnid, bridge_id, false);
    /*Negative test cases end*/

    sai_test_vxlan_encap_map_entry_create(&encap_map_entry_id_2, encap_map_id_2,
                                          bridge_id, vnid, true);

    sai_test_vxlan_decap_map_entry_create(&decap_map_entry_id_2, decap_map_id_2,
                                          vnid, bridge_id, true);

    sai_test_vxlan_tunnel_map_entry_remove (decap_map_entry_id_1, true);
    sai_test_vxlan_tunnel_map_entry_remove (encap_map_entry_id_1, true);
    sai_test_vxlan_tunnel_map_remove (decap_map_id_1, true);
    sai_test_vxlan_tunnel_map_remove (encap_map_id_1, true);
    sai_test_vxlan_tunnel_map_entry_remove (decap_map_entry_id_2, true);
    sai_test_vxlan_tunnel_map_entry_remove (encap_map_entry_id_2, true);
    sai_test_vxlan_tunnel_map_remove (decap_map_id_2, true);
    sai_test_vxlan_tunnel_map_remove (encap_map_id_2, true);

    sai_test_vxlan_1d_bridge_remove (bridge_id, true);
}

TEST_F (saiTunnelTest, set_and_get_vxlan_tunnel_map_entry_object)
{
    sai_object_id_t    bridge_id             = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_id_2           = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_id_1        = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_id_1        = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_entry_id_1  = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_entry_id_1  = SAI_NULL_OBJECT_ID;
    sai_uint32_t       vnid                  = 100;
    sai_uint32_t       vnid_2                = 200;
    sai_uint32_t       attr_count            = 4;
    sai_attribute_t    tunnel_map_entry_attr_list[attr_count];

    sai_kv_pair_t kvpair;
    kv_iter kviter;

    kvpair = kvpair_get();

    kviter = kvpair.find(std::string(SAI_KEY_VXLAN_SCALED_MODE_ENABLE));
    if (kviter == kvpair.end()) {
        printf("Test case can be run only in non-scaled mode\n");
        return;
    }

    /*If scale mode is disabled, skip test case*/
    if(kviter->second.c_str() == std::string("1")) {
        printf("Test case can be run only in non-scaled mode\n");
        return;
    }

    memset(&tunnel_map_entry_attr_list, 0, sizeof(tunnel_map_entry_attr_list));

    sai_test_vxlan_1d_bridge_create(&bridge_id, true);
    sai_test_vxlan_1d_bridge_create(&bridge_id_2, true);

    sai_test_vxlan_encap_tunnel_map_create(&encap_map_id_1, true);
    sai_test_vxlan_decap_tunnel_map_create(&decap_map_id_1, true);

    sai_test_vxlan_encap_map_entry_create(&encap_map_entry_id_1, encap_map_id_1,
                                          bridge_id, vnid, true);
    sai_test_vxlan_decap_map_entry_create(&decap_map_entry_id_1, decap_map_id_1,
                                          vnid, bridge_id, true);

    sai_test_vxlan_encap_map_entry_get(encap_map_entry_id_1, attr_count,
                                       tunnel_map_entry_attr_list, true);

    sai_test_vxlan_encap_map_entry_verify_bridge(attr_count,
                                                 tunnel_map_entry_attr_list,
                                                 bridge_id);

    sai_test_vxlan_encap_map_entry_verify_vnid(attr_count,
                                               tunnel_map_entry_attr_list,
                                               vnid);

    sai_test_vxlan_decap_map_entry_get(decap_map_entry_id_1, attr_count,
                                       tunnel_map_entry_attr_list, true);

    sai_test_vxlan_decap_map_entry_verify_bridge(attr_count,
                                                 tunnel_map_entry_attr_list,
                                                 bridge_id);

    sai_test_vxlan_decap_map_entry_verify_vnid(attr_count,
                                               tunnel_map_entry_attr_list,
                                               vnid);

    sai_test_vxlan_encap_map_entry_set(encap_map_entry_id_1, vnid_2, true);
    sai_test_vxlan_decap_map_entry_set(decap_map_entry_id_1, bridge_id_2, true);

    sai_test_vxlan_encap_map_entry_get(encap_map_entry_id_1, attr_count,
                                       tunnel_map_entry_attr_list, true);

    sai_test_vxlan_encap_map_entry_verify_vnid(attr_count,
                                               tunnel_map_entry_attr_list,
                                               vnid_2);

    sai_test_vxlan_decap_map_entry_get(decap_map_entry_id_1, attr_count,
                                       tunnel_map_entry_attr_list, true);

    sai_test_vxlan_decap_map_entry_verify_bridge(attr_count,
                                                 tunnel_map_entry_attr_list,
                                                 bridge_id_2);

    sai_test_vxlan_tunnel_map_entry_remove (decap_map_entry_id_1, true);
    sai_test_vxlan_tunnel_map_entry_remove (encap_map_entry_id_1, true);
    sai_test_vxlan_tunnel_map_remove (decap_map_id_1, true);
    sai_test_vxlan_tunnel_map_remove (encap_map_id_1, true);

    sai_test_vxlan_1d_bridge_remove (bridge_id, true);
    sai_test_vxlan_1d_bridge_remove (bridge_id_2, true);
}




TEST_F (saiTunnelTest, create_and_remove_vxlan_tunnel_object)
{
    sai_object_id_t    bridge_id             = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_entry_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_entry_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_id             = SAI_NULL_OBJECT_ID;
    const char         *tunnel_sip           = "10.0.0.1";
    sai_uint32_t       vnid                  = 100;

    sai_test_vxlan_1d_bridge_create(&bridge_id, true);

    sai_test_vxlan_encap_tunnel_map_create(&encap_map_id, true);
    sai_test_vxlan_decap_tunnel_map_create(&decap_map_id, true);

    sai_test_vxlan_encap_map_entry_create(&encap_map_entry_id, encap_map_id,
                                          bridge_id, vnid, true);
    sai_test_vxlan_decap_map_entry_create(&decap_map_entry_id, decap_map_id,
                                          vnid, bridge_id, true);

    sai_test_vxlan_tunnel_create(&tunnel_id, dflt_underlay_rif_id, dflt_overlay_rif_id,
                                 tunnel_sip, encap_map_id, decap_map_id, true);

    /*Negative test cases*/

    /*Try passing invalid underlay rif*/
    sai_test_vxlan_tunnel_create(&tunnel_id,SAI_NULL_OBJECT_ID, dflt_overlay_rif_id,
                                 tunnel_sip, encap_map_id, decap_map_id, false);

    /*Try passing invalid overlay rif*/
    sai_test_vxlan_tunnel_create(&tunnel_id,dflt_underlay_rif_id, SAI_NULL_OBJECT_ID,
                                 tunnel_sip, encap_map_id, decap_map_id, false);

    /*Try passing invalid encap map object*/
    sai_test_vxlan_tunnel_create(&tunnel_id, dflt_underlay_rif_id, dflt_overlay_rif_id,
                                 tunnel_sip, SAI_NULL_OBJECT_ID, decap_map_id, false);

    /*Try passing invalid decap map object*/
    sai_test_vxlan_tunnel_create(&tunnel_id, dflt_underlay_rif_id, dflt_overlay_rif_id,
                                 tunnel_sip, encap_map_id, SAI_NULL_OBJECT_ID, false);

    sai_test_vxlan_tunnel_remove (tunnel_id, true);
    /*Try removing the tunnel id again, it should fail*/
    sai_test_vxlan_tunnel_remove (tunnel_id, false);
    sai_test_vxlan_tunnel_map_entry_remove (decap_map_entry_id, true);
    sai_test_vxlan_tunnel_map_entry_remove (encap_map_entry_id, true);
    sai_test_vxlan_tunnel_map_remove (decap_map_id, true);
    sai_test_vxlan_tunnel_map_remove (encap_map_id, true);
    sai_test_vxlan_1d_bridge_remove (bridge_id, true);
}

TEST_F (saiTunnelTest, create_and_remove_vxlan_tunnel_term_object)
{
    sai_object_id_t    bridge_id             = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_entry_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_entry_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_id             = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_term_id        = SAI_NULL_OBJECT_ID;
    const char         *tunnel_sip           = "10.0.0.1";
    const char         *tunnel_dip           = "20.0.0.1";
    sai_uint32_t       vnid                  = 100;

    sai_test_vxlan_1d_bridge_create(&bridge_id, true);

    sai_test_vxlan_encap_tunnel_map_create(&encap_map_id, true);
    sai_test_vxlan_decap_tunnel_map_create(&decap_map_id, true);

    sai_test_vxlan_encap_map_entry_create(&encap_map_entry_id, encap_map_id,
                                          bridge_id, vnid, true);
    sai_test_vxlan_decap_map_entry_create(&decap_map_entry_id, decap_map_id,
                                          vnid, bridge_id, true);

    sai_test_vxlan_tunnel_create(&tunnel_id, dflt_underlay_rif_id, dflt_overlay_rif_id,
                                 tunnel_sip, encap_map_id, decap_map_id, true);

    /*Negative case, pass NULL object as tunnel id*/
    sai_test_vxlan_tunnel_term_create(&tunnel_term_id, dflt_vr_id, tunnel_dip,
                                      tunnel_sip, SAI_NULL_OBJECT_ID, false);

    sai_test_vxlan_tunnel_term_create(&tunnel_term_id, dflt_vr_id, tunnel_dip,
                                      tunnel_sip, tunnel_id, true);

    /*Negative case*/
    /*Try creating the same tunnel term entry*/
    sai_test_vxlan_tunnel_term_create(&tunnel_term_id, dflt_vr_id, tunnel_dip,
                                      tunnel_sip, tunnel_id, false);

    sai_test_vxlan_tunnel_term_remove (tunnel_term_id, true);
    /*Negative case, remove the same tunnel terminator object*/
    sai_test_vxlan_tunnel_term_remove (tunnel_term_id, false);

    sai_test_vxlan_tunnel_remove (tunnel_id, true);
    sai_test_vxlan_tunnel_map_entry_remove (decap_map_entry_id, true);
    sai_test_vxlan_tunnel_map_entry_remove (encap_map_entry_id, true);
    sai_test_vxlan_tunnel_map_remove (decap_map_id, true);
    sai_test_vxlan_tunnel_map_remove (encap_map_id, true);
    sai_test_vxlan_1d_bridge_remove (bridge_id, true);
}

TEST_F (saiTunnelTest, create_and_remove_vxlan_tunnel_bridge_ports)
{
    sai_object_id_t    bridge_id_1           = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_id_2           = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_tunnel_port_1  = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_tunnel_port_2  = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_tunnel_port_3  = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_entry_id_1  = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_entry_id_1  = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_entry_id_2  = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_entry_id_2  = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_id             = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_term_id        = SAI_NULL_OBJECT_ID;
    sai_uint32_t       vnid_1                = 100;
    sai_uint32_t       vnid_2                = 200;
    const char         *tunnel_sip           = "10.0.0.1";
    const char         *tunnel_dip           = "20.0.0.1";

    sai_test_vxlan_1d_bridge_create(&bridge_id_1, true);
    sai_test_vxlan_1d_bridge_create(&bridge_id_2, true);

    sai_test_vxlan_encap_tunnel_map_create(&encap_map_id, true);
    sai_test_vxlan_decap_tunnel_map_create(&decap_map_id, true);

    sai_test_vxlan_encap_map_entry_create(&encap_map_entry_id_1, encap_map_id,
                                          bridge_id_1, vnid_1, true);
    sai_test_vxlan_decap_map_entry_create(&decap_map_entry_id_1, decap_map_id,
                                          vnid_1, bridge_id_1, true);

    sai_test_vxlan_encap_map_entry_create(&encap_map_entry_id_2, encap_map_id,
                                          bridge_id_2, vnid_2, true);
    sai_test_vxlan_decap_map_entry_create(&decap_map_entry_id_2, decap_map_id,
                                          vnid_2, bridge_id_2, true);

    sai_test_vxlan_tunnel_create(&tunnel_id, dflt_underlay_rif_id, dflt_overlay_rif_id,
                                 tunnel_sip, encap_map_id, decap_map_id, true);

    /* Negative case, create tunnel bridge port with invalid tunnel and bridge
     * objects*/
    sai_test_vxlan_tunnel_port_create(&bridge_tunnel_port_1, SAI_NULL_OBJECT_ID,
                                      tunnel_id, false);
    sai_test_vxlan_tunnel_port_create(&bridge_tunnel_port_1, bridge_id_1,
                                      SAI_NULL_OBJECT_ID, false);

    sai_test_vxlan_tunnel_port_create(&bridge_tunnel_port_1, bridge_id_1, tunnel_id, true);
    sai_test_vxlan_tunnel_port_create(&bridge_tunnel_port_2, bridge_id_2, tunnel_id, true);
    /*negative test case, create duplicate tunnel bridge port*/
    sai_test_vxlan_tunnel_port_create(&bridge_tunnel_port_3, bridge_id_2, tunnel_id, false);

    sai_test_vxlan_tunnel_term_create(&tunnel_term_id, dflt_vr_id, tunnel_dip,
                                      tunnel_sip, tunnel_id, true);

    sai_test_vxlan_tunnel_term_remove (tunnel_term_id, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_tunnel_port_1, false, true);
    sai_test_vxlan_bridge_port_remove (bridge_tunnel_port_1, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_tunnel_port_2, false, true);
    sai_test_vxlan_bridge_port_remove (bridge_tunnel_port_2, true);
    /*Negative case, try removing deleted bridge port*/
    sai_test_vxlan_bridge_port_remove (bridge_tunnel_port_2, false);

    sai_test_vxlan_tunnel_remove (tunnel_id, true);
    sai_test_vxlan_tunnel_map_entry_remove (decap_map_entry_id_1, true);
    sai_test_vxlan_tunnel_map_entry_remove (encap_map_entry_id_1, true);
    sai_test_vxlan_tunnel_map_entry_remove (decap_map_entry_id_2, true);
    sai_test_vxlan_tunnel_map_entry_remove (encap_map_entry_id_2, true);
    sai_test_vxlan_tunnel_map_remove (decap_map_id, true);
    sai_test_vxlan_tunnel_map_remove (encap_map_id, true);
    sai_test_vxlan_1d_bridge_remove (bridge_id_1, true);
    sai_test_vxlan_1d_bridge_remove (bridge_id_2, true);
}

TEST_F (saiTunnelTest, modify_encap_decap_mapping)
{
    sai_object_id_t    bridge_id_1           = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_id_2           = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_tunnel_port_1  = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_tunnel_port_2  = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_entry_id_1  = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_entry_id_1  = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_entry_id_2  = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_entry_id_2  = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_entry_id_3  = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_id             = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_term_id        = SAI_NULL_OBJECT_ID;
    sai_uint32_t       vnid_1                = 100;
    sai_uint32_t       vnid_2                = 200;
    sai_uint32_t       vnid_3                = 300;
    const char         *tunnel_sip           = "10.0.0.1";
    const char         *tunnel_dip           = "20.0.0.1";

    sai_kv_pair_t kvpair;
    kv_iter kviter;

    kvpair = kvpair_get();

    kviter = kvpair.find(std::string(SAI_KEY_VXLAN_SCALED_MODE_ENABLE));
    if (kviter == kvpair.end()) {
        printf("Test case can be run only in non-scaled mode\n");
        return;
    }

    /*If scale mode is disabled, skip test case*/
    if(kviter->second.c_str() == std::string("1")) {
        printf("Test case can be run only in non-scaled mode\n");
        return;
    }

    sai_test_vxlan_1d_bridge_create(&bridge_id_1, true);
    sai_test_vxlan_1d_bridge_create(&bridge_id_2, true);

    sai_test_vxlan_encap_tunnel_map_create(&encap_map_id, true);
    sai_test_vxlan_decap_tunnel_map_create(&decap_map_id, true);

    sai_test_vxlan_encap_map_entry_create(&encap_map_entry_id_1, encap_map_id,
                                          bridge_id_1, vnid_1, true);
    sai_test_vxlan_decap_map_entry_create(&decap_map_entry_id_1, decap_map_id,
                                          vnid_1, bridge_id_1, true);

    sai_test_vxlan_encap_map_entry_create(&encap_map_entry_id_2, encap_map_id,
                                          bridge_id_2, vnid_2, true);
    sai_test_vxlan_decap_map_entry_create(&decap_map_entry_id_2, decap_map_id,
                                          vnid_2, bridge_id_2, true);

    sai_test_vxlan_tunnel_create(&tunnel_id, dflt_underlay_rif_id, dflt_overlay_rif_id,
                                 tunnel_sip, encap_map_id, decap_map_id, true);

    sai_test_vxlan_tunnel_port_create(&bridge_tunnel_port_1, bridge_id_1, tunnel_id, true);
    sai_test_vxlan_tunnel_port_create(&bridge_tunnel_port_2, bridge_id_2, tunnel_id, true);

    sai_test_vxlan_tunnel_term_create(&tunnel_term_id, dflt_vr_id, tunnel_dip,
                                      tunnel_sip, tunnel_id, true);

    /* Try changing bridge2's mapping to vnid_3 by removing the existing entry
     * This should fail as the tunnel port 2 is using the mapping*/
    sai_test_vxlan_tunnel_map_entry_remove (decap_map_entry_id_2, false);
    sai_test_vxlan_tunnel_map_entry_remove (encap_map_entry_id_2, false);

    /* Try changing the vnid value in encap entry 2 by setting to new vnid
     * This should pass*/
    sai_test_vxlan_encap_map_entry_set(encap_map_entry_id_2, vnid_3, true);

    /*Create a new mapping for vnid_3 to bridge 2*/
    sai_test_vxlan_decap_map_entry_create(&decap_map_entry_id_3, decap_map_id,
                                          vnid_3, bridge_id_2, true);
    /*Now remove old decap entry 2*/
    sai_test_vxlan_tunnel_map_entry_remove (decap_map_entry_id_2, true);

    sai_test_vxlan_tunnel_term_remove (tunnel_term_id, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_tunnel_port_1, false, true);
    sai_test_vxlan_bridge_port_remove (bridge_tunnel_port_1, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_tunnel_port_2, false, true);
    sai_test_vxlan_bridge_port_remove (bridge_tunnel_port_2, true);
    sai_test_vxlan_tunnel_remove (tunnel_id, true);
    sai_test_vxlan_tunnel_map_entry_remove (decap_map_entry_id_1, true);
    sai_test_vxlan_tunnel_map_entry_remove (encap_map_entry_id_1, true);
    sai_test_vxlan_tunnel_map_entry_remove (decap_map_entry_id_3, true);
    sai_test_vxlan_tunnel_map_entry_remove (encap_map_entry_id_2, true);
    sai_test_vxlan_tunnel_map_remove (decap_map_id, true);
    sai_test_vxlan_tunnel_map_remove (encap_map_id, true);
    sai_test_vxlan_1d_bridge_remove (bridge_id_1, true);
    sai_test_vxlan_1d_bridge_remove (bridge_id_2, true);
}

TEST_F (saiTunnelTest, create_and_remove_vxlan_underlay_neighbor)
{
    sai_status_t       status;
    sai_object_id_t    bridge_id             = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_tunnel_port_id = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_entry_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_entry_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_id             = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_term_id        = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_sub_port_id    = SAI_NULL_OBJECT_ID;
    sai_uint32_t       vnid                  = 100;
    const char         *tunnel_sip           = "10.0.0.1";
    const char         *tunnel_dip           = "20.0.0.1";
    const char         *mac_str_1            = "00:22:22:22:22:22";
    const unsigned int vlan_id               = 10;
    sai_ip_addr_family_t  ip4_af             = SAI_IP_ADDR_FAMILY_IPV4;

    sai_test_vxlan_1d_bridge_create(&bridge_id, true);

    sai_test_vxlan_encap_tunnel_map_create(&encap_map_id, true);
    sai_test_vxlan_decap_tunnel_map_create(&decap_map_id, true);

    sai_test_vxlan_encap_map_entry_create(&encap_map_entry_id, encap_map_id,
                                          bridge_id, vnid, true);
    sai_test_vxlan_decap_map_entry_create(&decap_map_entry_id, decap_map_id,
                                          vnid, bridge_id, true);

    sai_test_vxlan_tunnel_create(&tunnel_id, dflt_underlay_rif_id, dflt_overlay_rif_id,
                                 tunnel_sip, encap_map_id, decap_map_id, true);

    sai_test_vxlan_access_port_create(&bridge_sub_port_id, bridge_id,
                                      sai_test_tunnel_port_id_get(0), vlan_id, true);

    sai_test_vxlan_tunnel_port_create(&bridge_tunnel_port_id, bridge_id,
                                      tunnel_id, true);

    sai_test_vxlan_tunnel_term_create(&tunnel_term_id, dflt_vr_id, tunnel_dip,
                                      tunnel_sip, tunnel_id, true);

    /*Tunnel dip is directly connected neighbor*/
    sai_test_vxlan_neighbor_create(dflt_port_rif_id, ip4_af, tunnel_dip, mac_str_1, true);

    /*Trapping packets destined to tunnel dip to cpu*/
    status = saiL3Test::sai_test_neighbor_attr_set (dflt_port_rif_id, ip4_af, tunnel_dip,
                                                    SAI_NEIGHBOR_ENTRY_ATTR_PACKET_ACTION,
                                                    SAI_PACKET_ACTION_TRAP, NULL);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    sai_test_vxlan_neighbor_remove(dflt_port_rif_id, ip4_af, tunnel_dip, true);
    sai_test_vxlan_tunnel_term_remove (tunnel_term_id, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_tunnel_port_id, false, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_sub_port_id, false, true);
    sai_test_vxlan_bridge_port_remove (bridge_tunnel_port_id, true);
    sai_test_vxlan_bridge_port_remove (bridge_sub_port_id, true);
    sai_test_vxlan_tunnel_remove (tunnel_id, true);
    sai_test_vxlan_tunnel_map_entry_remove (decap_map_entry_id, true);
    sai_test_vxlan_tunnel_map_entry_remove (encap_map_entry_id, true);
    sai_test_vxlan_tunnel_map_remove (decap_map_id, true);
    sai_test_vxlan_tunnel_map_remove (encap_map_id, true);
    sai_test_vxlan_1d_bridge_remove (bridge_id, true);
}

TEST_F (saiTunnelTest, create_and_remove_vxlan_vnid0_bfd_bridge)
{
    sai_status_t       status                = SAI_STATUS_SUCCESS;
    unsigned int       test_port             = 14;
    sai_object_id_t    test_port_id_1        = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_id             = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_tunnel_port_id = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_entry_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_entry_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_id             = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_term_id        = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_sub_port_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    port_rif_id           = SAI_NULL_OBJECT_ID;
    sai_uint32_t       vnid                  = 0;
    const char         *tunnel_sip           = "10.0.0.1";
    const char         *tunnel_dip           = "20.0.0.1";
    const char         *mac_str_1            = "00:22:22:22:22:22";
    const char         *port_mac_str         = "00:11:22:33:44:55";
    const char         *tenant_mac           = "00:00:00:00:00:bb";
    const unsigned int vlan_id               = 10;
    sai_ip_addr_family_t  ip4_af             = SAI_IP_ADDR_FAMILY_IPV4;

    test_port_id_1 = sai_test_tunnel_port_id_get (test_port);

    sai_test_vxlan_1d_bridge_create(&bridge_id, true);

    sai_test_vxlan_encap_tunnel_map_create(&encap_map_id, true);
    sai_test_vxlan_decap_tunnel_map_create(&decap_map_id, true);

    sai_test_vxlan_encap_map_entry_create(&encap_map_entry_id, encap_map_id,
                    bridge_id, vnid, true);
    sai_test_vxlan_decap_map_entry_create(&decap_map_entry_id, decap_map_id,
                    vnid, bridge_id, true);
    sai_test_vxlan_tunnel_create(&tunnel_id, dflt_underlay_rif_id, dflt_overlay_rif_id,
                    tunnel_sip, encap_map_id, decap_map_id, true);

    sai_test_vxlan_access_port_create(&bridge_sub_port_id, bridge_id,
                    sai_test_tunnel_port_id_get(12), vlan_id, true);

    sai_test_vxlan_tunnel_port_create(&bridge_tunnel_port_id, bridge_id,
                    tunnel_id, true);

    sai_test_vxlan_tunnel_term_create(&tunnel_term_id, dflt_vr_id, tunnel_dip,
                    tunnel_sip, tunnel_id, true);

    sai_test_vxlan_fdb_entry_create(bridge_id, tenant_mac, bridge_tunnel_port_id,
                    true, tunnel_dip, true);

    /* Create a Port RIF */
    status = saiL3Test::sai_test_rif_create (&port_rif_id,
                    4,
                    SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                    dflt_vr_id,
                    SAI_ROUTER_INTERFACE_ATTR_TYPE,
                    SAI_ROUTER_INTERFACE_TYPE_PORT,
                    SAI_ROUTER_INTERFACE_ATTR_PORT_ID,
                    test_port_id_1,
                    SAI_ROUTER_INTERFACE_ATTR_SRC_MAC_ADDRESS,
                    port_mac_str);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);
    /*Tunnel dip is directly connected neighbor*/
    sai_test_vxlan_neighbor_create(port_rif_id, ip4_af, tunnel_dip, mac_str_1, true);

    sai_test_vxlan_neighbor_remove(port_rif_id, ip4_af, tunnel_dip, true);

    /* Remove all RIF */
    status = saiL3Test::sai_test_rif_remove (port_rif_id);
    EXPECT_EQ (SAI_STATUS_SUCCESS, status);

    sai_test_vxlan_fdb_entry_remove(bridge_id, tenant_mac, true);
    sai_test_vxlan_tunnel_term_remove (tunnel_term_id, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_tunnel_port_id, false, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_sub_port_id, false, true);
    sai_test_vxlan_bridge_port_remove (bridge_tunnel_port_id, true);
    sai_test_vxlan_bridge_port_remove (bridge_sub_port_id, true);
    sai_test_vxlan_tunnel_remove (tunnel_id, true);
    sai_test_vxlan_tunnel_map_entry_remove (decap_map_entry_id, true);
    sai_test_vxlan_tunnel_map_entry_remove (encap_map_entry_id, true);
    sai_test_vxlan_tunnel_map_remove (decap_map_id, true);
    sai_test_vxlan_tunnel_map_remove (encap_map_id, true);
    sai_test_vxlan_1d_bridge_remove (bridge_id, true);
}

TEST_F (saiTunnelTest, create_and_remove_vxlan_underlay_nexthop_route)
{
    sai_status_t       status;
    sai_object_id_t    bridge_id             = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_tunnel_port_id = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_entry_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_entry_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_id             = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_term_id        = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_sub_port_id    = SAI_NULL_OBJECT_ID;
    sai_uint32_t       vnid                  = 100;
    const char         *tunnel_sip           = "10.0.0.1";
    const char         *tunnel_dip           = "20.0.0.1";
    const char         *prefix_str           = "20.0.0.0";
    const unsigned int prefix_len            = 24;
    const unsigned int vlan_id               = 10;
    sai_ip_addr_family_t  ip4_af             = SAI_IP_ADDR_FAMILY_IPV4;

    sai_test_vxlan_1d_bridge_create(&bridge_id, true);

    sai_test_vxlan_encap_tunnel_map_create(&encap_map_id, true);
    sai_test_vxlan_decap_tunnel_map_create(&decap_map_id, true);

    sai_test_vxlan_encap_map_entry_create(&encap_map_entry_id, encap_map_id,
                                          bridge_id, vnid, true);
    sai_test_vxlan_decap_map_entry_create(&decap_map_entry_id, decap_map_id,
                                          vnid, bridge_id, true);

    sai_test_vxlan_tunnel_create(&tunnel_id, dflt_underlay_rif_id, dflt_overlay_rif_id,
                                 tunnel_sip, encap_map_id, decap_map_id, true);

    sai_test_vxlan_access_port_create(&bridge_sub_port_id, bridge_id,
                                      sai_test_tunnel_port_id_get(1), vlan_id, true);

    sai_test_vxlan_tunnel_port_create(&bridge_tunnel_port_id, bridge_id,
                                      tunnel_id, true);

    sai_test_vxlan_tunnel_term_create(&tunnel_term_id, dflt_vr_id, tunnel_dip,
                                      tunnel_sip, tunnel_id, true);

    /* Create a Underlay route for the tunnel DIP with next hop pointing to
     * port rif*/
    sai_test_vxlan_route_create(dflt_vr_id, ip4_af, prefix_str, prefix_len,
                                dflt_underlay_port_nh_id, true);

    /* Change the nexthop which points to vlan based rif. Check if the VXLAN
     * packets are routed via the new nexthop*/
    status = saiL3Test ::sai_test_route_attr_set (dflt_vr_id, ip4_af,
                                                  prefix_str, prefix_len,
                                                  SAI_ROUTE_ENTRY_ATTR_NEXT_HOP_ID,
                                                  dflt_underlay_vlan_nh_id);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    /* Delete Underlay route */
    sai_test_vxlan_route_remove (dflt_vr_id, ip4_af, prefix_str, prefix_len, true);
    sai_test_vxlan_tunnel_term_remove (tunnel_term_id, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_tunnel_port_id, false, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_sub_port_id, false, true);
    sai_test_vxlan_bridge_port_remove (bridge_tunnel_port_id, true);
    sai_test_vxlan_bridge_port_remove (bridge_sub_port_id, true);
    sai_test_vxlan_tunnel_remove (tunnel_id, true);
    sai_test_vxlan_tunnel_map_entry_remove (decap_map_entry_id, true);
    sai_test_vxlan_tunnel_map_entry_remove (encap_map_entry_id, true);
    sai_test_vxlan_tunnel_map_remove (decap_map_id, true);
    sai_test_vxlan_tunnel_map_remove (encap_map_id, true);
    sai_test_vxlan_1d_bridge_remove (bridge_id, true);
}

TEST_F (saiTunnelTest, create_and_remove_vxlan_underlay_ecmp_route)
{
    sai_object_id_t    bridge_id             = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_tunnel_port_id = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_entry_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_entry_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_id             = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_term_id        = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_sub_port_id    = SAI_NULL_OBJECT_ID;
    sai_uint32_t       vnid                  = 100;
    const char         *tunnel_sip           = "10.0.0.1";
    const char         *tunnel_dip           = "20.0.0.1";
    const char         *prefix_str           = "20.0.0.0";
    const unsigned int prefix_len            = 24;
    const unsigned int vlan_id               = 10;
    sai_ip_addr_family_t  ip4_af             = SAI_IP_ADDR_FAMILY_IPV4;

    sai_test_vxlan_1d_bridge_create(&bridge_id, true);

    sai_test_vxlan_encap_tunnel_map_create(&encap_map_id, true);
    sai_test_vxlan_decap_tunnel_map_create(&decap_map_id, true);

    sai_test_vxlan_encap_map_entry_create(&encap_map_entry_id, encap_map_id,
                                          bridge_id, vnid, true);
    sai_test_vxlan_decap_map_entry_create(&decap_map_entry_id, decap_map_id,
                                          vnid, bridge_id, true);

    sai_test_vxlan_tunnel_create(&tunnel_id, dflt_underlay_rif_id, dflt_overlay_rif_id,
                                 tunnel_sip, encap_map_id, decap_map_id, true);

    sai_test_vxlan_access_port_create(&bridge_sub_port_id, bridge_id,
                                      sai_test_tunnel_port_id_get(1), vlan_id, true);

    sai_test_vxlan_tunnel_port_create(&bridge_tunnel_port_id, bridge_id,
                                      tunnel_id, true);

    sai_test_vxlan_bridge_port_admin_state_set(bridge_tunnel_port_id, false, true);

    sai_test_vxlan_bridge_port_learn_mode_set(bridge_tunnel_port_id,
                                              SAI_BRIDGE_PORT_FDB_LEARNING_MODE_CPU_LOG,
                                              true);

    sai_test_vxlan_tunnel_term_create(&tunnel_term_id, dflt_vr_id, tunnel_dip,
                                      tunnel_sip, tunnel_id, true);

    /* Create a Underlay route for the tunnel DIP */
    sai_test_vxlan_route_create(dflt_vr_id, ip4_af, prefix_str, prefix_len,
                                dflt_underlay_nhg_id, true);

    sai_test_vxlan_bridge_port_admin_state_set(bridge_tunnel_port_id, true, true);

    /* Delete Underlay route */
    sai_test_vxlan_route_remove(dflt_vr_id, ip4_af, prefix_str, prefix_len,true);
    sai_test_vxlan_tunnel_term_remove (tunnel_term_id, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_tunnel_port_id, false, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_sub_port_id, false, true);
    sai_test_vxlan_bridge_port_remove (bridge_tunnel_port_id, true);
    sai_test_vxlan_bridge_port_remove (bridge_sub_port_id, true);
    sai_test_vxlan_tunnel_remove (tunnel_id, true);
    sai_test_vxlan_tunnel_map_entry_remove (decap_map_entry_id, true);
    sai_test_vxlan_tunnel_map_entry_remove (encap_map_entry_id, true);
    sai_test_vxlan_tunnel_map_remove (decap_map_id, true);
    sai_test_vxlan_tunnel_map_remove (encap_map_id, true);
    sai_test_vxlan_1d_bridge_remove (bridge_id, true);
}

TEST_F (saiTunnelTest, create_and_remove_vxlan_fdb_entry)
{
    sai_object_id_t    bridge_id             = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_tunnel_port_id = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_entry_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_entry_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_id             = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_term_id        = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_sub_port_id    = SAI_NULL_OBJECT_ID;
    sai_uint32_t       vnid                  = 100;
    const char         *tunnel_sip           = "10.0.0.1";
    const char         *tunnel_dip           = "20.0.0.1";
    const char         *mac_str_1            = "00:22:22:22:22:22";
    const char         *tenant_mac           = "00:00:00:00:00:bb";
    const unsigned int vlan_id               = 10;
    sai_ip_addr_family_t  ip4_af             = SAI_IP_ADDR_FAMILY_IPV4;

    sai_test_vxlan_1d_bridge_create(&bridge_id, true);

    sai_test_vxlan_encap_tunnel_map_create(&encap_map_id, true);
    sai_test_vxlan_decap_tunnel_map_create(&decap_map_id, true);

    sai_test_vxlan_encap_map_entry_create(&encap_map_entry_id, encap_map_id,
                                          bridge_id, vnid, true);

    sai_test_vxlan_decap_map_entry_create(&decap_map_entry_id, decap_map_id,
                                          vnid, bridge_id, true);

    sai_test_vxlan_tunnel_create(&tunnel_id, dflt_underlay_rif_id, dflt_overlay_rif_id,
                                 tunnel_sip, encap_map_id, decap_map_id, true);

    sai_test_vxlan_access_port_create(&bridge_sub_port_id, bridge_id,
                                      sai_test_tunnel_port_id_get(1), vlan_id, true);

    sai_test_vxlan_tunnel_port_create(&bridge_tunnel_port_id, bridge_id,
                                      tunnel_id, true);

    /* This should fail as tunnel termination entry for the tunnel dip has not
     * been created first */
    sai_test_vxlan_fdb_entry_create(bridge_id, tenant_mac, bridge_tunnel_port_id,
                                    true, tunnel_dip, false);

    sai_test_vxlan_tunnel_term_create(&tunnel_term_id, dflt_vr_id, tunnel_dip,
                                      tunnel_sip, tunnel_id, true);

    sai_test_vxlan_fdb_entry_create(bridge_id, tenant_mac, bridge_tunnel_port_id,
                                    true, tunnel_dip, true);

    /*Tunnel dip is directly connected neighbor*/
    sai_test_vxlan_neighbor_create(dflt_port_rif_id, ip4_af, tunnel_dip, mac_str_1, true);

    sai_test_vxlan_neighbor_remove(dflt_port_rif_id, ip4_af, tunnel_dip, true);

    sai_test_vxlan_fdb_entry_remove(bridge_id, tenant_mac, true);
    sai_test_vxlan_tunnel_term_remove (tunnel_term_id, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_tunnel_port_id, false, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_sub_port_id, false, true);
    sai_test_vxlan_bridge_port_remove(bridge_tunnel_port_id, true);
    sai_test_vxlan_bridge_port_remove(bridge_sub_port_id, true);
    sai_test_vxlan_tunnel_remove (tunnel_id, true);
    sai_test_vxlan_tunnel_map_entry_remove (decap_map_entry_id, true);
    sai_test_vxlan_tunnel_map_entry_remove (encap_map_entry_id, true);
    sai_test_vxlan_tunnel_map_remove (decap_map_id, true);
    sai_test_vxlan_tunnel_map_remove (encap_map_id, true);
    sai_test_vxlan_1d_bridge_remove (bridge_id, true);
}

TEST_F (saiTunnelTest, vxlan_underlay_ecmp_route_with_l2mc_flooding)
{
    sai_object_id_t    bridge_id             = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_tunnel_port_id = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_entry_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_entry_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_id             = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_term_id        = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_sub_port_id_1  = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_sub_port_id_2  = SAI_NULL_OBJECT_ID;
    sai_object_id_t    l2mc_group_id         = SAI_NULL_OBJECT_ID;
    sai_object_id_t    l2mc_member_id_1      = SAI_NULL_OBJECT_ID;
    sai_object_id_t    l2mc_member_id_2      = SAI_NULL_OBJECT_ID;
    sai_object_id_t    l2mc_member_id_3      = SAI_NULL_OBJECT_ID;
    sai_uint32_t       vnid                  = 100;
    const char         *tunnel_sip           = "10.0.0.1";
    const char         *tunnel_dip           = "20.0.0.1";
    const char         *prefix_str           = "20.0.0.0";
    const unsigned int prefix_len            = 24;
    const unsigned int vlan_id_1             = 10;
    const unsigned int vlan_id_2             = 20;
    sai_ip_addr_family_t  ip4_af             = SAI_IP_ADDR_FAMILY_IPV4;

    int status;

    sai_test_vxlan_1d_bridge_create(&bridge_id, true);

    sai_test_vxlan_encap_tunnel_map_create(&encap_map_id, true);
    sai_test_vxlan_decap_tunnel_map_create(&decap_map_id, true);

    sai_test_vxlan_encap_map_entry_create(&encap_map_entry_id, encap_map_id,
                                          bridge_id, vnid, true);
    sai_test_vxlan_decap_map_entry_create(&decap_map_entry_id, decap_map_id,
                                          vnid, bridge_id, true);

    sai_test_vxlan_tunnel_create(&tunnel_id, dflt_underlay_rif_id, dflt_overlay_rif_id,
                                 tunnel_sip, encap_map_id, decap_map_id, true);

    sai_test_vxlan_access_port_create(&bridge_sub_port_id_1, bridge_id,
                                      sai_test_tunnel_port_id_get(1), vlan_id_1, true);

    sai_test_vxlan_access_port_create(&bridge_sub_port_id_2, bridge_id,
                                      sai_test_tunnel_port_id_get(1), vlan_id_2, true);

    sai_test_vxlan_tunnel_port_create(&bridge_tunnel_port_id, bridge_id, tunnel_id, true);

    sai_test_vxlan_tunnel_term_create(&tunnel_term_id, dflt_vr_id, tunnel_dip, tunnel_sip,
                                      tunnel_id, true);

    sai_test_vxlan_l2mc_group_create(&l2mc_group_id, true);

    sai_test_vxlan_l2mc_group_member_create(&l2mc_member_id_1, l2mc_group_id, bridge_sub_port_id_1,
                                            false, NULL, true);

    sai_test_vxlan_l2mc_group_member_create(&l2mc_member_id_2, l2mc_group_id, bridge_sub_port_id_2,
                                            false, NULL, true);

    sai_test_vxlan_1d_bridge_l2mc_groups_set(bridge_id, true, l2mc_group_id, true, l2mc_group_id,
                                             true, l2mc_group_id, true);

    sai_test_vxlan_l2mc_group_member_create(&l2mc_member_id_3, l2mc_group_id, bridge_tunnel_port_id,
                                            true, tunnel_dip, true);

    /* Create a Underlay route for the tunnel DIP */
    sai_test_vxlan_route_create(dflt_vr_id, ip4_af, prefix_str, prefix_len,
                                dflt_underlay_nhg_id, true);


    status = sai_test_bridge_port_stats_get(bridge_sub_port_id_1, 4, SAI_BRIDGE_PORT_STAT_OUT_PACKETS,
                                             SAI_BRIDGE_PORT_STAT_OUT_OCTETS,
                                            SAI_BRIDGE_PORT_STAT_IN_PACKETS, SAI_BRIDGE_PORT_STAT_IN_OCTETS);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    printf("\ngetting tunnrl intiator ");
    status = sai_test_tunnel_stats_get(tunnel_id, 4,  SAI_TUNNEL_STAT_OUT_OCTETS, SAI_TUNNEL_STAT_OUT_PACKETS,
                                      SAI_TUNNEL_STAT_IN_PACKETS, SAI_TUNNEL_STAT_IN_OCTETS);
    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    /* Delete Underlay route */
    sai_test_vxlan_route_remove(dflt_vr_id, ip4_af, prefix_str, prefix_len,true);
    sai_test_vxlan_tunnel_term_remove (tunnel_term_id, true);
    sai_test_vxlan_l2mc_group_member_remove(l2mc_member_id_1, true);
    sai_test_vxlan_l2mc_group_member_remove(l2mc_member_id_2, true);
    sai_test_vxlan_l2mc_group_member_remove(l2mc_member_id_3, true);
    /*Removal of L2MC group should failed as it is still attached to bridge*/
    sai_test_vxlan_l2mc_group_remove(l2mc_group_id, false);

    sai_test_vxlan_1d_bridge_l2mc_groups_set(bridge_id, false, SAI_NULL_OBJECT_ID,
                                             false, SAI_NULL_OBJECT_ID, false,
                                             SAI_NULL_OBJECT_ID, true);
    /*L2MC group removal should pass now*/
    sai_test_vxlan_l2mc_group_remove(l2mc_group_id, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_tunnel_port_id, false, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_sub_port_id_1, false, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_sub_port_id_2, false, true);
    sai_test_vxlan_bridge_port_remove(bridge_tunnel_port_id, true);
    sai_test_vxlan_bridge_port_remove(bridge_sub_port_id_1, true);
    sai_test_vxlan_bridge_port_remove(bridge_sub_port_id_2, true);
    sai_test_vxlan_tunnel_remove (tunnel_id, true);
    sai_test_vxlan_tunnel_map_entry_remove (decap_map_entry_id, true);
    sai_test_vxlan_tunnel_map_entry_remove (encap_map_entry_id, true);
    sai_test_vxlan_tunnel_map_remove (decap_map_id, true);
    sai_test_vxlan_tunnel_map_remove (encap_map_id, true);
    sai_test_vxlan_1d_bridge_remove (bridge_id, true);
}

TEST_F (saiTunnelTest, vxlan_underlay_nexthop_lag_with_l2mc_flooding)
{
    sai_object_id_t    bridge_id             = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_tunnel_port_id = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_entry_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_entry_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_id             = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_term_id        = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_sub_port_id_1  = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_sub_port_id_2  = SAI_NULL_OBJECT_ID;
    sai_object_id_t    l2mc_group_id         = SAI_NULL_OBJECT_ID;
    sai_object_id_t    l2mc_member_id_1      = SAI_NULL_OBJECT_ID;
    sai_object_id_t    l2mc_member_id_2      = SAI_NULL_OBJECT_ID;
    sai_object_id_t    l2mc_member_id_3      = SAI_NULL_OBJECT_ID;
    sai_object_id_t    lag_id                = SAI_NULL_OBJECT_ID;
    sai_object_id_t    lag_bridge_port_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    lag_member_id_1       = SAI_NULL_OBJECT_ID;
    sai_object_id_t    lag_member_id_2       = SAI_NULL_OBJECT_ID;
    sai_object_id_t    lag_member_id_3       = SAI_NULL_OBJECT_ID;
    sai_object_id_t    lag_port_1            = SAI_NULL_OBJECT_ID;
    sai_object_id_t    lag_port_2            = SAI_NULL_OBJECT_ID;
    sai_object_id_t    lag_port_3            = SAI_NULL_OBJECT_ID;
    sai_object_id_t    lag_port_rif          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    lag_nh_id             = SAI_NULL_OBJECT_ID;
    sai_uint32_t       vnid                  = 100;
    const char         *tunnel_sip           = "10.0.0.1";
    const char         *tunnel_dip           = "20.0.0.1";
    const char         *nh_str               = "200.0.0.2";
    const char         *nh_mac_str           = "00:00:00:ab:cd:ef";
    const char         *prefix_str           = "20.0.0.0";
    const unsigned int prefix_len            = 24;
    const unsigned int vlan_id_1             = 10;
    const unsigned int vlan_id_2             = 20;
    sai_ip_addr_family_t  ip4_af             = SAI_IP_ADDR_FAMILY_IPV4;

    sai_test_vxlan_1d_bridge_create(&bridge_id, true);

    sai_test_vxlan_encap_tunnel_map_create(&encap_map_id, true);
    sai_test_vxlan_decap_tunnel_map_create(&decap_map_id, true);

    sai_test_vxlan_encap_map_entry_create(&encap_map_entry_id, encap_map_id,
                                          bridge_id, vnid, true);
    sai_test_vxlan_decap_map_entry_create(&decap_map_entry_id, decap_map_id,
                                          vnid, bridge_id, true);

    sai_test_vxlan_tunnel_create(&tunnel_id, dflt_underlay_rif_id, dflt_overlay_rif_id,
                                 tunnel_sip, encap_map_id, decap_map_id, true);

    sai_test_vxlan_access_port_create(&bridge_sub_port_id_1, bridge_id,
                                      sai_test_tunnel_port_id_get(1), vlan_id_1, true);

    sai_test_vxlan_access_port_create(&bridge_sub_port_id_2, bridge_id,
                                      sai_test_tunnel_port_id_get(1), vlan_id_2, true);

    sai_test_vxlan_tunnel_port_create(&bridge_tunnel_port_id, bridge_id, tunnel_id, true);

    sai_test_vxlan_tunnel_term_create(&tunnel_term_id, dflt_vr_id, tunnel_dip, tunnel_sip,
                                      tunnel_id, true);

    sai_test_vxlan_l2mc_group_create(&l2mc_group_id, true);

    sai_test_vxlan_l2mc_group_member_create(&l2mc_member_id_1, l2mc_group_id, bridge_sub_port_id_1,
                                            false, NULL, true);

    sai_test_vxlan_l2mc_group_member_create(&l2mc_member_id_2, l2mc_group_id, bridge_sub_port_id_2,
                                            false, NULL, true);

    sai_test_vxlan_1d_bridge_l2mc_groups_set(bridge_id, true, l2mc_group_id, true, l2mc_group_id,
                                             true, l2mc_group_id, true);

    sai_test_vxlan_l2mc_group_member_create(&l2mc_member_id_3, l2mc_group_id, bridge_tunnel_port_id,
                                            true, tunnel_dip, true);

    sai_test_vxlan_lag_create(&lag_id, true);

    sai_test_vxlan_1q_bridge_port_create(&lag_bridge_port_id, lag_id, true);

    lag_port_1 = sai_test_tunnel_port_id_get(3);
    lag_port_2 = sai_test_tunnel_port_id_get(4);
    lag_port_3 = sai_test_tunnel_port_id_get(5);

    sai_test_vxlan_default_1q_port_remove(lag_port_1, true);
    sai_test_vxlan_default_1q_port_remove(lag_port_2, true);
    sai_test_vxlan_default_1q_port_remove(lag_port_3, true);

    sai_test_vxlan_lag_member_create(&lag_member_id_1, lag_id, lag_port_1, true);
    sai_test_vxlan_lag_member_create(&lag_member_id_2, lag_id, lag_port_2, true);
    sai_test_vxlan_lag_member_create(&lag_member_id_3, lag_id, lag_port_3, true);

    sai_test_vxlan_port_lag_rif_create(&lag_port_rif, dflt_vr_id, lag_id, true);

    sai_test_vxlan_nexthop_create(&lag_nh_id, lag_port_rif, ip4_af, nh_str, true);

    sai_test_vxlan_neighbor_create(lag_port_rif, ip4_af, nh_str, nh_mac_str, true);

    /* Create a Underlay route for the tunnel DIP */
    sai_test_vxlan_route_create(dflt_vr_id, ip4_af, prefix_str, prefix_len,
                                lag_nh_id, true);

    sai_test_vxlan_bridge_port_admin_state_set(bridge_tunnel_port_id, false, true);

    sai_test_vxlan_bridge_port_admin_state_set(bridge_tunnel_port_id, true, true);

    /*Delete lag members to see if they get updated in l2mc flooding*/
    sai_test_vxlan_lag_member_remove(lag_member_id_1, true);
    sai_test_vxlan_lag_member_remove(lag_member_id_2, true);

    sai_test_vxlan_lag_member_create(&lag_member_id_2, lag_id, lag_port_2, true);
    sai_test_vxlan_lag_member_remove(lag_member_id_2, true);

    sai_test_vxlan_lag_member_remove(lag_member_id_3, true);

    sai_test_vxlan_lag_member_create(&lag_member_id_1, lag_id, lag_port_1, true);
    sai_test_vxlan_lag_member_remove(lag_member_id_1, true);

    sai_test_vxlan_default_1q_port_create(lag_port_1, true);
    sai_test_vxlan_default_1q_port_create(lag_port_2, true);
    sai_test_vxlan_default_1q_port_create(lag_port_3, true);

    sai_test_vxlan_neighbor_remove(lag_port_rif, ip4_af, nh_str, true);

    /* Delete Underlay route */
    sai_test_vxlan_route_remove(dflt_vr_id, ip4_af, prefix_str, prefix_len, true);

    sai_test_vxlan_nexthop_remove(lag_nh_id, true);

    sai_test_vxlan_rif_remove(lag_port_rif, true);

    sai_test_vxlan_bridge_port_admin_state_set(lag_bridge_port_id, false, true);
    sai_test_vxlan_bridge_port_remove(lag_bridge_port_id, true);

    sai_test_vxlan_lag_remove(lag_id, true);

    sai_test_vxlan_tunnel_term_remove (tunnel_term_id, true);
    sai_test_vxlan_l2mc_group_member_remove(l2mc_member_id_1, true);
    sai_test_vxlan_l2mc_group_member_remove(l2mc_member_id_2, true);
    sai_test_vxlan_l2mc_group_member_remove(l2mc_member_id_3, true);
    /*Removal of L2MC group should failed as it is still attached to bridge*/
    sai_test_vxlan_l2mc_group_remove(l2mc_group_id, false);

    sai_test_vxlan_1d_bridge_l2mc_groups_set(bridge_id, false, SAI_NULL_OBJECT_ID,
                                             false, SAI_NULL_OBJECT_ID, false,
                                             SAI_NULL_OBJECT_ID, true);
    /*L2MC group removal should pass now*/
    sai_test_vxlan_l2mc_group_remove(l2mc_group_id, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_tunnel_port_id, false, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_sub_port_id_1, false, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_sub_port_id_2, false, true);
    sai_test_vxlan_bridge_port_remove(bridge_tunnel_port_id, true);
    sai_test_vxlan_bridge_port_remove(bridge_sub_port_id_1, true);
    sai_test_vxlan_bridge_port_remove(bridge_sub_port_id_2, true);
    sai_test_vxlan_tunnel_remove (tunnel_id, true);
    sai_test_vxlan_tunnel_map_entry_remove (decap_map_entry_id, true);
    sai_test_vxlan_tunnel_map_entry_remove (encap_map_entry_id, true);
    sai_test_vxlan_tunnel_map_remove (decap_map_id, true);
    sai_test_vxlan_tunnel_map_remove (encap_map_id, true);
    sai_test_vxlan_1d_bridge_remove (bridge_id, true);
}

TEST_F (saiTunnelTest, vxlan_underlay_ecmp_lag_with_l2mc_flooding)
{
    sai_object_id_t    bridge_id             = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_tunnel_port_id = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_entry_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_entry_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_id             = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_term_id        = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_sub_port_id_1  = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_sub_port_id_2  = SAI_NULL_OBJECT_ID;
    sai_object_id_t    l2mc_group_id         = SAI_NULL_OBJECT_ID;
    sai_object_id_t    l2mc_member_id_1      = SAI_NULL_OBJECT_ID;
    sai_object_id_t    l2mc_member_id_2      = SAI_NULL_OBJECT_ID;
    sai_object_id_t    l2mc_member_id_3      = SAI_NULL_OBJECT_ID;
    sai_object_id_t    port_nh_id            = SAI_NULL_OBJECT_ID;
    sai_object_id_t    port_rif              = SAI_NULL_OBJECT_ID;
    sai_object_id_t    lag1_id               = SAI_NULL_OBJECT_ID;
    sai_object_id_t    lag1_bridge_port_id   = SAI_NULL_OBJECT_ID;
    sai_object_id_t    lag1_member_id_1      = SAI_NULL_OBJECT_ID;
    sai_object_id_t    lag1_member_id_2      = SAI_NULL_OBJECT_ID;
    sai_object_id_t    lag1_port_1           = SAI_NULL_OBJECT_ID;
    sai_object_id_t    lag1_port_2           = SAI_NULL_OBJECT_ID;
    sai_object_id_t    lag1_port_rif         = SAI_NULL_OBJECT_ID;
    sai_object_id_t    lag1_nh_id            = SAI_NULL_OBJECT_ID;
    sai_object_id_t    lag2_id               = SAI_NULL_OBJECT_ID;
    sai_object_id_t    lag2_bridge_port_id   = SAI_NULL_OBJECT_ID;
    sai_object_id_t    lag2_member_id_1      = SAI_NULL_OBJECT_ID;
    sai_object_id_t    lag2_member_id_2      = SAI_NULL_OBJECT_ID;
    sai_object_id_t    lag2_member_id_3      = SAI_NULL_OBJECT_ID;
    sai_object_id_t    lag2_port_1           = SAI_NULL_OBJECT_ID;
    sai_object_id_t    lag2_port_2           = SAI_NULL_OBJECT_ID;
    sai_object_id_t    lag2_port_3           = SAI_NULL_OBJECT_ID;
    sai_object_id_t    lag2_port_rif         = SAI_NULL_OBJECT_ID;
    sai_object_id_t    lag2_nh_id            = SAI_NULL_OBJECT_ID;
    sai_object_id_t    nh_group_id           = SAI_NULL_OBJECT_ID;
    const unsigned int nh_count              = 3;
    sai_object_id_t    nh_list[nh_count];
    sai_uint32_t       vnid                  = 100;
    const char         *tunnel_sip           = "10.0.0.1";
    const char         *tunnel_dip           = "20.0.0.1";
    const char         *nh_str               = "60.0.0.2";
    const char         *nh_mac_str           = "00:00:00:aa:aa:aa";
    const char         *nh1_str              = "65.0.0.2";
    const char         *nh1_mac_str          = "00:00:00:bb:bb:bb";
    const char         *nh2_str              = "70.0.0.2";
    const char         *nh2_mac_str          = "00:00:00:cc:cc:cc";
    const char         *prefix_str           = "20.0.0.0";
    const unsigned int prefix_len            = 24;
    const unsigned int vlan_id_1             = 10;
    const unsigned int vlan_id_2             = 20;
    sai_ip_addr_family_t  ip4_af             = SAI_IP_ADDR_FAMILY_IPV4;

    sai_test_vxlan_1d_bridge_create(&bridge_id, true);

    sai_test_vxlan_encap_tunnel_map_create(&encap_map_id, true);
    sai_test_vxlan_decap_tunnel_map_create(&decap_map_id, true);

    sai_test_vxlan_encap_map_entry_create(&encap_map_entry_id, encap_map_id,
                                          bridge_id, vnid, true);
    sai_test_vxlan_decap_map_entry_create(&decap_map_entry_id, decap_map_id,
                                          vnid, bridge_id, true);

    sai_test_vxlan_tunnel_create(&tunnel_id, dflt_underlay_rif_id, dflt_overlay_rif_id,
                                 tunnel_sip, encap_map_id, decap_map_id, true);

    sai_test_vxlan_access_port_create(&bridge_sub_port_id_1, bridge_id,
                                      sai_test_tunnel_port_id_get(1), vlan_id_1, true);

    sai_test_vxlan_access_port_create(&bridge_sub_port_id_2, bridge_id,
                                      sai_test_tunnel_port_id_get(1), vlan_id_2, true);

    sai_test_vxlan_tunnel_port_create(&bridge_tunnel_port_id, bridge_id, tunnel_id, true);

    sai_test_vxlan_tunnel_term_create(&tunnel_term_id, dflt_vr_id, tunnel_dip, tunnel_sip,
                                      tunnel_id, true);

    sai_test_vxlan_l2mc_group_create(&l2mc_group_id, true);

    sai_test_vxlan_l2mc_group_member_create(&l2mc_member_id_1, l2mc_group_id, bridge_sub_port_id_1,
                                            false, NULL, true);

    sai_test_vxlan_l2mc_group_member_create(&l2mc_member_id_2, l2mc_group_id, bridge_sub_port_id_2,
                                            false, NULL, true);

    sai_test_vxlan_1d_bridge_l2mc_groups_set(bridge_id, true, l2mc_group_id, true, l2mc_group_id,
                                             true, l2mc_group_id, true);

    sai_test_vxlan_l2mc_group_member_create(&l2mc_member_id_3, l2mc_group_id, bridge_tunnel_port_id,
                                            true, tunnel_dip, true);

    /*set up port nexthop*/
    sai_test_vxlan_port_lag_rif_create(&port_rif, dflt_vr_id,
                                       sai_test_tunnel_port_id_get(7), true);

    sai_test_vxlan_neighbor_create(port_rif, ip4_af, nh_str, nh_mac_str, true);

    sai_test_vxlan_nexthop_create(&port_nh_id, port_rif, ip4_af, nh_str, true);

    nh_list[0] = port_nh_id;

    /*set up lag 1 nexthop*/
    sai_test_vxlan_lag_create(&lag1_id, true);

    lag1_port_1 = sai_test_tunnel_port_id_get(8);
    lag1_port_2 = sai_test_tunnel_port_id_get(9);

    sai_test_vxlan_default_1q_port_remove(lag1_port_1, true);
    sai_test_vxlan_default_1q_port_remove(lag1_port_2, true);

    sai_test_vxlan_lag_member_create(&lag1_member_id_1, lag1_id, lag1_port_1, true);
    sai_test_vxlan_lag_member_create(&lag1_member_id_2, lag1_id, lag1_port_2, true);

    sai_test_vxlan_1q_bridge_port_create(&lag1_bridge_port_id, lag1_id, true);

    sai_test_vxlan_port_lag_rif_create(&lag1_port_rif, dflt_vr_id, lag1_id, true);

    sai_test_vxlan_neighbor_create(lag1_port_rif, ip4_af, nh1_str, nh1_mac_str, true);

    sai_test_vxlan_nexthop_create(&lag1_nh_id, lag1_port_rif, ip4_af, nh1_str, true);

    nh_list[1] = lag1_nh_id;

    /*Set up lag 2 nexthop*/
    sai_test_vxlan_lag_create(&lag2_id, true);

    sai_test_vxlan_1q_bridge_port_create(&lag2_bridge_port_id, lag2_id, true);

    lag2_port_1 = sai_test_tunnel_port_id_get(10);
    lag2_port_2 = sai_test_tunnel_port_id_get(11);
    lag2_port_3 = sai_test_tunnel_port_id_get(12);

    sai_test_vxlan_default_1q_port_remove(lag2_port_1, true);
    sai_test_vxlan_default_1q_port_remove(lag2_port_2, true);
    sai_test_vxlan_default_1q_port_remove(lag2_port_3, true);

    sai_test_vxlan_lag_member_create(&lag2_member_id_1, lag2_id, lag2_port_1, true);
    sai_test_vxlan_lag_member_create(&lag2_member_id_2, lag2_id, lag2_port_2, true);
    sai_test_vxlan_lag_member_create(&lag2_member_id_3, lag2_id, lag2_port_3, true);

    sai_test_vxlan_port_lag_rif_create(&lag2_port_rif, dflt_vr_id, lag2_id, true);

    sai_test_vxlan_nexthop_create(&lag2_nh_id, lag2_port_rif, ip4_af, nh2_str, true);

    sai_test_vxlan_neighbor_create(lag2_port_rif, ip4_af, nh2_str, nh2_mac_str, true);

    nh_list[2] = lag2_nh_id;

    /* Create next hop group*/
    sai_test_vxlan_nh_group_create(&nh_group_id, true);

    sai_test_vxlan_add_nh_to_group(nh_group_id, nh_count, nh_list, true);

    /* Create a Underlay route for the tunnel DIP*/
    sai_test_vxlan_route_create(dflt_vr_id, ip4_af, prefix_str, prefix_len,
                                nh_group_id, true);

    /* Remove next hop group member to see if they get updated in l2mc
     * flooding*/
    sai_test_vxlan_remove_nh_from_group(nh_group_id, 1, &port_nh_id, true);

    /* Remove lag 2 members to see if they get updated in l2mc flooding*/
    sai_test_vxlan_lag_member_remove(lag2_member_id_3, true);
    sai_test_vxlan_lag_member_remove(lag2_member_id_2, true);
    sai_test_vxlan_lag_member_remove(lag2_member_id_1, true);

    /* Remove next hop group member to see if they get updated in l2mc
     * flooding*/
    sai_test_vxlan_remove_nh_from_group(nh_group_id, 1, &lag2_nh_id, true);

    /* Delete lag 1 member to see if they get updated in l2mc flooding*/
    sai_test_vxlan_lag_member_remove(lag1_member_id_1, true);

    /* Remove next hop group member to see if they get updated in l2mc
     * flooding*/
    sai_test_vxlan_remove_nh_from_group(nh_group_id, 1, &lag1_nh_id, true);

    /* Add back the lag member 1 to lag 1*/
    sai_test_vxlan_lag_member_create(&lag1_member_id_1, lag1_id, lag1_port_1, true);


    sai_test_vxlan_add_nh_to_group(nh_group_id, 1, &lag1_nh_id, true);
    sai_test_vxlan_add_nh_to_group(nh_group_id, 1, &port_nh_id, true);
    sai_test_vxlan_remove_nh_from_group(nh_group_id, 1, &lag1_nh_id, true);
    sai_test_vxlan_remove_nh_from_group(nh_group_id, 1, &port_nh_id, true);

    /* Delete Underlay route */
    sai_test_vxlan_route_remove(dflt_vr_id, ip4_af, prefix_str, prefix_len, true);

    sai_test_vxlan_nh_group_remove(nh_group_id, true);

    /*Clean up lag2 nexthop*/
    sai_test_vxlan_neighbor_remove(lag2_port_rif, ip4_af, nh2_str, true);

    sai_test_vxlan_nexthop_remove(lag2_nh_id, true);

    sai_test_vxlan_rif_remove(lag2_port_rif, true);

    /*Clean up lag1 nexthop*/
    sai_test_vxlan_neighbor_remove(lag1_port_rif, ip4_af, nh1_str, true);

    sai_test_vxlan_nexthop_remove(lag1_nh_id, true);

    sai_test_vxlan_rif_remove(lag1_port_rif, true);

    /*clean up port nexthop*/
    sai_test_vxlan_neighbor_remove(port_rif, ip4_af, nh_str, true);

    sai_test_vxlan_nexthop_remove(port_nh_id, true);

    sai_test_vxlan_rif_remove(port_rif, true);

    sai_test_vxlan_bridge_port_admin_state_set(lag2_bridge_port_id, false, true);
    sai_test_vxlan_bridge_port_remove(lag2_bridge_port_id, true);

    sai_test_vxlan_default_1q_port_create(lag2_port_1, true);
    sai_test_vxlan_default_1q_port_create(lag2_port_2, true);
    sai_test_vxlan_default_1q_port_create(lag2_port_3, true);

    sai_test_vxlan_lag_remove(lag2_id, true);

    sai_test_vxlan_bridge_port_admin_state_set(lag1_bridge_port_id, false, true);
    sai_test_vxlan_bridge_port_remove(lag1_bridge_port_id, true);

    sai_test_vxlan_lag_member_remove(lag1_member_id_2, true);
    sai_test_vxlan_lag_member_remove(lag1_member_id_1, true);

    sai_test_vxlan_default_1q_port_create(lag1_port_1, true);
    sai_test_vxlan_default_1q_port_create(lag1_port_2, true);

    sai_test_vxlan_lag_remove(lag1_id, true);

    sai_test_vxlan_tunnel_term_remove (tunnel_term_id, true);
    sai_test_vxlan_l2mc_group_member_remove(l2mc_member_id_1, true);
    sai_test_vxlan_l2mc_group_member_remove(l2mc_member_id_2, true);
    sai_test_vxlan_l2mc_group_member_remove(l2mc_member_id_3, true);

    sai_test_vxlan_1d_bridge_l2mc_groups_set(bridge_id, false, SAI_NULL_OBJECT_ID,
                                             false, SAI_NULL_OBJECT_ID, false,
                                             SAI_NULL_OBJECT_ID, true);
    sai_test_vxlan_l2mc_group_remove(l2mc_group_id, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_tunnel_port_id, false, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_sub_port_id_1, false, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_sub_port_id_2, false, true);
    sai_test_vxlan_bridge_port_remove(bridge_tunnel_port_id, true);
    sai_test_vxlan_bridge_port_remove(bridge_sub_port_id_1, true);
    sai_test_vxlan_bridge_port_remove(bridge_sub_port_id_2, true);
    sai_test_vxlan_tunnel_remove (tunnel_id, true);
    sai_test_vxlan_tunnel_map_entry_remove (decap_map_entry_id, true);
    sai_test_vxlan_tunnel_map_entry_remove (encap_map_entry_id, true);
    sai_test_vxlan_tunnel_map_remove (decap_map_id, true);
    sai_test_vxlan_tunnel_map_remove (encap_map_id, true);
    sai_test_vxlan_1d_bridge_remove (bridge_id, true);
}

TEST_F (saiTunnelTest, vxlan_underlay_vlan_rif_route_update)
{
    sai_status_t       status                = SAI_STATUS_SUCCESS;
    sai_object_id_t    bridge_id             = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_sub_port_id_1  = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_entry_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_entry_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_id_1           = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_term_id_1      = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_tun_port_id_1  = SAI_NULL_OBJECT_ID;
    sai_object_id_t    l2mc_group_id         = SAI_NULL_OBJECT_ID;
    sai_object_id_t    l2mc_member_id_1      = SAI_NULL_OBJECT_ID;
    sai_object_id_t    l2mc_member_id_2      = SAI_NULL_OBJECT_ID;
    sai_object_id_t    underlay_vlan_obj_id  = SAI_NULL_OBJECT_ID;
    sai_object_id_t    underlay_vlan_rif     = SAI_NULL_OBJECT_ID;
    sai_object_id_t    nh_id                 = SAI_NULL_OBJECT_ID;
    sai_object_id_t    underlay_bridge_port  = SAI_NULL_OBJECT_ID;
    sai_uint32_t       vnid                  = 100;
    const char         *tunnel_sip           = "10.0.0.1";
    const char         *tunnel_dip           = "20.0.0.1";
    const char         *nh_str               = "200.0.0.2";
    const char         *nh_mac_str           = "00:00:00:ab:cd:ef";
    const char         *prefix_str           = "20.0.0.0";
    const unsigned int prefix_len            = 24;
    const unsigned int vlan_id_1             = 10;
    const unsigned int underlay_vlan         = 300;
    sai_ip_addr_family_t  ip4_af             = SAI_IP_ADDR_FAMILY_IPV4;

    sai_test_vxlan_1d_bridge_create(&bridge_id, true);

    sai_test_vxlan_encap_tunnel_map_create(&encap_map_id, true);
    sai_test_vxlan_decap_tunnel_map_create(&decap_map_id, true);

    sai_test_vxlan_encap_map_entry_create(&encap_map_entry_id, encap_map_id,
                                          bridge_id, vnid, true);
    sai_test_vxlan_decap_map_entry_create(&decap_map_entry_id, decap_map_id,
                                          vnid, bridge_id, true);

    sai_test_vxlan_tunnel_create(&tunnel_id_1, dflt_underlay_rif_id, dflt_overlay_rif_id,
                                 tunnel_sip, encap_map_id, decap_map_id, true);

    sai_test_vxlan_tunnel_term_create(&tunnel_term_id_1, dflt_vr_id, tunnel_dip, tunnel_sip,
                                      tunnel_id_1, true);

    sai_test_vxlan_access_port_create(&bridge_sub_port_id_1, bridge_id,
                                      sai_test_tunnel_port_id_get(1), vlan_id_1, true);

    sai_test_vxlan_tunnel_port_create(&bridge_tun_port_id_1, bridge_id, tunnel_id_1, true);

    sai_test_vxlan_l2mc_group_create(&l2mc_group_id, true);

    sai_test_vxlan_l2mc_group_member_create(&l2mc_member_id_1, l2mc_group_id, bridge_sub_port_id_1,
                                            false, NULL, true);

    sai_test_vxlan_l2mc_group_member_create(&l2mc_member_id_2, l2mc_group_id, bridge_tun_port_id_1,
                                            true, tunnel_dip, true);

    sai_test_vxlan_1d_bridge_l2mc_groups_set(bridge_id, true, l2mc_group_id, true, l2mc_group_id,
                                             true, l2mc_group_id, true);

    sai_test_vxlan_vlan_create(&underlay_vlan_obj_id, underlay_vlan, true);

    sai_test_vxlan_vlan_rif_create(&underlay_vlan_rif, dflt_vr_id, underlay_vlan, true);

    sai_test_vxlan_nexthop_create(&nh_id, underlay_vlan_rif, ip4_af, nh_str, true);

    sai_test_vxlan_neighbor_create(underlay_vlan_rif, ip4_af, nh_str, nh_mac_str, true);

    underlay_bridge_port = sai_test_tunnel_bridge_port_id_get(2);

    sai_test_vxlan_route_create(dflt_vr_id, ip4_af, prefix_str, prefix_len, nh_id, true);

    sai_test_vxlan_fdb_entry_create(underlay_vlan_obj_id, nh_mac_str, underlay_bridge_port,
                                    false, NULL, true);

    status = saiTunnelTest::sai_test_tunnel_fdb_entry_create(SAI_FDB_ENTRY_TYPE_DYNAMIC,
                                                             underlay_vlan_obj_id,
                                                             nh_mac_str,
                                                             SAI_PACKET_ACTION_TRAP,
                                                             SAI_NULL_OBJECT_ID,
                                                             false, NULL);
    ASSERT_EQ(status, SAI_STATUS_SUCCESS);

    sai_test_vxlan_fdb_entry_create(underlay_vlan_obj_id, nh_mac_str, underlay_bridge_port,
                                                                        false, NULL, true);

    sai_test_vxlan_fdb_entry_remove(underlay_vlan_obj_id, nh_mac_str, true);

    sai_test_vxlan_route_remove(dflt_vr_id, ip4_af, prefix_str, prefix_len, true);

    sai_test_vxlan_nexthop_remove(nh_id, true);

    sai_test_vxlan_neighbor_remove(underlay_vlan_rif, ip4_af, nh_str, true);

    sai_test_vxlan_rif_remove(underlay_vlan_rif, true);

    sai_test_vxlan_vlan_remove(underlay_vlan_obj_id, true);

    sai_test_vxlan_1d_bridge_l2mc_groups_set(bridge_id, false, SAI_NULL_OBJECT_ID,
                                             false, SAI_NULL_OBJECT_ID, false,
                                             SAI_NULL_OBJECT_ID, true);

    sai_test_vxlan_l2mc_group_member_remove(l2mc_member_id_1, true);
    sai_test_vxlan_l2mc_group_member_remove(l2mc_member_id_2, true);

    sai_test_vxlan_l2mc_group_remove(l2mc_group_id, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_tun_port_id_1, false, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_sub_port_id_1, false, true);

    sai_test_vxlan_bridge_port_remove(bridge_tun_port_id_1, true);
    sai_test_vxlan_bridge_port_remove(bridge_sub_port_id_1, true);

    sai_test_vxlan_tunnel_term_remove (tunnel_term_id_1, true);
    sai_test_vxlan_tunnel_remove (tunnel_id_1, true);
    sai_test_vxlan_tunnel_map_entry_remove (decap_map_entry_id, true);
    sai_test_vxlan_tunnel_map_entry_remove (encap_map_entry_id, true);
    sai_test_vxlan_tunnel_map_remove (decap_map_id, true);
    sai_test_vxlan_tunnel_map_remove (encap_map_id, true);
    sai_test_vxlan_1d_bridge_remove (bridge_id, true);
}

TEST_F (saiTunnelTest, vxlan_stat)
{
    sai_status_t       status;
    sai_object_id_t    bridge_id             = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_tunnel_port_id = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_entry_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_entry_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_id             = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_term_id        = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_sub_port_id    = SAI_NULL_OBJECT_ID;
    sai_uint32_t       vnid                  = 100;
    const char         *tunnel_sip           = "10.0.0.1";
    const char         *tunnel_dip           = "20.0.0.1";
    const char         *prefix_str           = "20.0.0.0";
    const unsigned int prefix_len            = 24;
    const unsigned int vlan_id               = 10;
    sai_ip_addr_family_t  ip4_af             = SAI_IP_ADDR_FAMILY_IPV4;

    sai_test_vxlan_1d_bridge_create(&bridge_id, true);

    sai_test_vxlan_encap_tunnel_map_create(&encap_map_id, true);
    sai_test_vxlan_decap_tunnel_map_create(&decap_map_id, true);

    sai_test_vxlan_encap_map_entry_create(&encap_map_entry_id, encap_map_id,
                                          bridge_id, vnid, true);
    sai_test_vxlan_decap_map_entry_create(&decap_map_entry_id, decap_map_id,
                                          vnid, bridge_id, true);

    sai_test_vxlan_tunnel_create(&tunnel_id, dflt_underlay_rif_id, dflt_overlay_rif_id,
                                 tunnel_sip, encap_map_id, decap_map_id, true);

    sai_test_vxlan_access_port_create(&bridge_sub_port_id, bridge_id,
                                      sai_test_tunnel_port_id_get(1), vlan_id, true);

    sai_test_vxlan_tunnel_port_create(&bridge_tunnel_port_id, bridge_id,
                                      tunnel_id, true);



    sai_test_vxlan_tunnel_term_create(&tunnel_term_id, dflt_vr_id, tunnel_dip,
                                      tunnel_sip, tunnel_id, true);
    /* Create a Underlay route for the tunnel DIP with next hop pointing to
     * port rif*/
    sai_test_vxlan_route_create(dflt_vr_id, ip4_af, prefix_str, prefix_len,
                                dflt_underlay_port_nh_id, true);

    /* Change the nexthop which points to vlan based rif. Check if the VXLAN
     * packets are routed via the new nexthop*/
    status = saiL3Test ::sai_test_route_attr_set (dflt_vr_id, ip4_af,
                                                  prefix_str, prefix_len,
                                                  SAI_ROUTE_ENTRY_ATTR_NEXT_HOP_ID,
                                                  dflt_underlay_vlan_nh_id);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    status = sai_test_bridge_port_stats_get(bridge_sub_port_id, 4,
                                            SAI_BRIDGE_PORT_STAT_OUT_PACKETS,
                                            SAI_BRIDGE_PORT_STAT_OUT_OCTETS,
                                            SAI_BRIDGE_PORT_STAT_IN_PACKETS,
                                            SAI_BRIDGE_PORT_STAT_IN_OCTETS);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    status = sai_test_bridge_port_stats_clear(bridge_sub_port_id, 4,
                                              SAI_BRIDGE_PORT_STAT_OUT_PACKETS,
                                              SAI_BRIDGE_PORT_STAT_OUT_OCTETS,
                                              SAI_BRIDGE_PORT_STAT_IN_PACKETS,
                                              SAI_BRIDGE_PORT_STAT_IN_OCTETS);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    status = sai_test_tunnel_stats_get(tunnel_id, 4,
                                       SAI_TUNNEL_STAT_OUT_OCTETS,
                                       SAI_TUNNEL_STAT_OUT_PACKETS,
                                       SAI_TUNNEL_STAT_IN_PACKETS,
                                       SAI_TUNNEL_STAT_IN_OCTETS);

    ASSERT_EQ (SAI_STATUS_SUCCESS, status);



    status = sai_test_tunnel_stats_clear(tunnel_id, 4,
                                         SAI_TUNNEL_STAT_OUT_OCTETS,
                                         SAI_TUNNEL_STAT_OUT_PACKETS,
                                         SAI_TUNNEL_STAT_IN_PACKETS,
                                         SAI_TUNNEL_STAT_IN_OCTETS);
    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    /* Delete Underlay route */
    sai_test_vxlan_route_remove (dflt_vr_id, ip4_af, prefix_str, prefix_len, true);
    sai_test_vxlan_tunnel_term_remove (tunnel_term_id, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_tunnel_port_id, false, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_sub_port_id, false, true);
    sai_test_vxlan_bridge_port_remove (bridge_tunnel_port_id, true);
    sai_test_vxlan_bridge_port_remove (bridge_sub_port_id, true);
    sai_test_vxlan_tunnel_remove (tunnel_id, true);
    sai_test_vxlan_tunnel_map_entry_remove (decap_map_entry_id, true);
    sai_test_vxlan_tunnel_map_entry_remove (encap_map_entry_id, true);
    sai_test_vxlan_tunnel_map_remove (decap_map_id, true);
    sai_test_vxlan_tunnel_map_remove (encap_map_id, true);
    sai_test_vxlan_1d_bridge_remove (bridge_id, true);
}

TEST_F(saiTunnelTest, vxlan_1d_router_bridge_port_create_delete)
{
    sai_status_t sai_rc = SAI_STATUS_SUCCESS;
    sai_object_id_t vr_id_with_src_mac = SAI_NULL_OBJECT_ID;
    sai_object_id_t rif_id = SAI_NULL_OBJECT_ID;
    sai_object_id_t bridge_id = SAI_NULL_OBJECT_ID;
    sai_object_id_t rif_bridge_port_id = SAI_NULL_OBJECT_ID;
    char vrf_mac_str[] = "00:00:00:00:aa:aa";
    char rif_mac_str[] = "00:00:00:00:bb:bb";

    /*Create VRF*/
    sai_rc = saiL3Test::sai_test_vrf_create (&vr_id_with_src_mac, 1,
                                  SAI_VIRTUAL_ROUTER_ATTR_SRC_MAC_ADDRESS,
                                  vrf_mac_str);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /*Create 1D Bridge RIF*/
    sai_rc = saiL3Test::sai_test_rif_create (&rif_id, 3,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id_with_src_mac,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_BRIDGE,
                                  SAI_ROUTER_INTERFACE_ATTR_SRC_MAC_ADDRESS,
                                  rif_mac_str);

    if((sai_rc == SAI_STATUS_NOT_SUPPORTED) ||
       ((SAI_STATUS_CODE(sai_rc) & ~0xFFFF) ==
        SAI_STATUS_CODE(SAI_STATUS_ATTR_NOT_SUPPORTED_0))) {
        printf("Bridge RIF creation not suported\r\n");
        sai_rc = saiL3Test::sai_test_vrf_remove (vr_id_with_src_mac);
        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
        return;
    }

    /*Create 1D Bridge*/
    sai_rc = sai_test_bridge_create(&bridge_id, 1, SAI_BRIDGE_ATTR_TYPE,
                                    SAI_BRIDGE_TYPE_1D);
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_rc);

    /*Create 1D Router bridge port*/
    sai_rc = sai_test_bridge_port_create(&rif_bridge_port_id, 3,
                                         SAI_BRIDGE_PORT_ATTR_TYPE,
                                         SAI_BRIDGE_PORT_TYPE_1D_ROUTER,
                                         SAI_BRIDGE_PORT_ATTR_BRIDGE_ID,
                                         bridge_id,
                                         SAI_BRIDGE_PORT_ATTR_RIF_ID,
                                         rif_id);
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_rc);

    /*Remove 1D Router bridge port*/
    sai_rc = sai_test_bridge_port_remove(rif_bridge_port_id);
    EXPECT_EQ(SAI_STATUS_SUCCESS, sai_rc);

    /*Remove 1D Bridge*/
    sai_rc = sai_test_bridge_remove(bridge_id);
    EXPECT_EQ(SAI_STATUS_SUCCESS, sai_rc);

    /*Remove 1D RIF*/
    sai_rc = saiL3Test::sai_test_rif_remove(rif_id);
    EXPECT_EQ(SAI_STATUS_SUCCESS, sai_rc);

    /*Remove VRF*/
    sai_rc = saiL3Test::sai_test_vrf_remove(vr_id_with_src_mac);
    EXPECT_EQ(SAI_STATUS_SUCCESS, sai_rc);
}

TEST_F (saiTunnelTest, create_and_remove_vxlan_overlay_neighbor)
{
    sai_status_t       sai_rc;
    sai_object_id_t    bridge_id             = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_tunnel_port_id = SAI_NULL_OBJECT_ID;
    sai_object_id_t    vr_id_with_src_mac    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    underlay_rif_id       = dflt_port_rif_id;
    sai_object_id_t    rif_id                = SAI_NULL_OBJECT_ID;
    sai_object_id_t    rif_bridge_port_id    = SAI_NULL_OBJECT_ID;
    char               vrf_mac_str[]         = "00:00:00:00:aa:aa";
    char               rif_mac_str[]         = "00:00:00:00:bb:bb";
    sai_object_id_t    encap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_entry_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_entry_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_id             = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_term_id        = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_sub_port_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_sub_port_id2   = SAI_NULL_OBJECT_ID;
    sai_uint32_t       vnid                  = 100;
    const char         *tunnel_sip           = "10.0.0.1";
    const char         *tunnel_dip           = "20.0.0.1";
    const char         *overlay_ip_1         = "150.0.0.2";
    const char         *overlay_mac_1        = "00:44:44:44:44:44";
    const char         *overlay_ip_2         = "100.0.0.2";
    const char         *overlay_mac_2        = "00:33:33:33:33:33";
    const char         *mac_str_1            = "00:22:22:22:22:22";
    const unsigned int vlan_id               = 10;
    const unsigned int vlan_id2              = 20;
    sai_ip_addr_family_t  ip4_af             = SAI_IP_ADDR_FAMILY_IPV4;
    uint32_t           access_port_idx       = 2;

    /*Create VRF*/
    sai_rc = saiL3Test::sai_test_vrf_create (&vr_id_with_src_mac, 1,
                                  SAI_VIRTUAL_ROUTER_ATTR_SRC_MAC_ADDRESS,
                                  vrf_mac_str);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /*Create 1D Bridge RIF*/
    sai_rc = saiL3Test::sai_test_rif_create (&rif_id, 3,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id_with_src_mac,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_BRIDGE,
                                  SAI_ROUTER_INTERFACE_ATTR_SRC_MAC_ADDRESS,
                                  rif_mac_str);

    if((sai_rc == SAI_STATUS_NOT_SUPPORTED) ||
       ((SAI_STATUS_CODE(sai_rc) & ~0xFFFF) ==
        SAI_STATUS_CODE(SAI_STATUS_ATTR_NOT_SUPPORTED_0))) {
        printf("Bridge RIF creation not suported\r\n");
        sai_rc = saiL3Test::sai_test_vrf_remove (vr_id_with_src_mac);
        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
        return;
    }

    sai_test_vxlan_1d_bridge_create(&bridge_id, true);

    /*Create 1D Router bridge port*/
    sai_rc = sai_test_bridge_port_create(&rif_bridge_port_id, 3,
                                         SAI_BRIDGE_PORT_ATTR_TYPE,
                                         SAI_BRIDGE_PORT_TYPE_1D_ROUTER,
                                         SAI_BRIDGE_PORT_ATTR_BRIDGE_ID,
                                         bridge_id,
                                         SAI_BRIDGE_PORT_ATTR_RIF_ID,
                                         rif_id);

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_rc);

    sai_test_vxlan_encap_tunnel_map_create(&encap_map_id, true);
    sai_test_vxlan_decap_tunnel_map_create(&decap_map_id, true);

    sai_test_vxlan_encap_map_entry_create(&encap_map_entry_id, encap_map_id,
                                          bridge_id, vnid, true);
    sai_test_vxlan_decap_map_entry_create(&decap_map_entry_id, decap_map_id,
                                          vnid, bridge_id, true);

    sai_test_vxlan_tunnel_create(&tunnel_id, dflt_underlay_rif_id, dflt_overlay_rif_id,
                                 tunnel_sip, encap_map_id, decap_map_id, true);

    sai_test_vxlan_access_port_create(&bridge_sub_port_id, bridge_id,
                                      sai_test_tunnel_port_id_get(access_port_idx),
                                      vlan_id, true);

    sai_test_vxlan_access_port_create(&bridge_sub_port_id2, bridge_id,
                                      sai_test_tunnel_port_id_get(access_port_idx),
                                      vlan_id2, true);

    sai_test_vxlan_tunnel_port_create(&bridge_tunnel_port_id, bridge_id,
                                      tunnel_id, true);

    sai_test_vxlan_tunnel_term_create(&tunnel_term_id, dflt_vr_id, tunnel_dip,
                                      tunnel_sip, tunnel_id, true);

    /*Create fdb entries for overlay macs before overlay neighbors are created*/

    /*Point overlay mac 1 to access sub-port*/
    sai_test_vxlan_fdb_entry_create(bridge_id, overlay_mac_1, bridge_sub_port_id,
                                    false, NULL, true);

    /*Point overlay mac 2 to tunnel bridge-port*/
    sai_test_vxlan_fdb_entry_create(bridge_id, overlay_mac_2, bridge_tunnel_port_id,
                                    true, tunnel_dip, true);

    /*Overlay neighbor 1*/
    sai_test_vxlan_neighbor_create(rif_id, ip4_af, overlay_ip_1, overlay_mac_1, true);

    /*Overlay neighbor 2*/
    sai_test_vxlan_neighbor_create(rif_id, ip4_af, overlay_ip_2, overlay_mac_2, true);

    /*Tunnel dip is directly connected neighbor*/
    sai_test_vxlan_neighbor_create(underlay_rif_id, ip4_af, tunnel_dip, mac_str_1, true);

    /* Remove the fdb entries and see if the overlay neighbors are updated
     * properly*/
    sai_test_vxlan_fdb_entry_remove(bridge_id, overlay_mac_1, true);
    sai_test_vxlan_fdb_entry_remove(bridge_id, overlay_mac_2, true);

    /*Create the fdb entries back again*/
    /*Point overlay mac 1 to access sub-port*/
    sai_test_vxlan_fdb_entry_create(bridge_id, overlay_mac_1, bridge_sub_port_id,
                                    false, NULL, true);

    /*Point overlay mac 2 to tunnel bridge-port*/
    sai_test_vxlan_fdb_entry_create(bridge_id, overlay_mac_2, bridge_tunnel_port_id,
                                    true, tunnel_dip, true);

    /*Move the macs*/
    /*Now point overlay mac 2 to subport*/
    sai_test_vxlan_fdb_entry_create(bridge_id, overlay_mac_2, bridge_sub_port_id,
                                    false, NULL, true);

    /*Point overlay mac 1 to tunnel bridge-port*/
    sai_test_vxlan_fdb_entry_create(bridge_id, overlay_mac_1, bridge_tunnel_port_id,
                                    true, tunnel_dip, true);

    /*Trapping packets destined to tunnel dip to cpu*/
    sai_rc = saiL3Test::sai_test_neighbor_attr_set (underlay_rif_id, ip4_af, tunnel_dip,
                                                    SAI_NEIGHBOR_ENTRY_ATTR_PACKET_ACTION,
                                                    SAI_PACKET_ACTION_TRAP, NULL);
    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_vxlan_neighbor_remove(underlay_rif_id, ip4_af, tunnel_dip, true);
    sai_test_vxlan_neighbor_remove(rif_id, ip4_af, overlay_ip_1, true);
    sai_test_vxlan_neighbor_remove(rif_id, ip4_af, overlay_ip_2, true);
    sai_test_vxlan_fdb_entry_remove(bridge_id, overlay_mac_1, true);
    sai_test_vxlan_fdb_entry_remove(bridge_id, overlay_mac_2, true);
    sai_test_vxlan_tunnel_term_remove(tunnel_term_id, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_tunnel_port_id, false, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_sub_port_id, false, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_sub_port_id2, false, true);
    sai_test_vxlan_bridge_port_remove(bridge_tunnel_port_id, true);
    sai_test_vxlan_bridge_port_remove(bridge_sub_port_id, true);
    sai_test_vxlan_bridge_port_remove(bridge_sub_port_id2, true);
    sai_test_vxlan_tunnel_remove(tunnel_id, true);
    sai_test_vxlan_tunnel_map_entry_remove(decap_map_entry_id, true);
    sai_test_vxlan_tunnel_map_entry_remove(encap_map_entry_id, true);
    sai_test_vxlan_tunnel_map_remove(decap_map_id, true);
    sai_test_vxlan_tunnel_map_remove(encap_map_id, true);

    /*Remove 1D Router bridge port*/
    sai_rc = sai_test_bridge_port_remove(rif_bridge_port_id);
    EXPECT_EQ(SAI_STATUS_SUCCESS, sai_rc);

    sai_test_vxlan_1d_bridge_remove (bridge_id, true);

    /*Remove 1D RIF*/
    sai_rc = saiL3Test::sai_test_rif_remove(rif_id);
    EXPECT_EQ(SAI_STATUS_SUCCESS, sai_rc);

    /*Remove VRF*/
    sai_rc = saiL3Test::sai_test_vrf_remove(vr_id_with_src_mac);
    EXPECT_EQ(SAI_STATUS_SUCCESS, sai_rc);

}

TEST_F (saiTunnelTest, create_and_remove_vxlan_overlay_route)
{
    sai_status_t       sai_rc;
    sai_object_id_t    bridge_id             = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_tunnel_port_id = SAI_NULL_OBJECT_ID;
    sai_object_id_t    vr_id_with_src_mac    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    underlay_rif_id       = dflt_port_rif_id;
    sai_object_id_t    rif_id                = SAI_NULL_OBJECT_ID;
    sai_object_id_t    overlay_bridge_nh_id1 = SAI_NULL_OBJECT_ID;
    sai_object_id_t    overlay_bridge_nh_id2 = SAI_NULL_OBJECT_ID;
    sai_object_id_t    rif_bridge_port_id    = SAI_NULL_OBJECT_ID;
    char               vrf_mac_str[]         = "00:00:00:00:aa:aa";
    char               rif_mac_str[]         = "00:00:00:00:bb:bb";
    sai_object_id_t    encap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_entry_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_entry_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_id             = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_term_id        = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_sub_port_id    = SAI_NULL_OBJECT_ID;
    sai_uint32_t       vnid                  = 100;
    const char         *tunnel_sip           = "10.0.0.1";
    const char         *tunnel_dip           = "20.0.0.1";
    const char         *overlay_ip_1         = "150.0.0.2";
    const char         *overlay_mac_1        = "00:44:44:44:44:44";
    const char         *overlay_ip_2         = "100.0.0.2";
    const char         *overlay_mac_2        = "00:33:33:33:33:33";
    const char         *mac_str_1            = "00:22:22:22:22:22";
    const char         *prefix_str1          = "40.0.0.0";
    const char         *prefix_str2          = "50.0.0.0";
    const unsigned int prefix_len            = 24;
    const unsigned int vlan_id               = 10;
    sai_ip_addr_family_t  ip4_af             = SAI_IP_ADDR_FAMILY_IPV4;
    uint32_t           access_port_idx       = 2;

    /*Create VRF*/
    sai_rc = saiL3Test::sai_test_vrf_create (&vr_id_with_src_mac, 1,
                                  SAI_VIRTUAL_ROUTER_ATTR_SRC_MAC_ADDRESS,
                                  vrf_mac_str);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    /*Create 1D Bridge RIF*/
    sai_rc = saiL3Test::sai_test_rif_create (&rif_id, 3,
                                  SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
                                  vr_id_with_src_mac,
                                  SAI_ROUTER_INTERFACE_ATTR_TYPE,
                                  SAI_ROUTER_INTERFACE_TYPE_BRIDGE,
                                  SAI_ROUTER_INTERFACE_ATTR_SRC_MAC_ADDRESS,
                                  rif_mac_str);

    if((sai_rc == SAI_STATUS_NOT_SUPPORTED) ||
       ((SAI_STATUS_CODE(sai_rc) & ~0xFFFF) ==
        SAI_STATUS_CODE(SAI_STATUS_ATTR_NOT_SUPPORTED_0))) {
        printf("Bridge RIF creation not suported\r\n");
        sai_rc = saiL3Test::sai_test_vrf_remove (vr_id_with_src_mac);
        EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
        return;
    }

        sai_test_vxlan_1d_bridge_create(&bridge_id, true);

    /*Create 1D Router bridge port*/
    sai_rc = sai_test_bridge_port_create(&rif_bridge_port_id, 3,
                                         SAI_BRIDGE_PORT_ATTR_TYPE,
                                         SAI_BRIDGE_PORT_TYPE_1D_ROUTER,
                                         SAI_BRIDGE_PORT_ATTR_BRIDGE_ID,
                                         bridge_id,
                                         SAI_BRIDGE_PORT_ATTR_RIF_ID,
                                         rif_id);

    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_rc);

    sai_test_vxlan_encap_tunnel_map_create(&encap_map_id, true);
    sai_test_vxlan_decap_tunnel_map_create(&decap_map_id, true);

    sai_test_vxlan_encap_map_entry_create(&encap_map_entry_id, encap_map_id,
                                          bridge_id, vnid, true);
    sai_test_vxlan_decap_map_entry_create(&decap_map_entry_id, decap_map_id,
                                          vnid, bridge_id, true);

    sai_test_vxlan_tunnel_create(&tunnel_id, dflt_underlay_rif_id, dflt_overlay_rif_id,
                                 tunnel_sip, encap_map_id, decap_map_id, true);

    sai_test_vxlan_access_port_create(&bridge_sub_port_id, bridge_id,
                                      sai_test_tunnel_port_id_get(access_port_idx),
                                      vlan_id, true);

    sai_test_vxlan_tunnel_port_create(&bridge_tunnel_port_id, bridge_id,
                                      tunnel_id, true);

    sai_test_vxlan_tunnel_term_create(&tunnel_term_id, dflt_vr_id, tunnel_dip,
                                      tunnel_sip, tunnel_id, true);

    /*Create fdb entries for overlay macs before overlay neighbors are created*/

    /*Point overlay mac 1 to access sub-port*/
    sai_test_vxlan_fdb_entry_create(bridge_id, overlay_mac_1, bridge_sub_port_id,
                                    false, NULL, true);

    /*Point overlay mac 2 to tunnel bridge-port*/
    sai_test_vxlan_fdb_entry_create(bridge_id, overlay_mac_2, bridge_tunnel_port_id,
                                    true, tunnel_dip, true);

    sai_rc = saiL3Test::sai_test_nexthop_create (&overlay_bridge_nh_id1,
                                                 3,
                                                 SAI_NEXT_HOP_ATTR_TYPE,
                                                 SAI_NEXT_HOP_TYPE_IP,
                                                 SAI_NEXT_HOP_ATTR_IP,
                                                 SAI_IP_ADDR_FAMILY_IPV4,
                                                 overlay_ip_1,
                                                 SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
                                                 rif_id);
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_rc);

    sai_rc = saiL3Test::sai_test_nexthop_create (&overlay_bridge_nh_id2,
                                                 3,
                                                 SAI_NEXT_HOP_ATTR_TYPE,
                                                 SAI_NEXT_HOP_TYPE_IP,
                                                 SAI_NEXT_HOP_ATTR_IP,
                                                 SAI_IP_ADDR_FAMILY_IPV4,
                                                 overlay_ip_2,
                                                 SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID,
                                                 rif_id);
    ASSERT_EQ(SAI_STATUS_SUCCESS, sai_rc);

    /*Overlay route*/
    sai_test_vxlan_route_create(vr_id_with_src_mac, ip4_af, prefix_str1, prefix_len,
                                overlay_bridge_nh_id1, true);

    /*Overlay route*/
    sai_test_vxlan_route_create(vr_id_with_src_mac, ip4_af, prefix_str2, prefix_len,
                                overlay_bridge_nh_id2, true);

    /*Overlay neighbor 1*/
    sai_test_vxlan_neighbor_create(rif_id, ip4_af, overlay_ip_1, overlay_mac_1, true);

    /*Overlay neighbor 2*/
    sai_test_vxlan_neighbor_create(rif_id, ip4_af, overlay_ip_2, overlay_mac_2, true);

    /*Tunnel dip is directly connected neighbor*/
    sai_test_vxlan_neighbor_create(underlay_rif_id, ip4_af, tunnel_dip, mac_str_1, true);

    /* Remove the fdb entries and see if the overlay neighbors are updated
     * properly*/
    sai_test_vxlan_fdb_entry_remove(bridge_id, overlay_mac_1, true);
    sai_test_vxlan_fdb_entry_remove(bridge_id, overlay_mac_2, true);

    /*Create the fdb entries back again*/
    /*Point overlay mac 1 to access sub-port*/
    sai_test_vxlan_fdb_entry_create(bridge_id, overlay_mac_1, bridge_sub_port_id,
                                    false, NULL, true);

    /*Point overlay mac 2 to tunnel bridge-port*/
    sai_test_vxlan_fdb_entry_create(bridge_id, overlay_mac_2, bridge_tunnel_port_id,
                                    true, tunnel_dip, true);

    /*Move the macs*/
    /*Now point overlay mac 2 to subport*/
    sai_test_vxlan_fdb_entry_create(bridge_id, overlay_mac_2, bridge_sub_port_id,
                                    false, NULL, true);

    /*Point overlay mac 1 to tunnel bridge-port*/
    sai_test_vxlan_fdb_entry_create(bridge_id, overlay_mac_1, bridge_tunnel_port_id,
                                    true, tunnel_dip, true);

    /*Trapping packets destined to tunnel dip to cpu*/
    sai_rc = saiL3Test::sai_test_neighbor_attr_set (underlay_rif_id, ip4_af, tunnel_dip,
                                                    SAI_NEIGHBOR_ENTRY_ATTR_PACKET_ACTION,
                                                    SAI_PACKET_ACTION_TRAP, NULL);

    ASSERT_EQ (SAI_STATUS_SUCCESS, sai_rc);

    sai_test_vxlan_neighbor_remove(underlay_rif_id, ip4_af, tunnel_dip, true);
    sai_test_vxlan_neighbor_remove(rif_id, ip4_af, overlay_ip_1, true);
    sai_test_vxlan_neighbor_remove(rif_id, ip4_af, overlay_ip_2, true);
    sai_test_vxlan_route_remove (vr_id_with_src_mac, ip4_af, prefix_str1, prefix_len, true);
    sai_test_vxlan_route_remove (vr_id_with_src_mac, ip4_af, prefix_str2, prefix_len, true);
    sai_rc = saiL3Test::sai_test_nexthop_remove(overlay_bridge_nh_id1);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    sai_rc = saiL3Test::sai_test_nexthop_remove(overlay_bridge_nh_id2);
    EXPECT_EQ (SAI_STATUS_SUCCESS, sai_rc);
    sai_test_vxlan_fdb_entry_remove(bridge_id, overlay_mac_1, true);
    sai_test_vxlan_fdb_entry_remove(bridge_id, overlay_mac_2, true);
    sai_test_vxlan_tunnel_term_remove(tunnel_term_id, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_tunnel_port_id, false, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_sub_port_id, false, true);
    sai_test_vxlan_bridge_port_remove(bridge_tunnel_port_id, true);
    sai_test_vxlan_bridge_port_remove(bridge_sub_port_id, true);
    sai_test_vxlan_tunnel_remove(tunnel_id, true);
    sai_test_vxlan_tunnel_map_entry_remove(decap_map_entry_id, true);
    sai_test_vxlan_tunnel_map_entry_remove(encap_map_entry_id, true);
    sai_test_vxlan_tunnel_map_remove(decap_map_id, true);
    sai_test_vxlan_tunnel_map_remove(encap_map_id, true);

    /*Remove 1D Router bridge port*/
    sai_rc = sai_test_bridge_port_remove(rif_bridge_port_id);
    EXPECT_EQ(SAI_STATUS_SUCCESS, sai_rc);

    sai_test_vxlan_1d_bridge_remove (bridge_id, true);

    /*Remove 1D RIF*/
    sai_rc = saiL3Test::sai_test_rif_remove(rif_id);
    EXPECT_EQ(SAI_STATUS_SUCCESS, sai_rc);

        /*Remove VRF*/
    sai_rc = saiL3Test::sai_test_vrf_remove(vr_id_with_src_mac);
    EXPECT_EQ(SAI_STATUS_SUCCESS, sai_rc);
}

TEST_F (saiTunnelTest, flood_data_packet_on_vxlan_l3_mc_group)
{
    sai_object_id_t    bridge_id             = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_tunnel_port_id = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_id          = SAI_NULL_OBJECT_ID;
    sai_object_id_t    encap_map_entry_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    decap_map_entry_id    = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_id             = SAI_NULL_OBJECT_ID;
    sai_object_id_t    tunnel_term_id        = SAI_NULL_OBJECT_ID;
    sai_object_id_t    bridge_sub_port_id    = SAI_NULL_OBJECT_ID;
    sai_uint32_t       vnid                  = 100;
    const char         *tunnel_sip           = "10.0.0.1";
    const char         *tunnel_dip           = "20.0.0.1";
    const char         *mac_str_1            = "00:22:22:22:22:22";
    const char         *tenant_mac           = "00:00:00:00:00:bb";
    const char         *access_mac           = "00:00:00:00:00:cc";
    const unsigned int vlan_id               = 10;
    uint32_t           access_port_idx       = 1;
    sai_ip_addr_family_t  ip4_af             = SAI_IP_ADDR_FAMILY_IPV4;
    sai_object_id_t    l2mc_group_id         = SAI_NULL_OBJECT_ID;
    sai_object_id_t    l2mc_member_id_1      = SAI_NULL_OBJECT_ID;
    sai_object_id_t    l2mc_member_id_2      = SAI_NULL_OBJECT_ID;

    sai_test_vxlan_1d_bridge_create(&bridge_id, true);

    sai_test_vxlan_encap_tunnel_map_create(&encap_map_id, true);
    sai_test_vxlan_decap_tunnel_map_create(&decap_map_id, true);

    sai_test_vxlan_encap_map_entry_create(&encap_map_entry_id, encap_map_id,
                                          bridge_id, vnid, true);

    sai_test_vxlan_decap_map_entry_create(&decap_map_entry_id, decap_map_id,
                                          vnid, bridge_id, true);

    sai_test_vxlan_tunnel_create(&tunnel_id, dflt_underlay_rif_id, dflt_overlay_rif_id,
                                 tunnel_sip, encap_map_id, decap_map_id, true);

    sai_test_vxlan_access_port_create(&bridge_sub_port_id, bridge_id,
                                      sai_test_tunnel_port_id_get(access_port_idx), vlan_id, true);

    sai_test_vxlan_tunnel_port_create(&bridge_tunnel_port_id, bridge_id,
                                      tunnel_id, true);

    /* This should fail as tunnel termination entry for the tunnel dip has not
     * been created first */
    sai_test_vxlan_fdb_entry_create(bridge_id, tenant_mac, bridge_tunnel_port_id,
                                    true, tunnel_dip, false);

    sai_test_vxlan_tunnel_term_create(&tunnel_term_id, dflt_vr_id, tunnel_dip,
                                      tunnel_sip, tunnel_id, true);

    sai_test_vxlan_fdb_entry_create(bridge_id, tenant_mac, bridge_tunnel_port_id,
                                    true, tunnel_dip, true);

    sai_test_vxlan_fdb_entry_create(bridge_id, access_mac, bridge_sub_port_id,
                                    false, NULL, true);


    /*Tunnel dip is directly connected neighbor*/
    sai_test_vxlan_neighbor_create(dflt_port_rif_id, ip4_af, tunnel_dip, mac_str_1, true);

    sai_test_vxlan_l2mc_group_create(&l2mc_group_id, true);
    sai_test_vxlan_l2mc_group_member_create(&l2mc_member_id_1, l2mc_group_id, bridge_sub_port_id,
                                                false, NULL, true);
    sai_test_vxlan_l2mc_group_member_create(&l2mc_member_id_2, l2mc_group_id, bridge_tunnel_port_id,
                                                    true, tunnel_dip, true);

    sai_test_vxlan_1d_bridge_l2mc_groups_set(bridge_id, true, l2mc_group_id, true, l2mc_group_id,
                                             true, l2mc_group_id, true);

    sai_test_vxlan_flood_packet_on_vxlan_l3mc_index(bridge_id, l2mc_group_id);

    sai_test_vxlan_l2mc_group_member_remove(l2mc_member_id_1, true);
    sai_test_vxlan_l2mc_group_member_remove(l2mc_member_id_2, true);
    sai_test_vxlan_1d_bridge_l2mc_groups_set(bridge_id, false ,  SAI_NULL_OBJECT_ID,
                                            false, SAI_NULL_OBJECT_ID, false,
                                            SAI_NULL_OBJECT_ID, true);
    sai_test_vxlan_l2mc_group_remove(l2mc_group_id, true);

    sai_test_vxlan_neighbor_remove(dflt_port_rif_id, ip4_af, tunnel_dip, true);

    sai_test_vxlan_fdb_entry_remove(bridge_id, tenant_mac, true);
    sai_test_vxlan_fdb_entry_remove(bridge_id, access_mac, true);
    sai_test_vxlan_tunnel_term_remove (tunnel_term_id, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_tunnel_port_id, false, true);
    sai_test_vxlan_bridge_port_admin_state_set(bridge_sub_port_id, false, true);
    sai_test_vxlan_bridge_port_remove(bridge_tunnel_port_id, true);
    sai_test_vxlan_bridge_port_remove(bridge_sub_port_id, true);
    sai_test_vxlan_tunnel_remove (tunnel_id, true);
    sai_test_vxlan_tunnel_map_entry_remove (decap_map_entry_id, true);
    sai_test_vxlan_tunnel_map_entry_remove (encap_map_entry_id, true);
    sai_test_vxlan_tunnel_map_remove (decap_map_id, true);
    sai_test_vxlan_tunnel_map_remove (encap_map_id, true);
    sai_test_vxlan_1d_bridge_remove (bridge_id, true);
}

TEST_F (saiTunnelTest, overlay_next_hop_group_create_remove){

    sai_object_id_t  overlay_nhg_id = SAI_NULL_OBJECT_ID;
    sai_object_id_t  status;
    status = saiL3Test::sai_test_nh_group_create (&overlay_nhg_id, NULL,
                                                  2,
                                                  SAI_NEXT_HOP_GROUP_ATTR_TYPE,
                                                  SAI_NEXT_HOP_GROUP_TYPE_ECMP,
                                                  SAI_NEXT_HOP_GROUP_ATTR_EXTENSIONS_OVERLAY,
                                                  true);
    ASSERT_EQ (SAI_STATUS_SUCCESS, status);

    status =  saiL3Test::sai_test_nh_group_remove(overlay_nhg_id);

    EXPECT_EQ (SAI_STATUS_SUCCESS, status);
}



int main (int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
