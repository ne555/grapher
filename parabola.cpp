/*
	This will draw a parabola using bresenham
	adjusting the vertex and a point to pass
*/
#include <iostream>
#include <GL/glut.h>
using namespace std;

int w,h;
float
	fondo[] = {1,1,1};

void initialize();

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

}
