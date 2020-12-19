# recursive file search
rfilelist=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rfilelist,$d/,$2))
# the name of program
TARGET							= hexlab
CXX								= clang++
# C++ compiler flags
CXXFLAGS						+= -Wall -std=c++17 -c $(shell pkg-config --cflags gtkmm-3.0 sigc++-2.0)
LIBS							+= $(shell pkg-config --libs gtkmm-3.0 sigc++-2.0) -lstdc++
EXT								= cc
# source files
SRC								= ./src
SRCS							:= $(call rfilelist,$(SRC),*.$(EXT))
INCLUDES						= ./include
CFLAGS							+= $(addprefix -I$(INCLUDES))
OBJS							= $(SRCS:%.$(EXT)=%.o)
DEBUG_HELPERS					= $(SRCS:%.$(EXT)=%.debug)
OPTIMIZE_HELPERS				= $(SRCS:%.$(EXT)=%.optim)
OBJDEBOUT						= $(@:%.debug=%.o)
OBJOPTOUT						= $(@:%.optim=%.o)
DEBOUT							= $(@:%.debug=%.$(EXT))
OPTOUT							= $(@:%.optim=%.$(EXT))

# rules for debug build and optimized build
%.debug: %.$(EXT)
	$(CC) $(CXXFLAGS) -I$(INCLUDES) -ggdb -D_DEBUG -DDEBUG -o $(OBJDEBOUT) $(DEBOUT)
	rm -f $(@.debug=%.debug)
	touch -f $@

%.optim: %.$(EXT)
	$(CC) $(CXXFLAGS) -I$(INCLUDES) -O2 -DNDEBUG -o $(OBJOPTOUT) $(OPTOUT)
	rm -f $(@.optim=%.optim)
	touch -f $@

# default build
all: debug

# debug build
debug: $(DEBUG_HELPERS)
	test -s $@ || mkdir $@
	$(CC) $(OBJS) -o debug/$(TARGET) $(LIBS)
	rm -f $(DEBUG_HELPERS)

# optimized build
optim: $(OPTIMIZE_HELPERS)
	test -s $@ || mkdir $@
	$(CC) $(OBJS) -o optim/$(TARGET) $(LIBS)
	rm -f $(OPTIMIZE_HELPERS)
	strip optim/$(TARGET)

docs:
	doxygen $(TARGET).dox

# clean rule
clean:
	rm -f $(OBJS) $(DEBUG_HELPERS) $(OPTIMIZE_HELPERS)

