/*
 * Copyright (c) 2017 Dell Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 * LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
 * FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
 *
 * See the Apache Version 2.0 License for specific language governing
 * permissions and limitations under the License.
 */

/*
 * filename: sai.c
 */

#include "sai.h"

sai_status_t sai_api_initialize(
        _In_ uint64_t flags,
        _In_ const sai_service_method_table_t *services)
{
    return SAI_STATUS_NOT_IMPLEMENTED;
}

sai_status_t sai_api_query(
        _In_ sai_api_t sai_api_id,
        _Out_ void** api_method_table)
{
    return SAI_STATUS_NOT_IMPLEMENTED;
}

sai_status_t sai_api_uninitialize(void)
{
    return SAI_STATUS_NOT_IMPLEMENTED;
}

sai_status_t sai_log_set(
        _In_ sai_api_t sai_api_id,
        _In_ sai_log_level_t log_level)
{
    return SAI_STATUS_NOT_IMPLEMENTED;
}

sai_object_type_t sai_object_type_query(
        _In_ sai_object_id_t sai_object_id)

{
    return SAI_STATUS_NOT_IMPLEMENTED;
}

sai_status_t sai_dbg_generate_dump(
        _In_ const char *dump_file_name)
{
    return SAI_STATUS_NOT_IMPLEMENTED;
}
