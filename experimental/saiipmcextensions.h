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
 *    FOR A PARTICULAR PURPOSE, MERCHANTABILITY OR NON-INFRINGEMENT.
 *
 *    See the Apache Version 2.0 License for specific language governing
 *    permissions and limitations under the License.
 *
 *    Microsoft would like to thank the following companies for their review and
 *    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
 *    Dell Products, L.P., Facebook, Inc., Marvell International Ltd.
 *
 * @file    saiipmcextensions.h
 *
 * @brief   This module defines SAI IPMC extensions of the Switch Abstraction Interface (SAI)
 */

#if !defined (__SAIIPMCEXTENSIONS_H_)
#define __SAIIPMCEXTENSIONS_H_

#include <saiipmc.h>

/**
 * @brief Custom IPMC entry key
 */
typedef struct _sai_ipmc_entry_extn_t
{
    /**
     * @brief Switch ID
     *
     * @objects SAI_OBJECT_TYPE_SWITCH
     */
    sai_object_id_t switch_id;

    /**
     * @brief Virtual Router ID
     *
     * @objects SAI_OBJECT_TYPE_VIRTUAL_ROUTER
     */
    sai_object_id_t vr_id;

    /** IPMC entry type */
    sai_ipmc_entry_type_t type;

    /** IP destination address */
    sai_ip_address_t destination;

    /** IP source address */
    sai_ip_address_t source;

    /**
     * @brief Incoming Router Interface ID
     *
     * @objects SAI_OBJECT_TYPE_ROUTER_INTERFACE
     */
    sai_object_id_t inrif_id;

} sai_ipmc_entry_extn_t;

/**
 * @brief Extensions Attribute Ids for sai_ipmc_entry_attr_t
 */
typedef enum _sai_ipmc_entry_extension_attr_t
{
    SAI_IPMC_ENTRY_EXTENSION_ATTR_RANGE_START = SAI_IPMC_ENTRY_ATTR_CUSTOM_RANGE_END,

    /**
      * @brief IPMC (S,G) Entry Hit bit status
      *
      * @type boolean
      * @flags READ_ONLY
      */
    SAI_IPMC_ENTRY_EXTENSION_ATTR_IPMC_SG_HIT,

    /**
      * @brief End of attributes
      */
    SAI_IPMC_ENTRY_EXTENSION_ATTR_RANGE_END
} sai_ipmc_entry_extension_attr_t;

/**
 * @defgroup SAIIPMCREPLGROUP SAI - IPMC REPLICATION group specific API definitions
 *
 * @{
 */

typedef enum _sai_ipmc_repl_group_owner_t
{
     /* SAI IPMC Replication Group Owner is L2MC */
    SAI_IPMC_REPLICATION_GROUP_OWNER_L2MC,

     /* SAI IPMC Replication Group Owner is IPMC */
    SAI_IPMC_REPLICATION_GROUP_OWNER_IPMC,

     /* SAI IPMC Replication Group Owner is VXLAN */
   SAI_IPMC_REPLICATION_GROUP_OWNER_VXLAN,

   /*  Need to add new SAI Replication Group Owner above this line */
  SAI_IPMC_REPLICATION_GROUP_OWNER_NONE

} sai_ipmc_repl_group_owner_t;

/**
 * @brief Attributes for IPMC Replication group
 *
 * @flags Contains flags
 */
typedef enum _sai_ipmc_repl_group_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_IPMC_REPL_GROUP_ATTR_START,

    /**
     * @brief Replication Group owner, who intended to allocate a new IPMC Replication resource
     *
     * @type sai_ipmc_repl_group_owner_t
     * @flags CREATE_AND_SET
     * @default  SAI_IPMC_REPLICATION_GROUP_OWNER_NONE
     */
    SAI_IPMC_REPL_GROUP_ATTR_REPL_OWNER = SAI_IPMC_REPL_GROUP_ATTR_START,

    /**
     * @brief End of attributes
     */
    SAI_IPMC_REPL_GROUP_ATTR_END,

    /** Custom range base value */
    SAI_IPMC_REPL_GROUP_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_IPMC_REPL_GROUP_ATTR_CUSTOM_RANGE_END

} sai_ipmc_repl_group_attr_t;

/**
 * @brief Create IPMC Replication group
 *
 * @param[out] ipmc_repl_group_id IPMC Replication group id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_ipmc_repl_group_fn)(
        _Out_ sai_object_id_t *ipmc_repl_group_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove IPMC Replication group
 *
 * @param[in] ipmc_repl_group_id IPMC Replication group id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_ipmc_repl_group_fn)(
        _In_ sai_object_id_t ipmc_repl_group_id);

/**
 * @brief Set IPMC Replication Group attribute
 *
 * @param[in] ipmc_repl_group_id IPMC repl group id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_ipmc_repl_group_attribute_fn)(
        _In_ sai_object_id_t ipmc_repl_group_id,
        _In_ const sai_attribute_t *attr);
/**
 * @brief Get IPMC Replication Group attribute
 *
 * @param[in] ipmc_repl_group_id IPMC Replication group id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_ipmc_repl_group_attribute_fn)(
        _In_ sai_object_id_t ipmc_repl_group_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief IPMC Replication group methods table retrieved with sai_api_query()
 */
typedef struct _sai_ipmc_repl_group_api_t
{
    sai_create_ipmc_repl_group_fn         create_ipmc_repl_group;
    sai_remove_ipmc_repl_group_fn         remove_ipmc_repl_group;
    sai_set_ipmc_repl_group_attribute_fn  set_ipmc_repl_group_attribute;
    sai_get_ipmc_repl_group_attribute_fn  get_ipmc_repl_group_attribute;
} sai_ipmc_repl_group_api_t;

/**
 * @}
 */

#endif /** __SAIIPMCEXTENSIONS_H_ */
