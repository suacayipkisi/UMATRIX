VERSION   := 0.0.1

ifeq ($(origin CC),default)
  CC := gcc
endif
AR        ?= ar

# Build mode: debug (default) or release, e.g. `make MODE=release`
MODE      ?= debug

STD       := -std=c23
WARNINGS  := -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wstrict-prototypes
INCLUDES  := -Isrc
DEPFLAGS  := -MMD -MP
LDLIBS    := -lm

ifeq ($(MODE),debug)
  MODE_CFLAGS  := -O0 -g3 -fsanitize=address,undefined -fno-omit-frame-pointer
  MODE_LDFLAGS := -fsanitize=address,undefined
else ifeq ($(MODE),release)
  MODE_CFLAGS  := -O2 -DNDEBUG
  MODE_LDFLAGS :=
else
  $(error MODE must be 'debug' or 'release', got '$(MODE)')
endif

ALL_CFLAGS  := $(STD) $(WARNINGS) $(MODE_CFLAGS) $(CFLAGS)
ALL_LDFLAGS := $(MODE_LDFLAGS) $(LDFLAGS)

BUILD_DIR := build/$(MODE)
OBJ_DIR   := $(BUILD_DIR)/obj

# Recursive: picks up .c files in any nested subdirectory
LIB_SRC   := $(sort $(shell find src -type f -name '*.c'))
LIB_OBJ   := $(LIB_SRC:src/%.c=$(OBJ_DIR)/src/%.o)
LIB       := $(BUILD_DIR)/libumatrix.a

EX_SRC    := $(sort $(shell find examples -type f -name '*.c'))
EX_BIN    := $(EX_SRC:examples/%.c=$(BUILD_DIR)/examples/%)

DEPS      := $(LIB_OBJ:.o=.d) $(EX_SRC:examples/%.c=$(OBJ_DIR)/examples/%.d)

.PHONY: all lib examples debug release run clean

# Keep object files instead of deleting them as intermediates
.SECONDARY:

all: lib examples

lib: $(LIB)
examples: $(EX_BIN)

debug:
	$(MAKE) MODE=debug all
release:
	$(MAKE) MODE=release all

# Library: compile each source file, then archive into a static library
$(OBJ_DIR)/src/%.o: src/%.c Makefile
	@mkdir -p $(@D)
	$(CC) $(ALL_CFLAGS) $(INCLUDES) $(DEPFLAGS) $(CPPFLAGS) -c $< -o $@

# Recreate the archive from scratch: nested dirs may contain objects with the
# same basename, and 'ar r' would replace one with the other on rebuilds
$(LIB): $(LIB_OBJ)
	rm -f $@
	$(AR) rcs $@ $^

# Examples: each examples/<name>.c becomes one executable linked against the library
$(OBJ_DIR)/examples/%.o: examples/%.c Makefile
	@mkdir -p $(@D)
	$(CC) $(ALL_CFLAGS) $(INCLUDES) $(DEPFLAGS) $(CPPFLAGS) -c $< -o $@

$(BUILD_DIR)/examples/%: $(OBJ_DIR)/examples/%.o $(LIB)
	@mkdir -p $(@D)
	$(CC) $(ALL_LDFLAGS) $< -L$(BUILD_DIR) -lumatrix $(LDLIBS) -o $@

run: $(BUILD_DIR)/examples/main
	./$<

clean:
	rm -rf build

-include $(DEPS)
