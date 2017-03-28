#include <graphics_lib.h>

#include "constants.h"
#include "assignment_graphics.h"
#include "processes.h"
#include "displays.h"



/* Draws the Menu Screen */
void menuScreen() {
    while(1) {
        wait_for_event(); /* Post-testing, this line was moved here from line 44 */
        cleardevice();

        /* Title */
        char* titleText = "Stickman Archery!";
        int xPosition = (WINDOW_WIDTH/2) - (19*WIDTH_OF_CHARACTER);
        outtextxy(xPosition, WINDOW_HEIGHT/5, titleText);

        /* The Play button */
        button((WINDOW_WIDTH/2)-(MENU_BUTTON_WIDTH/2),
               (WINDOW_HEIGHT/3),
               (WINDOW_WIDTH/2)+(MENU_BUTTON_WIDTH/2),
               (WINDOW_HEIGHT/3) + MENU_BUTTON_HEIGHT,
               MENU_BUTTON_THICKNESS,
               "Play",
               selectDifficulty);

        /* The Exit Button */
        button((WINDOW_WIDTH/2)-(MENU_BUTTON_WIDTH/2),
               (2*WINDOW_HEIGHT/3),
               (WINDOW_WIDTH/2)+(MENU_BUTTON_WIDTH/2),
               (2*WINDOW_HEIGHT/3) + MENU_BUTTON_HEIGHT,
               MENU_BUTTON_THICKNESS,
               "Exit",
               finish);

        update_display();
        /* Check whether the user tried to close the display */
        if(event_close_display()) {
            finish();
        }

    }
}



/* Draws the basic layout of the game: The stickman & target */
void drawGame(target* t, collisionBox* obstacle, int* wind, int* attemptNumber, int* score, double* bowAngle, double* bowMag) {
    setbkcolor(BK_COLOUR);
    /* Draw the ground */
    setcolor(GREEN);
    filled_rectangle(0, GROUND_LEVEL, WINDOW_WIDTH, WINDOW_HEIGHT, GREEN);

    /*Draw stickman*/
    drawStickman(STICKMAN_POSITION_X, STICKMAN_POSITION_Y, 1, WHITE, bowAngle, bowMag);

    /* Draw the target */
    drawTarget(t->topX, t->topY, t->height);

    /* If we need to draw an obstacle, then do so */
    if(obstacle->type != 0) {
        setcolor(OBSTACLE_COLOUR);
        filled_rectangle(obstacle->x1, obstacle->y1, obstacle->x2, obstacle->y2, OBSTACLE_COLOUR);
    }

    /* Number of attempts text */
    setcolor(WHITE);
    char attemptNumberStr[15];
    sprintf(attemptNumberStr, "Attempt %d of %d", *attemptNumber, MAX_ATTEMPTS);
    outtextxy(WIDTH_OF_CHARACTER, HEIGHT_OF_CHARACTER, attemptNumberStr);

    /* Score text */
    char scoreStr[10];
    sprintf(scoreStr, "Score: %d", *score);
    outtextxy(WINDOW_WIDTH/2, HEIGHT_OF_CHARACTER, scoreStr);

    /* Wind speed text */
    char windStr[9];
    sprintf(windStr, "Wind: %d", *wind);
    outtextxy(9*WINDOW_WIDTH/10, HEIGHT_OF_CHARACTER, windStr);

    /* Then draw the wind direction arrow */
    double angle;
    int x = 9*WINDOW_WIDTH/10,
        y = HEIGHT_OF_CHARACTER *5; /* The position of the wind speed arrow */
    if(*wind > 0) {
        angle = 0;
    } else {
        angle = PI;
    }
    /* Draw the arrow, make its length equal to the wind strength */
    drawArrow(&angle, &x, &y, abs(*wind));

    /* Draw the back to menu button */
    button(WIDTH_OF_CHARACTER,
           HEIGHT_OF_CHARACTER*5,
           WIDTH_OF_CHARACTER+75,
           HEIGHT_OF_CHARACTER+50,
           1,
           "Menu",
           menuScreen);
}





/* Draws the 'select difficulty' screen */
void selectDifficulty() {
    while(1) {
        wait_for_event(); /* Post testing, this call was moved here from line 163 */
        cleardevice();
        /* Display the text "Select Difficulty" in the middle of the screen */
        char titleText[19] = "Select Difficulty:";
        int xPosition = (WINDOW_WIDTH/2) - (19*WIDTH_OF_CHARACTER);
        outtextxy(xPosition, WINDOW_HEIGHT/5, titleText);

        /* Difficulty 1 button*/
        button((WINDOW_WIDTH/4) - LEVEL_SELECT_BUTTON_WIDTH,
               (WINDOW_HEIGHT/2) - LEVEL_SELECT_BUTTON_HEIGHT,
               (WINDOW_WIDTH/4) + LEVEL_SELECT_BUTTON_WIDTH,
               (WINDOW_HEIGHT/2) + LEVEL_SELECT_BUTTON_HEIGHT,
               MENU_BUTTON_THICKNESS,
               "1",
               difficulty1);

        /* Difficulty 2 button*/
        button((2*WINDOW_WIDTH/4) - LEVEL_SELECT_BUTTON_WIDTH,
               (WINDOW_HEIGHT/2) - LEVEL_SELECT_BUTTON_HEIGHT,
               (2*WINDOW_WIDTH/4) + LEVEL_SELECT_BUTTON_WIDTH,
               (WINDOW_HEIGHT/2) + LEVEL_SELECT_BUTTON_HEIGHT,
               MENU_BUTTON_THICKNESS,
               "2",
               difficulty2);

        /* Difficulty 3 button*/
        button((3*WINDOW_WIDTH/4) - LEVEL_SELECT_BUTTON_WIDTH,
               (WINDOW_HEIGHT/2) - LEVEL_SELECT_BUTTON_HEIGHT,
               (3*WINDOW_WIDTH/4) + LEVEL_SELECT_BUTTON_WIDTH,
               (WINDOW_HEIGHT/2) + LEVEL_SELECT_BUTTON_HEIGHT,
               MENU_BUTTON_THICKNESS,
               "3",
               difficulty3);

        /* Back to menu button*/
        button((5*WINDOW_WIDTH/6) - BACK_TO_MENU_BUTTON_WIDTH_HEIGHT,
               (5*WINDOW_HEIGHT/6) - BACK_TO_MENU_BUTTON_WIDTH_HEIGHT,
               (5*WINDOW_WIDTH/6) + BACK_TO_MENU_BUTTON_WIDTH_HEIGHT,
               (5*WINDOW_HEIGHT/6) + BACK_TO_MENU_BUTTON_WIDTH_HEIGHT,
               MENU_BUTTON_THICKNESS,
               "Menu",
               menuScreen);

        update_display();

        /* Has the user tried to close the window? */
        if(event_close_display()) {
            finish();
        }

    }
}


/* Plays the game with difficulty set to 1*/
void difficulty1() {
    playGame(1);
}
/* Plays the game with difficulty set to 2*/
void difficulty2() {
    playGame(2);
}
/* Plays the game with difficulty set to 3*/
void difficulty3() {
    playGame(3);
}




void playGame(int difficulty) {

    collisionBox** solids = malloc(MAX_OBSTACLES * sizeof(collisionBox*));    /* This array will contain 'box' structs which contain the coordinates of anything the arrow can hit */
    int attempt = 1;                                        /* How many attempts has the user had? */
    int score = 0;                                          /* Current Cumultive score */
    double angle = -PI/4,
           mag = 0;                                         /* Current BOW angle and magnitude of pullback */
    int collidedWith;                                       /* What the arrow hit (See SOLIDS_xxx in displays.h) */
    int wind = 0;                                           /* The wind for this round */
    int obstacleBuilt = 0;                                  /* Defines whether the obstacle has been generated for this round yet */
    int windGenerated = 0;                                  /* Defines whether the wind has been generated for this round yet */

    /* Set the size and position of the target for this round */
    target  theTarget;
    theTarget.topX = TARGET_DEFAULT_Y;
    theTarget.topY = 200;
    theTarget.height = TARGET_DEFAULT_SIZE;

    /* Sets the coordinates of the target's collision box*/
    collisionBox targetBox;
    targetBox.x1 = theTarget.topX;
    targetBox.y1 = theTarget.topY;
    targetBox.x2 = WINDOW_WIDTH;
    targetBox.y2 = theTarget.topY + theTarget.height;
    targetBox.type = SOLID_TARGET;
    solids[0] = &targetBox;


    /* While the user hasn't had all 3 of their attempts */
    while(attempt <= MAX_ATTEMPTS) {
        cleardevice();

        int currentX = 0, currentY = 0;     /* The current position of the arrow */

        /* If the difficulty level is 3, put an obstacle in the path */
        collisionBox obstacle;
        if(difficulty == 3) {
            if(!obstacleBuilt) {
                /* makes a collision box for the obstacle and puts it in the list of solid objects */
                obstacle.x1 = rand_number(WINDOW_WIDTH/4, 3*WINDOW_WIDTH/4);
                obstacle.y1 = rand_number(0, WINDOW_HEIGHT);
                obstacle.x2 = rand_number(WINDOW_WIDTH/4, 3*WINDOW_WIDTH/4);
                obstacle.y2 = rand_number(0, WINDOW_HEIGHT);
                obstacle.type = SOLID_OBSTACLE;
                solids[1] = &obstacle;
                obstacleBuilt = 1;
            }
        } else {
            /* If were not going to generate an object, set type to 0 so drawGame knows not to try to draw it*/
            obstacle.type = 0;
            solids[1] = 0;/* NULL */
        }
        if(difficulty > 1 && !windGenerated) {
            /* Generate the wind. */
            wind = rand_number(MIN_WIND, MAX_WIND);
            windGenerated = 1;
        }
        /* Draw the level */
        drawGame(&theTarget, &obstacle, &wind, &attempt, &score, &angle, &mag);

        /* while keeps looking until the mouse gets dragged */
        if(getMouseDrag(&angle, &mag, &theTarget, &obstacle, &wind, &attempt, &score)) {
            if((angle != 0) && (mag != 0)) {
                double i = 0; /* The current 'time' - used to simulate the arrow trajectory */

                /* While the arrow hasn;t hit anything */
                while(!hasCollided(&currentX, &currentY, solids, MAX_OBSTACLES, &collidedWith)) {
                    cleardevice();
                    /* Workout arrow's trajectory and draw it */
                    fireArrow(&angle,
                              &mag,
                              &wind,
                              i,
                              STICKMAN_POSITION_X + ARM_LENGTH,
                              STICKMAN_POSITION_Y + (BODY_LENGTH/2)+LEG_LENGTH/2,
                              &currentX,
                              &currentY);
                    /* Draw the level */
                    drawGame(&theTarget, &obstacle, &wind, &attempt, &score, &angle, &mag);

                    /* Wait so that the user has time to see it */
                    pausefor(TICK_TIME_MILLIS);
                    update_display();

                    /* Check to see if the user tried to exit */
                    if(event_close_display()) {
                        finish();
                    }
                    /* Increase the time variable by 1/10*TICK_TIME so that it looks more natural */
                    i += (0.1/TICK_TIME_MILLIS);
                }


                /* If were here then the arrow has hit something */
                if(collidedWith == SOLID_TARGET) {
                    /* Arrow hit the target, now find out what score it hit.*/
                    score += findScore(&currentX, &currentY, &theTarget);
                } else {
                    /* It hit an obstacle or missed*/

                }

                /* Set variables so the level will be generated differently */
                attempt++;
                obstacleBuilt = 0;
                windGenerated = 0;
            }
        }
        update_display();
        wait_for_event();
        if(event_close_display()) {
            finish();
        }
    }/* While (attempts <= 3) */

    /* Release the memory allocated for the solids */
    free(solids);

    /* Display the player's final score */
    displayTotalScore(&score);

}

/* Displays the overall score screen */
void displayTotalScore(int* score) {
    /* build the result string */
    char scoreStr[16];
    sprintf(scoreStr, "Total Score: %d", *score);

    /* The only way to exit is by clicking the button */
    while (1) {
        wait_for_event(); /* Post testing, this line was moved here from line 344 */
        cleardevice();
        /* Comment on how well they did */
        if(*score == 27){
            outtextxy(WINDOW_WIDTH/2 - (WIDTH_OF_CHARACTER * strlen(scoreStr)), WINDOW_HEIGHT/3, "Perfect!");
        }else if(*score == 0){
            outtextxy(WINDOW_WIDTH/2 - (WIDTH_OF_CHARACTER * strlen(scoreStr)), WINDOW_HEIGHT/3, "Better luck next time!");
        }else{
            outtextxy(WINDOW_WIDTH/2 - (WIDTH_OF_CHARACTER * strlen(scoreStr)), WINDOW_HEIGHT/3, "Well Done!");
        }

        /* Write the score */
        outtextxy(WINDOW_WIDTH/2 - (WIDTH_OF_CHARACTER * strlen(scoreStr)), WINDOW_HEIGHT/2, scoreStr);

        /* Draw the back to menu button */
        button(WINDOW_WIDTH/3,
               2*WINDOW_HEIGHT/3,
               2*WINDOW_WIDTH/3,
               (2*WINDOW_HEIGHT/3) + MENU_BUTTON_HEIGHT,
               MENU_BUTTON_THICKNESS,
               "Back to menu",
               menuScreen);

        update_display();

        /* Check whether the user closed the display */
        if(event_close_display()){
            finish();
        }

    }

}
