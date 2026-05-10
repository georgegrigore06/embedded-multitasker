#ifndef SNAKE_LOGIC_H
#define SNAKE_LOGIC_H
#include "oled.h"

#define MAX_PAGE 7
#define MAX_SEG 120 
#define SNAKE_LENGTH 4

typedef struct {
    uint8_t seg;  
    uint8_t page; 
} snake_coord_t;

typedef struct {
    snake_coord_t body[SNAKE_LENGTH];
    uint8_t head_idx;
    uint8_t tail_idx;
} SnakeGame_t;

typedef enum {
		LEFT,
		DOWN,
		RIGHT,
		UP,
		IDLE
} DIRECTION;

extern SnakeGame_t mySnake;

void moveSnake(SnakeGame_t *game, uint8_t current_direction);

#endif