#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#define WINDOW_WIDTH 1260
#define WINDOW_HEIGHT 480
#define TICK_TIME_MILLIS 10


#define SOLID_TARGET 1
#define SOLID_OBSTACLE 2
#define SOLID_MISS 3

#define MAX_ATTEMPTS 3

#define ACCEL_GRAVITY -9.81*10

#define SHAFT_LENGTH 100
#define SHAFT_THICKNESS 2
#define BARB_LENGTH 15
#define BARB_ANGLE 0.5


typedef struct {
    int topX,
        topY,
        height;
} target;

typedef struct {
    int x1,
        y1,
        x2,
        y2;
    int type;
} collisionBox;


#endif /* __CONSTANTS_H__ */
