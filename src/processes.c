#include <graphics_lib.h>
#include <math.h>

#include "constants.h"
#include "assignment_graphics.h"
#include "allegro_wrappers.h"
#include "displays.h"
#include "processes.h"

/* Cleanup and stop */
void finish() {
    closegraph();
    closemouse();
    exit(0);
}


/* Checks whether *x and *y are withing any of the boxes defined in solids[].  If so returns 1 and populates *collidedWith with the name of the box it hit */
int hasCollided(int* x, int* y, collisionBox* solids[], int length, int* collidedWith) {

    /* If the given position is outside the window (Except vertically) then indicate the arrow missed everything and indicate that it hit something*/
    if((*x > WINDOW_WIDTH) || (*y > WINDOW_HEIGHT) || (*x < 0)) {
        *collidedWith = SOLID_MISS;
        return 1;
    }

    /* Cycle through everything in solids[] and check whether the given position is within any of them */
    int i;
    for(i = 0; i < length; i++) {
        if(solids[i] != 0) {
            int isWithinXBounds = ((*x > solids[i]->x1) && (*x < solids[i]->x2)) || ((*x < solids[i]->x1) && (*x > solids[i]->x2));
            int isWithinYBounds = ((*y > solids[i]->y1) && (*y < solids[i]->y2)) || ((*y < solids[i]->y1) && (*y > solids[i]->y2));

            if (isWithinXBounds && isWithinYBounds) {
                *collidedWith = solids[i]->type;
                return 1;
            }
        }
    }
    return 0;
}

/* Given the position of where the arrow hit the target, find the corresponding score */
int findScore(int* x, int* y, target* theTarget) {
    /* The number of points each target region equates to */
    int divisionValues[NUMBER_OF_TARGET_DIVISIONS] = {1, 3, 5, 7, 9, 9, 7, 5, 3, 1};
    /* The size of each region */
    int divSize = (theTarget->height)/NUMBER_OF_TARGET_DIVISIONS;

    /* Cycle through each region and check to see whether the give position is within it */
    int i;
    for (i=0; i < NUMBER_OF_TARGET_DIVISIONS; i++) {
        int currentRegionTop = (theTarget->topY) + (divSize * i);
        int currentRegionBottom = (theTarget->topY) + (divSize * (i+1));

        if((*y > currentRegionTop ) && (*y < currentRegionBottom)) {
            return divisionValues[i];
        }
    }
    return 0;
}


/* Works out where along its trajectory, the arrow should be for any given t */
void fireArrow(double* angle, double* magnitude, int* horizontalAcceleration, double t, int xStart, int yStart, int* x, int* y) {
    int currentX, currentY;
    double currentGradient, directionOfTravel;

    currentX = xStart + ((*magnitude*t)*cos(*angle) + (0.5)*(*horizontalAcceleration)*(t*t));/*Current x offset =  mtSin(theta) + 0.5at^2 */
    currentY = yStart - ((*magnitude*t)*sin(*angle) + (0.5)*ACCEL_GRAVITY*(t*t));           /*Cyrrent -y offset =  mtCos(theta) + 0.5(9.81)t^2 */
    currentGradient = (*magnitude* sin(*angle) + ACCEL_GRAVITY*t) / (*magnitude*cos(*angle) + (*horizontalAcceleration*t)); /*Current gradient =  mCos(theta) - 9.81t / mSin(theta) + at*/
    directionOfTravel = atan(currentGradient); /* Current angle of the gradient taken from the x-axis */

    /* Draw the arrow at its expected position and facing its expected direction */
    drawArrow(&directionOfTravel, &currentX, &currentY, SHAFT_LENGTH);
    *x = currentX;
    *y = currentY;
}




/* This function fills angle* and manitude* based on where the mouse is clicked and dragged from
 * Magnitude is taken from how far the mouse is dragged
*/
int getMouseDrag(double* angle, double* magnitude, target* t, collisionBox* obstacle, int* wind, int* attemptNumber, int* score) {
    int downX, downY,
        upX, upY;
    /* Double because of the division for atan later */
    double dx, dy;

    /* If the last event was a click of the mouse, store the coordinates of the click in downX, downY*/
    if(event_mouse_button_down_and_coords(&downX, &downY)) {
        /* Then wait until the mouse button is released and store those coords in upX, upY */
        do {
            wait_for_event();
            /* If the mouse position has changed then update the direction the bow is pointing and update the line to the original button press*/
            if(event_mouse_position_changed()) {
                get_mouse_coordinates();
                int x = XMOUSE,
                    y = YMOUSE;
                double bowAngle = 0;
                double bowMag = 0;

                dx = x - downX;
                dy = x - downY;
                /* Don't divide by 0... */
                if (dx != 0) {
                    /* Take the absolute value so the arrow can't be fired backwards */
                    bowAngle = atan(fabs(dy / dx));
                    bowMag = sqrt(fabs((dy*dy) + (dx*dx)));
                }
                /* Redraws the game with the bow facing the correct direction and the line to where the mouse was originally pressed down */
                cleardevice();
                drawGame(t, obstacle, wind, attemptNumber, score, &bowAngle, &bowMag);
                line(downX, downY, x, y, 1);
                update_display();
            }
        } while(!event_mouse_button_up_and_coords(&upX, &upY));

        dx = upX - downX;
        dy = upY - downY;

        /* Again, don't divide by 0 */
        if((dx != 0)) {
            *angle = atan(fabs(dy / dx));
            *magnitude = sqrt(fabs((dy*dy) + (dx*dx)));
        } else {
            *angle = 0;
            *magnitude = 0;
        }
        return 1;
    } else {
        return 0;
    }
}


