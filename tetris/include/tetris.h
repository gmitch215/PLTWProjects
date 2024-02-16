#ifndef _TETRIS_H_
#define _TETRIS_H_

#define CELL       12
#define FIELD_WIDTH      10
#define FIELD_HEIGHT     19
#define SHAPE_COUNT      7
#define SEVEN_SEG_SIZE          0.8

void tetrisInit(void);
void tetrisStart(void);
void tetrisTitle(void);
void drawCell(int x, int y, uint8_t color);

#endif  /* _TETRIS_H_ */