#### alexa ai voice ####
AMA_VOICE := 0

#### xiaodu ai voice ####
DMA_VOICE := 0

#### ali ai voice ####
GMA_VOICE := 0

#### BES ai voice ####
SMART_VOICE := 0

#### xiaowei ai voice ####
TENCENT_VOICE := 0

#### customize ai voice ####
CUSTOMIZE_VOICE := 0

#### Google related feature ####
# the overall google service switch
# currently, google service includes BISTO and GFPS
# NOTE: GSOUND_HOTWORD_ENABLE and GSOUND_HOTWORD_EXTERNAL is NOT
# controlled by this macro
export GOOGLE_SERVICE_ENABLE := 0

# BISTO is a GVA service on Bluetooth audio device
# BISTO is an isolated service relative to GFPS
export BISTO_ENABLE ?= 0

# macro switch for reduced_guesture
export REDUCED_GUESTURE_ENABLE ?= 0

# GSOUND_HOTWORD is a hotword library running on Bluetooth audio device
# GSOUND_HOTWORD is a subset of BISTO
export GSOUND_HOTWORD_ENABLE ?= 0

# this is a subset choice for gsound hotword
export GSOUND_HOTWORD_EXTERNAL ?= 0

# GFPS is google fastpair service
# GFPS is an isolated service relative to BISTO
export GFPS_ENABLE ?= 1
#### Google related feature ####

SLAVE_ADV_BLE_ENABLED := 0

export GATT_OVER_BR_EDR ?= 0
ifeq ($(GATT_OVER_BR_EDR),1)
KBUILD_CPPFLAGS += -D__GATT_OVER_BR_EDR__
endif

BLE := 1
FAST_XRAM_SECTION_SIZE ?= 0x1A000

#### VAD config are both for bisto and alexa ####
# 1 to enable the VAD feature, 0 to disable the VAD feature
export VOICE_DETECTOR_EN ?= 0

#### use the hot word lib of amazon ####
export ALEXA_WWE := 0
#### a subset choice for the hot word lib of amazon -- lite mode####
export ALEXA_WWE_LITE := 0
ifeq ($(ALEXA_WWE),1)
KBUILD_CPPFLAGS += -D__ALEXA_WWE
export USE_THIRDPARTY := 1
TRACE_BUF_SIZE := 8*1024
FAST_XRAM_SECTION_SIZE := 0xE100

ifeq ($(ALEXA_WWE_LITE),1)
KBUILD_CPPFLAGS += -D__ALEXA_WWE_LITE
export THIRDPARTY_LIB := kws/alexa_lite
else
export THIRDPARTY_LIB := kws/alexa
export MCU_HIGH_PERFORMANCE_MODE := 1
endif
endif

#### use the hot word lib of BES ####
export KWS_ALEXA := 0
ifeq ($(KWS_ALEXA),1)
export MCU_HIGH_PERFORMANCE_MODE :=1
export USE_THIRDPARTY := 1
export THIRDPARTY_LIB := kws/bes

FAST_XRAM_SECTION_SIZE := 0x14000
KBUILD_CPPFLAGS += -D__KWS_ALEXA__
KBUILD_CPPFLAGS += \
                   -Ithirdparty/kws/bes_lib/include/
endif

KBUILD_CPPFLAGS += -DFAST_XRAM_SECTION_SIZE=$(FAST_XRAM_SECTION_SIZE)

ifeq ($(ALEXA_WWE)_$(KWS_ALEXA),0_0)
KBUILD_CPPFLAGS += -DRTOS_IN_RAM
endif

include config/best2300p_ibrt/target.mk

core-y += utils/sha256/

