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
 * @file    saihostifextensions.h
 *
 * @brief   This module defines SAI host interface extensions
 *
 * @par Abstract
 *    This module defines SAI Host Interface which is responsible for
 *    creating/deleting Linux netdev corresponding to the host interface type.
 *    All the management operations of the netdevs such as changing IP address
 *    are outside the scope of SAI.
 */

#if !defined (__SAIHOSTIFEXTENSIONS_H_)
#define __SAIHOSTIFEXTENSIONS_H_

#include <saihostif.h>


typedef enum _sai_hostif_tx_extensions_type_t
{
    SAI_HOSTIF_TX_TYPE_EXTENSIONS_RANGE_START = SAI_HOSTIF_TX_TYPE_CUSTOM_RANGE_END,

    /** HYBRID mode which takes parameters and floods packets
     * on the L2MC group provided    */
    SAI_HOSTIF_TX_TYPE_HYBRID,


    /* End of Extenstions */
    SAI_HOSTIF_TX_TYPE_EXTENSIONS_RANGE_END
}sai_hostif_tx_extensions_type_t;

/**
 * @brief Host interface packet attributes extenstions
 */
typedef enum _sai_hostif_packet_attr_extensions_t
{
    SAI_HOSTIF_PACKET_ATTR_EXTENSIONS_RANGE_START = SAI_HOSTIF_PACKET_ATTR_CUSTOM_RANGE_END,

    /**
     * @brief Egress Bridge ID
     *
     * The .1D Egress bridge on which the packet needs to be transmitted.
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_BRIDGE
     * @condition SAI_HOSTIF_PACKET_ATTR_HOSTIF_TX_TYPE == SAI_HOSTIF_TX_TYPE_HYBRID
     */
    SAI_HOSTIF_PACKET_ATTR_EGR_BRIDGE_ID,

    /**
     ** @brief L2MC Group Object Id
     *
     * This specifies the L2MC group object id on which Packet needs to be flooded
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_L2MC_GROUP
     * @condition SAI_HOSTIF_PACKET_ATTR_HOSTIF_TX_TYPE == SAI_HOSTIF_TX_TYPE_HYBRID
     **/
    SAI_HOSTIF_PACKET_ATTR_EGR_L2MC_GROUP,

    /* End of Extenstions */
    SAI_HOSTIF_PACKET_ATTR_EXTENSIONS_RANGE_END
}sai_hostif_packet_attr_extensions_t;


#endif /** __SAIHOSTIFEXTENSIONS_H_ */
