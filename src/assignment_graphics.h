#ifndef __ASSIGNMENT_GRAPHICS_H__
#define __ASSIGNMENT_GRAPHICS_H__


/* For tryign to center text */
#define WIDTH_OF_CHARACTER 4
#define HEIGHT_OF_CHARACTER 5

/* Default stickman dimentions */
#define ARM_LENGTH 50
#define LEG_LENGTH 60
#define BODY_LENGTH 150
#define HEAD_RADIUS 50
#define STICKMAN_THICKNESS 2

#define OBSTACLE_COLOUR BLUE

/* Some constants for the target */
#define NUMBER_OF_TARGET_DIVISIONS 10
#define TARGET_THICKNESS 2
#define TARGET_LABEL_SHIFT 5

/* Button sizes */
#define MENU_BUTTON_WIDTH 300
#define MENU_BUTTON_HEIGHT 100
#define MENU_BUTTON_THICKNESS 1

/* For drawing the level */
#define STICKMAN_POSITION_X 100
#define STICKMAN_POSITION_Y 210
#define GROUND_LEVEL 420

/* For Drawing the Bow */
#define BOW_BEND 100
#define BOW_LENGTH 200
#define BOW_THICKNESS 2
#define BOW_MAGNITUDE_SCALE_FACTOR 0.1


/* A Function pointer for the button function.  May point to a void function with no arguments*/

typedef void (*VoidFunctionVoid)();



void button(int upperLeftX, int upperLeftY, int bottomRightX, int bottomRightY, int thickness, char text[], VoidFunctionVoid onClick);

void drawStickman(int xPos, int yPos, float scale, int color, double* angle, double* mag);

void drawTarget(int xPos, int yPos, int length);

void drawArrow(double* angle, int* tipXPos, int* tipYPos, int shaftLength);

void drawBow(double* angle, double* magnitude, int x, int y);





#endif /* __ASSIGNMENT_GRAPHICS_H__ */
