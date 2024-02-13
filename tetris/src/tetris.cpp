#include "vex.h"
#include "buttons.h"
#include "tetris.h"

using namespace vex;

extern vex::brain Brain;

#define SEVEN_SEG_HEIGHT            SEVEN_SEG_SIZE*3
#define SEVEN_SEG_WIDTH             SEVEN_SEG_HEIGHT*3
#define SEVEN_SEG_CHAR_WIDTH        ((SEVEN_SEG_SIZE*4)+(SEVEN_SEG_HEIGHT*2)+SEVEN_SEG_WIDTH)
#define SEVEN_SEG_CHAR_HEIGHT       ((SEVEN_SEG_SIZE*4)+(SEVEN_SEG_HEIGHT*3)+(SEVEN_SEG_WIDTH*2))
#define TETRIS_SEVEN_SEG_SCORE_X    (TETRIS_FIELD_WIDTH*TETRIS_CELL_WIDTH) + TETRIS_FIELD_HEIGHT-2 + (SEVEN_SEG_CHAR_WIDTH*i)


const uint8_t sevenSegNumbers[10] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F }; 
const vex::color tetrisShapeColors[9] = { 
    vex::color(0x000000), 
    vex::color(0xFF00FF), 
    vex::color(0xFFFF00), 
    vex::color(0xFF0000), 
    vex::color(0x00FF00),
    vex::color(0xFF8000), 
    vex::color(0x0000FF), 
    vex::color(0x00FFFF), 
    vex::color(0xFFFFFF) 
}; 
const vex::color lineColor( 0x00FF00 );

const int tetrisShapes[TETRIS_SHAPE_COUNT][4][2] = {
                                                                          {{4, TETRIS_FIELD_HEIGHT  },{4, TETRIS_FIELD_HEIGHT-1},{5, TETRIS_FIELD_HEIGHT-1},{4, TETRIS_FIELD_HEIGHT-2}},
                                                                          {{4, TETRIS_FIELD_HEIGHT-1},{5, TETRIS_FIELD_HEIGHT-1},{4, TETRIS_FIELD_HEIGHT-2},{5, TETRIS_FIELD_HEIGHT-2}},
                                                                          {{4, TETRIS_FIELD_HEIGHT  },{5, TETRIS_FIELD_HEIGHT-1},{4, TETRIS_FIELD_HEIGHT-1},{5, TETRIS_FIELD_HEIGHT-2}},
                                                                          {{5, TETRIS_FIELD_HEIGHT  },{5, TETRIS_FIELD_HEIGHT-1},{4, TETRIS_FIELD_HEIGHT-1},{4, TETRIS_FIELD_HEIGHT-2}},
                                                                          {{5, TETRIS_FIELD_HEIGHT  },{5, TETRIS_FIELD_HEIGHT-1},{5, TETRIS_FIELD_HEIGHT-2},{4, TETRIS_FIELD_HEIGHT-2}},
                                                                          {{4, TETRIS_FIELD_HEIGHT  },{4, TETRIS_FIELD_HEIGHT-1},{4, TETRIS_FIELD_HEIGHT-2},{5, TETRIS_FIELD_HEIGHT-2}},
                                                                          {{4, TETRIS_FIELD_HEIGHT+1},{4, TETRIS_FIELD_HEIGHT  },{4, TETRIS_FIELD_HEIGHT-1},{4, TETRIS_FIELD_HEIGHT-2}}
                                                                        };

int             tetrisField[TETRIS_FIELD_HEIGHT][TETRIS_FIELD_WIDTH];
unsigned int    tetrisGameSpeed                                       = 300;
unsigned int    tetrisKeySpeed                                        = 250;
uint32_t        tetrisPreviousGameTime                                = 0;
uint32_t        tetrisPreviousKeyTime                                 = 0;
int             tetrisCurrentShape[4][2];
int             tetrisNextShape[4][2];
int             tetrisOldShape[4][2];
int             tetrisNextShapeNum, tetrisOldShapeNum;
unsigned long   tetrisLines                                           = 0;
unsigned long   tetrisScore                                           = 0;
bool            tetrisKeysPressed[5]                                  = {false, false, false, false, false}; // left/down/right/up/pause
bool            tetrisPaused                                          = false;
bool            tetrisGameOver                                        = false;
bool            tetrisNewGame                                         = false;

button_ui       ui;

int32_t
dispGetHeight() {
    return( SYSTEM_DISPLAY_HEIGHT - 32 );
}

static void initRng(void) {
  srand( vexSystemPowerupTimeGet() );
}

static int uitoa(unsigned int value, char * buf, int max) {
  int n = 0;
  int i = 0;
  int tmp = 0;

  if (!buf) return -3;
  if (2 > max) return -4;
  i=1;
  tmp = value;
  if (0 > tmp) {
    tmp *= -1;
    i++;
  }
  for (;;) {
    tmp /= 10;
    if (0 >= tmp) break;
    i++;
  }
  if (i >= max) {
    buf[0] = '?';
    buf[1] = 0x0;
    return 2;
  }
  n = i;
  tmp = value;
  if (0 > tmp) {
    tmp *= -1;
  }
  buf[i--] = 0x0;
  for (;;) {
    buf[i--] = (tmp % 10) + '0';
    tmp /= 10;
    if (0 >= tmp) break;
  }
  if (-1 != i) {
    buf[i--] = '-';
  }
  return n;
}

static void sevenSegDraw(int x, int y, uint8_t number, vex::color color) {
  Brain.Screen.setPenColor(color);
  if (number & 0x01) Brain.Screen.drawRectangle(x+SEVEN_SEG_HEIGHT+SEVEN_SEG_SIZE, y, SEVEN_SEG_WIDTH, SEVEN_SEG_HEIGHT, color); // A
  if (number & 0x02) Brain.Screen.drawRectangle(x+SEVEN_SEG_HEIGHT+(SEVEN_SEG_SIZE*2)+SEVEN_SEG_WIDTH, y+SEVEN_SEG_HEIGHT+SEVEN_SEG_SIZE, SEVEN_SEG_HEIGHT, SEVEN_SEG_WIDTH, color); // B
  if (number & 0x04) Brain.Screen.drawRectangle(x+SEVEN_SEG_HEIGHT+(SEVEN_SEG_SIZE*2)+SEVEN_SEG_WIDTH, y+(SEVEN_SEG_HEIGHT*2)+SEVEN_SEG_WIDTH+(SEVEN_SEG_SIZE*3), SEVEN_SEG_HEIGHT, SEVEN_SEG_WIDTH, color); // C
  if (number & 0x08) Brain.Screen.drawRectangle(x+SEVEN_SEG_HEIGHT+SEVEN_SEG_SIZE, y+(SEVEN_SEG_HEIGHT*2)+(SEVEN_SEG_WIDTH*2)+(SEVEN_SEG_SIZE*4), SEVEN_SEG_WIDTH, SEVEN_SEG_HEIGHT, color); // D
  if (number & 0x10) Brain.Screen.drawRectangle(x, y+(SEVEN_SEG_HEIGHT*2)+SEVEN_SEG_WIDTH+(SEVEN_SEG_SIZE*3), SEVEN_SEG_HEIGHT, SEVEN_SEG_WIDTH, color); // E
  if (number & 0x20) Brain.Screen.drawRectangle(x, y+SEVEN_SEG_HEIGHT+SEVEN_SEG_SIZE, SEVEN_SEG_HEIGHT, SEVEN_SEG_WIDTH, color); // F
  if (number & 0x40) Brain.Screen.drawRectangle(x+SEVEN_SEG_HEIGHT+SEVEN_SEG_SIZE, y+SEVEN_SEG_HEIGHT+SEVEN_SEG_WIDTH+(SEVEN_SEG_SIZE*2), SEVEN_SEG_WIDTH, SEVEN_SEG_HEIGHT, color); // G
}

static void drawShape(uint8_t color) {
  int i;
  for (i = 0; i <= 3; i++) {
    if (tetrisCurrentShape[i][1] < TETRIS_FIELD_HEIGHT || tetrisCurrentShape[i][1] >= TETRIS_FIELD_HEIGHT+2 ) {
      Brain.Screen.setPenColor( tetrisShapeColors[color] );
      Brain.Screen.drawRectangle((tetrisCurrentShape[i][0]*TETRIS_CELL_WIDTH)+2, dispGetHeight()-TETRIS_CELL_HEIGHT-(tetrisCurrentShape[i][1]*TETRIS_CELL_HEIGHT)-3, TETRIS_CELL_WIDTH-2, TETRIS_CELL_HEIGHT-2, tetrisShapeColors[color]);
       if (color != 0) {
         Brain.Screen.setPenColor( tetrisShapeColors[8] );
         Brain.Screen.drawRectangle((tetrisCurrentShape[i][0]*TETRIS_CELL_WIDTH)+2, dispGetHeight()-TETRIS_CELL_HEIGHT-(tetrisCurrentShape[i][1]*TETRIS_CELL_HEIGHT)-3, TETRIS_CELL_WIDTH-1, TETRIS_CELL_HEIGHT-1, vex::color::transparent );
       } else {
         Brain.Screen.setPenColor( tetrisShapeColors[0] );
         Brain.Screen.drawRectangle((tetrisCurrentShape[i][0]*TETRIS_CELL_WIDTH)+2, dispGetHeight()-TETRIS_CELL_HEIGHT-(tetrisCurrentShape[i][1]*TETRIS_CELL_HEIGHT)-3, TETRIS_CELL_WIDTH-1, TETRIS_CELL_HEIGHT-1,  vex::color::transparent );
       }
    }
  }
}

static uint32_t randomInt(uint32_t max) {
  return rand() % max;
}

static void createShape(void) {
  int i;
  memcpy(tetrisNextShape, tetrisShapes[tetrisNextShapeNum], sizeof(tetrisNextShape)); // assign from tetrisShapes arr;
  memcpy(tetrisCurrentShape, tetrisNextShape, sizeof(tetrisCurrentShape));            // tetrisCurrentShape = tetrisNextShape;
  memcpy(tetrisOldShape, tetrisNextShape, sizeof(tetrisOldShape));                    // tetrisOldShape = tetrisNextShape;
  for (i = 0; i <= 3; i++) {
    tetrisCurrentShape[i][0] += 7;
    tetrisCurrentShape[i][1] -= 4;
  }
  drawShape(0);
  tetrisOldShapeNum = tetrisNextShapeNum;
  tetrisNextShapeNum = randomInt(TETRIS_SHAPE_COUNT);
  memcpy(tetrisNextShape, tetrisShapes[tetrisNextShapeNum], sizeof(tetrisNextShape)); // assign from tetrisShapes arr;
  memcpy(tetrisCurrentShape, tetrisNextShape, sizeof(tetrisCurrentShape)); // tetrisCurrentShape = tetrisNextShape;
  for (i = 0; i <= 3; i++) {
    tetrisCurrentShape[i][0] += 7;
    tetrisCurrentShape[i][1] -= 4;
  }
  drawShape(tetrisNextShapeNum+1);
  memcpy(tetrisCurrentShape, tetrisOldShape, sizeof(tetrisCurrentShape)); // tetrisCurrentShape = tetrisOldShape;
}

static void tellScore(uint8_t color) {
  char pps_str[12];
  uint8_t i;

  i = 0;
  uitoa(tetrisLines, pps_str, sizeof(pps_str));
  Brain.Screen.setPenColor( black );
  Brain.Screen.drawRectangle( TETRIS_SEVEN_SEG_SCORE_X, 198, SEVEN_SEG_CHAR_WIDTH * 8,  SEVEN_SEG_CHAR_HEIGHT + 4, black );
  for (i = 0; i < strlen(pps_str); i++) {
    if (pps_str[i] == '0') sevenSegDraw(TETRIS_SEVEN_SEG_SCORE_X, 200, sevenSegNumbers[0], lineColor );
    if (pps_str[i] == '1') sevenSegDraw(TETRIS_SEVEN_SEG_SCORE_X, 200, sevenSegNumbers[1], lineColor );
    if (pps_str[i] == '2') sevenSegDraw(TETRIS_SEVEN_SEG_SCORE_X, 200, sevenSegNumbers[2], lineColor );
    if (pps_str[i] == '3') sevenSegDraw(TETRIS_SEVEN_SEG_SCORE_X, 200, sevenSegNumbers[3], lineColor );
    if (pps_str[i] == '4') sevenSegDraw(TETRIS_SEVEN_SEG_SCORE_X, 200, sevenSegNumbers[4], lineColor );
    if (pps_str[i] == '5') sevenSegDraw(TETRIS_SEVEN_SEG_SCORE_X, 200, sevenSegNumbers[5], lineColor );
    if (pps_str[i] == '6') sevenSegDraw(TETRIS_SEVEN_SEG_SCORE_X, 200, sevenSegNumbers[6], lineColor );
    if (pps_str[i] == '7') sevenSegDraw(TETRIS_SEVEN_SEG_SCORE_X, 200, sevenSegNumbers[7], lineColor );
    if (pps_str[i] == '8') sevenSegDraw(TETRIS_SEVEN_SEG_SCORE_X, 200, sevenSegNumbers[8], lineColor );
    if (pps_str[i] == '9') sevenSegDraw(TETRIS_SEVEN_SEG_SCORE_X, 200, sevenSegNumbers[9], lineColor );
  }

  i = 0;
  uitoa(tetrisScore, pps_str, sizeof(pps_str));
  Brain.Screen.setPenColor( black );
  Brain.Screen.drawRectangle( TETRIS_SEVEN_SEG_SCORE_X, 138, SEVEN_SEG_CHAR_WIDTH * 8,  SEVEN_SEG_CHAR_HEIGHT + 4, black );
  for (i = 0; i < strlen(pps_str); i++) {
    if (pps_str[i] == '0') sevenSegDraw(TETRIS_SEVEN_SEG_SCORE_X, 140, sevenSegNumbers[0], lineColor );
    if (pps_str[i] == '1') sevenSegDraw(TETRIS_SEVEN_SEG_SCORE_X, 140, sevenSegNumbers[1], lineColor );
    if (pps_str[i] == '2') sevenSegDraw(TETRIS_SEVEN_SEG_SCORE_X, 140, sevenSegNumbers[2], lineColor );
    if (pps_str[i] == '3') sevenSegDraw(TETRIS_SEVEN_SEG_SCORE_X, 140, sevenSegNumbers[3], lineColor );
    if (pps_str[i] == '4') sevenSegDraw(TETRIS_SEVEN_SEG_SCORE_X, 140, sevenSegNumbers[4], lineColor );
    if (pps_str[i] == '5') sevenSegDraw(TETRIS_SEVEN_SEG_SCORE_X, 140, sevenSegNumbers[5], lineColor );
    if (pps_str[i] == '6') sevenSegDraw(TETRIS_SEVEN_SEG_SCORE_X, 140, sevenSegNumbers[6], lineColor );
    if (pps_str[i] == '7') sevenSegDraw(TETRIS_SEVEN_SEG_SCORE_X, 140, sevenSegNumbers[7], lineColor );
    if (pps_str[i] == '8') sevenSegDraw(TETRIS_SEVEN_SEG_SCORE_X, 140, sevenSegNumbers[8], lineColor );
    if (pps_str[i] == '9') sevenSegDraw(TETRIS_SEVEN_SEG_SCORE_X, 140, sevenSegNumbers[9], lineColor );
  }
}

static void initField(void) {
  int i,j;
  tellScore(0); // clear score
  for (i = 0; i < TETRIS_FIELD_HEIGHT; i++) {
    for (j = 0; j < TETRIS_FIELD_WIDTH; j++) {
      tetrisField[i][j] = 0;
    }
  }
  createShape();
  drawShape(tetrisOldShapeNum+1);
  tetrisLines = 0;
  tetrisScore = 0;
  tellScore(8);
}

void drawCell(int x, int y, uint8_t color) {
  Brain.Screen.setPenColor( tetrisShapeColors[color] );
  Brain.Screen.drawRectangle((x*TETRIS_CELL_WIDTH)+2, dispGetHeight()-TETRIS_CELL_HEIGHT-(y*TETRIS_CELL_HEIGHT)-3, TETRIS_CELL_WIDTH-2, TETRIS_CELL_HEIGHT-2, tetrisShapeColors[color]);
  if (color != 0) {
    Brain.Screen.setPenColor( tetrisShapeColors[8] );
    Brain.Screen.drawRectangle((x*TETRIS_CELL_WIDTH)+2, dispGetHeight()-TETRIS_CELL_HEIGHT-(y*TETRIS_CELL_HEIGHT)-3, TETRIS_CELL_WIDTH-1, TETRIS_CELL_HEIGHT-1, vex::color::transparent );
  } else {
    Brain.Screen.setPenColor( tetrisShapeColors[0] );
    Brain.Screen.drawRectangle((x*TETRIS_CELL_WIDTH)+2, dispGetHeight()-TETRIS_CELL_HEIGHT-(y*TETRIS_CELL_HEIGHT)-3, TETRIS_CELL_WIDTH-1, TETRIS_CELL_HEIGHT-1, vex::color::transparent );
  }
}

static void printText(uint8_t color) {
  Brain.Screen.setFont( mono20 );
  Brain.Screen.printAt(( TETRIS_FIELD_WIDTH*TETRIS_CELL_WIDTH)+TETRIS_CELL_WIDTH, dispGetHeight() - (TETRIS_FIELD_HEIGHT*TETRIS_CELL_HEIGHT) + 10, "Next" );
  Brain.Screen.printAt(( TETRIS_FIELD_WIDTH*TETRIS_CELL_WIDTH)+TETRIS_CELL_WIDTH, dispGetHeight() - 110, "Score" );
  Brain.Screen.printAt(( TETRIS_FIELD_WIDTH*TETRIS_CELL_WIDTH)+TETRIS_CELL_WIDTH, dispGetHeight() - 50, "Lines" );
}

static void printGameOver(void) {
  char str[] = "Game Over !";

  Brain.Screen.setFont( mono15 );
  
  if (tetrisGameOver)  {
    Brain.Screen.setPenColor( red );
    Brain.Screen.printAt( ((TETRIS_FIELD_WIDTH*TETRIS_CELL_WIDTH)/2)-Brain.Screen.getStringWidth(str)/2, dispGetHeight()-(TETRIS_FIELD_HEIGHT*TETRIS_CELL_HEIGHT)/2, str );
  }
  else {
    Brain.Screen.setPenColor( black );
    Brain.Screen.drawRectangle(((TETRIS_FIELD_WIDTH*TETRIS_CELL_WIDTH)/2)-Brain.Screen.getStringWidth(str)/2, dispGetHeight()-(TETRIS_FIELD_HEIGHT*TETRIS_CELL_HEIGHT)/2, Brain.Screen.getStringWidth(str)+4,  Brain.Screen.getStringWidth("A")+2, black );
  }
}

static bool stay(bool down) {
  int sk, k;
  bool stay;
  if (down) sk = 1; else sk = 0;
  stay = false;
  for (k = 0; k <= 3; k++) {
    if (tetrisCurrentShape[k][1] == 0) {
      return true;
    }
  }
  for (k = 0; k <= 3; k++) {
    if ((tetrisCurrentShape[k][0] < 0) || (tetrisCurrentShape[k][0] >= TETRIS_FIELD_WIDTH)) return true;
    if (tetrisCurrentShape[k][1] < TETRIS_FIELD_HEIGHT)
      if (tetrisField[tetrisCurrentShape[k][1]-sk][tetrisCurrentShape[k][0]] != 0) return true;
  }
  return stay;
}

static void clearCompleteLines(void) {
  bool t;
  uint8_t reiz = 0;
  int l,k,j;
  l = 0;
  while (l < TETRIS_FIELD_HEIGHT) {
    t = true;
    for (j = 0; j < TETRIS_FIELD_WIDTH; j++)
      if (tetrisField[l][j] == 0) t = false;
    if (t) {
      for (j = 4; j >= 0; j--) { // cheap & dirty line removal animation :D
        drawCell(j,l, 0);
        drawCell(TETRIS_FIELD_WIDTH-1-j,l, 0);
        this_thread::sleep_for(40);
      }
      reiz++;
      for (k = 0; k < TETRIS_FIELD_WIDTH; k++) {
        for (j = l; j < TETRIS_FIELD_HEIGHT-1; j++) {
          tetrisField[j][k] = tetrisField[j+1][k];
          drawCell(k,j, tetrisField[j][k]);
        }
      }
      for (j = 0; j < TETRIS_FIELD_WIDTH; j++) {
        tetrisField[TETRIS_FIELD_HEIGHT-1][j] = 0;
        drawCell(j,TETRIS_FIELD_HEIGHT-1,0);
      }
    } else {
      l++;
    }
  }
  if (reiz > 0) {
    tetrisLines += reiz;
    tetrisScore += (reiz*10)*reiz;
    tellScore(8);
  }
}

static void goDown(void) {
  int i;
  if (!stay(true)) {
    drawShape(0);
    for (i = 0; i <= 3; i++) {
      tetrisCurrentShape[i][1]--;
    }
    drawShape(tetrisOldShapeNum+1);
  } else {
    for (i = 0; i <= 3; i++) {
      if (tetrisCurrentShape[i][1] >=TETRIS_FIELD_HEIGHT) {
        tetrisGameOver = true;
        return;
      } else {
       tetrisField[tetrisCurrentShape[i][1]][tetrisCurrentShape[i][0]] = tetrisOldShapeNum+1;
      }
    }
    clearCompleteLines();
    createShape();
    if (stay(false)) {
      tetrisGameOver = true;
      return;
    }
    drawShape(tetrisOldShapeNum+1);
  }
}

static void clearField(void) {
  int j, k;
  for (k = TETRIS_FIELD_HEIGHT-1; k >= 0; k--) {
    for (j = 0; j <= TETRIS_FIELD_WIDTH-1; j++) {
      drawCell(j,TETRIS_FIELD_HEIGHT-1-k, randomInt(8)+1);
      this_thread::sleep_for(10);
    }
  }
  for (k = 0; k <= TETRIS_FIELD_HEIGHT-1; k++) {
    for (j = 0; j <= TETRIS_FIELD_WIDTH-1; j++) {
      drawCell(j,TETRIS_FIELD_HEIGHT-1-k, tetrisShapeColors[0]);
      this_thread::sleep_for(10);
    }
  }
}

static void rotateShape(void) {
  int i, ox, oy, tx, ty;
  ox = tetrisCurrentShape[1][0];
  oy = tetrisCurrentShape[1][1];
  memcpy(tetrisOldShape, tetrisCurrentShape, sizeof(tetrisOldShape)); // tetrisOldShape = tetrisCurrentShape;
  for (i = 0; i <= 3; i++) {
    tx = tetrisCurrentShape[i][0];
    ty = tetrisCurrentShape[i][1];
    tetrisCurrentShape[i][0] = ox+(round((tx-ox)*cos(90*(3.14/180))-(ty-oy)*sin(90*(3.14/180))));
    tetrisCurrentShape[i][1] = oy+(round((tx-ox)*sin(90*(3.14/180))+(ty-oy)*cos(90*(3.14/180))));
  }
  if (!stay(false)) {
    memcpy(tetrisNextShape, tetrisCurrentShape, sizeof(tetrisNextShape)); // tetrisNextShape = tetrisCurrentShape;
    memcpy(tetrisCurrentShape, tetrisOldShape, sizeof(tetrisCurrentShape)); // tetrisCurrentShape = tetrisOldShape;
    drawShape(0);
    memcpy(tetrisCurrentShape, tetrisNextShape, sizeof(tetrisCurrentShape)); // tetrisCurrentShape = tetrisNextShape;
    drawShape(tetrisOldShapeNum+1);
  } else {
    memcpy(tetrisCurrentShape, tetrisOldShape, sizeof(tetrisCurrentShape)); // tetrisCurrentShape = tetrisOldShape;
  }
}

static bool checkSides(bool left) {
  int sk,k;
  if (left) sk = 1; else sk = -1;
  for (k = 0; k <= 3; k++) {
    if ((tetrisCurrentShape[k][0]+sk < 0) || (tetrisCurrentShape[k][0]+sk >= TETRIS_FIELD_WIDTH)) return true;
    if (tetrisCurrentShape[k][1] < TETRIS_FIELD_HEIGHT)
      if (tetrisField[tetrisCurrentShape[k][1]][tetrisCurrentShape[k][0]+sk] != 0) return true;
  }
  return false;
}

static void goRight(void) {
  int i;
  if (!checkSides(true)) {
    drawShape(0);
    for (i = 0; i <= 3; i++) {
      tetrisCurrentShape[i][0]++;
    }
    drawShape(tetrisOldShapeNum+1);
  }
}`

static void goLeft(void) {
  int i;
  if (!checkSides(false)) {
    drawShape(0);
    for (i = 0; i <= 3; i++) {
      tetrisCurrentShape[i][0]--;
    }
    drawShape(tetrisOldShapeNum+1);
  } 
}

uint8_t action     = 0;

static 
int thdTetris() {
  uint8_t i;
  const   uint8_t doGoLeft   = 1;
  const   uint8_t doGoRight  = 2;
  const   uint8_t doGoRotate = 4;
  const   uint8_t doGoDown   = 8;

  auto userTouchAction =  []( int index, bool state, int id ) -> bool {     
    if( state == true ) {
      switch(id) {
        case  0:
          action = doGoLeft;
          break;
        case  1:
          action = doGoRight;
          break;
        case  2:
          action = doGoRotate;
          break;
        case  3:
          action = doGoDown;
          break;
        case  4:
          tetrisPaused = true;
          break;
        case  5:
          tetrisNewGame = true;
          break;
      }
    }
    else {
      switch(id) {
        case  4:
          tetrisPaused = false;
          break;
      }
      action = 0;
    }
    return true;   
  };

  ui.removeAll();

  ui.addButton( 260, 10, 60, 60,  0xE00000, "<-", userTouchAction, 0 );
  ui.addButton( 340, 10, 60, 60,  0xE00000, "Rot", userTouchAction, 2 );
  ui.addButton( 420, 10, 60, 60,  0xE00000, "->", userTouchAction, 1 );

  ui.addButton( 340,100, 60, 60,  0xE00000, "V", userTouchAction, 3 );
  ui.addButton( 420,180, 60, 60,  0xE00000, "Pause", userTouchAction, 4, true );

  ui.enable();
  ui.display();

  while (!tetrisGameOver) {    
    if (vex::timer::system() - tetrisPreviousKeyTime >= (tetrisKeySpeed) ) {
      for (i = 0; i < sizeof(tetrisKeysPressed); i++) {
        if (tetrisKeysPressed[i]) {
          tetrisKeysPressed[i] = false;
        }
      }
      tetrisPreviousKeyTime = vex::timer::system();
    }

    if (vex::timer::system() - tetrisPreviousGameTime >= (tetrisGameSpeed) ) {
      if ( !tetrisPaused) 
        goDown(); // gives smooth move_down when down button pressed! :D
      tetrisPreviousGameTime = vex::timer::system();
    }

    if ( (action == doGoLeft) && !tetrisKeysPressed[0] && !tetrisPaused) {
      goLeft();
      tetrisKeysPressed[0] = true;
      tetrisPreviousKeyTime = vex::timer::system();
    }
    
    if ( (action == doGoRight) && !tetrisKeysPressed[2] && !tetrisPaused) {
      goRight();
      tetrisKeysPressed[2] = true;
      tetrisPreviousKeyTime = vex::timer::system();
    }
    
    if ( (action == doGoRotate) && !tetrisKeysPressed[3] && !tetrisPaused) {
      rotateShape();
      tetrisKeysPressed[3] = true;
      tetrisPreviousKeyTime = vex::timer::system();
    }

    if ( (action == doGoDown) && !tetrisKeysPressed[1] && !tetrisPaused) {
      goDown();
      tetrisKeysPressed[1] = true;
      tetrisPreviousKeyTime = vex::timer::system();
    }
    
    this_thread::sleep_for(10);
  }

  ui.removeAll();
  ui.addButton( 300,90, 120, 60,  0xE00000, "New Game", userTouchAction, 5 );
  ui.display();
  while (!tetrisNewGame) {
    this_thread::sleep_for(100);
  }

  this_thread::sleep_for(200);
  ui.removeAll();
  
  return 0;
}

static void tetrisDeinit(void) {
}

void tetrisStart(void) {
  // Show the help first
  Brain.Screen.clearScreen( black );
  tetrisTitle();
  
  // Draw the board
  Brain.Screen.clearScreen( black );
  Brain.Screen.setPenColor( white );
  Brain.Screen.drawRectangle( 0, dispGetHeight()-(TETRIS_FIELD_HEIGHT*TETRIS_CELL_HEIGHT)-5, (TETRIS_FIELD_WIDTH*TETRIS_CELL_WIDTH)+3, (TETRIS_FIELD_HEIGHT*TETRIS_CELL_HEIGHT)+3, vex::color::transparent );
  printText(8);

  // Away we go
  initField();
  tetrisGameOver = false;
  tetrisNewGame  = false;
  printGameOver(); // removes "Game Over!" if tetrisGameOver == false
  tetrisPreviousGameTime = vex::timer::system();
  this_thread::sleep_for(1000);
  vex::thread t( thdTetris );

  while (!tetrisGameOver ) {
    this_thread::sleep_for(100);
  }

  clearField();
  printGameOver();
  tetrisDeinit();

  while (!tetrisNewGame ) {
    this_thread::sleep_for(100);
  }
}

void tetrisInit(void) {
  initRng();
  tetrisNextShapeNum = randomInt(TETRIS_SHAPE_COUNT);
}
