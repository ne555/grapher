/*
	This will draw a parabola using bresenham
	adjusting the vertex and a point to pass
*/
#include <iostream>
#include <valarray>
#include <GL/glut.h>
using namespace std;

int w,h;
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
	//  cout << "reshape " << width << "x" << height << endl;
	if (!width||!height) return; // minimizado ==> nada
	w=width; h=height;
	glViewport(0,0,w,h); // región donde se dibuja (toda la ventana)
	// rehace la matriz de proyección (la porcion de espacio visible)
	glMatrixMode(GL_PROJECTION);  glLoadIdentity();
	glOrtho(0,w,0,h,-1,1);
	glMatrixMode(GL_MODELVIEW);
	Display_cb(); // Redibuja mientras hace el reshape
}

void Display_cb(){
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3fv(red);
	glBegin(GL_POINTS);{
		std::valarray<double> begin(2),end(2);
		end[0] = w; end[1] = h;
		recursive::parabola(end[1]/square(end[0]),begin,end);
	}glEnd();

	glutSwapBuffers();
}

namespace recursive{
	void parabola(double a, const point2d &begin, const point2d &end){
		glVertex2i(begin[0], begin[1]);

		point2d delta = end-begin;
		if(norm1(delta) <= 1) return;

		point2d midpoint(2);
		midpoint[0] = begin[0]+delta[0]/2;
		midpoint[1] = a*square(midpoint[0]);

		parabola(a, begin, midpoint);
		parabola(a, midpoint, end);
	}
}
