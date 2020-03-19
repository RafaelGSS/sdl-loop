CXX ?= g++
LDFLAGS = -Wall
LDLIBS = -lSDL2 -lSDL2_ttf

TARGET = $(notdir $(basename $<))
SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(addsuffix .o,$(basename $(SRCS)))
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

FLAGS ?= $(INC_FLAGS) -MMD -MP

app: $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@ $(LDLIBS)

clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS)

-include $(DEPS)
