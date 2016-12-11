#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <cassert>

#include "PatternDatabase.h"
#include "Utility.h"
#include "Cubies.h"
#include "cube.h"
#include "RubikSolver.h"
#include "TableGenerator.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define ATV(array) array[0], array[1], array[2] /* Arr to val */

enum facesEnum { e_None, e_ClockTop, e_ClockFront, e_ClockBottom, e_ClockBack, e_ClockRight, e_ClockLeft,
                 e_CounterTop, e_CounterFront, e_CounterBottom, e_CounterBack, e_CounterRight, e_CounterLeft };
enum rotateVectorEnum { e_LeftClockRightCounter, e_LeftCounterRightClock, e_TopCounterBottomClock,
                        e_TopClockBottomCounter, e_FrontClockBackCounter, e_FrontCounterBackClock };
enum rotateCountEnum { e_ClockRotationCount, e_CounterRotationCount };
enum rotateSpeedEnum { e_NormalSpeed, e_FastSpeed};
enum sides { e_Front, e_Back, e_Left, e_Right, e_Top, e_Bottom };
enum cubeColors { e_Red, e_Blue, e_Yellow, e_Green, e_White, e_Orange };
int rotateStatus ;
int rotateSpeed = 0;

float colorsRGB[6][3] = {
    { 1,0,0 }, { 0,0,1 }, { 1,1,0 }, { 0,1,0 }, { 1,1,1 }, { 1,0.5,0 }
};
float rotationSpeeds[2] = { 0.0625, 3 } ;
int rotationsCount[2] = { 1, 3 } ;

// angle of rotation for the camera direction
float angle = 0.0f;
// actual vector representing the camera's direction
float lx=0.0f,lz=-1.0f;
// XZ position of the camera
float x=0.0f, z=5.0f;
// the key states. These variables will be zero
//when no key is being presses
float deltaAngle = 0.0f;
float deltaMove = 0;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
	float ratio =  w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

double rotateAngel  ;
double center_dis = 1.1;

/* 0  - > 8 back
 * 9  - > 17 mid
 * 18 - > 26 front
 * i%3 == 0 , i - > left
 * i%3 == 2 , i - > right
 * i%9 < 3  , i - > bottom
 * i%9 > 5  , i - > top
*/
float positions[27][3] = {
    /* first slide */
    { 0 , 0  , 0   } , { 1.1 , 0   , 0   } , { 2.2 , 0   , 0 } ,
    { 0 , 1.1, 0   } , { 1.1 , 1.1 , 0   } , { 2.2 , 1.1 , 0 } ,
    { 0 , 2.2, 0   } , { 1.1 , 2.2 , 0   } , { 2.2 , 2.2 , 0 } ,
    /* second slide */
    { 0 , 0  , 1.1 } , { 1.1 , 0   , 1.1 } , { 2.2 , 0   , 1.1 } ,
    { 0 , 1.1, 1.1 } , { 1.1 , 1.1 , 1.1 } , { 2.2 , 1.1 , 1.1 } ,
    { 0 , 2.2, 1.1 } , { 1.1 , 2.2 , 1.1 } , { 2.2 , 2.2 , 1.1 } ,
    /* third slide */
    { 0 , 0  , 2.2 } , { 1.1 , 0   , 2.2 } , { 2.2 , 0   , 2.2 } ,
    { 0 , 1.1, 2.2 } , { 1.1 , 1.1 , 2.2 } , { 2.2 , 1.1 , 2.2 } ,
    { 0 , 2.2, 2.2 } , { 1.1 , 2.2 , 2.2 } , { 2.2 , 2.2 , 2.2 } ,
};
struct cube {
    int sideColor[6] ;
    cube()
    {
        for ( int i = 0 ; i < 6 ; i++ )
            sideColor[i] = -1 ;
    }
}cubes[27];
int getColorOfChar( char c )
{
    switch (c) {
        case 'r' :
            return e_Red ;
        case 'y' :
            return e_Yellow ;
        case 'g' :
            return e_Green ;
        case 'b' :
            return e_Blue ;
        case 'w' :
            return e_White ;
        case 'o' :
            return e_Orange ;
    }
    assert(false) ;
}
void constructFaces(std::string s)
{
    #ifndef top
    {
        for ( int j = 0 ; j < 3 ; j++ )
            for ( int k = 0 ; k < 3 ; k++ )
                cubes[6+j*9+k].sideColor[e_Top] = getColorOfChar(s[j*3+k]) ;
    }
    #endif
    #ifndef front
    {
        for ( int j = 0 ; j < 3 ; j++ )
            for ( int k = 0 ; k < 3 ; k++ )
                cubes[24-j*3+k].sideColor[e_Front] = getColorOfChar(s[j*3+k+9]) ;
    }
    #endif
    #ifndef bottom
    {
        for ( int j = 0 ; j < 3 ; j++ )
            for ( int k = 0 ; k < 3 ; k++ )
                cubes[j*9+k].sideColor[e_Bottom] = getColorOfChar(s[24-j*3+k]) ;
    }
    #endif
    #ifndef back
    {
        for ( int j = 0 ; j < 9 ; j++ )
            cubes[j].sideColor[e_Back] = getColorOfChar(s[35-j]) ;
    }
    #endif
    #ifndef right
    {
        for ( int j = 0 ; j < 3 ; j++ )
            for ( int k = 0 ; k < 3 ; k++ )
                cubes[26-j*3-k*9].sideColor[e_Right] = getColorOfChar(s[j*3+k+36]) ;
    }
    #endif
    #ifndef left
    {
        for ( int j = 0 ; j < 3 ; j++ )
            for ( int k = 0 ; k < 3 ; k++ )
                cubes[6-j*3+k*9].sideColor[e_Left] = getColorOfChar(s[j*3+k+45]) ;
    }
    #endif
}
float rotateVector[6][3] = {
    {  1 ,  0 ,  0 } ,
    { -1 ,  0 ,  0 } ,
    {  0 ,  1 ,  0 } ,
    {  0 , -1 ,  0 } ,
    {  0 ,  0 , -1 } ,
    {  0 ,  0 ,  1 }
};
/*
    left - right
    glRotated(cnt,1,0,0);
    top - bottom
    glRotated(cnt,0,1,0);
    front - back
    glRotated(cnt,0,0,1);
*/
int faceTopEquation(int i, int j)
{
    return i*9+j+6 ;
}
bool isFaceTop(int i)
{
    return i%9 > 5  ;
}
int faceBottomEquation(int i, int j)
{
    return i+j*9;
}

bool isFaceBottom(int i)
{
    return i%9 < 3 ;
}
int faceRightEquation(int i, int j)
{
    return (i*3)+(j*9)+2 ;
}

bool isFaceRight(int i)
{
    return i%3 == 2 ;
}
int faceLeftEquation(int i, int j)
{
    return (i*9)+(j*3) ;
}

bool isFaceLeft(int i)
{
    return i%3 == 0 ;
}
int faceBackEquation(int i, int j)
{
    return (i*3)+j ;
}

bool isFaceBack(int i)
{
    return i<=8 ;
}
int faceFrontEquation(int i, int j)
{
    return 26-i-(j*3) ;
}
bool isFaceFront(int i)
{
    return i>=18;
}
int getFaceIndex(int i, int j)
{
    switch (rotateStatus) {
        case e_ClockBack:
        case e_CounterBack:
            return faceBackEquation(i,j) ;
        case e_ClockFront:
        case e_CounterFront:
            return faceFrontEquation(i,j) ;
        case e_ClockRight:
        case e_CounterRight:
            return faceRightEquation(i,j) ;
        case e_ClockLeft:
        case e_CounterLeft:
            return faceLeftEquation(i,j) ;
        case e_ClockTop:
        case e_CounterTop:
            return faceTopEquation(i,j) ;
        case e_ClockBottom:
        case e_CounterBottom:
            return faceBottomEquation(i,j) ;
    }
    assert(false);
}
bool isRotatedIndex(int i)
{
    switch (rotateStatus) {
        case e_ClockBack:
        case e_CounterBack:
            return isFaceBack(i) ;
        case e_ClockFront:
        case e_CounterFront:
            return isFaceFront(i) ;
        case e_ClockRight:
        case e_CounterRight:
            return isFaceRight(i) ;
        case e_ClockLeft:
        case e_CounterLeft:
            return isFaceLeft(i) ;
        case e_ClockTop:
        case e_CounterTop:
            return isFaceTop(i) ;
        case e_ClockBottom:
        case e_CounterBottom:
            return isFaceBottom(i) ;
    }
    return false ;
}
int getRotateVector()
{
    switch (rotateStatus) {
        case e_ClockBack:
        case e_CounterFront:
            return e_FrontCounterBackClock ;
        case e_CounterBack:
        case e_ClockFront:
            return e_FrontClockBackCounter ;
        case e_ClockRight:
        case e_CounterLeft:
            return e_LeftCounterRightClock ;
        case e_CounterRight:
        case e_ClockLeft:
            return e_LeftClockRightCounter ;
        case e_ClockTop:
        case e_CounterBottom:
            return e_TopClockBottomCounter ;
        case e_CounterTop:
        case e_ClockBottom:
            return e_TopCounterBottomClock ;
    }
    assert(false) ;
}
int getRotationCount()
{
        switch (rotateStatus) {
        case e_ClockBack:
        case e_ClockFront:
        case e_ClockRight:
        case e_ClockLeft:
        case e_ClockTop:
        case e_ClockBottom:
            return rotationsCount[e_ClockRotationCount] ;
        case e_CounterBack:
        case e_CounterFront:
        case e_CounterRight:
        case e_CounterLeft:
        case e_CounterTop:
        case e_CounterBottom:
            return rotationsCount[e_CounterRotationCount] ;
    }
    assert(false) ;
}
float getRotationSpeed()
{
        switch (rotateStatus) {
        case e_None:
            return 0;
        default :
            return rotationSpeeds[rotateSpeed] ;
    }
    assert(false) ;
}
void rotateFace()
{
    for ( int rotationCount = 0 ; rotationCount <  getRotationCount() ; rotationCount++) {

        cube tmpCubes[3][3] , newCubes[3][3] ;
        for ( int i = 0 ; i < 3 ; i++ )
            for ( int j = 0 ; j < 3 ; j++ )
                for ( int k = 0 ; k < 6 ; k++ )
                    tmpCubes[i][j].sideColor[k] = cubes[getFaceIndex(i,j)].sideColor[k] ;

        for ( int i = 0 ; i < 3 ; i++ )
            for ( int j = 0 ; j < 3 ; j++ )
            {
                if ( rotateStatus == e_ClockLeft || rotateStatus == e_ClockRight
                    || rotateStatus == e_CounterLeft || rotateStatus == e_CounterRight )
                {
                    newCubes[i][j].sideColor[e_Top] = tmpCubes[3-j-1][i].sideColor[( rotateStatus == e_ClockLeft || rotateStatus == e_CounterLeft ? e_Back : e_Front)];
                    newCubes[i][j].sideColor[( rotateStatus == e_ClockLeft || rotateStatus == e_CounterLeft ? e_Back : e_Front)] = tmpCubes[3-j-1][i].sideColor[e_Bottom];
                    newCubes[i][j].sideColor[e_Bottom] = tmpCubes[3-j-1][i].sideColor[( rotateStatus == e_ClockLeft || rotateStatus == e_CounterLeft ? e_Front : e_Back)];
                    newCubes[i][j].sideColor[( rotateStatus == e_ClockLeft || rotateStatus == e_CounterLeft ? e_Front : e_Back)] = tmpCubes[3-j-1][i].sideColor[e_Top];
                    newCubes[i][j].sideColor[e_Left] = tmpCubes[3-j-1][i].sideColor[e_Left];
                    newCubes[i][j].sideColor[e_Right] = tmpCubes[3-j-1][i].sideColor[e_Right];
                }
                else if ( rotateStatus == e_ClockFront || rotateStatus == e_ClockBack
                    || rotateStatus == e_CounterBack || rotateStatus == e_CounterFront )
                {
                    newCubes[i][j].sideColor[e_Top] = tmpCubes[3-j-1][i].sideColor[( rotateStatus == e_ClockFront || rotateStatus == e_CounterFront ? e_Left : e_Right)];
                    newCubes[i][j].sideColor[( rotateStatus == e_ClockFront || rotateStatus == e_CounterFront ? e_Left : e_Right)] = tmpCubes[3-j-1][i].sideColor[e_Bottom];
                    newCubes[i][j].sideColor[e_Bottom] = tmpCubes[3-j-1][i].sideColor[( rotateStatus == e_ClockFront || rotateStatus == e_CounterFront ? e_Right : e_Left)];
                    newCubes[i][j].sideColor[( rotateStatus == e_ClockFront || rotateStatus == e_CounterFront ? e_Right : e_Left)] = tmpCubes[3-j-1][i].sideColor[e_Top];
                    newCubes[i][j].sideColor[e_Front] = tmpCubes[3-j-1][i].sideColor[e_Front];
                    newCubes[i][j].sideColor[e_Back] = tmpCubes[3-j-1][i].sideColor[e_Back];
                }
                else if ( rotateStatus == e_ClockTop || rotateStatus == e_ClockBottom
                    || rotateStatus == e_CounterTop || rotateStatus == e_CounterBottom )
                {
                    newCubes[i][j].sideColor[e_Back] = tmpCubes[3-j-1][i].sideColor[( rotateStatus == e_ClockTop || rotateStatus == e_CounterTop ? e_Left : e_Right)];
                    newCubes[i][j].sideColor[( rotateStatus == e_ClockTop || rotateStatus == e_CounterTop ? e_Left : e_Right)] = tmpCubes[3-j-1][i].sideColor[e_Front];
                    newCubes[i][j].sideColor[e_Front] = tmpCubes[3-j-1][i].sideColor[( rotateStatus == e_ClockTop || rotateStatus == e_CounterTop ? e_Right : e_Left)];
                    newCubes[i][j].sideColor[( rotateStatus == e_ClockTop || rotateStatus == e_CounterTop ? e_Right : e_Left)] = tmpCubes[3-j-1][i].sideColor[e_Back];
                    newCubes[i][j].sideColor[e_Top] = tmpCubes[3-j-1][i].sideColor[e_Top];
                    newCubes[i][j].sideColor[e_Bottom] = tmpCubes[3-j-1][i].sideColor[e_Bottom];
                }
            }

        for ( int i = 0 ; i < 3 ; i++ )
            for ( int j = 0 ; j < 3 ; j++ )
                for ( int k = 0 ; k < 6 ; k++ )
                    cubes[getFaceIndex(i,j)].sideColor[k] = newCubes[i][j].sideColor[k];
    }

}

float squaresVectors[6][4][3] = {
    { {-0.5,-0.5,0.51}, {-0.5,0.5,0.51}, {0.5,0.5,0.51}, {0.5,-0.5,0.51} } ,
    { {-0.5,-0.5,-0.51}, {-0.5,0.5,-0.51}, {0.5,0.5,-0.51}, {0.5,-0.5,-0.51} } ,
    { {-0.51,-0.5,-0.5}, {-0.51,-0.5,0.5}, {-0.51,0.5,0.5}, {-0.51,0.5,-0.5} } ,
    { {0.51,-0.5,-0.5}, {0.51,-0.5,0.5}, {0.51,0.5,0.5}, {0.51,0.5,-0.5} } ,
    { {-0.5,0.51,-0.5}, {-0.5,0.51,0.5}, {0.5,0.51,0.5}, {0.5,0.51,-0.5} } ,
    { {-0.5,-0.51,-0.5}, {-0.5,-0.51,0.5}, {0.5,-0.51,0.5}, {0.5,-0.51,-0.5} }
};
void drawSquare(int faceIndex)
{
    glBegin(GL_QUADS) ;
        glVertex3f(ATV(squaresVectors[faceIndex][0])) ;
        glVertex3f(ATV(squaresVectors[faceIndex][1])) ;
        glVertex3f(ATV(squaresVectors[faceIndex][2])) ;
        glVertex3f(ATV(squaresVectors[faceIndex][3])) ;
    glEnd() ;
}

void drawCube() {

    for ( int i = 0 ; i < 27 ; i++ )
    {
        glPushMatrix();
        glColor3f(0,0,0/*ATV(colors[i])*/);
        if ( isRotatedIndex(i) )
        {
            glTranslatef(1.1,1.1,1.1);
            glRotated(rotateAngel,ATV(rotateVector[getRotateVector()]));
            glTranslatef(-1.1,-1.1,-1.1);
        }
        glTranslatef(ATV(positions[i]));
        glutSolidCube  (1.0);
        if ( isFaceTop(i) )
        {
            glColor3f(ATV(colorsRGB[cubes[i].sideColor[e_Top]])) ;
            drawSquare(e_Top) ;
        }
        if ( isFaceBottom(i) )
        {
            glColor3f(ATV(colorsRGB[cubes[i].sideColor[e_Bottom]])) ;
            drawSquare(e_Bottom) ;
        }
        if ( isFaceFront(i) )
        {
            glColor3f(ATV(colorsRGB[cubes[i].sideColor[e_Front]])) ;
            drawSquare(e_Front) ;
        }
        if ( isFaceBack(i) )
        {
            glColor3f(ATV(colorsRGB[cubes[i].sideColor[e_Back]])) ;
            drawSquare(e_Back) ;
        }
        if ( isFaceRight(i) )
        {
            glColor3f(ATV(colorsRGB[cubes[i].sideColor[e_Right]])) ;
            drawSquare(e_Right) ;
        }
        if ( isFaceLeft(i) )
        {
            glColor3f(ATV(colorsRGB[cubes[i].sideColor[e_Left]])) ;
            drawSquare(e_Left) ;
        }
        glPopMatrix();
    }
    if ( rotateStatus )
        rotateAngel += getRotationSpeed() ;

}

void computePos(float deltaMove) {

	x += deltaMove * lx * 0.1f;
	z += deltaMove * lz * 0.1f;
}

void computeDir(float deltaAngle) {

	angle += deltaAngle;
	lx = sin(angle);
	lz = -cos(angle);
}

/// @@@@ yaaaaaa3
int idx = 0 ;
int n = 5 ;
vector<int> sol;
//int sol[5] = {e_ClockBottom,e_ClockLeft,e_CounterRight,e_ClockTop,e_ClockBottom} ;
int cnt = 0 ;
bool k = 0 ;

/// @@@@@@@

void renderScene(void) {

	if (deltaMove)
		computePos(deltaMove);
	if (deltaAngle)
		computeDir(deltaAngle);

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
    if ( rotateAngel < 0 ) rotateAngel += 360 ;
    /// @@@@@@@@@@@@@
    cnt++ ;
    if ( cnt > 10000 )
    {
        cnt = 100 ;
        //k = 1 ; //w7eee4
        if ( !k )
            rotateStatus = sol[idx++] ;
        k = 1 ;
    }
    /// @@@@@@@@@@@@@@
    if ( rotateAngel == 90 + getRotationSpeed() || rotateAngel == 270-getRotationSpeed() )
    {
        rotateFace() ;
        rotateAngel = 0 ;
        rotateStatus = e_None ;
        if ( idx == sol.size() ) ;
        else rotateStatus = sol[idx++] ;
    }
	glLoadIdentity();
	// Set the camera
	/*
	gluLookAt(	x, 5.0f, 10,
				x+lx, 0.0f,  z+lz,
				0.0f, 1.0f,  0.0f);
    */
    gluLookAt(	10, 10.0f, 10,
				0.38194, 0.0f,  0.951643,
				0.0f, 1.0f,  0.0f);

    //0.38194 0.951643

// Draw ground
    /*
	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
		glVertex3f(-100.0f, 0.0f, -100.0f);
		glVertex3f(-100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f, -100.0f);
	glEnd();
	*/

	for(int i = 0; i < 1; i++)
		for(int j=0; j < 1; j++) {
			glPushMatrix();
			glTranslatef(i*10.0,0,j * 10.0);
			drawCube();
			glPopMatrix();
		}

	glutSwapBuffers();
}


void pressKey(int key, int xx, int yy) {

	switch (key) {
		case GLUT_KEY_LEFT : deltaAngle = -0.01f; break;
		case GLUT_KEY_RIGHT : deltaAngle = 0.01f; break;
		case GLUT_KEY_UP : deltaMove = 0.5f; break;
		case GLUT_KEY_DOWN : deltaMove = -0.5f; break;
	}
}

void releaseKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_LEFT :
		case GLUT_KEY_RIGHT : deltaAngle = 0.0f;break;
		case GLUT_KEY_UP :
		case GLUT_KEY_DOWN : deltaMove = 0;break;
	}
}

void processRotateMenuEvents(int option)
{
    rotateStatus = option ;
}


void processMainMenuEvents(int option)
{
    // nothing
}

void createGLUTMenus()
{
    int clockWiseMenu = glutCreateMenu(processRotateMenuEvents) ;
    glutAddMenuEntry("Left",e_ClockLeft) ;
    glutAddMenuEntry("Right",e_ClockRight) ;
    glutAddMenuEntry("Top",e_ClockTop) ;
    glutAddMenuEntry("Bottom",e_ClockBottom) ;
    glutAddMenuEntry("Front",e_ClockFront) ;
    glutAddMenuEntry("Back",e_ClockBack) ;
    int counterClockWiseMenu = glutCreateMenu(processRotateMenuEvents) ;
    glutAddMenuEntry("Left",e_CounterLeft) ;
    glutAddMenuEntry("Right",e_CounterRight) ;
    glutAddMenuEntry("Top",e_CounterTop) ;
    glutAddMenuEntry("Bottom",e_CounterBottom) ;
    glutAddMenuEntry("Front",e_CounterFront) ;
    glutAddMenuEntry("Back",e_CounterBack) ;
    int mainMenu = glutCreateMenu(processMainMenuEvents) ;
    glutAddSubMenu("Clock",clockWiseMenu) ;
    glutAddSubMenu("Counter",counterClockWiseMenu) ;

    glutAttachMenu(GLUT_RIGHT_BUTTON) ;

}


void printSolution(short* path)
{
    const short *t = path;
    char face = '\0';
    while (*t != -1) {
        switch (*t) {
            case 0:
                sol.push_back(e_ClockTop);
                face = 'U';
                break;
            case 6:
                sol.push_back(e_CounterTop);
                face = 'U';
                break;
            case 12:
                sol.push_back(e_ClockTop);
                sol.push_back(e_ClockTop);
                face = 'U';
                break;
            case 1:
                sol.push_back(e_ClockFront);
                face = 'F';
                break;
            case 7:
                sol.push_back(e_CounterFront);
                face = 'F';
                break;
            case 13:
                sol.push_back(e_ClockFront);
                sol.push_back(e_ClockFront);
                face = 'F';
                break;
            case 2:
                sol.push_back(e_ClockBottom);
                face = 'D';
                break;
            case 8:
                sol.push_back(e_CounterBottom);
                face = 'D';
                break;
            case 14:
                sol.push_back(e_ClockBottom);
                sol.push_back(e_ClockBottom);
                face = 'D';
                break;
            case 3:
                sol.push_back(e_ClockBack);
                face = 'B';
                break;
            case 9:
                sol.push_back(e_CounterBack);
                face = 'B';
                break;
            case 15:
                sol.push_back(e_ClockBack);
                sol.push_back(e_ClockBack);
                face = 'B';
                break;
            case 4:
                sol.push_back(e_ClockRight);
                face = 'R';
                break;
            case 10:
                sol.push_back(e_CounterRight);
                face = 'R';
                break;
            case 16:
                sol.push_back(e_ClockRight);
                sol.push_back(e_ClockRight);
                face = 'R';
                break;
            case 5:
                sol.push_back(e_ClockLeft);
                face = 'L';
                break;
            case 11:
                sol.push_back(e_CounterLeft);
                face = 'L';
                break;
            case 17:
                sol.push_back(e_ClockLeft);
                sol.push_back(e_ClockLeft);
                face = 'L';
                break;
        }
        if (*t >= 12) {
            fprintf(stdout, "%c2 ", face);
        } else if (*t >= 6) {
            fprintf(stdout, "%c' ", face);
        } else {
            fprintf(stdout, "%c ", face);
        }
        ++t;
    }



}

void InitApp()
{
    InitPatternTables(CORNER_TABLE, EDGE1_TABLE, EDGE2_TABLE);
    LoadPatternTables(CORNER_TABLE, EDGE1_TABLE, EDGE2_TABLE);
}


string Solve()
{
    Cube c = Cube(cube);
	shared_ptr<Cubies> cbs = Cubies::Copy(c.toCubiesFromSides());

    for (int i = 0; i < 20; i++) cout << cbs->positions[i] << " \n"[i == 19];
    for (int i = 0; i < 20; i++) cout << cbs->orientations[i] << " \n"[i == 19];

    // 16 MOVES
    string movs[] = {"B", "F", "U", "B", "U", "B2", "D2", "L", "F2", "R2", "B", "F", "D'", "R'", "B'", "F"};

    // 5 MOVES
    //string movs[] = { "D'", "U'", "R", "L'", "D'" };
    // 14 MOVES
    //string movs[] = {"U'", "L'", "R'", "B", "F'", "D'", "U", "L", "R'", "U", "L2", "F2", "L2", "B2" };
    //shared_ptr<Cubies>
    cbs = Cubies::Copy(Cube::CubeFromMovesList(movs, 16));

	cbs->SetPatternTables(CORNER_TABLE, EDGE1_TABLE, EDGE2_TABLE);


	for (int i = 0; i < 20; i++) cout << cbs->positions[i] << " \n"[i == 19];
    for (int i = 0; i < 20; i++) cout << cbs->orientations[i] << " \n"[i == 19];

    cout << cbs->GetCornerHash() << endl;
	cout << cbs->GetEdge1Hash() << endl;
	cout << cbs->GetEdge2Hash() << endl;
	cout << (int)cbs->Heuristic() << endl;

	RubikSolver solver(cbs);

	solver.Solve();

	printSolution(solver.actionLog);

	delete[] CORNER_TABLE;
	delete[] EDGE1_TABLE;
	delete[] EDGE2_TABLE;

    cube c;
	return c.fromCubiesToCubeString(cbs);
}


int main(int argc, char **argv) {
    InitApp();

    //string cubestr = "goroyrorgwgyrrbrwbboobwrrgbgbogooyywrbwwgwwwoyggybybyy";
    //string cubestr = "wyooyrrgwgoborrywwoboywrbwgyoryowygyogbygggbrbbwrbwrbg";
    //"brbwrwgrgoboowoygwbygooobygrgrryrybwygwygwrbrybwybwogo";
    string cubestr = Solve();

    cout << endl;
    for (int i = 0;i < sol.size(); i++) cout << sol[i] << " \n"[i == sol.size()];
    //sol = vector<int>();

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(360,360);
	glutCreateWindow("Rubic Cube");

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	glutSpecialFunc(pressKey);

	glutIgnoreKeyRepeat(1);
	glutSpecialUpFunc(releaseKey);

    //std::string cubeString = "brbwrwgrgoboowoygwbygooobygrgrryrybwygwygwrbrybwybwogo";
    constructFaces(cubestr) ;

	// OpenGL init
	glEnable(GL_DEPTH_TEST);

    // menus
    createGLUTMenus() ;

	// enter GLUT event processing cycle
	glutMainLoop();

    //delete[] CORNER_TABLE;
	//delete[] EDGE1_TABLE;
	//delete[] EDGE2_TABLE;
	return 1;
}
