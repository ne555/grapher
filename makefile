objects=parabola.o
project=parabola
LIBRARIES=$(addprefix -l,GL GLU glut)

$(project): $(objects)
	$(CXX) $^ $(LIBRARIES) -o $@

all: $(objects)

.PHONY: clean
clean:
	-rm $(objects)
	-rm $(project)
