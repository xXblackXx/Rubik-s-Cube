#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <thread>

#include "PatternDatabase.h"
#include "Utility.h"
#include "Cubies.h"
#include "cube.h"
#include "RubikSolver.h"
#include "TableGenerator.h"
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define ATV(array) array[0], array[1], array[2] /* Arr to val */

using namespace cv ;

enum facesEnum { e_None, e_ClockUp, e_ClockFront, e_ClockDown, e_ClockBack, e_ClockRight, e_ClockLeft,
                 e_CounterUp, e_CounterFront, e_CounterDown, e_CounterBack, e_CounterRight, e_CounterLeft
               };
enum rotateVectorEnum { e_LeftClockRightCounter, e_LeftCounterRightClock, e_UpCounterDownClock,
                        e_UpClockDownCounter, e_FrontClockBackCounter, e_FrontCounterBackClock
                      };
enum rotateCountEnum { e_ClockRotationCount, e_CounterRotationCount };
enum rotateSpeedEnum { e_NormalSpeed, e_FastSpeed};
enum sides { e_Front, e_Back, e_Left, e_Right, e_Up, e_Down };
enum cubeColors { e_Red, e_Blue, e_Yellow, e_Green, e_White, e_Orange };
int rotateStatus = 0 ;
bool cubeLoaded = false ;

float colorsRGB[6][3] =
{
    { 1,0,0 }, { 0,0,1 }, { 1,1,0 }, { 0,1,0 }, { 1,1,1 }, { 1,0.5,0 }
};
float rotationSpeeds[2] = {1, 3 } ;
int rotateSpeed = 1;
int rotationsCount[2] = { 1, 3 } ;
time_t timer = 0 ;
bool runningAnimation = false ; // for visualization
double rotateAngel = 0 ;


void changeSize(int w, int h)
{

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


/* 0  - > 8 back
 * 9  - > 17 mid
 * 18 - > 26 front
 * i%3 == 0 , i - > left
 * i%3 == 2 , i - > right
 * i%9 < 3  , i - > Down
 * i%9 > 5  , i - > Up
*/
float positions[27][3] =
{
    /* first slide */
    { 0, 0, 0   }, { 1.1, 0, 0   }, { 2.2, 0, 0 },
    { 0, 1.1, 0   }, { 1.1, 1.1, 0   }, { 2.2, 1.1, 0 },
    { 0, 2.2, 0   }, { 1.1, 2.2, 0   }, { 2.2, 2.2, 0 },
    /* second slide */
    { 0, 0, 1.1 }, { 1.1, 0, 1.1 }, { 2.2, 0, 1.1 },
    { 0, 1.1, 1.1 }, { 1.1, 1.1, 1.1 }, { 2.2, 1.1, 1.1 },
    { 0, 2.2, 1.1 }, { 1.1, 2.2, 1.1 }, { 2.2, 2.2, 1.1 },
    /* third slide */
    { 0, 0, 2.2 }, { 1.1, 0, 2.2 }, { 2.2, 0, 2.2 },
    { 0, 1.1, 2.2 }, { 1.1, 1.1, 2.2 }, { 2.2, 1.1, 2.2 },
    { 0, 2.2, 2.2 }, { 1.1, 2.2, 2.2 }, { 2.2, 2.2, 2.2 },
};
struct cube
{
    int sideColor[6] ;
    cube()
    {
        for ( int i = 0 ; i < 6 ; i++ )
            sideColor[i] = -1 ;
    }
} cubes[27];
int getColorOfChar( char c )
{
    switch (c)
    {
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
#ifndef Up
    {
        for ( int j = 0 ; j < 3 ; j++ )
            for ( int k = 0 ; k < 3 ; k++ )
                cubes[6+j*9+k].sideColor[e_Up] = getColorOfChar(s[j*3+k]) ;
    }
#endif
#ifndef front
    {
        for ( int j = 0 ; j < 3 ; j++ )
            for ( int k = 0 ; k < 3 ; k++ )
                cubes[24-j*3+k].sideColor[e_Front] = getColorOfChar(s[j*3+k+9]) ;
    }
#endif
#ifndef Down
    {
        for ( int j = 0 ; j < 3 ; j++ )
            for ( int k = 0 ; k < 3 ; k++ )
                cubes[j*9+k].sideColor[e_Down] = getColorOfChar(s[24-j*3+k]) ;
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
float rotateVector[6][3] =
{
    {  1,  0,  0 },
    { -1,  0,  0 },
    {  0,  1,  0 },
    {  0, -1,  0 },
    {  0,  0, -1 },
    {  0,  0,  1 }
};
/*
    left - right
    glRotated(cnt,1,0,0);
    Up - Down
    glRotated(cnt,0,1,0);
    front - back
    glRotated(cnt,0,0,1);
*/
int faceUpEquation(int i, int j)
{
    return i*9+j+6 ;
}
bool isFaceUp(int i)
{
    return i%9 > 5  ;
}
int faceDownEquation(int i, int j)
{
    return i+j*9;
}

bool isFaceDown(int i)
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
    switch (rotateStatus)
    {
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
    case e_ClockUp:
    case e_CounterUp:
        return faceUpEquation(i,j) ;
    case e_ClockDown:
    case e_CounterDown:
        return faceDownEquation(i,j) ;
    }
    assert(false);
}
bool isRotatedIndex(int i)
{
    switch (rotateStatus)
    {
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
    case e_ClockUp:
    case e_CounterUp:
        return isFaceUp(i) ;
    case e_ClockDown:
    case e_CounterDown:
        return isFaceDown(i) ;
    }
    return false ;
}
int getRotateVector()
{
    switch (rotateStatus)
    {
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
    case e_ClockUp:
    case e_CounterDown:
        return e_UpClockDownCounter ;
    case e_CounterUp:
    case e_ClockDown:
        return e_UpCounterDownClock ;
    }
    assert(false) ;
}
int getRotationCount()
{
    switch (rotateStatus)
    {
    case e_ClockBack:
    case e_ClockFront:
    case e_ClockRight:
    case e_ClockLeft:
    case e_ClockUp:
    case e_ClockDown:
        return rotationsCount[e_ClockRotationCount] ;
    case e_CounterBack:
    case e_CounterFront:
    case e_CounterRight:
    case e_CounterLeft:
    case e_CounterUp:
    case e_CounterDown:
        return rotationsCount[e_CounterRotationCount] ;
    }
    assert(false) ;
}
float getRotationSpeed()
{
    switch (rotateStatus)
    {
    case e_None:
        return 0;
    default :
        return rotationSpeeds[rotateSpeed] ;
    }
    assert(false) ;
}
void rotateFace()
{
    for ( int rotationCount = 0 ; rotationCount <  getRotationCount() ; rotationCount++)
    {

        cube tmpCubes[3][3], newCubes[3][3] ;
        for ( int i = 0 ; i < 3 ; i++ )
            for ( int j = 0 ; j < 3 ; j++ )
                for ( int k = 0 ; k < 6 ; k++ )
                    tmpCubes[i][j].sideColor[k] = cubes[getFaceIndex(i,j)].sideColor[k] ;

        for ( int i = 0 ; i < 3 ; i++ )
            for ( int j = 0 ; j < 3 ; j++ )
            {
                if ( rotateStatus == e_ClockLeft
                        || rotateStatus == e_CounterLeft  )
                {
                    newCubes[i][j].sideColor[e_Up] = tmpCubes[3-j-1][i].sideColor[e_Back];
                    newCubes[i][j].sideColor[e_Back] = tmpCubes[3-j-1][i].sideColor[e_Down];
                    newCubes[i][j].sideColor[e_Down] = tmpCubes[3-j-1][i].sideColor[e_Front];
                    newCubes[i][j].sideColor[e_Front] = tmpCubes[3-j-1][i].sideColor[e_Up];
                    newCubes[i][j].sideColor[e_Left] = tmpCubes[3-j-1][i].sideColor[e_Left];
                    newCubes[i][j].sideColor[e_Right] = tmpCubes[3-j-1][i].sideColor[e_Right];
                }
                else if ( rotateStatus == e_ClockRight
                          || rotateStatus == e_CounterRight )
                {
                    newCubes[i][j].sideColor[e_Up] = tmpCubes[3-j-1][i].sideColor[e_Front];
                    newCubes[i][j].sideColor[e_Front] = tmpCubes[3-j-1][i].sideColor[e_Down];
                    newCubes[i][j].sideColor[e_Down] = tmpCubes[3-j-1][i].sideColor[e_Back];
                    newCubes[i][j].sideColor[e_Back] = tmpCubes[3-j-1][i].sideColor[e_Up];
                    newCubes[i][j].sideColor[e_Left] = tmpCubes[3-j-1][i].sideColor[e_Left];
                    newCubes[i][j].sideColor[e_Right] = tmpCubes[3-j-1][i].sideColor[e_Right];
                }
                else if ( rotateStatus == e_ClockFront
                          || rotateStatus == e_CounterFront )
                {
                    newCubes[i][j].sideColor[e_Up] = tmpCubes[3-j-1][i].sideColor[e_Left];
                    newCubes[i][j].sideColor[e_Left] = tmpCubes[3-j-1][i].sideColor[e_Down];
                    newCubes[i][j].sideColor[e_Down] = tmpCubes[3-j-1][i].sideColor[e_Right];
                    newCubes[i][j].sideColor[e_Right] = tmpCubes[3-j-1][i].sideColor[e_Up];
                    newCubes[i][j].sideColor[e_Front] = tmpCubes[3-j-1][i].sideColor[e_Front];
                    newCubes[i][j].sideColor[e_Back] = tmpCubes[3-j-1][i].sideColor[e_Back];
                }
                else if ( rotateStatus == e_ClockBack
                          || rotateStatus == e_CounterBack )
                {
                    newCubes[i][j].sideColor[e_Up] = tmpCubes[3-j-1][i].sideColor[e_Right];
                    newCubes[i][j].sideColor[e_Right] = tmpCubes[3-j-1][i].sideColor[e_Down];
                    newCubes[i][j].sideColor[e_Down] = tmpCubes[3-j-1][i].sideColor[e_Left];
                    newCubes[i][j].sideColor[e_Left] = tmpCubes[3-j-1][i].sideColor[e_Up];
                    newCubes[i][j].sideColor[e_Front] = tmpCubes[3-j-1][i].sideColor[e_Front];
                    newCubes[i][j].sideColor[e_Back] = tmpCubes[3-j-1][i].sideColor[e_Back];
                }
                else if ( rotateStatus == e_ClockUp
                          || rotateStatus == e_CounterUp )
                {
                    newCubes[i][j].sideColor[e_Back] = tmpCubes[3-j-1][i].sideColor[e_Left];
                    newCubes[i][j].sideColor[e_Left] = tmpCubes[3-j-1][i].sideColor[e_Front];
                    newCubes[i][j].sideColor[e_Front] = tmpCubes[3-j-1][i].sideColor[e_Right];
                    newCubes[i][j].sideColor[e_Right] = tmpCubes[3-j-1][i].sideColor[e_Back];
                    newCubes[i][j].sideColor[e_Up] = tmpCubes[3-j-1][i].sideColor[e_Up];
                    newCubes[i][j].sideColor[e_Down] = tmpCubes[3-j-1][i].sideColor[e_Down];
                }
                else if ( rotateStatus == e_ClockDown
                          || rotateStatus == e_CounterDown )
                {
                    newCubes[i][j].sideColor[e_Back] = tmpCubes[3-j-1][i].sideColor[e_Right];
                    newCubes[i][j].sideColor[e_Right] = tmpCubes[3-j-1][i].sideColor[e_Front];
                    newCubes[i][j].sideColor[e_Front] = tmpCubes[3-j-1][i].sideColor[e_Left];
                    newCubes[i][j].sideColor[e_Left] = tmpCubes[3-j-1][i].sideColor[e_Back];
                    newCubes[i][j].sideColor[e_Up] = tmpCubes[3-j-1][i].sideColor[e_Up];
                    newCubes[i][j].sideColor[e_Down] = tmpCubes[3-j-1][i].sideColor[e_Down];
                }
            }

        for ( int i = 0 ; i < 3 ; i++ )
            for ( int j = 0 ; j < 3 ; j++ )
                for ( int k = 0 ; k < 6 ; k++ )
                    cubes[getFaceIndex(i,j)].sideColor[k] = newCubes[i][j].sideColor[k];
    }

}

float squaresVectors[6][4][3] =
{
    { {-0.5,-0.5,0.51}, {-0.5,0.5,0.51}, {0.5,0.5,0.51}, {0.5,-0.5,0.51} },
    { {-0.5,-0.5,-0.51}, {-0.5,0.5,-0.51}, {0.5,0.5,-0.51}, {0.5,-0.5,-0.51} },
    { {-0.51,-0.5,-0.5}, {-0.51,-0.5,0.5}, {-0.51,0.5,0.5}, {-0.51,0.5,-0.5} },
    { {0.51,-0.5,-0.5}, {0.51,-0.5,0.5}, {0.51,0.5,0.5}, {0.51,0.5,-0.5} },
    { {-0.5,0.51,-0.5}, {-0.5,0.51,0.5}, {0.5,0.51,0.5}, {0.5,0.51,-0.5} },
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

void drawCube()
{

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
        if ( isFaceUp(i) )
        {
            glColor3f(ATV(colorsRGB[cubes[i].sideColor[e_Up]])) ;
            drawSquare(e_Up) ;
        }
        if ( isFaceDown(i) )
        {
            glColor3f(ATV(colorsRGB[cubes[i].sideColor[e_Down]])) ;
            drawSquare(e_Down) ;
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
    {
        rotateAngel += getRotationSpeed() ;
        timer = time(0) ;
    }

}



int currentStep = 0 ; // in solution steps
vector<int> sol; // solution step

void Solve(string tmp ) ;
void InitApp();

void renderScene(void)
{


    // Clear Color and Depth Buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    if ( rotateAngel < 0 ) rotateAngel += 360 ;
    if ( rotateAngel == 90 + getRotationSpeed() || rotateAngel == 270-getRotationSpeed() )
    {
        rotateFace() ;
        rotateAngel = 0 ;
        rotateStatus = e_None ;
        if ( runningAnimation )
        {
            if ( currentStep == sol.size() )
            {
                runningAnimation = false ;
                glutAttachMenu(GLUT_RIGHT_BUTTON);
            }
            else rotateStatus = sol[currentStep++] ;
        }
    }
    glLoadIdentity();
    // Set the camera

    gluLookAt(	10.0f, 10.0f, 10.0f,
                0.38194f, 0.0f,  0.951643f,
                0.0f, 1.0f,  0.0f);

    //0.38194 0.951643

    glPushMatrix();
    glTranslatef(0.0,0,0.0);
    drawCube();
    glPopMatrix();


    glutSwapBuffers();
}



string cubeStr ;
void replay()
{
    if ( runningAnimation || !cubeLoaded ) return ;
    currentStep = 0 ;
    timer = time(0) ;
    constructFaces(cubeStr) ;
}

void runNewCube() ;
void startAnimation()
{
    if ( runningAnimation || !cubeLoaded || currentStep == sol.size() ) return ;
    rotateStatus = sol[currentStep++] ;
    glutDetachMenu(GLUT_RIGHT_BUTTON);
    runningAnimation = true ;
}
void ExitApp();
void runCamera() ;
void pressNormalKey(unsigned char key, int xx, int yy)
{
    if ( runningAnimation ) return ;

    switch (key)
    {
    case 'r' :
    case 'R' :
        replay() ;
        break;
    case 'n' :
    case 'N' :
        runNewCube() ;
        break;
    case 's' :
    case 'S' :
        startAnimation() ;
        break;
    case 'e' :
    case 'E' :
        ExitApp() ;
        break;
    case 'c' :
    case 'C' :
        runCamera() ;
        break;
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
    glutAddMenuEntry("Up",e_ClockUp) ;
    glutAddMenuEntry("Down",e_ClockDown) ;
    glutAddMenuEntry("Front",e_ClockFront) ;
    glutAddMenuEntry("Back",e_ClockBack) ;
    int counterClockWiseMenu = glutCreateMenu(processRotateMenuEvents) ;
    glutAddMenuEntry("Left",e_CounterLeft) ;
    glutAddMenuEntry("Right",e_CounterRight) ;
    glutAddMenuEntry("Up",e_CounterUp) ;
    glutAddMenuEntry("Down",e_CounterDown) ;
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
    printf("Solution : ") ;
    while (*t != -1)
    {
        switch (*t)
        {
        case 0:
            sol.push_back(e_ClockUp);
            face = 'U';
            break;
        case 6:
            sol.push_back(e_CounterUp);
            face = 'U';
            break;
        case 12:
            sol.push_back(e_ClockUp);
            sol.push_back(e_ClockUp);
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
            sol.push_back(e_ClockDown);
            face = 'D';
            break;
        case 8:
            sol.push_back(e_CounterDown);
            face = 'D';
            break;
        case 14:
            sol.push_back(e_ClockDown);
            sol.push_back(e_ClockDown);
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
        if (*t >= 12)
        {
            fprintf(stdout, "%c2 ", face);
        }
        else if (*t >= 6)
        {
            fprintf(stdout, "%c' ", face);
        }
        else
        {
            fprintf(stdout, "%c ", face);
        }
        ++t;
    }
    puts("") ;
}

void InitApp()
{
    InitPatternTables(CORNER_TABLE, EDGE1_TABLE, EDGE2_TABLE);
    LoadPatternTables(CORNER_TABLE, EDGE1_TABLE, EDGE2_TABLE);
}
void ExitApp()
{
    delete[] CORNER_TABLE;
    delete[] EDGE1_TABLE;
    delete[] EDGE2_TABLE;
    exit(0) ;
}
string readCubeFromFile()
{
    FILE *input ;
    int stringSize = 54;
    char cubeChar[stringSize] ;
    if ( input = fopen("cube.txt", "r") )
    {
        fread(cubeChar,1,stringSize,input) ;
        fclose(input) ;
        return string(cubeChar) ;
    }
    return "goroyrorgwgyrrbrwbboobwrrgbgbogooyywrbwwgwwwoyggybybyy" ;
}

void Solve(string cubeStr)
{
    clock_t be = clock() ;
    Cube c = Cube(cubeStr);
    Cubies cbs = Cubies::Copy(c.toCubiesFromSides());


    cbs.SetPatternTables(CORNER_TABLE, EDGE1_TABLE, EDGE2_TABLE);


    RubikSolver solver(&cbs);

    solver.Solve();

    printSolution(solver.actionLog);
    clock_t en = clock() ;
    printf("Time in seconds : %.4f\n",(double)(en-be)/CLOCKS_PER_SEC) ;

}

void runNewCube()
{
    if ( runningAnimation ) return ;
    if ( !cubeLoaded ) cubeLoaded = true ;
    sol.clear() ;
    currentStep = 0 ;
    cubeStr = readCubeFromFile() ;
    constructFaces(cubeStr) ;
    Solve(cubeStr) ;
    timer = time(0) ;
}

Point RecsPoints[9][2] ;
Vec3i ColorsVecs[9] ;
void initRecsPoints( Mat imgOriginal)
{
    double midW = imgOriginal.cols /2 ;
    double midH = imgOriginal.rows /2;
    double len = 50 ;
    RecsPoints[0][0] = Point(midW-len-len/2-20,midH-len-len/2) ;
    RecsPoints[0][1] = Point(midW-len/2-25,midH-len/2-5);
    RecsPoints[1][0] = Point(midW-len/2,midH-len-len/2);
    RecsPoints[1][1] = Point(midW+len/2-5,midH-len/2-5);
    RecsPoints[2][0] = Point(midW+len/2+20,midH-len-len/2);
    RecsPoints[2][1] = Point(midW+len+len/2+15,midH-len/2-5);
    RecsPoints[3][0] = Point(midW-len-len/2-20,midH-len/2+20);
    RecsPoints[3][1] = Point(midW-len/2-25,midH+len/2+15);
    RecsPoints[4][0] = Point(midW-len/2,midH-len/2+20);
    RecsPoints[4][1] = Point(midW+len/2-5,midH+len/2+15);
    RecsPoints[5][0] = Point(midW+len/2+20,midH-len/2+20);
    RecsPoints[5][1] = Point(midW+len+len/2+15,midH+len/2+15);
    RecsPoints[6][0] = Point(midW-len-len/2-20,midH+len/2+40);
    RecsPoints[6][1] = Point(midW-len/2-25,midH+len+len/2+35);
    RecsPoints[7][0] = Point(midW-len/2,midH+len/2+40);
    RecsPoints[7][1] = Point(midW+len/2-5,midH+len+len/2+35);
    RecsPoints[8][0] = Point(midW+len/2+20,midH+len/2+40);
    RecsPoints[8][1] = Point(midW+len+len/2+15,midH+len+len/2+35);
}
void drawRectangle(Mat imgOriginal, Point p1, Point p2, int color = -1 )
{
    cv::rectangle(
        imgOriginal,
        p1,
        p2,
        ( color == -1 ? Scalar( 0, 255, 255 ) : ColorsVecs[color]),
        ( color == -1 ? 0 : -1));
}
void drawRectangles(Mat imgOriginal)
{
    for ( int i = 0 ; i < 9 ; i++ )
        drawRectangle(imgOriginal,RecsPoints[i][0],RecsPoints[i][1]) ;
}

int getColorFromVec(int index)
{
    int R = ColorsVecs[index][2] , G = ColorsVecs[index][1] , B = ColorsVecs[index][0] ;
    if ( B >= R && B >= G  && B-G <= 50 ) return e_White ;
    if ( B >= R && B >= G ) return e_Blue ;
    if ( abs(R-G) <= 20) return e_Yellow ;
    if ( G >= B && G >= R ) return e_Green ;
    if ( R >= B && B >= G ) return e_Red ;
    return e_Orange ;
}
string getColorName(int color)
{
    switch (color )
    {
    case e_White :
        return "White";
    case e_Yellow :
        return "Yellow";
    case e_Blue:
        return "Blue";
    case e_Green:
        return "Green";
    case e_Red:
        return "Red" ;
    default:
        return "Orange";
    }
}
void drawPredicted(Mat imgOriginal)
{
    for ( int i = 0 ; i < 9 ; i++ )
    {
        drawRectangle(imgOriginal,Point(RecsPoints[i][0].x-205,RecsPoints[i][0].y),Point(RecsPoints[i][1].x-205,RecsPoints[i][1].y),i) ;
        putText(imgOriginal, getColorName(getColorFromVec(i)), Point(RecsPoints[i][0].x-205,RecsPoints[i][0].y),
        FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);
    }
}
string getFaceStr()
{
    string str = "" ;
    for ( int i = 0 ; i < 3 ; i++ )
        for ( int j = 0 ; j < 3 ; j++ )
        str += tolower(getColorName(getColorFromVec(i*3+(2-j)))[0]) ;
    return str;
}
void readColors(Mat imgOriginal)
{
    for ( int i = 0 ; i < 9 ; i++ )
    {
        ColorsVecs[i] = {0,0,0};
        int cntPix = 0 ;
        for ( double j = RecsPoints[i][0].x ; j < RecsPoints[i][1].x ; j++ )
            for ( double k = RecsPoints[i][0].y ; k < RecsPoints[i][1].y ; k++ )
            {
                cntPix++ ;
                ColorsVecs[i] += imgOriginal.at<Vec3b>(Point(j,k)) ;
            }
        ColorsVecs[i] /= cntPix ;
    }
}
void runCamera()
{

    if ( runningAnimation ) return ;
    if ( !cubeLoaded ) cubeLoaded = true ;
    runningAnimation = true ;
    sol.clear() ;
    currentStep = 0 ;
    cubeStr = "" ;


    VideoCapture cap(0); //capture the video from webcam

    if ( !cap.isOpened() )  // if not success, exit program
    {
        cout << "Cannot open the web cam" << endl;
        return ;
    }


    //cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
    //cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);


    Mat imgOriginal;

    bool bSuccess = cap.read(imgOriginal); // read a new frame from video

    if (!bSuccess) //if not success, return
    {
        cout << "Cannot read a frame from video stream" << endl;
        return ;
    }
    initRecsPoints(imgOriginal) ;
    while (true)
    {
        bSuccess = cap.read(imgOriginal); // read a new frame from video

        if (!bSuccess) //if not success, break loop
        {
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }

        // Mat grayIMG;

        //cvtColor(imgOriginal, grayIMG, CV_BGR2GRAY); //Convert the captured frame from BGR to HSV

        //Mat cany ;
        //Canny(imgOriginal,cany,50,100) ;
        flip(imgOriginal,imgOriginal,1) ;
        drawRectangles(imgOriginal) ;
        Mat h ;
        cvtColor(imgOriginal,h,CV_RGB2HSV) ;
        readColors(imgOriginal) ;
        drawPredicted(imgOriginal) ;
        imshow("Original", imgOriginal); //show the original image
        int keyPressed = waitKey(30) ;
        if (keyPressed == 27) // 'esc' key press
        {
            cout << "esc key is pressed by user" << endl;
            break;
        }
        else if (keyPressed == 13) // 'enter' key press
        {
            cout << getFaceStr() << endl;
            cubeStr += getFaceStr() ;
        }
        if ( cubeStr.size() == 54 ) break;
    }
    destroyWindow("Original") ;
    cout <<cubeStr << endl;
    constructFaces(cubeStr) ;
    Solve(cubeStr) ;
    timer = time(0) ;
    runningAnimation = false;

    return ;
}
int main(int argc, char **argv)
{
    //thread t1(cam) ;
    //t1.join() ;
    //return 0 ;
    /*
    TableGenerator tg;
    FILE *output;
    output = fopen("table_edge2.newTable", "w");
    tg.EdgeTableGenerator(EDGE2_TABLE,false);
    int written;
    written = fwrite(EDGE2_TABLE, 1, 21288960, output); //21288960
    fclose(output);
    cout << written << endl;

    return 0;
    */
    InitApp();
    // runNewCube();
    // 16 moves new
    // boyoyrrggwwyrrgbygobobwbwrrryogoowwgrybrgyywgwgbwbbooy
    //16 moves
    //string cubeStr = "wyooyrrgwgoborrywwoboywrbwgyoryowygyogbygggbrbbwrbwrbg";
    //15 moves
    // owyrywoyyyobbrbbgorogywgwwyggwyorgorrbrrgrwyobgbwbogbw
    //14 moves
    //yrwoygogrywwbrbrrowgwowrrrrgwgooyyyygyoygwbbbobbobwggb
    // 13 moves
    //string cubeStr = "goroyrorgwgyrrbrwbboobwrrgbgbogooyywrbwwgwwwoyggybybyy";
    //5 moves
    //string cubeStr = "brbwrwgrgoboowoygwbygooobygrgrryrybwygwygwrbrybwybwogo";


    // init GLUT and create window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(400,400);
    glutCreateWindow("Rubic Cube");

    // register callbacks
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
    glutKeyboardFunc(pressNormalKey) ;

    glutIgnoreKeyRepeat(1);


    // OpenGL init
    glEnable(GL_DEPTH_TEST);

    // menus
    createGLUTMenus() ;

    // enter GLUT event processing cycle
    glutMainLoop();

    return 1;
}
