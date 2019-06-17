/**
 * Copyright (c) 2018 Microsoft Open Technologies, Inc.
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
 * @file    sainexthopgroupextensions.h
 *
 * @brief   This module defines Next hop group extensions of the
 *          Switch Abstraction Interface (SAI)
 */

#if !defined (__SAINEXTHOPGROUPEXTENSIONS_H_)
#define __SAINEXTHOPGROUPEXTENSIONS_H_

#include <sainexthopgroup.h>

/**
 * @brief List of extension attributes for next hop group object
 */

typedef enum _sai_next_hop_group_attr_extensions_t
{
    SAI_NEXT_HOP_GROUP_ATTR_EXTENSIONS_RANGE_START = SAI_NEXT_HOP_GROUP_ATTR_CUSTOM_RANGE_END,

    /**
     * @brief ECMP group resilient hash enable status
     *
     * when enabled, on ecmp member addition, there will be very minimal
     * re-mapping of traffic flows on the pre-existing ecmp members; similarly,
     * on ecmp member deletion, traffic flowing on remaining available ecmp
     * members will not be re-mapped.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_NEXT_HOP_GROUP_ATTR_EXTENSIONS_RESILIENT_HASH_ENABLE,

    /**
     * @brief Overlay ECMP enable status 
     * 
     * When enabled, an overlay hierarchial ECMP group is created.
     * The overlay ECMP group can contain only overlay nexthops i.e.
     * nexthops with bridge router interfaces.
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     *
     */

    SAI_NEXT_HOP_GROUP_ATTR_EXTENSIONS_OVERLAY,

    SAI_NEXT_HOP_GROUP_ATTR_EXTENSIONS_RANGE_END


} sai_next_hop_group_attr_extensions_t;

#endif /** __SAINEXTHOPGROUPEXTENSIONS_H_ */
