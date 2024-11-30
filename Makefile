# CORE (needed to compile)

ifndef CC
$(warning C compiler not detected. Setting default compiler to GCC...)
	CC :=	gcc
endif

# CORE OPTIONAL (compiler flags)

ifndef CFLAGS
	CFLAGS :=
endif

# ACTION definitions

ifndef BUILD_TYPE
	BUILD_TYPE = Debug
endif

ifeq ($(CC), tcc)
	CFLAGS += -std=c99
else
	CFLAGS += --std=c99
endif

ifeq ($(BUILD_TYPE), Release)
	CFLAGS += -O3
else
	CFLAGS += -O0 -g3 -ggdb -Wall
endif

CFLAGS += $(shell pkg-config --cflags gtk+-3.0)

ESCAPE        := \033
RED           := $(ESCAPE)[31m
BOLD_RED      := $(ESCAPE)[1;31m
GREEN         := $(ESCAPE)[32m
BOLD_GREEN    := $(ESCAPE)[1;32m
YELLOW        := $(ESCAPE)[33m
BOLD_YELLOW   := $(ESCAPE)[1;33m
BLUE          := $(ESCAPE)[34m
BOLD_BLUE     := $(ESCAPE)[1;34m
PURPLE        := $(ESCAPE)[35m
BOLD_PURPLE   := $(ESCAPE)[1;35m
CYAN          := $(ESCAPE)[36m
BOLD_CYAN     := $(ESCAPE)[1;36m
RESET_COLOUR  := $(ESCAPE)[0m

PWD           := $(shell pwd | sed 's/ /\\ /g')
OBJ_DIR       := $(PWD)/.obj
SRC_DIR       := $(PWD)/src
BIN_DIR       := $(PWD)/.
DIRS          := . .obj .obj/network .obj/util
HEADERS       := -I./src -I./src/network -I./src/util
LIBRARIES     := $(shell pkg-config --libs gtk+-3.0) -lcurl

ifndef MINGW_CHOST
	LIBRARIES   +=
else
	LIBRARIES   +=
endif

_TARGETS      := legac_launcher
_SOURCES      := $(shell find $(SRC_DIR) -name "*.c")
_SOURCES      := $(patsubst $(SRC_DIR)/%,%,$(_SOURCES))
_OBJECTS      := $(patsubst %.c,%.o,$(_SOURCES))

TARGETS       := $(addprefix $(BIN_DIR)/, $(_TARGETS))
SOURCES       := $(addprefix $(SRC_DIR)/, $(_SOURCES))
OBJECTS       := $(addprefix $(OBJ_DIR)/, $(_OBJECTS))

.PHONY: all

all: directories $(TARGETS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@printf "[CC] ""$(GREEN)""Building object '$<'""$(RESET_COLOUR)""\n"
	@$(CC) $(CFLAGS) $(LIBRARIES) $(HEADERS) -c $< -o $@

$(TARGETS): $(OBJECTS)
	@printf "[BIN] ""$(BOLD_GREEN)""Linking binary '$@'""$(RESET_COLOUR)""\n"
	@$(CC) $^ $(LIBRARIES) $(HEADERS) -o $@

directories: $(DIRS)

$(DIRS):
	@printf "[DIR] ""$(BLUE)""Directory ""$(BOLD_BLUE)""'$@'""$(RESET_COLOUR)$(BLUE)"" created""$(RESET_COLOUR).""\n"
	@mkdir -p $(PWD)/$@

clean:
	@rm -rv $(BIN_DIR)/$(_TARGETS) 2>/dev/null || true
	@rm -rv $(BIN_DIR) 2>/dev/null || true
	@rm -rv $(OBJ_DIR) 2>/dev/null || true
	@rm -rv ./jars     2>/dev/null || true

