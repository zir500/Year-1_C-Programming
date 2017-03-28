#include <math.h>
#include <graphics_lib.h>

#include "constants.h"
#include "displays.h"
#include "assignment_graphics.h"



/* Draws a rectangular button, tries to center the text within it and fires a function when clicked. */
void button(int upperLeftX, int upperLeftY, int bottomRightX, int bottomRightY, int thickness, char text[], VoidFunctionVoid onClick) {
    int mouse_x, mouse_y;                   /* Will store the current mouse position. */
    int isWithinButtonX, isWithinButtonY;

    get_mouse_coordinates();
    mouse_x = XMOUSE;
    mouse_y = YMOUSE;

    /* Is the mouse within the dimentions os this button? */
    isWithinButtonX = (mouse_x > upperLeftX) && (mouse_x < bottomRightX);
    isWithinButtonY = (mouse_y > upperLeftY) && (mouse_y < bottomRightY);

    if(isWithinButtonX && isWithinButtonY) {
        /* If mouse is inside, make the box bold */
        thickness *= 2;
        if(event_mouse_button_down()) {
            /* Run the function  */
            onClick();
        }
    }

    /* Draw the box */
    rectangle(upperLeftX, upperLeftY, bottomRightX, bottomRightY, thickness);

    /* Positions the text such that it is centered by shifting it left depending on the number of characters*/
    outtextxy((int)(upperLeftX + (bottomRightX - upperLeftX)/2) - strlen(text)*WIDTH_OF_CHARACTER,
              (int)(upperLeftY + (bottomRightY - upperLeftY)/2 - HEIGHT_OF_CHARACTER),
              text);
}



/* Puts a stickman in the display buffer
   Positions given are at center of head */

void drawStickman(int xPos, int yPos, float scale, int color, double* angle, double* mag) {
    setcolor(color);
    /*Head*/
    circle(xPos, yPos, HEAD_RADIUS, 2);

    /*Body*/
    line(xPos,
         yPos+HEAD_RADIUS,
         xPos,
         yPos+BODY_LENGTH,
         STICKMAN_THICKNESS);

    /*Left arm */
    line(xPos,
         yPos+(BODY_LENGTH/2),
         xPos-ARM_LENGTH,
         yPos+(BODY_LENGTH/2)+ARM_LENGTH/2,
         STICKMAN_THICKNESS);

    /*Right arm*/
    line(xPos,
         yPos+(BODY_LENGTH/2),
         xPos+ARM_LENGTH,
         yPos+(BODY_LENGTH/2)+ARM_LENGTH/2,
         STICKMAN_THICKNESS);

    /*Right Leg*/
    line(xPos,
         yPos+BODY_LENGTH,
         xPos+LEG_LENGTH/2,
         yPos+BODY_LENGTH+LEG_LENGTH,
         STICKMAN_THICKNESS);

    /*Left Leg*/
    line(xPos,
         yPos+BODY_LENGTH,
         xPos-LEG_LENGTH/2,
         yPos+BODY_LENGTH+LEG_LENGTH,
         STICKMAN_THICKNESS);

    drawBow(angle, mag, xPos+ARM_LENGTH, yPos+(BODY_LENGTH/2)+ARM_LENGTH/2);
}


/* Draws the target, positions given set the top of the target*/
void drawTarget(int xPos, int yPos, int length) {
    /* The Colours of each region of the target, in the order they appear. */
    int targetColours[NUMBER_OF_TARGET_DIVISIONS] = {WHITE, DARKGRAY, BLUE, RED, YELLOW, YELLOW, RED, BLUE, DARKGRAY, WHITE};
    /* The labels to be put next to each region of the target */
    char divisionValues[NUMBER_OF_TARGET_DIVISIONS][2] = {"1", "3", "5", "7", "9", "9", "7", "5", "3", "1"};
    /* How many pixels tall should each region be? */
    int divSize = length/NUMBER_OF_TARGET_DIVISIONS;

    int i;
    /* Loops through each region, draws the line making the target and adds the label next to it. */
    for (i=0; i < NUMBER_OF_TARGET_DIVISIONS; i++) {
        setcolor(targetColours[i]);
        line(xPos, yPos + (divSize*i), xPos,  yPos + divSize + (divSize*i), TARGET_THICKNESS);
        outtextxy(xPos + TARGET_LABEL_SHIFT,
                  (yPos + (divSize*i)+ (divSize/2)- HEIGHT_OF_CHARACTER),
                  divisionValues[i]);
    }
}

/* Draws the arrow in a given position facing a given direction */
void drawArrow(double* angle, int* tipXPos, int* tipYPos, int shaftLength) {
    int arrowTailX, arrowTailY; /* Position of the end of the shaft */
    int barbEndX, barbEndY;     /* Position of the end of the barbs at the tip of the arrow */

    /* Workout where the arrow shaft finishes */
    arrowTailY = *tipYPos + (shaftLength * sin(*angle));
    arrowTailX = *tipXPos - (shaftLength * cos(*angle));

    line(*tipXPos, *tipYPos, arrowTailX, arrowTailY, SHAFT_THICKNESS);          /* The Shaft*/

    /* Now we draw the barbs at the tip of the arrow,  first calculate the xy positions of where the tips end then draw them*/
    barbEndY = *tipYPos + (BARB_LENGTH * sin(*angle + BARB_ANGLE));
    barbEndX = *tipXPos - (BARB_LENGTH * cos(*angle + BARB_ANGLE));
    line(*tipXPos, *tipYPos, barbEndX, barbEndY, SHAFT_THICKNESS/2);

    /* Now the barb in the other direction, angle is reversed */
    barbEndY = *tipYPos + (BARB_LENGTH * sin(*angle - BARB_ANGLE));
    barbEndX = *tipXPos - (BARB_LENGTH * cos(*angle - BARB_ANGLE));
    line(*tipXPos, *tipYPos, barbEndX, barbEndY, SHAFT_THICKNESS/2);
}


/* Draws the bow at a given position, pointing in a given direction */
void drawBow(double* angle, double* magnitude, int x, int y) {
    /* Workout the angles required to use arc() later */
    double angleStart = *angle + atan(BOW_LENGTH/2*BOW_BEND),
           angleEnd = angleStart + 2*atan(BOW_LENGTH/2*BOW_BEND);

    /* Draw the arc of the bow (Note y direction is down so -angleStart.  Also argument 3 of arc() is the change in angle not the end angle) */
    arc(x, y, BOW_BEND, -angleStart*180/PI, PI * 180/PI, BOW_THICKNESS);

    /* Work out the position of the tip of the bow arc */
    double dy = (BOW_LENGTH/2) * sin(angleStart - PI),
           dx = (BOW_LENGTH/2) * cos(angleEnd - PI);

    /* Work out the how far back the string should be drawn for the given magnitude of the mouse drag */
    double dy2 = (*magnitude*BOW_MAGNITUDE_SCALE_FACTOR ) * sin((*angle)),
           dx2 = (*magnitude*BOW_MAGNITUDE_SCALE_FACTOR ) * cos((*angle));

    /* Draw the string from the tips of the bow to the end of the arrow */
    line(x - dx,
         y - dy,
         x - dx2,
         y + dy2,
         BOW_THICKNESS);

    line(x + dx,
         y + dy,
         x - dx2,
         y + dy2,
         BOW_THICKNESS);
}


