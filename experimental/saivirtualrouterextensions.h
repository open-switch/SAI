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
 * @file    saivirtualrouterextensions.h
 *
 * @brief   This module defines VIRTUALROUTER extensions of the Switch Abstraction Interface (SAI)
 */

#if !defined (__SAIVIRTUALROUTEREXTENSIONS_H_)
#define __SAIVIRTUALROUTEREXTENSIONS_H_

#include <saivirtualrouter.h>

/**
 * @brief Extensions Attribute Ids for sai_virtual_router_attr
 *
 * @flags Contains flags
 */
typedef enum _sai_virtual_router_extensions_attr_t
{
    SAI_VIRTUAL_ROUTER_EXTENSIONS_ATTR_RANGE_START = SAI_VIRTUAL_ROUTER_ATTR_CUSTOM_RANGE_END,

    /**
     * @brief Admin V4IPMC state
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_VIRTUAL_ROUTER_EXTENSIONS_ATTR_ADMIN_V4IPMC_STATE,

    /**
     * @brief Admin V6IPMC state
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_VIRTUAL_ROUTER_EXTENSIONS_ATTR_ADMIN_V6IPMC_STATE,

    SAI_VIRTUAL_ROUTER_EXTENSIONS_ATTR_RANGE_END

} sai_virtual_router_extensions_attr_t;

#endif /** __SAIVIRTUALROUTEREXTENSIONS_H_ */
