
ifeq (,$(lib_dirs_aux))
$(error No library directories specified)
endif

lib_dir := $(word 1, $(lib_dirs_aux))
ifeq (1,$(words $(lib_dirs_aux)))
lib_dirs_aux:=
else
lib_dirs_aux:=$(wordlist 2, $(words $(lib_dirs_aux)), $(lib_dirs_aux))
endif

lib_sources := $(wildcard lib/$(lib_dir)/*.cpp)
lib_objects := $(lib_sources:lib/$(lib_dir)/%.cpp=$(BUILD)/$(lib_dir)/%.o)

-include $(lib_objects:%.o=%.d)
include tests.mk

$(BUILD)/$(lib_dir).a: lib_objects := $(lib_objects)
$(BUILD)/$(lib_dir).a: $(lib_objects)
	$(AR) crfs $@ $(lib_objects)

$(BUILD)/$(lib_dir)/%.o $(BUILD)/$(lib_dir)/%.d: lib_dir := $(lib_dir)
$(BUILD)/$(lib_dir)/%.o $(BUILD)/$(lib_dir)/%.d: lib/$(lib_dir)/%.cpp $(BUILD)/$(lib_dir)/.sentinel
	$(CXX) $(DEPFLAGS) $(CPPFLAGS) $(CXXFLAGS) -c -o "$(BUILD)/$(lib_dir)/$*.o" $<
	$(POSTCOMPILE)


