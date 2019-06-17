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
 *    Dell EMC, Facebook, Inc., Marvell International Ltd.
 *
 * @file    saiaclextensions.h
 *
 * @brief   This module defines ACL extensions of the Switch Abstraction Interface (SAI)
 */

#if !defined (__SAIACLEXTENSIONS_H_)
#define __SAIACLEXTENSIONS_H_

#include <saiacl.h>

/**
 * @brief Attribute Id for sai_acl_table
 *
 * @flags Contains flags
 */
typedef enum _sai_acl_table_attr_extensions_t
{
    SAI_ACL_TABLE_ATTR_EXTENSIONS_RANGE_START = SAI_ACL_TABLE_ATTR_CUSTOM_RANGE_END,

    /**
     * @brief Number of used entries for all pipes
     *        1st entry in the list points to pipe-0,next points to pipe-1 and so on.
     * @type sai_u32_list_t
     * @flags READ_ONLY
     */
    SAI_ACL_TABLE_ATTR_EXTENSIONS_USED_ACL_ENTRY_LIST,

    /**
     * @brief Number of free entry space available in
     *        the current table for all pipes
     *        1st entry in the list points to pipe-0,next points to pipe-1 and so on.
     * @type sai_u32_list_t
     * @flags READ_ONLY
     */
    SAI_ACL_TABLE_ATTR_EXTENSIONS_AVAILABLE_ACL_ENTRY_LIST,

    /**
     * @brief Layer 3 Interface Lookup Meta Data.
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_ACL_TABLE_ATTR_EXTENSIONS_FIELD_LAYER3_INTERFACE_USER_META,

    /**
     * @brief Lookup match in Multicast Route table
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_ACL_TABLE_ATTR_EXTENSIONS_FIELD_MCAST_ROUTE_NPU_META_DST_HIT,

    SAI_ACL_TABLE_ATTR_EXTENSIONS_RANGE_END

} sai_acl_table_attr_extensions_t;

/**
 * @brief Attribute data for Layer 3 Interface User Meta Data
 */
typedef enum _sai_acl_layer3_intf_protocol_class_t
{
    /** Layer3 Interface Protocol class None*/
    SAI_ACL_LAYER3_INTERFACE_PRTCL_CLASS_NONE,

    /** Layer3 Interface IPv4 PIMv2 Protocol class */
    SAI_ACL_LAYER3_INTERFACE_IPV4_PIMV2_PRTCL_CLASS,

    /** Layer3 Interface IPv6 PIMv2 Protocol class */
    SAI_ACL_LAYER3_INTERFACE_IPV6_PIMV2_PRTCL_CLASS

} sai_acl_layer3_intf_protocol_class_t;

/**
 * @brief Attribute Id extensions for sai_acl_entry
 *
 * @flags Contains flags
 */
typedef enum _sai_acl_entry_attr_extensions_t
{
    SAI_ACL_ENTRY_ATTR_EXTENSIONS_RANGE_START = SAI_ACL_ENTRY_ATTR_CUSTOM_RANGE_END,
    /**
     * @brief Layer 3 Interface Lookup Meta Data - Vendor extension.
     *
     * @type sai_acl_field_data_t sai_acl_layer3_intf_protocol_class_t
     * @flags CREATE_AND_SET
     * @default disabled
     */
    SAI_ACL_ENTRY_ATTR_EXTENSIONS_FIELD_LAYER3_INTERFACE_USER_META=SAI_ACL_ENTRY_ATTR_EXTENSIONS_RANGE_START,

    /**
     * @brief Lookup match in Multicast Route table - Vendor extension
     *
     * @type sai_acl_field_data_t bool
     * @flags CREATE_AND_SET
     * @default disabled
     */
    SAI_ACL_ENTRY_ATTR_EXTENSIONS_FIELD_MCAST_ROUTE_NPU_META_DST_HIT,

    SAI_ACL_ENTRY_ATTR_EXTENSIONS_RANGE_END

} sai_acl_entry_attr_extensions_t;

#endif /** __SAIACLEXTENSIONS_H_ */
