#ifndef __PROCESSES_H__
#define __PROCESSES_H__

void finish();

int hasCollided(int* x, int* y, collisionBox* solids[], int length, int* collidedWith);

int findScore(int* x, int* y, target* theTarget);

void fireArrow(double* angle, double* magnitude, int* horizontalAcceleration, double t, int xStart, int yStart, int* x, int* y);

int getMouseDrag(double* angle, double* magnitude, target* t, collisionBox* obstacle, int* wind, int* attemptNumber, int* score);

#endif // __PROCESSES_H__
