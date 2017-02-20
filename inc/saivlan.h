/**
 * Copyright (c) 2014 Microsoft Open Technologies, Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License"); you may
 *    not use this file except in compliance with the License. You may obtain
 *    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 *    THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
 *    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 *    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
 *    FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
 *
 *    See the Apache Version 2.0 License for specific language governing
 *    permissions and limitations under the License.
 *
 *    Microsoft would like to thank the following companies for their review and
 *    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
 *    Dell Products, L.P., Facebook, Inc
 *
 * @file    saivlan.h
 *
 * @brief   This module defines SAI VLAN interface
 */

#if !defined (__SAIVLAN_H_)
#define __SAIVLAN_H_

#include <saitypes.h>

/**
 * @defgroup SAIVLAN SAI - VLAN specific API definitions
 *
 * @{
 */

/**
 * @def VLAN_COUNTER_SET_DEFAULT
 */
#define VLAN_COUNTER_SET_DEFAULT 0

/**
 * @brief Attribute data for tagging_mode parameter
 */
typedef enum _sai_vlan_tagging_mode_t
{
    SAI_VLAN_TAGGING_MODE_UNTAGGED,

    SAI_VLAN_TAGGING_MODE_TAGGED,

    SAI_VLAN_TAGGING_MODE_PRIORITY_TAGGED

} sai_vlan_tagging_mode_t;

/**
 *   @brief Port/vlan membership structure
 */
typedef struct _sai_vlan_port_t
{
    sai_object_id_t port_id;

    sai_vlan_tagging_mode_t tagging_mode;

} sai_vlan_port_t;

/**
 * @brief Attribute Id in sai_set_vlan_attribute() and
 * sai_get_vlan_attribute() calls
 */
typedef enum _sai_vlan_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_VLAN_ATTR_START,

    /**
     * @brief List of vlan members in a VLAN
     *
     * @type sai_vlan_port_list_t
     * @flags READ_ONLY
     */
    SAI_VLAN_ATTR_PORT_LIST = SAI_VLAN_ATTR_START,

    /**
     * @brief Maximum number of learned MAC addresses
     *
     * Zero means learning limit disable
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_VLAN_ATTR_MAX_LEARNED_ADDRESSES,

    /**
     * @brief STP Instance that the VLAN is associated to
     *
     * Ddefault to default stp instance id
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_STP
     * @flags CREATE_AND_SET
     */
    SAI_VLAN_ATTR_STP_INSTANCE,

    /**
     * @brief To disable learning on a VLAN
     *
     * This should override port learn settings. If this is set to true on a
     * vlan, then the source mac learning is disabled for this vlan on a member
     * port even if learn is enable on the port(based on port learn attribute)
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_VLAN_ATTR_LEARN_DISABLE,

    /**
     * @brief User based Meta Data
     *
     * Value Range #SAI_SWITCH_ATTR_VLAN_USER_META_DATA_RANGE
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_VLAN_ATTR_META_DATA,

    /**
     * @brief End of attributes
     */
    SAI_VLAN_ATTR_END,

    /** Custom range base value */
    SAI_VLAN_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End oo custom range base */
    SAI_VLAN_ATTR_CUSTOM_RANGE_END

} sai_vlan_attr_t;

/**
 * @brief VLAN counter IDs in sai_get_vlan_stats() call
 */
typedef enum _sai_vlan_stat_t
{
    SAI_VLAN_STAT_IN_OCTETS,
    SAI_VLAN_STAT_IN_PACKETS,
    SAI_VLAN_STAT_IN_UCAST_PKTS,
    SAI_VLAN_STAT_IN_NON_UCAST_PKTS,
    SAI_VLAN_STAT_IN_DISCARDS,
    SAI_VLAN_STAT_IN_ERRORS,
    SAI_VLAN_STAT_IN_UNKNOWN_PROTOS,
    SAI_VLAN_STAT_OUT_OCTETS,
    SAI_VLAN_STAT_OUT_PACKETS,
    SAI_VLAN_STAT_OUT_UCAST_PKTS,
    SAI_VLAN_STAT_OUT_NON_UCAST_PKTS,
    SAI_VLAN_STAT_OUT_DISCARDS,
    SAI_VLAN_STAT_OUT_ERRORS,
    SAI_VLAN_STAT_OUT_QLEN

} sai_vlan_stat_t;

/**
 * @brief Create a VLAN
 *
 * @param[in] vlan_id VLAN id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_vlan_fn)(
        _In_ sai_vlan_id_t vlan_id);

/**
 * @brief Remove a VLAN
 *
 * @param[in] vlan_id VLAN id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_vlan_fn)(
        _In_ sai_vlan_id_t vlan_id);

/**
 * @brief Set VLAN attribute Value
 *
 * @param[in] vlan_id VLAN id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_vlan_attribute_fn)(
        _In_ sai_vlan_id_t vlan_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get VLAN attribute Value
 *
 * @param[in] vlan_id VLAN id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_vlan_attribute_fn)(
        _In_ sai_vlan_id_t vlan_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * Routine Description:
 *    @brief Add Port to VLAN
 *
 * Arguments:
 *    @param[in] vlan_id - VLAN id
 *    @param[in] port_count - number of ports
 *    @param[in] port_list - pointer to membership structures
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_add_ports_to_vlan_fn)(
    _In_ sai_vlan_id_t vlan_id,
    _In_ uint32_t port_count,
    _In_ const sai_vlan_port_t *port_list
    );


/**
 * Routine Description:
 *    @brief Remove Port from VLAN
 *
 * Arguments:
 *    @param[in] vlan_id - VLAN id
 *    @param[in] port_count - number of ports
 *    @param[in] port_list - pointer to membership structures
 *
 * Return Values:
 *    @return SAI_STATUS_SUCCESS on success
 *            Failure status code on error
 */
typedef sai_status_t (*sai_remove_ports_from_vlan_fn)(
    _In_ sai_vlan_id_t vlan_id,
    _In_ uint32_t port_count,
    _In_ const sai_vlan_port_t* port_list
    );

/**
 * @brief Get vlan statistics counters.
 *
 * @param[in] vlan_id VLAN id
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] number_of_counters Number of counters in the array
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_vlan_stats_fn)(
        _In_ sai_vlan_id_t vlan_id,
        _In_ const sai_vlan_stat_t *counter_ids,
        _In_ uint32_t number_of_counters,
        _Out_ uint64_t *counters);

/**
 * @brief Clear vlan statistics counters.
 *
 * @param[in] vlan_id Vlan id
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] number_of_counters Number of counters in the array
 *
 * @return SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_clear_vlan_stats_fn)(
        _In_ sai_vlan_id_t vlan_id,
        _In_ const sai_vlan_stat_t *counter_ids,
        _In_ uint32_t number_of_counters);

/**
 * @brief VLAN methods table retrieved with sai_api_query()
 */
typedef struct _sai_vlan_api_t
{
    sai_create_vlan_fn                  create_vlan;
    sai_remove_vlan_fn                  remove_vlan;
    sai_set_vlan_attribute_fn           set_vlan_attribute;
    sai_get_vlan_attribute_fn           get_vlan_attribute;
    sai_add_ports_to_vlan_fn            add_ports_to_vlan;
    sai_remove_ports_from_vlan_fn       remove_ports_from_vlan;
    sai_get_vlan_stats_fn               get_vlan_stats;
    sai_clear_vlan_stats_fn             clear_vlan_stats;

} sai_vlan_api_t;

/**
 *@}
 */
#endif /** __SAIVLAN_H_ */
