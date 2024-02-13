#ifndef _TETRIS_H_
#define _TETRIS_H_

#define TETRIS_CELL_WIDTH       12
#define TETRIS_CELL_HEIGHT      12
#define TETRIS_FIELD_WIDTH      10
#define TETRIS_FIELD_HEIGHT     19
#define TETRIS_SHAPE_COUNT      7
#define SEVEN_SEG_SIZE          0.8

void tetrisInit(void);
void tetrisStart(void);
void tetrisTitle(void);
void drawCell(int x, int y, uint8_t color);

#endif  /* _TETRIS_H_ */