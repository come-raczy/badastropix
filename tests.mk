# unit tests for a single library
#
# In practice, each compilation unit (CU) in the library should have a
# unique test fixture (TF). However, as unit testing is not enforced,
# we have to account for untested CUs.
# TODO: enforce unit testing for all CUs
unit_test_sources:=$(wildcard lib/$(lib_dir)/tests/unit/*Fixture.cpp)
unit_tests:=$(unit_test_sources:lib/$(lib_dir)/tests/unit/%Fixture.cpp=%)

ifneq (,$(unit_tests))
# the pattern in most of these rules is the name of the test (same as the CU)
#
# Wrapper for the CU:
-include $(unit_tests:%=$(BUILD)/$(lib_dir)/unit/%Wrapper.d)
.PRECIOUS: $(BUILD)/$(lib_dir)/unit/%Wrapper.cpp $(BUILD)/$(lib_dir)/unit/%Wrapper.d
$(BUILD)/$(lib_dir)/unit/%Wrapper.cpp: $(BUILD)/$(lib_dir)/unit/.sentinel
	echo \#include \"$*Mocks.hh\" > $@ ; echo \#include \"$*.cpp\" >> $@

$(BUILD)/$(lib_dir)/unit/%Wrapper.o $(BUILD)/$(lib_dir)/unit/%Wrapper.d: lib_dir := $(lib_dir)
$(BUILD)/$(lib_dir)/unit/%Wrapper.o $(BUILD)/$(lib_dir)/unit/%Wrapper.d: DEPSUFFIX := Wrapper
$(BUILD)/$(lib_dir)/unit/%Wrapper.o $(BUILD)/$(lib_dir)/unit/%Wrapper.d: $(BUILD)/$(lib_dir)/unit/%Wrapper.cpp
	$(CXX) $(DEPFLAGS) $(CPPFLAGS) $(CXXFLAGS) -I"lib/$(lib_dir)/tests/unit" -I"lib/$(lib_dir)" -c -o "$(BUILD)/$(lib_dir)/unit/$*$(DEPSUFFIX).o" $<
	$(POSTCOMPILE)

# Fixture for the CU:
-include $(unit_tests:%=$(BUILD)/$(lib_dir)/unit/%Fixture.d)
.PRECIOUS: $(BUILD)/$(lib_dir)/unit/%Fixture.d

$(BUILD)/$(lib_dir)/unit/%Fixture.o $(BUILD)/$(lib_dir)/unit/%Fixture.d: lib_dir := $(lib_dir)
$(BUILD)/$(lib_dir)/unit/%Fixture.o $(BUILD)/$(lib_dir)/unit/%Fixture.d: DEPSUFFIX := Fixture
$(BUILD)/$(lib_dir)/unit/%Fixture.o $(BUILD)/$(lib_dir)/unit/%Fixture.d: lib/$(lib_dir)/tests/unit/%Fixture.cpp $(BUILD)/$(lib_dir)/unit/.sentinel
	$(CXX) $(DEPFLAGS) $(CPPFLAGS) $(CXXFLAGS) -I"lib/$(lib_dir)/tests/unit" -c -o "$(BUILD)/$(lib_dir)/unit/$*$(DEPSUFFIX).o" $<
	$(POSTCOMPILE)

# Test runner for the library
.PRECIOUS: $(BUILD)/$(lib_dir)/unit/testRunner $(BUILD)/$(lib_dir)/unit/passed
$(BUILD)/$(lib_dir)/unit/testRunner: $(BUILD)/testRunner.o $(unit_tests:%=$(BUILD)/$(lib_dir)/unit/%Fixture.o) $(unit_tests:%=$(BUILD)/$(lib_dir)/unit/%Wrapper.o)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ -lcppunit
#	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) -lcppunit

$(BUILD)/$(lib_dir)/unit/passed: $(BUILD)/$(lib_dir)/unit/testRunner
	set -o pipefail; $< && touch $@

# ensure that the unit test suite for this library are executed
$(BUILD)/$(lib_dir).a: $(BUILD)/$(lib_dir)/unit/passed
endif

#integration tests
integration_test_sources:=$(wildcard lib/$(lib_dir)/tests/integration/test*.cpp)
integration_tests:=$(integration_test_sources:lib/$(lib_dir)/tests/integration/test%.cpp=%)

ifneq (,$(integration_tests))
integration_tests_aux:=$(integration_tests)
endif

