BUILDDIR ?= build
CFG      ?= default
NAME     ?= wasserstein
SRCDIR   ?= src

Q ?= @

BINDIR := $(BUILDDIR)/$(CFG)
BIN    := $(BINDIR)/$(NAME)
SRC    := $(sort $(wildcard $(SRCDIR)/*.cpp))
OBJ    := $(SRC:$(SRCDIR)/%.cpp=$(BINDIR)/%.o)
DEP    := $(OBJ:%.o=%.d)

# -fno-omit-frame-pointer is for profiling with perf only
CFLAGS   += -O3 -Wall -DNDEBUG
INCLUDE_FLAGS = -Isrc/include -Isrc 
CXXFLAGS += $(CFLAGS) -std=c++11 $(INCLUDE_FLAGS)  

DUMMY := $(shell mkdir -p $(sort $(dir $(OBJ))))

.PHONY: all clean 

all: $(BIN)

# -include $(CFG).cfg

-include $(DEP)

clean:
	@echo "===> CLEAN"
	$(Q)rm -f $(BINDIR)/*.d
	$(Q)rm -f $(BINDIR)/*.o
	$(Q)rm -f $(BINDIR)/$(NAME)

$(BIN): $(OBJ)
	@echo "===> LD $@"
	$(Q)$(CXX) -o $(BIN) $(OBJ) $(LINK_FLAGS)

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	@echo "===> CXX $<"
	$(Q)$(CXX) $(CXXFLAGS) -MMD -c -o $@ $<

link:
	@echo "===> LD $@"
	$(Q)$(CXX) -o $(BIN) $(OBJ) $(LINK_FLAGS)

prerequisites: all

target: test 

