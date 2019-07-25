# Application name
APPL ?= emsk

# Optimization Level
# Please Refer to toolchain_xxx.mk for this option
OLEVEL ?= O2

##
# select the operation cpu core
##
BOARD ?= emsk
BD_VER ?=23
CUR_CORE = arcem7d
TOOLCHAIN = gnu
##
# select debugging jtag
##
JTAG ?= usb

#
# root dir of embARC
#
EMBARC_ROOT = ../../../../
MID_SEL = common u8glib


# application source dirs
APPL_CSRC_DIR = .  driver/pump driver/mlx9640 driver/stepper
APPL_ASMSRC_DIR = .

# application include dirs
APPL_INC_DIR = .  driver/pump driver/mlx9640 driver/stepper

# include current project makefile
COMMON_COMPILE_PREREQUISITES += makefile

### Options above must be added before include options.mk ###
# include key embARC build system makefile
override EMBARC_ROOT := $(strip $(subst \,/,$(EMBARC_ROOT)))
include $(EMBARC_ROOT)/options/options.mk