
PLUGIN_EXT	?= .so

CXX	?= g++
CC	?= gcc

CFLAGS := $(CFLAGS) 
CXXFLAGS := -I. $(CXXFLAGS)

PLUGIN := chp(PLUGIN_EXT)

SOURCES := ConstrainedHarmonicPeak.cpp

PLUGIN_MAIN := plugins.cpp

TESTS := 

OBJECTS := $(SOURCES:.cpp=.o)
OBJECTS := $(OBJECTS:.c=.o)

PLUGIN_OBJECTS := $(OBJECTS) $(PLUGIN_MAIN:.cpp=.o)

all: $(PLUGIN) $(TESTS)
	for t in $(TESTS); do echo "Running $$t"; ./"$$t" || exit 1; done

plugin: $(PLUGIN)

$(PLUGIN): $(PLUGIN_OBJECTS)
	$(CXX) -o $@ $^ $(PLUGIN_LDFLAGS)

clean:		
		rm -f $(PLUGIN_OBJECTS) test/*.o

distclean:	clean
		rm -f $(PLUGIN) $(TESTS)

depend:
		makedepend -Y -fMakefile.inc *.cpp test/*.cpp *.h test/*.h

# DO NOT DELETE

ConstrainedHarmonicPeak.o: ConstrainedHarmonicPeak.h
plugins.o: ConstrainedHarmonicPeak.h
