# Project Name
TARGET = SeminarSynthEngine

# Source
CPP_SOURCES := $(shell find src -name "*.cpp")
C_INCLUDES += -Isrc

# Library Locations
LIBDAISY_DIR = ../../libDaisy/
DAISYSP_DIR = ../../DaisySP/
DAISYSP-LGPL_DIR = ../../DaisySP/DaisySP-LGPL

# LGPL Support
C_DEFS += -DUSE_DAISYSP_LGPL
C_DEFS += -I$(DAISYSP-LGPL_DIR)/Source
LDFLAGS += -L$(DAISYSP-LGPL_DIR)/build -ldaisysp-lgpl

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
