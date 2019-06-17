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
 * @file    sairpfgroupextensions.h
 *
 * @brief   This module defines SAI RPF GROUP extensions of the Switch Abstraction Interface (SAI)
 */

#if !defined (__SAIRPFGROUPEXTENSIONS_H_)
#define __SAIRPFGROUPEXTENSIONS_H_

#include <sairpfgroup.h>

/**
 * @brief Extensions Attribute Ids for sai_rpf_group_attr_t
 *
 * @flags Contains flags
 */
typedef enum _sai_rpf_group_extension_attr_t {
      SAI_RPF_GROUP_EXTENSION_ATTR_RANGE_START = SAI_RPF_GROUP_ATTR_CUSTOM_RANGE_END,
    /**
      * @brief IPMC Replication group id
      *
      * @type sai_object_id_t
      * @flags CREATE_AND_SET
      * @objects SAI_OBJECT_TYPE_IPMC_REPL_GROUP
      * @default SAI_NULL_OBJECT_ID
      */
      SAI_RPF_GROUP_EXTENSION_ATTR_IPMC_REPL_GROUP_ID,
     /**
       * @brief End of attributes
       */
      SAI_RPF_GROUP_EXTENSION_ATTR_RANGE_END

} sai_rpf_group_extension_attr_t;

/**
 * @brief Extensions Attribute Ids for sai_rpf_group_member_attr_t
 *
 * @flags Contains flags
 */
typedef enum _sai_rpf_group_member_extension_attr_t {
      SAI_RPF_GROUP_MEMBER_EXTENSION_ATTR_RANGE_START = SAI_RPF_GROUP_MEMBER_ATTR_CUSTOM_RANGE_END,
    /**
      * @brief IPMC Port List Incoming L3 Interface
      *
      * @type sai_object_list_t
      * @flags CREATE_AND_SET
      * @objects SAI_OBJECT_TYPE_PORT
      * @default List having SAI_NULL_OBJECT_ID
      */
      SAI_RPF_GROUP_MEMBER_EXTENSION_ATTR_IPMC_PORT_LIST,
      /**
       * @brief End of attributes
       */
       SAI_RPF_GROUP_MEMBER_EXTENSION_ATTR_RANGE_END

} sai_rpf_group_member_extension_attr_t;

#endif /** __SAIRPFGROUPEXTENSIONS_H_ */
