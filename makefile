objects=parabola.o
project=parabola.bin
CXXFLAGS+=-ggdb
LIBRARIES=$(addprefix -l,GL GLU glut)

$(project): $(objects)
	$(CXX) $^ $(LIBRARIES) -o $@

all: $(objects)

.PHONY: clean
clean:
	-rm $(objects)
	-rm $(project)
