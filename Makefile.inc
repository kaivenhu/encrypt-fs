MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
CURRENT_DIR := $(realpath $(patsubst %/,%,$(dir $(MKFILE_PATH))))
PROJ_PATH=$(CURRENT_DIR)

LIB_NAME=encryptfs

SUB_DIR := src
SHLIB = lib$(LIB_NAME).so

########### Compile Options #############

LIBS_INNER = -L$(PROJ_PATH)/src -l$(LIB_NAME)

LDFLAGS += $(LIBS_INNER) -Wl,-rpath,$(PROJ_PATH)/src \
           `pkg-config fuse --libs`

CXXFLAGS += -std=gnu++17 -g -Wall -W -Werror -fPIC $(INCS_INNER) \
            -DFUSE_USE_VERSION=30 `pkg-config fuse --cflags`
