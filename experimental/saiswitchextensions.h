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
 *    Dell EMC, Facebook, Inc., Marvell International Ltd.
 *
 * @file    saiswitchextensions.h
 *
 * @brief   This module defines switch extensions of the Switch Abstraction Interface (SAI)
 */

#ifndef __SAISWITCHEXTENSIONS_H_
#define __SAISWITCHEXTENSIONS_H_

#include <saitypes.h>
#include <saiswitch.h>

#define SAI_KEY_VXLAN_OVERLAY_ECMP_ENABLE      "SAI_KEY_VXLAN_OVERLAY_ECMP_ENABLE"

/**
 * @brief Attribute data for #SAI_SWITCH_ATTR_EXTENSIONS_BST_TRACKING_MODE
 */
typedef enum _sai_switch_bst_tracking_mode
{
    /** Peak value tracking mode */
    SAI_SWITCH_BST_TRACKING_MODE_PEAK,

    /** Current value tracking mode */
    SAI_SWITCH_BST_TRACKING_MODE_CURRENT,

} sai_switch_bst_tracking_mode_t;

/**
 * @brief SAI switch attribute extensions.
 *
 * @flags
 */
typedef enum _sai_switch_attr_extensions_t
{
    SAI_SWITCH_ATTR_EXTENSIONS_RANGE_START = SAI_SWITCH_ATTR_CUSTOM_RANGE_END,

    /**
     * @brief Get all ACL slice objects
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_ACL_SLICE
     * @default internal
     */
    SAI_SWITCH_ATTR_EXTENSIONS_ACL_SLICE_LIST,

    /**
     * @brief Buffer Statistics Tracking mode type.
     *
     * @type sai_switch_bst_tracking_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_SWITCH_BST_TRACKING_MODE_CURRENT
     */
    SAI_SWITCH_ATTR_EXTENSIONS_BST_TRACKING_MODE,

    /**
     * @brief Buffer Statistics Tracking admin enable/disable.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_SWITCH_ATTR_EXTENSIONS_BST_TRACKING_ENABLE,

    /**
     * @brief L3 Interface  user-based ACL meta data range
     *
     * @type sai_u32_range_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_EXTENSIONS_LAYER3_INTERFACE_USER_META_DATA_RANGE,

    /**
     * @brief Update QoS rate adjust value of
     *  policer and shaper
     *
     *  @type sai_uint8_t
     *  @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_EXTENSIONS_QOS_RATE_ADJUST,

    SAI_SWITCH_ATTR_EXTENSIONS_RANGE_END

} sai_switch_attr_extensions_t;

#endif /* __SAISWITCHEXTENSIONS_H_ */
