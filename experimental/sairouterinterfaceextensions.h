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
 * @file    sairouterinterfaceextensions.h
 *
 * @brief   This module defines ROUTERINTERFACE extensions of the Switch Abstraction Interface (SAI)
 */

#if !defined (__SAIROUTERINTERFACEEXTENSIONS_H_)
#define __SAIROUTERINTERFACEEXTENSIONS_H_

#include <sairouterinterface.h>

/**
 * @brief Extensions Attribute Ids for sai_router_interface_attr
 *
 * @flags Contains flags
 */
typedef enum _sai_router_interface_attr_extensions_t
{
    SAI_ROUTER_INTERFACE_EXTENSIONS_ATTR_RANGE_START = SAI_ROUTER_INTERFACE_ATTR_CUSTOM_RANGE_END,

    /**
     * @brief associated router interface’s admin PIMv2 state
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_ROUTER_INTERFACE_EXTENSIONS_ATTR_PIMV2_ENABLE,

    /**
     * @brief associated router interface’s admin PIMv6 state
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_ROUTER_INTERFACE_EXTENSIONS_ATTR_PIMV6_ENABLE,

    SAI_ROUTER_INTERFACE_EXTENSIONS_ATTR_RANGE_END

} sai_router_interface_attr_extensions_t;

#endif /** __SAIROUTERINTERFACEEXTENSIONS_H_ */
