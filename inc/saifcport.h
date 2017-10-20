/************************************************************************
* LEGALESE:   "Copyright (c) 2016, Dell Inc. All rights reserved."
*
* This source code is confidential, proprietary, and contains trade
* secrets that are the sole property of Dell Inc.
* Copy and/or distribution of this source code or disassembly or reverse
* engineering of the resultant object code are strictly forbidden without
* the written consent of Dell Inc.
*
************************************************************************/
/**
* @file saifcport.h
*
* @brief This file defines SAI FC Port functionality API
* @description Supported only Dell Inc. SAI Implementation
* @warning This module is a SAI extension module
*
*************************************************************************/

#if !defined (__SAIFCPORT_H_)
#define __SAIFCPORT_H_

#include "saitypes.h"

/**
 * @defgroup SAIFCPORT SAI - FC Port specific API definitions
 *
 * @{
 */

/**
 *  @brief Attribute data for SAI_FC_PORT_ATTR_OPER_STATUS
 */
typedef enum _sai_fc_port_oper_status_t
{
    /** Unknown */
    SAI_FC_PORT_OPER_STATUS_UNKNOWN,

    /** Up */
    SAI_FC_PORT_OPER_STATUS_UP,

    /** Down */
    SAI_FC_PORT_OPER_STATUS_DOWN,
} sai_fc_port_oper_status_t;


/**
 * @brief Attributes data for SAI_FC_MAP_MAC_MODE
 */
typedef enum _sai_fc_map_mac_mode_t
{
   SAI_FC_MAP_MAC_FPMA_MODE,

   SAI_FC_MAP_MAC_NULL_MODE,

   SAI_FC_MAP_MAC_USR_MODE

} sai_fc_map_mac_mode_t;


/**
 * @brief Defines the operational status of the port
 */
typedef struct _sai_fc_port_oper_status_notification_t {

    /** Port id */
    sai_object_id_t port_id;

    /** Port operational status */
    sai_fc_port_oper_status_t port_state;

} sai_fc_port_oper_status_notification_t;

/**
 * @brief Attribute data for SAI_FC_PORT_ATTR_MEDIA_TYPE
 */
typedef enum _sai_fc_port_media_type_t
{
    /** Media not present */
    SAI_FC_PORT_MEDIA_TYPE_NOT_PRESENT,

    /** Media type not known */
    SAI_FC_PORT_MEDIA_TYPE_UNKNONWN,

    /** Media type QSFP fiber optic */
    SAI_FC_PORT_MEDIA_TYPE_QSFP_FIBER,

    /** Media type QSFP copper optic */
    SAI_FC_PORT_MEDIA_TYPE_QSFP_COPPER,

    /** Media type SFP fiber optic */
    SAI_FC_PORT_MEDIA_TYPE_SFP_FIBER,

    /** Media type SFP copper optic */
    SAI_FC_PORT_MEDIA_TYPE_SFP_COPPER,

    /** Media type QSFP28 fiber optic */
    SAI_FC_PORT_MEDIA_TYPE_QSFP28_FIBER,

    /** Media type QSFP28 copper optic */
    SAI_FC_PORT_MEDIA_TYPE_QSFP28_COPPER,

} sai_fc_port_media_type_t;

/**
 * @brief Attribute data for SAI_FC_PORT_Attribute data for
 * SAI_FC_PORT_ATTR_MEDIA_SPEED_CAPABILITY
 */
typedef enum _sai_fc_port_media_speed_capability_t {
    /** Media speed 4G */
    SAI_FC_MEDIA_CAP_SPEED_FOUR_GIG          = (1 << 0),

    /** Media speed 8G */
    SAI_FC_MEDIA_CAP_SPEED_EIGHT_GIG         = (1 << 1),

    /** Media speed 16G */
    SAI_FC_MEDIA_CAP_SPEED_SIXTEEN_GIG       = (1 << 2),

    /** Media speed 32G */
    SAI_FC_MEDIA_CAP_SPEED_THIRTY_TWO_GIG    = (1 << 3),

    SAI_FC_PORT_CAP_SPEED_MAX             = (1 << 4),
} sai_fc_port_media_speed_capability_t;

/**
 * @brief Attribute Id in sai_set_fc_port_attribute() and
 * sai_get_fc_port_attribute() calls
 */
typedef enum _sai_fc_port_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_FC_PORT_ATTR_START,

    /* READ-ONLY */

    /**
     * @brief BB credit received
     *
     * @type sai_int32_t
     * @flags READ_ONLY
     */
    SAI_FC_PORT_ATTR_BB_CREDIT_RX = SAI_FC_PORT_ATTR_START,

    /**
     * @brief Operational Status
     *
     * @type sai_fc_port_oper_status_t
     * @flags READ_ONLY
     */
    SAI_FC_PORT_ATTR_OPER_STATUS,

    /**
     * @brief Query list of supported FC speeds(full-duplex) in Mbps
     *
     * @type sai_u32_list_t
     * @flags READ_ONLY
     */
    SAI_FC_PORT_ATTR_SUPPORTED_SPEED,

    /* READ-WRITE */

    /**
     * @brief Hardware Lane list
     *
     * @type sai_u32_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY | KEY
     */
    SAI_FC_PORT_ATTR_HW_LANE_LIST,

    /**
     * @brief Speed in Mbps
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_FC_PORT_ATTR_SPEED,

    /**
     * @brief Auto Negotiation configuration
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_FC_PORT_ATTR_AUTO_NEG_MODE,

    /**
     * @brief Admin Mode
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_FC_PORT_ATTR_ADMIN_STATE,

    /**
     * @brief FC Media type
     *
     * @type sai_fc_port_media_type_t
     * @flags CREATE_AND_SET
     * @default SAI_FC_PORT_MEDIA_TYPE_NOT_PRESENT
     */
    SAI_FC_PORT_ATTR_MEDIA_TYPE,

    /**
     * @brief FC Media speed capability
     *
     * @typesai_fc_port_media_speed_capability_t
     * @flags CREATE_AND_SET
     */
    SAI_FC_PORT_ATTR_MEDIA_SPEED_CAPB,

    /**
     * @brief BB Credit value
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_FC_PORT_ATTR_BB_CREDIT,

    /**
     * @brief BB Credit Recovery value
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_FC_PORT_ATTR_BB_CREDIT_RECOVERY,

    /**
     * @brief Max Frame size
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 2148
     */
    SAI_FC_PORT_ATTR_MAX_FRAME_SIZE,

    /**
     * @brief flow control mode
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default SAI_PORT_FLOW_CONTROL_MODE_DISABLE
     */
    SAI_FC_PORT_ATTR_FLOW_CONTROL_ENABLE,

    /**
     * @brief MAC mode for FCMAP SRC MAC address
     *
     * @type  sai_fc_map_mac_mode_t
     * @flags CREATE_AND_SET
     */
    SAI_FC_PORT_ATTR_MAP_SRC_MAC_MODE,

    /**
     * @brief MAP prefix for FCMAP SRC MAC address
     *
     * @type  sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_FC_PORT_ATTR_SRC_MAP_PREFIX,

    /**
     * @brief FCMAP SRC MAC address
     *
     * @type  sai_mac_t
     * @flags CREATE_AND_SET
     */
    SAI_FC_PORT_ATTR_INGRESS_SRC_MAC,

    /**
     * @brief MAC mode for FCMAP DST MAC address
     *
     * @type  sai_fc_map_mac_mode_t
     * @flags CREATE_AND_SET
     */
    SAI_FC_PORT_ATTR_DST_MAC_MODE,

    /**
     * @brief MAP prefix for FCMAP DST MAC address
     *
     * @type  sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_FC_PORT_ATTR_DST_MAP_PREFIX,

    /**
     * @brief FCMAP DST MAC address
     *
     * @type  sai_mac_t
     * @flags CREATE_AND_SET
     */
    SAI_FC_PORT_ATTR_INGRESS_DST_MAC,

    /**
     * @brief FCMAP Default VLAN Id
     *
     * @type  sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_FC_PORT_ATTR_VLAN_ID,

    /**
     * brief FCMAP VLAN PCP
     * @type  sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_FC_PORT_ATTR_PCP,

    /**
     * @brief FCMAP Default VFT
     *
     * @type  sai_uint32_t
     * @flags CREATE_AND_SET
     */
    SAI_FC_PORT_ATTR_DEFAULT_VFT,

    /**
     * @brief End of attributes
     */
    SAI_FC_PORT_ATTR_END,

} sai_fc_port_attr;


/**
 * @brief Port counter IDs in sai_get_port_stats() call
 */
typedef enum _sai_fc_port_counter_t
{

    SAI_FC_PORT_RX_DEBUG0,
    SAI_FC_PORT_RX_DEBUG1,
    SAI_FC_PORT_RX_UNICAST_PKTS,
    SAI_FC_PORT_RX_GOOD_FRAMES,
    SAI_FC_PORT_RX_BROADCAST_PKTS,
    SAI_FC_PORT_RX_BB_CREDIT0,
    SAI_FC_PORT_RX_INVALID_CRC,
    SAI_FC_PORT_RX_FRAME_TOO_LONG,
    SAI_FC_PORT_RX_TRUNC_FRAMES,
    SAI_FC_PORT_RX_DELIMITER_ERR,
    SAI_FC_PORT_RX_OTHER_ERR,
    SAI_FC_PORT_RX_RUNT_FRAMES,
    SAI_FC_PORT_RX_LIP_COUNT,
    SAI_FC_PORT_RX_NOS_COUNT,
    SAI_FC_PORT_RX_ERR_FRAMES,
    SAI_FC_PORT_RX_DROP_FRAMES,
    SAI_FC_PORT_RX_LINK_FAIL,
    SAI_FC_PORT_RX_LOSS_SYNC,
    SAI_FC_PORT_RX_LOSS_SIG,
    SAI_FC_PORT_RX_PRIM_SEQ_ERR,
    SAI_FC_PORT_RX_INVALID_WORD,
    SAI_FC_PORT_RX_INVALID_SET,
    SAI_FC_PORT_RX_ENCODE_DISPARITY,
    SAI_FC_PORT_RX_BYTES,
    SAI_FC_PORT_CLASS2_RX_GOOD_FRAMES,
    SAI_FC_PORT_CLASS2_RX_INVALIDCRC,
    SAI_FC_PORT_CLASS2_RX_FRAME_TOO_LONG,
    SAI_FC_PORT_CLASS3_RX_GOOD_FRAMES,
    SAI_FC_PORT_CLASS3_RX_INVALID_CRC,
    SAI_FC_PORT_CLASS3_RX_FRAME_TOO_LONG,
    SAI_FC_PORT_CLASSF_RX_GOOD_FRAMES,
    SAI_FC_PORT_CLASSF_RX_INVALIDCRC,
    SAI_FC_PORT_CLASSF_RX_FRAME_TOO_LONG,
    SAI_FC_PORT_RX_BBC0_DROP,
    SAI_FC_PORT_RX_SYNC_FAIL,
    SAI_FC_PORT_RX_BAD_XWORD,
    SAI_FC_PORT_CLASS2_RX_RUNT_FRAMES,
    SAI_FC_PORT_CLASS3_RX_RUNT_FRAMES,
    SAI_FC_PORT_CLASSF_RX_RUNT_FRAMES,
    SAI_FC_PORT_CLASS2_RX_BYTES,
    SAI_FC_PORT_CLASS3_RX_BYTES,
    SAI_FC_PORT_CLASSF_RX_BYTES,
    SAI_FC_PORT_TX_DEBUG0,
    SAI_FC_PORT_TX_DEBUG1,
    SAI_FC_PORT_TX_UNICAST_PKTS,
    SAI_FC_PORT_TX_BCAST_PKTS,
    SAI_FC_PORT_TX_BB_CREDIT0,
    SAI_FC_PORT_TX_GOOD_FRAMES,
    SAI_FC_PORT_TX_FIFO_UNDER_RUN,
    SAI_FC_PORT_TX_DROP_FRAMES,
    SAI_FC_PORT_TX_BYTES,
    SAI_FC_PORT_CLASS2_TX_FRAMES,
    SAI_FC_PORT_CLASS3_TX_FRAMES,
    SAI_FC_PORT_CLASSF_TX_FRAMES,
    SAI_FC_PORT_CLASS2_TX_OVERSIZED_FRAMES,
    SAI_FC_PORT_CLASS3_TX_OVERSIZED_FRAMES,
    SAI_FC_PORT_CLASSF_TX_OVERSIZED_FRAMES,
    SAI_FC_PORT_TX_FRAMES,
    SAI_FC_PORT_TX_OVERSIZED_FRAMES,
    SAI_FC_PORT_CLASS2_TX_BYTES,
    SAI_FC_PORT_CLASS3_TX_BYTES,
    SAI_FC_PORT_CLASSF_TX_BYTES,
    SAI_FC_PORT_TOTAL_RX_FRAMES,
    SAI_FC_PORT_TOTAL_TX_FRAMES,
    SAI_FC_PORT_TOTAL_ERRORS,
    SAI_FC_PORT_MAX_COUNTERS = SAI_FC_PORT_TOTAL_ERRORS,

} sai_fc_port_counter_t;

/**
 * @brief Create FC port object
 *
 * @param[out] port_id Port id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_fc_port_create_fn)(
    _Out_ sai_object_id_t *port_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list);


/**
 * @brief Remove FC port object
 *
 * @param[in] port_id Port id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_fc_port_remove_fn)(
    _In_ sai_object_id_t portId
);


/**
 * @brief Set port attribute value.
 *
 * @param[in] port_id Port id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_fc_port_attribute_fn)(
    _In_ sai_object_id_t port_id,
    _In_ const sai_attribute_t *attr
    );

/**
 * @brief Get port attribute value.
 *
 * @param[in] port_id Port id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_fc_port_attribute_fn)(
    _In_ sai_object_id_t port_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/**
 * @brief Get port statistics counters.
 *
 * @param[in] port_id Port id
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] number_of_counters Number of counters in the array
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_fc_port_stats_fn)(
    _In_ sai_object_id_t port_id,
    _In_ const sai_fc_port_counter_t *counter_ids,
    _In_ uint32_t number_of_counters,
    _Out_ uint64_t* counters
    );


/**
 * @brief Clear port statistics counters.
 *
 * @param[in] port_id Port id
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] number_of_counters Number of counters in the array
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_clear_fc_port_stats_fn)(
    _In_ sai_object_id_t port_id,
    _In_ const  sai_fc_port_counter_t *counter_ids,
    _In_ uint32_t number_of_counters
    );

/**
 * @brief Port state change notification
 *
 * Passed as a parameter into sai_initialize_switch()
 *
 * @param[in] count Number of notifications
 * @param[in] data Array of port operational status
 */
typedef void (*sai_fc_port_state_change_notification_fn)(
    _In_ uint32_t count,
    _In_ sai_fc_port_oper_status_notification_t *data
    );

/**
 * @brief FC Port methods table retrieved with sai_api_query()
 */
typedef struct _sai_fc_port_api_t
{
    sai_fc_port_create_fn              fc_port_create_fn;
    sai_fc_port_remove_fn              fc_port_remove_fn;
    sai_set_fc_port_attribute_fn       set_fc_port_attribute;
    sai_get_fc_port_attribute_fn       get_fc_port_attribute;
    sai_get_fc_port_stats_fn           get_fc_port_stats;
    sai_clear_fc_port_stats_fn         clear_fc_port_stats;

} sai_fc_port_api_t;

/**
 * @}
 */

#endif /* __SAIFCPORT_H_ */

