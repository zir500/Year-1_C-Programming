#ifndef __DISPLAYS__
#define __DISPLAYS__

#define LEVEL_SELECT_BUTTON_WIDTH 100
#define LEVEL_SELECT_BUTTON_HEIGHT 100
#define BACK_TO_MENU_BUTTON_WIDTH_HEIGHT 50

#define MAX_OBSTACLES 2

#define TARGET_DEFAULT_X 200
#define TARGET_DEFAULT_Y 9*WINDOW_WIDTH/10
#define TARGET_DEFAULT_SIZE 200

#define MAX_WIND 100
#define MIN_WIND -100

#define BK_COLOUR LIGHTBLUE

void drawGame(target* t, collisionBox* obstacle, int* wind, int* attemptNumber, int* score, double* angle, double* mag);


void selectDifficulty();

void difficulty1();
void difficulty2();
void difficulty3();
void difficulty4();

void menuScreen();

void playGame(int difficulty);

void displayTotalScore(int* score);

#endif // __DISPLAYS__
