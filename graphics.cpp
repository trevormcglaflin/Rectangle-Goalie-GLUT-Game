#include "circle.h"
#include "rect.h"
#include "graphics.h"
#include <ctime>
#include <iostream>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string>
using namespace std;



GLdouble width, height;
int wd, ballSize;
vector<Circle> balls;
Rect user;
point2D userCenter;
dimensions userDimensions;
int userHeight;
int userWidth;
bool ballMovesRight;
int points;
int level;
int addedPointsPerLevel;
int ballSpeed;
color ballColor;
screenLetter screen;
const color skyBlue(77/255.0, 213/255.0, 240/255.0);
const color grassGreen(26/255.0, 176/255.0, 56/255.0);
const color white(1, 1, 1);
const color brickRed(201/255.0, 20/255.0, 20/255.0);
const color darkBlue(1/255.0, 110/255.0, 214/255.0);
const color purple(119/255.0, 11/255.0, 224/255.0);
const color magenta(1, 0, 1);
const color orange(1, 163/255.0, 22/255.0);
const color cyan (0, 1, 1);
vector<color> colors;


// checks if a circle is on the screen
bool onScreen(Circle circle) {
    if (circle.getRightX() > 0 && circle.getLeftX() < width) {
        return true;
    }
    return false;
}

// checks if a circle has collided with a rectangle
bool circleHitRect(Circle &c, Rect &r) {
    if (c.getTopY() <= r.getBottomY() && c.getBottomY() >= r.getTopY() && c.getRightX() >= r.getLeftX()) {
        return true;
    }
    return false;
}


void init() {
    srand(time(0));
    width = 800;
    height = 800;
    ballSize = 20;

    // set initial user attributes
    userHeight = 400;
    userWidth = 30;
    userCenter.x = int(width - (userWidth/2));
    userCenter.y = int(height/2);
    userDimensions.width = userWidth;
    userDimensions.height = userHeight;
    user = Rect(cyan, userCenter, userDimensions);

    // make some balls at different heights
    for (int i = 0; i < 100; ++i) {
        balls.push_back(Circle(1, 0, 0, 1, -ballSize, (rand() % int(height - ballSize) + ballSize), ballSize));
    }

    // populate colors vector
    colors = { skyBlue, grassGreen, white, brickRed, darkBlue, purple, magenta, orange, cyan };

    // for game play
    ballMovesRight = true;
    points = 0;
    level = 1;
    addedPointsPerLevel = 5;
    ballSpeed = ballSize * (1 + (level * 0.2));
    screen = w;
}

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
}

/* Handler for window-repaint event. Call back when the window first appears and
 whenever the window needs to be re-painted. */
void display() {
    // Tell OpenGL to use the whole window for drawing
    glViewport(0, 0, width, height); // DO NOT CHANGE THIS LINE

    // Do an orthographic parallel projection with the coordinate
    // system set to first quadrant, limited by screen/window size
    glMatrixMode(GL_PROJECTION); // DO NOT CHANGE THIS LINE
    glLoadIdentity(); // DO NOT CHANGE THIS LINE
    glOrtho(0.0, width, height, 0.0, -1.f, 1.f); // DO NOT CHANGE THIS LINE

    // Clear the color buffer with current clearing color
    glClear(GL_COLOR_BUFFER_BIT); // DO NOT CHANGE THIS LINE

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // DO NOT CHANGE THIS LINE

    if (screen == w) {
        glColor3f(1, 1, 1);
        glRasterPos2i(200, 200);
        string message = "Welcome, hit 'p' to play rectangle goalie!!";
        for (const char &letter: message) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
        }
    }
    else if (screen == p) {
        user.draw();

        for (Circle &ball : balls) {
            ball.draw();
        }
    }
    else if (screen == b) {
        glColor3f(1, 1, 1);
        glRasterPos2i(200, 200);
        string message = "You passed level" + to_string(level) + "! Hit spacebar to go to start next level.";
        for (const char &letter: message) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
        }
    }
    else {
        glColor3f(1, 1, 1);
        glRasterPos2i(200, 200);
        string message = "Game over! You made it to level " + to_string(level) + "! Hit 'p' to play again!";
        for (const char &letter: message) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
        }
    }
    glFlush();  // Render now
}


void kbd(unsigned char key, int x, int y) {
    switch(key) {
        case 27: {
            // escape
            glutDestroyWindow(wd);
            exit(0);
        }

        // p: user can hit it whenever to start over
        case 112: {
            points = 0;
            level = 1;
            ballSpeed = ballSize * (1 + (level * 0.2));

            // reposition balls
            for (Circle &ball : balls) {
                if (onScreen(ball) || ball.getRightX() > width) {
                    ball.setCenter(-ballSize, rand() % int(height));
                }
            }
            // reposition user to center and reset size
            user.setCenter(userCenter);
            user.setHeight(userHeight);
            screen = p;
        }
    }

    // only allow spacebar to be hit spacebar if you are between turns
    if (screen == b && key == 32) {
        points = 0;
        level++;

        // update balls
        ballSpeed = ballSize * (1 + (level * 0.2));
        ballColor = colors[rand() % colors.size() - 1];
        for (Circle &ball : balls) {
            ball.setColor(ballColor);
            if (onScreen(ball) || ball.getRightX() > width) {
                ball.setCenter(-ballSize, rand() % int(height));
            }
        }

        // update user
        if (user.getHeight() > ballSize * 2) {
            user.changeHeight(-(userHeight/10));
        }
        user.setCenter(userCenter);
        user.setColor(colors[rand() % colors.size() - 1]);
        screen = p;
    }

    glutPostRedisplay();
}

void kbdUp(unsigned char key, int x, int y) {

    glutPostRedisplay();
}

void kbdS(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_DOWN:
            if (user.getBottomY() + 30 <= height) {
                user.move(0, 30);
            }
            else {
                user.move(0, height - user.getBottomY());
            }
            break;

        case GLUT_KEY_UP:
            if (user.getTopY() - 30 >= 0) {
                user.move(0, -30);
            }
            else {
                user.move(0, -(user.getTopY()));
            }
            break;
    }

    glutPostRedisplay();
}

void cursor(int x, int y) {

    glutPostRedisplay();
}

// button will be GLUT_LEFT_BUTTON or GLUT_RIGHT_BUTTON
// state will be GLUT_UP or GLUT_DOWN
void mouse(int button, int state, int x, int y) {
    // D3: What does this code do? What will it look like? Where will it be?
     string message = "You clicked the mouse at coordinate (" + to_string(x) + ", " + to_string(y) + ")";
     glColor3f(1, 1, 1);
     glRasterPos2i(0, height);
     for (char letter : message) {
         glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, letter);
     }


     glutPostRedisplay();
}

void timer(int dummy) {
    // only do this if screen is in playing mode
    if (screen == p) {
        // check if there is a ball already on screen, if so then move that one
        bool ballOnScreen = false;
        for (Circle &ball : balls) {
            if (onScreen(ball)) {
                // move it left if it has already collided with user
                if (ballMovesRight) {
                    ball.moveX(ballSpeed);
                } else {
                    ball.moveX(-ballSpeed);
                }

                // if it collides with user, change direction and add point
                if (circleHitRect(ball, user)) {
                    ballMovesRight = false;
                    points++;

                    // if points reach end of level change to between level screen
                    if (points >= (level * addedPointsPerLevel)) {
                        screen = b;
                    }
                }

                if (ball.getRightX() > width) {
                    ball.setCenter(-ballSize, rand() % int(height));
                    screen = e;
                }
                ballOnScreen = true;
            }
        }

        // if there is not a ball on the screen, move a ball in the balls vector at a random index
        if (!(ballOnScreen)) {
            ballMovesRight = true;
            balls[rand() % int(balls.size())].moveX(ballSize);
        }
    }

    glutPostRedisplay();
    glutTimerFunc(50, timer, dummy);
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {

    init();

    glutInit(&argc, argv);          // Initialize GLUT

    glutInitDisplayMode(GLUT_RGBA);

    glutInitWindowSize((int)width, (int)height);
    glutInitWindowPosition(0, 0); // Position the window's initial top-left corner
    /* create the window and store the handle to it */
    wd = glutCreateWindow("Rectangle Goalie!" /* title */ );

    // Register callback handler for window re-paint event
    glutDisplayFunc(display);

    // Our own OpenGL initialization
    initGL();

    // register keyboard press event processing function
    // works for numbers, letters, spacebar, etc.
    glutKeyboardFunc(kbd);
    glutKeyboardUpFunc(kbdUp);

    // register special event: function keys, arrows, etc.
    glutSpecialFunc(kbdS);

    // handles mouse movement
    glutPassiveMotionFunc(cursor);

    // handles mouse click
    glutMouseFunc(mouse);

    // handles timer
    glutTimerFunc(0, timer, 0);
    // Enter the event-processing loop
    glutMainLoop();
    return 0;
}