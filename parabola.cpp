/*
	This will draw a parabola using bresenham
	adjusting the vertex and a point to pass
*/
#include <iostream>
#include <valarray>
#include <GL/glut.h>
using namespace std;

int w,h;
int pointsize=10;

float
	white[] = {1,1,1},
	black[] = {0,0,0},
	blue[] = {0,0,1},
	green[] = {0,1,0},
	red[] = {1,0,0};

float *fondo=white;

void initialize();

template<class T>
T square(T x){
	return x*x;
}

template<class T>
T norm1(const std::valarray<T> &v){
	return v.max();
}

namespace bresenham{
	typedef std::valarray<int> point2d;
	void parabola(const point2d &end);
}
namespace recursive{
	typedef std::valarray<double> point2d;
	void parabola(double a, const point2d &begin, const point2d &end);
}

int main(int argc, char **argv){
	glutInit(&argc,argv);// inicializa glut
	initialize(); // condiciones iniciales de la ventana y OpenGL
	glutMainLoop(); // entra en loop de reconocimiento de eventos
	return 0;
}

void Keyboard_cb(unsigned char key,int x=0,int y=0);
void Mouse_cb(int button, int state, int x, int y);
void Motion_cb(int x, int y);
void Reshape_cb(int width, int height);
void Display_cb();

void initialize() {
	// pide color RGB y double buffering
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
	glutInitWindowSize(w,h);
	glutCreateWindow("Parabola Bresenham"); // crea el main window

	//declara los callbacks
	//los que no se usan no se declaran
	glutDisplayFunc(Display_cb); // redisplays
	glutReshapeFunc(Reshape_cb);
	glutKeyboardFunc(Keyboard_cb);
	//glutMouseFunc(Mouse_cb);

	// ========================
	// estado normal del OpenGL
	// ========================
	glClearColor(fondo[0],fondo[1],fondo[2],1);  // color de fondo
	glMatrixMode(GL_MODELVIEW); glLoadIdentity(); // constante
}

void Keyboard_cb(unsigned char key,int x,int y) {
	if (tolower(key)=='q')
		exit(EXIT_SUCCESS);
}

// Clicks del mouse
void Mouse_cb(int button, int state, int x, int y){
}

// Movimientos del mouse
void Motion_cb(int x, int y){
}

void Reshape_cb(int width, int height){
	if (!width||!height) return; // minimizado ==> nada
	w=width; h=height;
	glViewport(0,0,w,h); // región donde se dibuja (toda la ventana)
	// rehace la matriz de proyección (la porcion de espacio visible)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,w/pointsize,0,h/pointsize,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}

void Display_cb(){
	glClear(GL_COLOR_BUFFER_BIT);

	glPointSize(pointsize);
	glColor3fv(green);
	glBegin(GL_POINTS);{
		std::valarray<int> end(2);
		end[0] = w/pointsize; end[1] = h/pointsize;
		bresenham::parabola(end);
	}glEnd();

	glPointSize(1);
	glColor3fv(black);
	glBegin(GL_POINTS);{
		std::valarray<double> begin(2),end(2);
		end[0] = w/pointsize; end[1] = h/pointsize;
		recursive::parabola(end[1]/square(end[0]),begin,end);
	}glEnd();


	glutSwapBuffers();
}

namespace bresenham{
	void travel_x(const point2d &end){
		int 
			dx2 = square(end[0]),
			dy = end[1],
			discriminant = -2*dy + dx2,
			south = -6*dy,
			north = -6*dy + 2*dx2,
			increment = -4*dy;

		for(size_t x=0, y=0; x<=end[0]; ++x){
			glVertex2i(x,y);
			if(discriminant>0){ //south (same y)
				discriminant += south;
			}
			else{
				discriminant += north;
				++y;
			}

			south += increment;
			north += increment;
		}
	}

	void travel_y(const point2d &begin, const point2d &end){
		int 
			dx2 = square(end[0]),
			dy = end[1],
			discriminant = 4*dx2*(begin[1]+1) - dy*square(2*begin[0]+1),
			east = 4*dx2 - 8*dy*(begin[0]+1),
			west = 4*dx2;

		for(size_t x=begin[0], y=begin[1]; y<=end[1]; ++y){
			glVertex2i(x,y);
			if(discriminant>0){ //east (increase x)
				discriminant += east;
				east += -8*dy;
				++x;
			}
			else{
				discriminant += west;
			}

		}
	}

	void parabola(const point2d &end){
		//y-ax^2 = 0
		//a > 0
		int 
			dx2 = square(end[0]),
			dy = end[1];
		point2d change(2);
		change[0] = dx2 / (2*dy);
		change[1] = square(change[0])*dy/dx2;

		//0 <= y' <= 1
		travel_x(change);
		//1 < y'
		travel_y(change, end);

		glColor3fv(red);
		glVertex2i(change[0], change[1]);
	}
}

namespace recursive{
	void parabola(double a, const point2d &begin, const point2d &end){
		glVertex2d(begin[0], begin[1]);

		point2d delta = end-begin;
		if(norm1(delta) <= 1.0/pointsize) return;

		point2d midpoint(2);
		midpoint[0] = begin[0]+delta[0]/2;
		midpoint[1] = a*square(midpoint[0]);

		parabola(a, begin, midpoint);
		parabola(a, midpoint, end);
	}
}
