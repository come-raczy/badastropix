ifeq (,$(programs_aux))
$(error No programs specified)
endif

program := $(word 1, $(programs_aux))

ifeq (1, $(words $(programs_aux)))
programs_aux:=
else
programs_aux:=$(wordlist 2, $(words $(programs_aux)), $(programs_aux))
endif

$(BUILD)/$(program): $(program).cpp $(BUILD)/$(program).d $(libraries)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $< $(LDFLAGS) $(libraries)

$(BUILD)/$(program).d: $(program).cpp $(BUILD)/.sentinel
	@set -e; rm -f $@; \
	$(CC) -MM -MT "$@ $(@:%.d=%)" $(CPPFLAGS) $< > $@

-include $(BUILD)/$(program).d

