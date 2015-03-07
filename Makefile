# recursive file search
rfilelist=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rfilelist,$d/,$2))
# the name of program
TARGET				= hexlab
CXX 					= clang++
# C++ compiler flags
CXXFLAGS				+= -Wall -std=c++11 -Wno-mismatched-tags -Wno-deprecated-register -c $(shell pkg-config --cflags gtkmm-2.4 sigc++-2.0)
LIBS					+= $(shell pkg-config --libs gtkmm-2.4 sigc++-2.0)
EXT					= cc
# source files
SRCS     			:= $(call rfilelist,./,*.$(EXT))
INCLUDES				= ./include
OBJS 					= $(SRCS:%.$(EXT)=%.o)
DEBUG_HELPERS 		= $(SRCS:%.$(EXT)=%.debug)
OPTIMIZE_HELPERS 	= $(SRCS:%.$(EXT)=%.optim)

# rules for object files
%.o: %.$(EXT)
	$(CXX) $(CXXFLAGS) -I$(INCLUDES) $?

# rules for debug build and optimized build
%.debug: %.$(EXT)
	$(CXX) $(CXXFLAGS) -I$(INCLUDES) -g -O0 -D _DEBUG $(@:%.debug=%.$(EXT)) 
	rm -f $(@.debug=%.optim)
	touch -f $@

%.optim: %.$(EXT)
	$(CXX) $(CXXFLAGS) -I$(INCLUDES) -Os $(@:%.optim=%.$(EXT))
	rm -f $(@.optim=%.debug)
	touch -f $@

# default build
all: debug

# debug build
debug: $(DEBUG_HELPERS)
	test -s $@ || mkdir $@
	$(CXX) $(OBJS) -o debug/$(TARGET) $(LIBS) 
	rm -f *.debug

# optimized build
optim: $(OPTIMIZE_HELPERS)
	test -s $@ || mkdir $@
	$(CXX) $(OBJS) -o optim/$(TARGET) $(LIBS)
	rm -f *.optim

# clean rule
clean:
	rm -f *.optim *.debug $(OBJS)

