EXECUTABLE=clientEEG
BUILDDIR=build
CXX=g++
CFLAGS= -I. -O2
LDFLAGS=
SOURCES=$(wildcard *.cpp)
OBJECTS = $(patsubst %.cpp,$(BUILDDIR)/%.o,$(SOURCES))

all: dir $(BUILDDIR)/$(EXECUTABLE)

dir:
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/$(EXECUTABLE): $(OBJECTS)
	$(CXX) $^ $(LDFLAGS) -o $@ 

$(OBJECTS): $(BUILDDIR)/%.o : %.cpp
	$(CXX) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(BUILDDIR)/*o $(BUILDDIR)/$(EXECUTABLE)
