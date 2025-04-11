
TARGET = test

BUILD_DIR = build

CC  ?= gcc
SZ  ?= size

ONNDORI_PATHROOT := ../../src

C_SOURCES := 
C_SOURCES += example.c 
C_SOURCES += $(ONNDORI_PATHROOT)/Onndori.c

C_INCLUDES := 
C_INCLUDES += -I$(CURDIR)
C_INCLUDES += -I$(ONNDORI_PATHROOT)

CFLAGS := 
CFLAGS += -Wall 
CFLAGS += -Og -g 
CFLAGS += -pthread
CFLAGS += $(C_INCLUDES)

LDFLAGS := 
LDFLAGS += -pthread

all: $(TARGET)

#######################################
# build the application
#######################################
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))

vpath %.c $(sort $(dir $(C_SOURCES)))

$(TARGET): $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) $< -o $@
	
	
$(BUILD_DIR):
	mkdir -p $@

clean:
	-rm -fR $(BUILD_DIR)
	-rm -f $(TARGET)

# *** EOF ***
