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
 * @file    sailagextensions.h
 *
 * @brief   This module defines LAG extensions of the Switch Abstraction Interface (SAI)
 */

#ifndef __SAILAGEXTENSIONS_H_
#define __SAILAGEXTENSIONS_H_

#include <sailag.h>
/**
 * @brief LAG attribute: List of extension attributes for LAG object
 */

typedef enum  _sai_lag_attr_extensions_t
{
    SAI_LAG_ATTR_ATTR_EXTENSIONS_RANGE_START = SAI_LAG_ATTR_CUSTOM_RANGE_END,

    /**
     * @brief lag resilient hash enable status
     *
     * when enabled, on trunk member addition, there will be very minimal
     * re-mapping of traffic flows on the pre-existing trunk members; similarly,
     * on trunk member deletion, traffic flowing on remaining available trunk
     * members will not be re-mapped.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */

    SAI_LAG_ATTR_EXTENSIONS_RESILIENT_HASH_ENABLE,

    SAI_LAG_ATTR_ATTR_EXTENSIONS_RANGE_END 

} sai_lag_attr_extensions_t;

#endif /** __SAILAGEXTENSIONS_H_ */
