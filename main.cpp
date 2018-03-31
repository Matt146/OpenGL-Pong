/*
 *Author: Matt_146
 *Date: 3/28/18
 *Date Finished: 3/29/18
 *Approximate Time Taken: 2 hours
 *Objective: Create a Pong Clone in C++
 *License: NONE
 *Extra Notes: This code was made to be simple
 *Version 0.0.3
*/

/*
██████╗  ██████╗ ███╗   ██╗ ██████╗      ██████╗██╗      ██████╗ ███╗   ██╗███████╗
██╔══██╗██╔═══██╗████╗  ██║██╔════╝     ██╔════╝██║     ██╔═══██╗████╗  ██║██╔════╝
██████╔╝██║   ██║██╔██╗ ██║██║  ███╗    ██║     ██║     ██║   ██║██╔██╗ ██║█████╗
██╔═══╝ ██║   ██║██║╚██╗██║██║   ██║    ██║     ██║     ██║   ██║██║╚██╗██║██╔══╝
██║     ╚██████╔╝██║ ╚████║╚██████╔╝    ╚██████╗███████╗╚██████╔╝██║ ╚████║███████╗
╚═╝      ╚═════╝ ╚═╝  ╚═══╝ ╚═════╝      ╚═════╝╚══════╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
*/

/*
██╗   ██╗███████╗██████╗ ███████╗██╗ ██████╗ ███╗   ██╗     ██████╗     ██████╗    ██████╗
██║   ██║██╔════╝██╔══██╗██╔════╝██║██╔═══██╗████╗  ██║    ██╔═████╗   ██╔═████╗   ╚════██╗
██║   ██║█████╗  ██████╔╝███████╗██║██║   ██║██╔██╗ ██║    ██║██╔██║   ██║██╔██║    █████╔╝
╚██╗ ██╔╝██╔══╝  ██╔══██╗╚════██║██║██║   ██║██║╚██╗██║    ████╔╝██║   ████╔╝██║    ╚═══██╗
 ╚████╔╝ ███████╗██║  ██║███████║██║╚██████╔╝██║ ╚████║    ╚██████╔╝██╗╚██████╔╝██╗██████╔╝
  ╚═══╝  ╚══════╝╚═╝  ╚═╝╚══════╝╚═╝ ╚═════╝ ╚═╝  ╚═══╝     ╚═════╝ ╚═╝ ╚═════╝ ╚═╝╚═════╝
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <time.h>
#include <ctime>
#include <cmath>
#include <Windows.h>
#include <glut.h>

#define PI 3.14159265358979323846264338329950288 // I like to be accurate ;)
#define CIRCLE_PRECISION 100 // number of triangles to approximate a circle
#define BALL_FRAMES 43 // number of frames the ball has time to move
#define PASS 

/*Struct prototypes begin here*/
struct Point2d;
/*Struct prototypes end here*/

/*Function prototypes begin here*/
void display(); // main display function
void keyboard(unsigned char key, int x, int y); // keyboard
void mouse(int state, int button, int x, int y); // mouse
void initGL(); // initialize OpenGL
void drawPaddle(); // draws the paddle
void movePaddle(DWORD sleep,int iterations, double unit); // moves the paddle (if unit is -, move left, if +, move right)
void drawBall(double r, int precision);
void collisionCheck();
void moveBall(int frames, double unit, char axis);
void chooseSpeed();
double distance(double x1, double y1, double x2, double y2);
void printGameover();
/*Function prototypes end here*/

// starting ball speed
double xspeed = 0.01;
double yspeed = -0.01;

int score = 0;

typedef struct Point2d
{
	double x;
	double y;
};

void printGameOver()
{
	std::cout << "Game Over!" << std::endl;
	std::cout << "Your score: " << score << std::endl;
}

double distance(double x1, double y1, double x2, double y2)
{
	double distance = sqrt((pow((x2 - x1), 2)) + (pow((y2 - y1), 2)));
	return distance;
}

void chooseSpeed()
{
	/**If you realy want to*/
	char x[] = { 'a', 'b', 'c', 'd' };
	int random = rand() % 4;
	char choice = x[random];
	if (choice == 'a')
	{
		xspeed = -0.01;
		yspeed = 0.01;
	}
	if (choice == 'b')
	{
		xspeed = 0.01;
		yspeed = -0.01;
	}
	if (choice == 'c')
	{
		xspeed = 0.01;
		yspeed = 0.01;
	}
	if (choice == 'd')
	{
		xspeed = -0.01;
		yspeed = -0.01;
	}
}

// starting paddle coordinates
Point2d p = { -0.3, -0.9 };
Point2d p1 = { 0.3, -0.9 };
Point2d p2 = { -0.3, -1.0 };
Point2d p3 = { 0.3, -1.0 };

// starting ball 
Point2d b1 = { 0.0, 0.0 };
double radius = 0.1;

double randAdd()
{
	double i = 0, d = 0;
	i = rand() % 1 + 0.01; //Gives a number between -20 and +20;
	d = i / 100;
	return d;
}


void initGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void drawPaddle()
{
	/*
	// vertices
	Point2d p;			p.x = -0.3;	  p.y = -0.9;
	Point2d p1;			p1.x = 0.3;   p1.y = -0.9;
	Point2d p2;			p2.x = -0.3;  p2.y = -1;
	Point2d p3;			p3.x = 0.3;	  p3.y = -1;
	// rendering vertices
	*/
	glBegin(GL_QUADS);
	glVertex2d(p3.x, p3.y);
	glVertex2d(p1.x, p1.y);
	glVertex2d(p.x, p.y);
	glVertex2d(p2.x, p2.y);
	glEnd();
	//glFlush(); // flush the buffer
}

void movePaddle(DWORD sleep, int iterations, double unit)
{
	for (int i = 0; i < iterations; i++)
	{
		glBegin(GL_QUADS);
		glVertex2d(p3.x + unit, p3.y);
		glVertex2d(p1.x + unit, p1.y);
		glVertex2d(p.x + unit, p.y);
		glVertex2d(p2.x + unit, p2.y);
		glEnd();
		glFlush();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glutSwapBuffers();
		Sleep(sleep);

		// add unit values - to keep track
		p.x += unit;
		p1.x += unit;
		p2.x += unit;
		p3.x += unit;
		drawBall(0.1, CIRCLE_PRECISION);
	}
}

void drawBall(double r, int precision) {
	glBegin(GL_TRIANGLE_FAN);
	for (int ii = 0; ii < precision; ii++) 
	{
		double theta = 2.0f * 3.1415926f * double(ii) / double(precision); // get the current angle 
		double x = r * cos(theta); // calculate the x component 
		double y = r * sin(theta); // calculate the y component 
		glVertex2d(x + b1.x, y +b1.y); // output vertex 
	}
	glEnd();
}

void moveBall(int frames, double unit, char axis)
{
	for (int i = 0; i < frames; i++)
	{
		if (axis == 'x')
		{
			b1.x += unit;
		}
		if (axis == 'y')
		{
			b1.y += unit;
		}
	}
}

void collisionCheck()
{
	if (b1.x >= 1)
	{
		xspeed = -xspeed;
	}
	if (b1.x <= -1)
	{
		xspeed = -xspeed;
	}
	if (b1.y >= 1)
	{
		yspeed = -yspeed;
	}
	if (b1.y <= -1)
	{
		glutDestroyWindow(1);
		printGameOver();
		Sleep(5000);
		exit(0);
	}
	if (b1.x >= p.x)
	{
		if (b1.y <= p.y)
		{
			if (b1.x < p3.x)
			{
				if (b1.y <= p.y)
				{
					xspeed = -xspeed + randAdd() + 0.01;
					yspeed = -yspeed + randAdd() + 0.01;
					score++;
				}
			}
		}
	}
	moveBall(1, xspeed, 'x');
	moveBall(1, yspeed, 'y');
	Sleep(20.3333); // ~40-50 fps
	glFlush();
}

void display()
{	
	glClear(GL_COLOR_BUFFER_BIT);
	drawPaddle(); // drawing the paddle
	drawBall(radius, CIRCLE_PRECISION);
	collisionCheck();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glFlush(); // flushes the buffer
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == 'f')
	{
		glutFullScreen();
	}
	if (key == 27)
	{
		exit(0);
	}
	if (key == 'd' && p1.x < 1)
	{
		movePaddle(0, 10, 0.01);
	}
	if (key == 'a' && p2.x > -1)
	{
		movePaddle(0, 10, -0.01);
	}
}

void mouse(int state, int button, int x, int y)
{

}

void idle()
{
	glutPostRedisplay();
}

int main()
{
	srand(time(NULL));
	chooseSpeed();
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Pong in C++");
	initGL(); // initializes OpenGL
	glutSetWindow(1);
	Sleep(1200);
	glutDisplayFunc(display); // main display function
	glutKeyboardFunc(keyboard); // keyboard function
	glutMouseFunc(mouse); // mouse function
	glutIdleFunc(idle);
	glutMainLoop(); // main loop
	return 0;
}