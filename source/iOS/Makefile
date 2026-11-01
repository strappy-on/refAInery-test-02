ALTIVEC_ROOT ?= /altivec
include $(ALTIVEC_ROOT)/make/entry.mk
ifeq ($(_altivec_relocated),)
PROJECT_ROOT := $(abspath $(dir $(firstword $(MAKEFILE_LIST)))/../..)
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
FizzBuzz_SIGN = 1
include $(ALTIVEC_ROOT)/altivec_common_phone.mk
endif
