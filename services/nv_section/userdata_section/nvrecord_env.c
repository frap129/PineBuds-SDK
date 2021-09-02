/***************************************************************************
 *
 * Copyright 2015-2019 BES.
 * All rights reserved. All unpublished rights reserved.
 *
 * No part of this work may be used or reproduced in any form or by any
 * means, or stored in a database or retrieval system, without prior written
 * permission of BES.
 *
 * Use of this work is governed by a license granted by BES.
 * This work contains confidential and proprietary information of
 * BES. which is protected by copyright, trade secret,
 * trademark and other intellectual property rights.
 *
 ****************************************************************************/
#if defined(NEW_NV_RECORD_ENABLED)
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "nvrecord_extension.h"
#include "nvrecord_env.h"
#include "hal_trace.h"

static struct nvrecord_env_t localSystemInfo;

void nvrecord_rebuild_system_env(struct nvrecord_env_t* pSystemEnv)
{
    memset((uint8_t *)pSystemEnv, 0, sizeof(struct nvrecord_env_t));
    
    pSystemEnv->media_language.language = NVRAM_ENV_MEDIA_LANGUAGE_DEFAULT;
    pSystemEnv->ibrt_mode.mode = NVRAM_ENV_TWS_MODE_DEFAULT;
    pSystemEnv->ibrt_mode.tws_connect_success = 0;
    pSystemEnv->factory_tester_status.status = NVRAM_ENV_FACTORY_TESTER_STATUS_DEFAULT;

    pSystemEnv->aiManagerInfo.voice_key_enable = false;
    pSystemEnv->aiManagerInfo.setedCurrentAi = 0;
    pSystemEnv->aiManagerInfo.aiStatusDisableFlag = 0;
    pSystemEnv->aiManagerInfo.amaAssistantEnableStatus = 1;

    localSystemInfo = *pSystemEnv;
}

int nv_record_env_get(struct nvrecord_env_t **nvrecord_env)
{
    if (NULL == nvrecord_env)
    {
        return -1;
    }

    if (NULL == nvrecord_extension_p)
    {   
        return -1;
    }

    localSystemInfo = nvrecord_extension_p->system_info;
    *nvrecord_env = &localSystemInfo;

    return 0;
}

struct nvrecord_env_t *get_nv_env(void)
{
    static struct nvrecord_env_t *g_nvrecord_env = NULL;
	if(NULL == g_nvrecord_env)
	{
		nv_record_env_get(&g_nvrecord_env);
	}

	return g_nvrecord_env;
}

bool get_flag_uart_rx(void)
{
    struct nvrecord_env_t *m_nvrecord_env = get_nv_env();
    if(1 == m_nvrecord_env->flag_uart0_rx_mode)
    {
        //return if in use
        //TRACE(0,"qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq");
        return true;
    }
    //TRACE(0,"555555555555555555555555555555555555555555555555555555555");
    return false;
}

int nv_record_env_set(struct nvrecord_env_t *nvrecord_env)
{
    if (NULL == nvrecord_extension_p)
    {   
        return -1;
    }

    uint32_t lock = nv_record_pre_write_operation();
    nvrecord_extension_p->system_info = *nvrecord_env;
    
    nv_record_update_runtime_userdata();
    nv_record_post_write_operation(lock); 
    return 0;
}

void nv_record_update_ibrt_info(uint32_t newMode, bt_bdaddr_t *ibrtPeerAddr)
{
    if (NULL == nvrecord_extension_p)
    {   
        return;
    }
    
    uint32_t lock = nv_record_pre_write_operation();

    TRACE(2,"##%s,%d",__func__,newMode);
    nvrecord_extension_p->system_info.ibrt_mode.mode = newMode;
    memcpy(nvrecord_extension_p->system_info.ibrt_mode.record.bdAddr.address,
        ibrtPeerAddr->address, BTIF_BD_ADDR_SIZE);

    nv_record_update_runtime_userdata();
    nv_record_post_write_operation(lock);    
}


void nv_record_update_factory_tester_status(uint32_t status)
{
    if (NULL == nvrecord_extension_p)
    {   
        return;
    }

    uint32_t lock = nv_record_pre_write_operation();

    nvrecord_extension_p->system_info.factory_tester_status.status = status;

    nv_record_update_runtime_userdata();
    nv_record_post_write_operation(lock);   
}

int nv_record_env_init(void)
{
    nv_record_open(section_usrdata_ddbrecord);
    return 0;
}

/**
 * @brief
 *		update psx_data_info to nvrecord
 *
 * @param
 *		data_h:high
 *          data_l:low
 *          data_type:  1:JSA1225_PS_CALIBRATION 2:JSA1225_PS_HIGH_THRESHOLD  3:JSA1225_PS_LOW_THRESHOLD
 *
 * @return
 *		none
 */
void nv_record_update_psx_info(uint8_t data_h,uint8_t data_l,uint8_t data_type)
{
    struct nvrecord_env_t *m_nvrecord_env;
    
    TRACE(2,"%s data_h = %d data_l = %d",__func__,data_h,data_l);
    m_nvrecord_env = get_nv_env();
    switch(data_type)
    {
        case 1:
        	m_nvrecord_env->psx_info.CT_H = data_h;
        	m_nvrecord_env->psx_info.CT_L = data_l;
        	break;
        case 2:
        	m_nvrecord_env->psx_info.HT_H = data_h;
        	m_nvrecord_env->psx_info.HT_L = data_l;
        	break;
        case 3:
        	m_nvrecord_env->psx_info.LT_H = data_h;
        	m_nvrecord_env->psx_info.LT_L = data_l;
        	break;
        default:
            break;	
	}
	nv_record_env_set(m_nvrecord_env);
	nv_record_flash_flush();
}
#endif // #if defined(NEW_NV_RECORD_ENALBED)

