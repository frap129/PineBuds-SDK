CHIP		?= best1400

CHIP_SUBTYPE	?= best1402

DEBUG		?= 1

FPGA		?= 0

MBED		?= 0

RTOS		?= 1

LIBC_ROM	?= 1

export USER_SECURE_BOOT	?= 0
# enable:1
# disable:0

WATCHER_DOG ?= 0

DEBUG_PORT	?= 1
# 0: usb
# 1: uart0
# 2: uart1

FLASH_CHIP	?= ALL
# GD25Q80C
# GD25Q32C
# ALL

export FORCE_SIGNALINGMODE ?= 0

export FORCE_NOSIGNALINGMODE ?= 0

export FORCE_SCO_MAX_RETX ?= 0

export FA_RX_GAIN_CTRL ?= 1

export BT_FA_ECC ?= 0

export CONTROLLER_DUMP_ENABLE ?= 0

export INTERSYS_DEBUG ?= 0

export PROFILE_DEBUG ?= 0

export BTDUMP_ENABLE ?= 1

export BT_DEBUG_TPORTS ?= 0

export CONTROLLER_MEM_LOG_ENABLE ?= 0

export SNIFF_MODE_CHECK ?= 0

AUDIO_OUTPUT_MONO ?= 0

AUDIO_OUTPUT_DIFF ?= 0

HW_FIR_EQ_PROCESS ?= 0

SW_IIR_EQ_PROCESS ?= 0

HW_DAC_IIR_EQ_PROCESS ?= 1

HW_IIR_EQ_PROCESS ?= 0

AUDIO_DRC ?= 0

AUDIO_DRC2 ?= 0

AUDIO_RESAMPLE ?= 1

RESAMPLE_ANY_SAMPLE_RATE ?= 0

OSC_26M_X4_AUD2BB ?= 1

export ANA_26M_X6_ENABLE ?= 0

# big digital use digital 26x4 as clock source
# 1  sys and flash will use this clock as source
# 0  sys and flash will use analog x4/x6 (rf 0xF reg) clock as source
export DIG_OSC_X4_ENABLE ?= 0

# big digital use digital 26x2 as 52M clock source
# 1  sys and flash will use this clock as source
# 0  sys and flash will use analog x2 (diff port from x4/x6) clock as source
export DIG_OSC_X2_ENABLE ?= 1

AUDIO_OUTPUT_VOLUME_DEFAULT ?= 17
# range:1~16

AUDIO_INPUT_CAPLESSMODE ?= 0

AUDIO_INPUT_LARGEGAIN ?= 0

AUDIO_CODEC_ASYNC_CLOSE ?= 0

AUDIO_SCO_BTPCM_CHANNEL ?= 1

# For TWS SCO DMA snapshot and low delay
ifneq ($(CHIP_SUBTYPE),)
export PCM_FAST_MODE ?= 1
endif

export CVSD_BYPASS ?= 1

export LOW_DELAY_SCO ?= 0

SCO_OPTIMIZE_FOR_RAM ?= 1

SPEECH_TX_DC_FILTER ?= 1

SPEECH_TX_AEC2FLOAT ?= 1

SPEECH_TX_NS3 ?= 0

SPEECH_TX_2MIC_NS2 ?= 0

SPEECH_TX_COMPEXP ?= 1

SPEECH_TX_EQ ?= 1

SPEECH_TX_POST_GAIN ?= 0

SPEECH_RX_NS2FLOAT ?= 0

SPEECH_RX_EQ ?= 1

SPEECH_RX_POST_GAIN ?= 0

LARGE_RAM ?= 1

HSP_ENABLE ?= 0

HFP_1_6_ENABLE ?= 1

MSBC_PLC_ENABLE ?= 1

MSBC_PLC_ENCODER ?= 1

MSBC_16K_SAMPLE_RATE ?= 1

SBC_FUNC_IN_ROM ?= 0

ifeq ($(CHIP_SUBTYPE),)
ROM_UTILS_ON ?= 1
endif

APP_LINEIN_A2DP_SOURCE ?= 0

APP_I2S_A2DP_SOURCE ?= 0

VOICE_PROMPT ?= 1

#### Google related feature ####
# the overall google service switch
# currently, google service includes BISTO and GFPS
export GOOGLE_SERVICE_ENABLE ?= 0

# BISTO is a GVA service on Bluetooth audio device
# BISTO is an isolated service relative to GFPS
export BISTO_ENABLE ?= 0

# GSOUND_HOTWORD is a hotword library running on Bluetooth audio device
# GSOUND_HOTWORD is a subset of BISTO
export GSOUND_HOTWORD_ENABLE ?= 0

# GFPS is google fastpair service
# GFPS is an isolated service relative to BISTO
export GFPS_ENABLE ?= 0
#### Google related feature ####

AMA_VOICE ?= 0

DMA_VOICE ?= 0

SMART_VOICE ?= 0

TENCENT_VOICE ?= 0

BLE ?= 0

TOTA ?= 0

OTA_ENABLE ?= 0

TILE_DATAPATH_ENABLED ?= 0

CUSTOM_INFORMATION_TILE_ENABLE ?= 0

INTERCONNECTION ?= 0

INTERACTION ?= 0

INTERACTION_FASTPAIR ?= 0

BT_ONE_BRING_TWO ?= 0

DSD_SUPPORT ?= 0

A2DP_EQ_24BIT ?= 0

A2DP_AAC_ON ?= 1

A2DP_SCALABLE_ON ?= 0

A2DP_LHDC_ON ?= 0

A2DP_LDAC_ON ?= 0

export TX_RX_PCM_MASK ?= 0

A2DP_SCALABLE_ON ?= 0

FACTORY_MODE ?= 1

ENGINEER_MODE ?= 1

ULTRA_LOW_POWER	?= 1

DAC_CLASSG_ENABLE ?= 1

NO_SLEEP ?= 0

CORE_DUMP ?= 1

ENHANCED_STACK ?= 1

SNOOP_DATA_EXCHANGE_VIA_BLE ?= 0

export FLASH_SIZE ?= 0x200000

export A2DP_AVDTP_CP ?= 0

export A2DP_DECODER_VER := 2

export IBRT ?= 1

export IBRT_SEARCH_UI ?= 1

export BES_AUD ?= 1

export POWER_MODE   ?= DIG_DCDC

export BT_RF_PREFER ?= 2M

export SPEECH_CODEC ?= 1

export MIX_AUDIO_PROMPT_WITH_A2DP_MEDIA_ENABLED ?= 0
export TWS_PROMPT_SYNC ?= 0

export FLASH_SUSPEND ?= 0

export VCODEC_VOLT ?= 1.66V
#export VANA_VOLT ?= 1.35V

AAC_TEXT_PARTIAL_IN_FLASH ?= 1

export LAURENT_ALGORITHM ?= 0

export TX_IQ_CAL ?= 0

export BT_XTAL_SYNC ?= 1

export BTADDR_FOR_DEBUG ?= 1

export POWERKEY_I2C_SWITCH ?=0

AUTO_TEST ?= 0

export DUMP_NORMAL_LOG ?= 0

SUPPORT_BATTERY_REPORT ?= 1

SUPPORT_HF_INDICATORS ?= 0

SUPPORT_SIRI ?= 1

BES_AUDIO_DEV_Main_Board_9v0 ?= 0


init-y		:=
core-y		:= platform/ services/ apps/ utils/cqueue/ utils/list/ services/multimedia/ utils/intersyshci/

KBUILD_CPPFLAGS +=  -Iplatform/cmsis/inc \
                    -Iservices/audioflinger \
                    -Iplatform/hal \
                    -Iservices/fs/ \
                    -Iservices/fs/sd \
                    -Iservices/fs/fat \
                    -Iservices/fs/fat/ChaN

KBUILD_CPPFLAGS += -DAPP_AUDIO_BUFFER_SIZE=84*1024 \
                   -DCHARGER_PLUGINOUT_RESET=0

export TRACE_BAUD_RATE ?= 10*115200

FAST_XRAM_SECTION_SIZE ?= 0xF800
KBUILD_CPPFLAGS += -DFAST_XRAM_SECTION_SIZE=$(FAST_XRAM_SECTION_SIZE)

ifeq ($(BES_AUDIO_DEV_Main_Board_9v0),1)
KBUILD_CPPFLAGS += -DBES_AUDIO_DEV_Main_Board_9v0
endif

ifeq ($(BISTO_ENABLE),1)
export TRACE_BUF_SIZE := 1024
else
export TRACE_BUF_SIZE := 8*1024
endif

#KBUILD_CPPFLAGS += -D__3M_PACK__

#-D_AUTO_SWITCH_POWER_MODE__
#-D__APP_KEY_FN_STYLE_A__
#-D__APP_KEY_FN_STYLE_B__
#-D__EARPHONE_STAY_BOTH_SCAN__
#-D__POWERKEY_CTRL_ONOFF_ONLY__
#-DAUDIO_LINEIN

ifeq ($(CURRENT_TEST),1)
INTSRAM_RUN ?= 1
endif
ifeq ($(INTSRAM_RUN),1)
LDS_FILE    := best1000_intsram.lds
else
LDS_FILE    := best1000.lds
endif

ifeq ($(TOTA),1)
ifeq ($(BLE),1)
KBUILD_CPPFLAGS += -DBLE_TOTA_ENABLED
endif
KBUILD_CPPFLAGS += -DTEST_OVER_THE_AIR_ENANBLED
export TEST_OVER_THE_AIR ?= 1
endif

ifneq ($(A2DP_DECODER_VER), )
KBUILD_CPPFLAGS += -DA2DP_DECODER_VER=$(A2DP_DECODER_VER)
endif

ifeq ($(DIG_OSC_X2_ENABLE),1)
KBUILD_CPPFLAGS += -DFLASH_FREQ_DYNAMIC_ADJUST
endif

KBUILD_CPPFLAGS +=

KBUILD_CFLAGS +=

LIB_LDFLAGS += -lstdc++ -lsupc++

#CFLAGS_IMAGE += -u _printf_float -u _scanf_float

#LDFLAGS_IMAGE += --wrap main

