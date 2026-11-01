ALTIVEC_ROOT ?= /altivec
include $(ALTIVEC_ROOT)/make/entry.mk
ifeq ($(_altivec_relocated),)
PROJECT_ROOT := $(abspath $(dir $(firstword $(MAKEFILE_LIST)))/../..)
ARCHS ?= ppc i386 x86_64 arm64
PRODUCTS = FizzBuzz model
PACKAGE_PRODUCTS = FizzBuzz model
model_TYPE = static
model_SOURCES = ../shared/model.c
FizzBuzz_TYPE = executable
FizzBuzz_SOURCES = ../cli/main.c
FizzBuzz_FLAGS = -I../shared
FizzBuzz_DEPS = model
FizzBuzz_PACKAGE_MAIN_PATH = bin/FizzBuzz
model_PACKAGE_MAIN_PATH = lib/libmodel.a
model_PACKAGE_RESOURCES = ../shared/model.h=include/model.h ../../LICENSE=LICENSE
FizzBuzz_PACKAGE_RESOURCES = ../../LICENSE=LICENSE
include $(ALTIVEC_ROOT)/altivec_common_mac.mk
endif
