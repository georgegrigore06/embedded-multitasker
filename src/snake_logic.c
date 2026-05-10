#include "snake_logic.h"

SnakeGame_t mySnake = {
    .body = {
        {0, 0},    
        {1, 0},   
        {2, 0},  
        {3, 0}  
    },
    .head_idx = 3,
    .tail_idx = 0
};

void moveSnake(SnakeGame_t *game, uint8_t current_direction) 
{
    uint8_t next_seg = game->body[game->head_idx].seg;
    uint8_t next_page = game->body[game->head_idx].page;

    if (current_direction == UP) {
        next_page = (next_page == 0) ? MAX_PAGE : next_page - 1;
    }
    else if (current_direction == DOWN) {
        next_page = (next_page == MAX_PAGE) ? 0 : next_page + 1;
    }
    else if (current_direction == LEFT) {
        next_seg = (next_seg == 0) ? MAX_SEG : next_seg - 6; 
    }
    else if (current_direction == RIGHT) {
        next_seg = (next_seg >= MAX_SEG) ? 0 : next_seg + 6;
    }

    setPage(game->body[game->tail_idx].page);
    setSeg(game->body[game->tail_idx].seg);
    uint8_t empty_part[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    sendOLED(empty_part, 6, OLED_DATA);

    game->head_idx = (game->head_idx + 1) % SNAKE_LENGTH;
    game->tail_idx = (game->tail_idx + 1) % SNAKE_LENGTH;

    game->body[game->head_idx].seg = next_seg;
    game->body[game->head_idx].page = next_page;

    setPage(next_page);
    setSeg(next_seg);
    uint8_t solid_part[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    sendOLED(solid_part, 6, OLED_DATA);
}
