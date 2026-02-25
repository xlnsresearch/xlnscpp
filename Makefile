CXX      := g++
CXXFLAGS := -O2 -std=c++14 -Wall

# 16-bit tests (each file #includes xlns16.cpp internally)
TEST16_SRCS := xlns16test.cpp \
               xlns16funtest.cpp \
               xlns16_new_functions_test.cpp \
               time16test.cpp \
               sb16.cpp \
               db16.cpp \
               sbmit16.cpp \
               dbmit16.cpp

# 32-bit tests (each file #includes xlns32.cpp internally)
TEST32_SRCS := xlns32test.cpp \
               xlns32funtest.cpp \
               xlns32_new_functions_test.cpp

# Combined 16 + 32 test
BOTH_SRCS   := xlnsbothtest.cpp

# Derive binary names by stripping the .cpp extension
TEST16_BINS := $(TEST16_SRCS:.cpp=)
TEST32_BINS := $(TEST32_SRCS:.cpp=)
BOTH_BINS   := $(BOTH_SRCS:.cpp=)

ALL_BINS    := $(TEST16_BINS) $(TEST32_BINS) $(BOTH_BINS)

FORMAT_SRCS := $(wildcard *.cpp *.h)

.PHONY: all test16 test32 both format format-check clean help

all: test16 test32 both

## Build all 16-bit test programs
test16: $(TEST16_BINS)

## Build all 32-bit test programs
test32: $(TEST32_BINS)

## Build the combined 16+32-bit test program
both: $(BOTH_BINS)

# 16-bit binaries depend on their own source and the library headers
$(TEST16_BINS): %: %.cpp xlns16.cpp xlns16cvtbl.h xlns16revcvtbl.h xlns16sbdbtbl.h
	$(CXX) $(CXXFLAGS) -o $@ $<

# 32-bit binaries depend on their own source and the library header
$(TEST32_BINS): %: %.cpp xlns32.cpp xlns32tbl.h
	$(CXX) $(CXXFLAGS) -o $@ $<

# Both-test binary depends on both libraries
$(BOTH_BINS): %: %.cpp xlns16.cpp xlns32.cpp xlns16cvtbl.h xlns16revcvtbl.h xlns16sbdbtbl.h xlns32tbl.h
	$(CXX) $(CXXFLAGS) -o $@ $<

## Format all source files in-place using .clang-format
format:
	clang-format -i $(FORMAT_SRCS)

## Check formatting without modifying files (exits non-zero if changes needed)
format-check:
	clang-format --dry-run --Werror $(FORMAT_SRCS)

## Remove all compiled binaries
clean:
	rm -f $(ALL_BINS)

## Print available targets
help:
	@echo "Available targets:"
	@echo "  make test16   — build all 16-bit test programs"
	@echo "  make test32   — build all 32-bit test programs"
	@echo "  make both     — build the combined xlnsbothtest program"
	@echo "  make all      — build everything (default)"
	@echo "  make format       — format all sources in-place (clang-format)"
	@echo "  make format-check — check formatting without modifying files"
	@echo "  make clean         — remove all compiled binaries"
