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
* @file saifcswitch.h
*
* @brief This file defines SAI FC Switch functionality API
* @description Supported only Dell Inc. SAI Implementation
* @warning This module is a SAI extension module
*
*************************************************************************/

#if !defined (__SAIFCSWITCH_H_)
#define __SAIFCSWITCH_H_

#include "saitypes.h"
#include "saifcport.h"

/**
 * @defgroup SAIFCSWITCH SAI - FC Switch specific API definitions
 *
 * @{
 */

/**
 * @brief Maximum Hardware ID Length
 */
#define SAI_FC_MAX_HARDWARE_ID_LEN                 255

/**
 * @brief Maximum Firmware Path Name Length
 */
#define SAI_FC_MAX_FIRMWARE_PATH_NAME_LEN          PATH_MAX

/**
 * @brief Attribute data for #SAI_FC_SWITCH_ATTR_OPER_STATUS
 */
typedef enum _sai_fc_switch_oper_status_t
{
    /** Up */
    SAI_FC_SWITCH_OPER_STATUS_UP,

    /** Down */
    SAI_FC_SWITCH_OPER_STATUS_DOWN,

    /** Switch encountered a fatal error */
    SAI_FC_SWITCH_OPER_STATUS_FAILED,

} sai_fc_switch_oper_status_t;

/**
 * @brief Attribute Id in sai_set_switch_attribute() and
 * sai_get_switch_attribute() calls
 */
typedef enum _sai_fc_switch_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_FC_SWITCH_ATTR_START,

    /**
     * @brief Oper state
     *
     * @type sai_fc_switch_oper_status_t
     * @flags READ_ONLY
     */
    SAI_FC_SWITCH_ATTR_OPER_STATUS,

    /**
     * @brief Set to switch initialization or connect to NPU/SDK.
     *
     * TRUE - Initialize switch/SDK.
     * FALSE - Connect to SDK. This will connects library to the initialized SDK.
     * After this call the capability attributes should be ready for retrieval
     * via sai_get_fc_switch_attribute()
     *
     * @type bool
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_FC_SWITCH_ATTR_INIT_SWITCH,

    /**
     * @brief Set Switch Port state change notification callback function passed to the adapter.
     *
     * Use sai_fc_port_state_change_notification_fn as notification function.
     *
     * @type sai_pointer_t
     * @flags CREATE_AND_SET
     * @default NULL
     */
    SAI_FC_SWITCH_ATTR_PORT_STATE_CHANGE_NOTIFY,

    /**
     * @brief End of attributes
     */
    SAI_FC_SWITCH_ATTR_END


} sai_fc_switch_attr_t;



/**
 * @brief Create switch
 *
 *   SDK initialization/connect to SDK. After the call the capability attributes should be
 *   ready for retrieval via sai_get_switch_attribute(). Same Switch Object id should be
 *   given for create/connect for each NPU.
 *
 * @param[out] switch_id The Switch Object ID
 * @param[in] attr_count number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t(*sai_create_fc_switch_fn)(
        _Out_ sai_object_id_t* fc_switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove/disconnect Switch
 *   Release all resources associated with currently opened switch
 *
 * @param[in] switch_id The Switch id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef void (*sai_remove_fc_switch_fn)(
              _In_ sai_object_id_t fc_switch_id);

/**
 * @brief Set switch attribute value
 *
 * @param[in] fc_switch_id Switch id
 * @param[in] attr Switch attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_fc_switch_attribute_fn)(
        _In_ sai_object_id_t fc_switch_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get switch attribute value
 *
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of switch attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_fc_switch_attribute_fn)(
        _In_ sai_object_id_t fc_switch_id,
        _In_ sai_uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Switch method table retrieved with sai_api_query()
 */
typedef struct _sai_fc_switch_api_t
{
    sai_create_fc_switch_fn            create_fc_switch;
    sai_remove_fc_switch_fn            remove_fc_switch;
    sai_set_fc_switch_attribute_fn     set_fc_switch_attribute;
    sai_get_fc_switch_attribute_fn     get_fc_switch_attribute;

} sai_fc_switch_api_t;

/**
 * @}
 */
#endif /** __SAIFCSWITCH_H_ */

