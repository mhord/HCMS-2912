# Define the compiler to be used by default.
CC = g++

# Compile flags. 
#  -c - compile and assemble but don't link
#  -Wall - highest level of warnings
CFLAGS = -c -Wall

# Linker flags (unused)
LDFLAGS=

SOURCES = \
main.cpp \
SFE_SPI/SFE_SPI.cpp \
gpio/gpio.cpp \
hcms/hcms.cpp

OBJECTS = $(SOURCES:.cpp=.o)

EXECUTABLE = hcms_demo

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@:w

