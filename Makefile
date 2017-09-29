SHELL:=/bin/bash
RM := rm -f
MV := mv
TOUCH := touch
CC ?= /usr/bin/gcc
CXX ?= /usr/bin/g++
MAGICK++_CONFIG ?= GraphicsMagick++-config

CPPFLAGS+= -Iinclude
CPPFLAGS += $(shell $(MAGICK++_CONFIG) --cppflags)

#CPPFLAGS+= -isystem $(BOOST_INCLUDE)


CXXFLAGS+= -std=c++14
CXXFLAGS+= -Wall -Wno-unused-local-typedefs
#CXXFLAGS += $(shell $(MAGICK++_CONFIG) --cxxflags)

DEPDIR = $(@D)
DEPFLAGS = -MT "$(DEPDIR)/$*$(DEPSUFFIX).d" -MMD -MP -MF "$(DEPDIR)/$*$(DEPSUFFIX).Td"
POSTCOMPILE = $(MV) -f "$(DEPDIR)/$*$(DEPSUFFIX).Td" "$(DEPDIR)/$*$(DEPSUFFIX).d" && $(TOUCH) "$(DEPDIR)/$*$(DEPSUFFIX).o"

BUILD_DIR?=build

# set this to select the appropriate boost libraries
#BOOST_TAG?=-mt

ifdef DEBUG
CXXFLAGS += -O0 -g
BUILD ?= $(BUILD_DIR)/debug
else
CXXFLAGS += -O3
CXXFLAGS += -mtune=native
BUILD?=$(BUILD_DIR)/release
endif

LDFLAGS+= -lboost_filesystem${BOOST_TAG}
LDFLAGS+= -lboost_thread${BOOST_TAG}
LDFLAGS+= -lboost_system${BOOST_TAG}
LDFLAGS+= -lboost_regex${BOOST_TAG}
LDFLAGS+= -lpthread
LDFLAGS += $(shell $(MAGICK++_CONFIG) --ldflags)
LDFLAGS += $(shell $(MAGICK++_CONFIG) --libs)

sources := $(wildcard *.cpp)
programs := $(sources:%.cpp=%)
all_lib_sources := $(wildcard lib/*/*.cpp)
lib_dirs := $(sort $(patsubst %/, %, $(dir $(all_lib_sources:lib/%=%))))
libraries:=$(lib_dirs:%=$(BUILD)/%.a)

all: $(programs:%=$(BUILD)/%)

.PRECIOUS: %/.sentinel %.d

%/.sentinel:
	@mkdir -p $* && touch $@

$(BUILD)/testRunner.o: cppunit/testRunner.cpp $(BUILD)/.sentinel
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

clean:
	$(RM) -r build

programs_aux:=$(programs)
include $(foreach program, $(programs), program.mk)

lib_dirs_aux:=$(lib_dirs)
include $(foreach lib_dir, $(lib_dirs), lib.mk)

