CC = gcc
CXX = g++
CFLAGS = -Iinclude -Wall -O2 -fopenmp
CXXFLAGS = -Iinclude -Wall -O2 -std=c++11 -fopenmp

SRCDIR = .
OBJDIR = build

SOURCES = \
    main.c \
    Physics/Nbody.cpp \
    Physics/Leapfrog.c \
    Utilities/Config.c \
    Utilities/Logger.c \
    Preprocessing/data_generator.c \
    DataStructures/Vector3d.cpp \
    DataStructures/Octree.cpp

OBJECTS = $(patsubst %.c,$(OBJDIR)/%.o,$(filter %.c,$(SOURCES))) \
          $(patsubst %.cpp,$(OBJDIR)/%.o,$(filter %.cpp,$(SOURCES)))

all: simulator

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

simulator: $(OBJECTS)
	@mkdir -p datos/posiciones
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -rf $(OBJDIR) simulator datos/posiciones