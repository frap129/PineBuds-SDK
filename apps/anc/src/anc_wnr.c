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
#include "string.h"
#include "app_utils.h"

#include "hal_trace.h"
#include "hal_aud.h"
#include "hal_timer.h"
#include "anc_process.h"
#include "audioflinger.h"

#include "speech_cfg.h"
#include "anc_wnr.h"
#include "wind_detection_2mic.h"

// Wind detection algorithm
#define _SAMPLE_RATE        (8000)
#define _FRAME_LEN          (60)
#define _CHANNEL_NUM        (2)
#define _SAMPLE_BITS        (16)

#define _FRAME_LEN_MAX      (128)
#define _SAMPLE_BITS_MAX    (16)

WindDetection2MicState *wind_st = NULL;
// static uint8_t heap_buf[1024 * 12];

// Wind factor process
float windindicator[5]={1,1,1,1,1};
int16_t FFMute = 0;
int32_t gain_ch_l, gain_ch_r;
int32_t gain_target;

extern bool app_anc_work_status(void);

// Mic operation
#define ANC_WNR_STREAM_ID       AUD_STREAM_ID_0
static anc_wnr_open_mode_t g_open_mode = ANC_WNR_OPEN_MODE_QTY;
// 2ch, pingpong
#define AF_STREAM_BUFF_SIZE     (_FRAME_LEN * (_SAMPLE_BITS / 8) * 2 * 2)
static uint8_t __attribute__((aligned(4))) af_stream_buff[AF_STREAM_BUFF_SIZE];
static uint8_t __attribute__((aligned(4))) af_stream_mic1[_FRAME_LEN_MAX * (_SAMPLE_BITS_MAX / 8)];
static uint8_t __attribute__((aligned(4))) af_stream_mic2[_FRAME_LEN_MAX * (_SAMPLE_BITS_MAX / 8)];

static int32_t g_sample_rate = _SAMPLE_RATE;
static int32_t g_frame_len = _FRAME_LEN;

static void _open_mic(void);
static void _close_mic(void);

int32_t anc_wnr_ctrl(int32_t sample_rate, int32_t frame_len)
{
    TRACE(3,"[%s] sample_rate = %d, frame_len = %d", __func__, sample_rate, frame_len);

    g_sample_rate = sample_rate;
    g_frame_len = frame_len;    

    if (g_sample_rate == 16000) {
        g_frame_len >>= 1;
    }

    return 0;
}

int32_t anc_wnr_open(anc_wnr_open_mode_t mode)
{
    TRACE(4,"[%s] mode = %d, g_sample_rate = %d, g_frame_len = %d", __func__, mode, g_sample_rate, g_frame_len);

    hal_sysfreq_req(APP_SYSFREQ_USER_ANC_WNR, HAL_CMU_FREQ_104M);
    TRACE(2,"[%s] Sys freq: %d", __func__, hal_sys_timer_calc_cpu_freq(5, 0));
    
    // speech_heap_init(heap_buf, sizeof(heap_buf));

    if (mode == ANC_WNR_OPEN_MODE_STANDALONE) {
        g_sample_rate = _SAMPLE_RATE;
        g_frame_len = _FRAME_LEN;

        wind_st = WindDetection2Mic_create(_SAMPLE_RATE, _FRAME_LEN, NULL);

        _open_mic();
    } else if (mode == ANC_WNR_OPEN_MODE_CONFIGURE) {
        ASSERT(g_sample_rate == 8000 || g_sample_rate == 16000, "[%s] g_sample_rate(%d) is invalid.", __func__, g_sample_rate);
        ASSERT(g_frame_len == 60 || g_frame_len == 120, "[%s] g_frame_len(%d) is invalid.", __func__, g_frame_len);
        
        wind_st = WindDetection2Mic_create(_SAMPLE_RATE, g_frame_len, NULL);
    } else {
        ASSERT(0, "[%s] mode(%d) is invalid.", __func__, mode);
    }

    g_open_mode = mode;

    TRACE(1,"[%s] End...", __func__);

    return 0;
}

int32_t anc_wnr_close(void)
{
    TRACE(1,"[%s] ...", __func__);

    if (g_open_mode == ANC_WNR_OPEN_MODE_STANDALONE) {
        _close_mic();
        g_open_mode = ANC_WNR_OPEN_MODE_QTY;
    }

    WindDetection2Mic_destroy(wind_st);

    // size_t total = 0, used = 0, max_used = 0;
    // speech_memory_info(&total, &used, &max_used);
    // TRACE(3,"ANC WNR MALLOC MEM: total - %d, used - %d, max_used - %d.", total, used, max_used);
    // ASSERT(used == 0, "[%s] used != 0", __func__);

    hal_sysfreq_req(APP_SYSFREQ_USER_ANC_WNR, HAL_CMU_FREQ_32K);

    return 0;
}

// TODO: Provide API to configure performance
static int32_t anc_wnr_process_frame(int16_t *mic1, int16_t *mic2, uint32_t frame_len)
{
    float windindicatortmp = 0;
#if 0
    int32_t loop_cnt = frame_len / _FRAME_LEN;

    for (int32_t nt = 0; nt < loop_cnt; nt++) {
        windindicatortmp = windindicatortmp + WindDetection2Mic_process(wind_st, mic1 + nt * _FRAME_LEN, mic2 + nt * _FRAME_LEN);
    }
    windindicatortmp = windindicatortmp / loop_cnt;
#else
    windindicatortmp = WindDetection2Mic_process(wind_st, mic1, mic2, frame_len);
#endif

    // TRACE(3,"[%s] windindicator = %d FFMute = %d",__func__, (int32_t)(windindicatortmp * 100), FFMute);

    if ((windindicator[4] < 0.4) &&
        (windindicator[3] < 0.4) &&
        (windindicator[2] < 0.4) &&
        (windindicator[1] < 0.4) &&
        (windindicator[0] < 0.4) &&
        (windindicatortmp < 0.4) &&
        (FFMute == 0) &&
        (app_anc_work_status())) {

        anc_get_gain(&gain_ch_l, &gain_ch_r, ANC_FEEDFORWARD);
        gain_target = gain_ch_l;

        for(int32_t gain = 0; gain <= gain_target; gain++) {
            hal_sys_timer_delay_us(10);
            anc_set_gain(gain_target - gain, gain_target - gain, ANC_FEEDFORWARD);
        }

        FFMute = 1;

        TRACE(1,"[%s] Disable FF ANC.", __func__);
    }

    if ((windindicator[4] > 0.6) &&
        (windindicator[3] > 0.6) &&
        (windindicator[2] > 0.6) &&
        (windindicator[1] > 0.6) &&
        (windindicator[0] > 0.6) &&
        (windindicatortmp > 0.6) &&
        (FFMute == 1) &&
        (app_anc_work_status())) {
        
        for(int32_t gain=0; gain < gain_target; gain++) {
            hal_sys_timer_delay_us(10);
            anc_set_gain(gain, gain, ANC_FEEDFORWARD);
        }

        FFMute = 0;

        TRACE(1,"[%s] Enable FF ANC.", __func__);
    }

    windindicator[4] = windindicator[3];
    windindicator[3] = windindicator[2];
    windindicator[2] = windindicator[1];
    windindicator[1] = windindicator[0];
    windindicator[0] = windindicatortmp;

    return 0;
}

static void inline stereo_resample_16k(int16_t *pcm_buf, uint32_t pcm_len, int16_t *mic1, int16_t *mic2)
{
    uint32_t frame_len = pcm_len / _CHANNEL_NUM;

    const float num[3] = {0.972721, -1.945442, 0.972721};
    const float den[3] = {1.000000, -1.944692, 0.946192};

    static float y0 = 0, y1 = 0, y2 = 0, x0 = 0, x1 = 0, x2 = 0;
    static float Y0 = 0, Y1 = 0, Y2 = 0, X0 = 0, X1 = 0, X2 = 0;

    for (uint32_t i=0; i<frame_len; i++) {
        x0 = pcm_buf[_CHANNEL_NUM * i + 0];
        X0 = pcm_buf[_CHANNEL_NUM * i + 1];

        y0 = x0 * num[0] + x1 * num[1] + x2 * num[2] - y1 * den[1] - y2 * den[2];
        Y0 = X0 * num[0] + X1 * num[1] + X2 * num[2] - Y1 * den[1] - Y2 * den[2];

        y2 = y1;
        y1 = y0;
        x2 = x1;
        x1 = x0;

        Y2 = Y1;
        Y1 = Y0;
        X2 = X1;
        X1 = X0;

        if (i%2 == 0) {
            mic1[i/2] = speech_ssat_int16((int32_t)y0);
            mic2[i/2] = speech_ssat_int16((int32_t)Y0);
        }
    }
}

int32_t anc_wnr_process(int16_t *pcm_buf, uint32_t pcm_len)
{
    // TRACE(2,"[%s] pcm_len = %d", __func__, pcm_len);

    if (g_open_mode != ANC_WNR_OPEN_MODE_CONFIGURE) {
        return 1;
    }

    // resample 16k-->8k
    if (g_sample_rate == 16000) {
        stereo_resample_16k(pcm_buf, pcm_len, (int16_t *)af_stream_mic1, (int16_t *)af_stream_mic2);
    }
    
    // 2ch --> 1ch, 16k --> 8k
    pcm_len = pcm_len / _CHANNEL_NUM / 2;

    // TRACE(2,"[%s] new pcm_len = %d", __func__, pcm_len);

    anc_wnr_process_frame((int16_t *)af_stream_mic1, (int16_t *)af_stream_mic2, pcm_len);

    return 0;
}

static uint32_t anc_wnr_callback(uint8_t *buf, uint32_t len)
{
    // TRACE(2,"[%s] len = %d", __func__, len);

    int32_t frame_len = len / (_SAMPLE_BITS / 8) / _CHANNEL_NUM;
    int16_t *pcm_buf = (int16_t *)buf;

    int16_t *mic1 = (int16_t *)af_stream_mic1;
    int16_t *mic2 = (int16_t *)af_stream_mic2;

    for (int32_t i=0; i<frame_len; i++) {
        mic1[i] = pcm_buf[_CHANNEL_NUM*i + 0];
        mic2[i] = pcm_buf[_CHANNEL_NUM*i + 1];
    }

    anc_wnr_process_frame(mic1, mic2, frame_len);

    return len;
}

static void _open_mic(void)
{
    struct AF_STREAM_CONFIG_T stream_cfg;

    TRACE(1,"[%s] ...", __func__);

    memset(&stream_cfg, 0, sizeof(stream_cfg));
    stream_cfg.channel_num = (enum AUD_CHANNEL_NUM_T)_CHANNEL_NUM;
    stream_cfg.sample_rate = (enum AUD_SAMPRATE_T)_SAMPLE_RATE;
    stream_cfg.bits = (enum AUD_BITS_T)_SAMPLE_BITS;
    stream_cfg.vol = 12;
    stream_cfg.chan_sep_buf = false;
    stream_cfg.device       = AUD_STREAM_USE_INT_CODEC;
    stream_cfg.io_path      = AUD_INPUT_PATH_ANC_WNR;
    stream_cfg.handler      = anc_wnr_callback;
    stream_cfg.data_size    = sizeof(af_stream_buff);
    stream_cfg.data_ptr     = af_stream_buff;

    TRACE(3,"[%s] sample_rate:%d, data_size:%d", __func__, stream_cfg.sample_rate, stream_cfg.data_size);
    TRACE(2,"[%s] af_stream_buff = %p", __func__, af_stream_buff);

    af_stream_open(ANC_WNR_STREAM_ID, AUD_STREAM_CAPTURE, &stream_cfg);
    af_stream_start(ANC_WNR_STREAM_ID, AUD_STREAM_CAPTURE);
}

static void _close_mic(void)
{
    TRACE(1,"[%s] ...", __func__);

    af_stream_stop(ANC_WNR_STREAM_ID, AUD_STREAM_CAPTURE);
    af_stream_close(ANC_WNR_STREAM_ID, AUD_STREAM_CAPTURE);
}