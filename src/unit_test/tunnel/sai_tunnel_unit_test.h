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
* @file  sai_tunnel_unit_test_utils.h
*
* @brief This file contains class definition, utility and helper
*        function prototypes for testing the SAI Tunnel functionalities.
*
*************************************************************************/

#ifndef __SAI_TUNNEL_UNIT_TEST_H__
#define __SAI_TUNNEL_UNIT_TEST_H__

#include "gtest/gtest.h"

extern "C" {
#include "saitypes.h"
#include "saistatus.h"
#include "saiswitch.h"
#include "saitunnel.h"
#include "saibridge.h"
#include "sail2mcgroup.h"
#include "sailag.h"
#include <stdarg.h>
}

class saiTunnelTest : public ::testing::Test
{
    public:
        /* Method to setup the Tunnel API table pointers. */
        static void sai_test_tunnel_api_table_get (void);
        static void sai_test_bridge_api_table_get (void);
        static void sai_test_fdb_api_table_get (void);
        static void sai_test_l2mc_group_api_table_get (void);
        static void sai_test_lag_api_table_get(void);
        static void sai_test_port_api_table_get(void);

        /* Method to setup default underlay router objects for the tests. */
        static void sai_test_tunnel_underlay_router_setup (void);
        static void sai_test_tunnel_underlay_router_tear_down (void);

        /* Methods for Tunnel object functionality SAI API testing. */
        static sai_status_t sai_test_tunnel_create (
                                               sai_object_id_t *tunnel_id,
                                               unsigned int attr_count, ...);
        static sai_status_t sai_test_tunnel_remove (
                                               sai_object_id_t tunnel_id);
        static sai_status_t sai_test_tunnel_get (sai_object_id_t tunnel_id,
                                                 sai_attribute_t *p_attr_list,
                                                 unsigned int attr_count, ...);

        static void sai_test_tunnel_attr_value_fill (unsigned int attr_count,
                                                     va_list *p_varg_list,
                                                     sai_attribute_t *p_attr_list);

        static void sai_test_tunnel_attr_value_cleanup(unsigned int attr_count,
                                                       sai_attribute_t *p_attr_list);

        /*Methods for Bridge object functionality SAI API testing. */
        static sai_status_t sai_test_bridge_create (sai_object_id_t *bridge_id,
                                                    unsigned int attr_count, ...);

        static sai_status_t sai_test_bridge_remove (sai_object_id_t bridge_id);

        static void sai_test_bridge_attr_value_fill (unsigned int attr_count,
                                                     va_list *p_varg_list,
                                                     sai_attribute_t *p_attr_list);

        static sai_status_t sai_test_bridge_set (sai_object_id_t bridge_id, ...);

        static sai_status_t sai_test_bridge_port_create (sai_object_id_t *bridge_port_id,
                                                         unsigned int attr_count, ...);

        static sai_status_t sai_test_bridge_port_remove (sai_object_id_t bridge_port_id);

        static sai_status_t sai_test_bridge_port_set (sai_object_id_t bridge_port_id, ...);

        static void sai_test_bridge_port_attr_value_fill (unsigned int attr_count,
                                                          va_list *p_varg_list,
                                                          sai_attribute_t *p_attr_list);

        /* Methods for Tunnel Termination Entry SAI API testing. */
        static sai_status_t sai_test_tunnel_term_entry_create(sai_object_id_t *tunnel_term_id,
                                               unsigned int attr_count, ...);

        static sai_status_t sai_test_tunnel_term_entry_remove (
                                               sai_object_id_t tunnel_term_id);
        static sai_status_t sai_test_tunnel_term_entry_get (
                                                 sai_object_id_t tunnel_term_id,
                                                 sai_attribute_t *p_attr_list,
                                                 unsigned int attr_count, ...);
        static void sai_test_tunnel_term_attr_value_fill (unsigned int attr_count,
                                                          va_list *p_varg_list,
                                                          sai_attribute_t *p_attr_list);

        /*Methods for Tunnel map functionality for SAI API testing. */
        static sai_status_t sai_test_tunnel_map_create (
                                                    sai_object_id_t *tunnel_map_id,
                                                    unsigned int attr_count, ...);
        static sai_status_t sai_test_tunnel_map_remove (
                                                    sai_object_id_t tunnel_map_id);

        static sai_status_t sai_test_tunnel_map_get (sai_object_id_t tunnel_map_id,
                                                 sai_attribute_t *p_attr_list,
                                                 unsigned int attr_count, ...);

        static void sai_test_tunnel_map_attr_value_fill (unsigned int attr_count,
                                                     va_list *p_varg_list,
                                                     sai_attribute_t *p_attr_list);

        /*Methods for Tunnel map entry functionality for SAI API testing. */
        static sai_status_t sai_test_tunnel_map_entry_create (
                                                        sai_object_id_t *tunnel_map_entry_id,
                                                        unsigned int attr_count, ...);
        static sai_status_t sai_test_tunnel_map_entry_remove (
                                                        sai_object_id_t tunnel_map_entry_id);

        static sai_status_t sai_test_tunnel_map_entry_set (sai_object_id_t tunnel_map_entry_id,
                                                           ...);

        static sai_status_t sai_test_tunnel_map_entry_get (sai_object_id_t tunnel_map_entry_id,
                                                     sai_attribute_t *p_attr_list,
                                                     unsigned int attr_count, ...);

        static void sai_test_tunnel_map_entry_attr_value_fill (unsigned int attr_count,
                                                         va_list *p_varg_list,
                                                         sai_attribute_t *p_attr_list);

        /*Methods for tunnel stats functionality for SAI API testing */
        static sai_status_t sai_test_tunnel_stats_get (sai_object_id_t tunnel_id,
                                                       unsigned int stats_count, ...);

        static sai_status_t sai_test_tunnel_stats_clear (sai_object_id_t tunnel_id,
                                                         unsigned int stats_count, ...);

        static void sai_test_tunnel_stats_type_fill (unsigned int stats_count,
                                                     va_list *p_varg_list,
                                                     sai_tunnel_stat_t *p_tunnel_stats_list);

        /* Methods for Tunnel Encap Next Hop functionality SAI API testing. */
        static sai_status_t sai_test_tunnel_encap_nh_create (
                                               sai_object_id_t *tunnel_nh_id,
                                               unsigned int attr_count, ...);
        static sai_status_t sai_test_tunnel_encap_nh_remove (
                                               sai_object_id_t tunnel_nh_id);
        static sai_status_t sai_test_tunnel_encap_nh_get (
                                                 sai_object_id_t tunnel_nh_id,
                                                 sai_attribute_t *p_attr_list,
                                                 unsigned int attr_count, ...);

        static sai_object_id_t sai_test_tunnel_port_id_get (uint32_t port_index);
        static sai_object_id_t sai_test_tunnel_bridge_port_id_get (uint32_t port_index);

        /*Methods for FDB related functionality */
        static sai_status_t sai_test_tunnel_fdb_entry_create(sai_fdb_entry_type_t type,
                                                             sai_object_id_t bv_id,
                                                             const char *p_mac_str,
                                                             sai_packet_action_t packet_action,
                                                             sai_object_id_t bridge_port_id,
                                                             bool is_tunnel_port,
                                                             const char *p_remote_ip);

        static sai_status_t sai_test_tunnel_fdb_entry_remove(sai_object_id_t bv_id,
                                                             const char *p_mac_str);

        /*Methods for L2MC group related functionality */
        static sai_status_t sai_test_tunnel_l2mc_group_create(sai_object_id_t *l2mc_group_id);

        static sai_status_t sai_test_tunnel_l2mc_group_remove(sai_object_id_t l2mc_group_id);

        static void sai_test_l2mc_group_member_attr_value_fill (unsigned int attr_count,
                                                                va_list *p_varg_list,
                                                                sai_attribute_t *p_attr_list);

        static sai_status_t sai_test_tunnel_l2mc_group_member_create (
                                                        sai_object_id_t *l2mc_group_member_id,
                                                        unsigned int attr_count, ...);

        static sai_status_t sai_test_tunnel_l2mc_group_member_remove(
                                                       sai_object_id_t l2mc_group_member_id);

        /*Methods for LAG group related functionality */
        static sai_status_t sai_test_tunnel_lag_create(sai_object_id_t *lag_id);

        static sai_status_t sai_test_tunnel_lag_remove(sai_object_id_t lag_id);

        static sai_status_t sai_test_tunnel_lag_member_create(sai_object_id_t *lag_member,
                                                              sai_object_id_t lag_id,
                                                              sai_object_id_t port_id);

        static sai_status_t sai_test_tunnel_lag_member_remove(sai_object_id_t lag_member);

        /*Util for converting mac string to byte representation*/
        static void sai_test_mac_str_to_bytes_get (const char *p_mac_str, uint8_t *mac);

        /* Util for converting to attribute index based status code */
        static inline sai_status_t sai_test_invalid_attr_status_code (
                                                       sai_status_t status,
                                                       unsigned int attr_index)
        {
            return (status + SAI_STATUS_CODE (attr_index));
        }

        /* Method for retrieving the switch API table pointer */
        static inline sai_switch_api_t* switch_api_tbl_get (void)
        {
            return p_sai_switch_api_tbl;
        }

        /* Method for retrieving the port API table pointer */
        static inline sai_port_api_t* port_api_tbl_get (void)
        {
            return p_sai_port_api_tbl;
        }

        /* Method for retrieving the Tunnel API table pointer */
        static inline sai_tunnel_api_t* tunnel_api_tbl_get (void)
        {
            return p_sai_tunnel_api_tbl;
        }

        /* Method for retrieving the Bridge API table pointer */
        static inline sai_bridge_api_t* bridge_api_tbl_get (void)
        {
            return p_sai_bridge_api_tbl;
        }

        static inline sai_fdb_api_t* fdb_api_tbl_get (void)
        {
            return p_sai_fdb_api_tbl;
        }

        static inline sai_lag_api_t* lag_api_tbl_get (void)
        {
            return p_sai_lag_api_tbl;
        }

    protected:
        static void SetUpTestCase (void);
        static void TearDownTestCase (void);

        static const unsigned int max_tunnel_obj_attr_count = 18;
        static const unsigned int max_bridge_obj_attr_count = (SAI_BRIDGE_ATTR_END -
                                                               SAI_BRIDGE_ATTR_START);
        static const unsigned int max_bridge_port_obj_attr_count = (SAI_BRIDGE_PORT_ATTR_END -
                                                                    SAI_BRIDGE_PORT_ATTR_START);
        static const unsigned int max_tunnel_map_obj_attr_count = (SAI_TUNNEL_MAP_ATTR_END -
                                                                   SAI_TUNNEL_MAP_ATTR_START);
        static const unsigned int max_tunnel_map_entry_obj_attr_count =
                                                              (SAI_TUNNEL_MAP_ENTRY_ATTR_END -
                                                               SAI_TUNNEL_MAP_ENTRY_ATTR_START);
        static const unsigned int dflt_tunnel_obj_attr_count = 4;
        static const unsigned int max_tunnel_term_attr_count = 6;
        static const unsigned int max_tunnel_stats_count = 4;
        static const unsigned int max_l2mc_group_member_attr_count =
                                                              (SAI_L2MC_GROUP_MEMBER_ATTR_END -
                                                               SAI_L2MC_GROUP_MEMBER_ATTR_START);
        static const unsigned int dflt_tunnel_encap_nh_attr_count = 4;
        static const unsigned int SAI_TEST_MAX_PORTS = 256;
        static const unsigned int test_vlan = 100;

        static sai_object_id_t   test_vlan_obj_id;
        static sai_object_id_t   dflt_vr_id;
        static sai_object_id_t   dflt_overlay_vr_id;
        static sai_object_id_t   dflt_port_rif_id;
        static sai_object_id_t   dflt_vlan_rif_id;
        static sai_object_id_t   dflt_overlay_rif_id;
        static sai_object_id_t   dflt_underlay_rif_id;
        static sai_object_id_t   dflt_underlay_port_nh_id;
        static sai_object_id_t   dflt_underlay_vlan_nh_id;
        static sai_object_id_t   dflt_underlay_nhg_id;
        static const char       *nh_ip_str_1;
        static const char       *nh_ip_str_2;

    private:

        static sai_switch_api_t     *p_sai_switch_api_tbl;
        static sai_port_api_t       *p_sai_port_api_tbl;
        static sai_tunnel_api_t     *p_sai_tunnel_api_tbl;
        static sai_bridge_api_t     *p_sai_bridge_api_tbl;
        static sai_fdb_api_t        *p_sai_fdb_api_tbl;
        static sai_l2mc_group_api_t *p_sai_l2mc_group_api_tbl;
        static sai_lag_api_t        *p_sai_lag_api_tbl;
        static unsigned int          port_count;
        static sai_object_id_t       port_list[SAI_TEST_MAX_PORTS];
        static unsigned int          bridge_port_count;
        static sai_object_id_t       bridge_port_list[SAI_TEST_MAX_PORTS];
        static sai_object_id_t       default_bridge_id;
};

#endif /* __SAI_TUNNEL_UNIT_TEST_H__ */
