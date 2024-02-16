#include "vex.h"
#include "tetris.h"

using namespace vex;

#define  SX  (5)
#define  SY  (FIELD_HEIGHT - 5)

static int title[][2] = {
  // T
  { SX     , SY },
  { SX +  1, SY },
  { SX +  2, SY },
  { SX +  3, SY },
  { SX +  4, SY },

  { SX +  2,  SY - 1 },
  { SX +  2,  SY - 2 },
  { SX +  2,  SY - 3 },
  { SX +  2,  SY - 4 },
  { SX +  2,  SY - 5 },
  { SX +  2,  SY - 6 },

  // E
  { SX +  6,  SY },
  { SX +  7,  SY },
  { SX +  8,  SY },
  { SX +  9,  SY },

  { SX +  6,  SY - 1 },
  { SX +  6,  SY - 2 },
  { SX +  6,  SY - 3 },
  { SX +  6,  SY - 4 },
  { SX +  6,  SY - 5 },
  { SX +  6,  SY - 6 },

  { SX +  6,  SY - 3 },
  { SX +  7,  SY - 3 },
  { SX +  8,  SY - 3 },
  { SX +  9,  SY - 3 },

  { SX +  6,  SY - 6 },
  { SX +  7,  SY - 6 },
  { SX +  8,  SY - 6 },
  { SX +  9,  SY - 6 },

  // T
  { SX + 11, SY },
  { SX + 12, SY },
  { SX + 13, SY },
  { SX + 14, SY },
  { SX + 15, SY },
  
  { SX + 13,  SY - 1 },
  { SX + 13,  SY - 2 },
  { SX + 13,  SY - 3 },
  { SX + 13,  SY - 4 },
  { SX + 13,  SY - 5 },
  { SX + 13,  SY - 6 },

  // R
  { SX + 17,  SY },
  { SX + 18,  SY },
  { SX + 19,  SY },

  { SX + 17,  SY - 1 },
  { SX + 17,  SY - 2 },
  { SX + 17,  SY - 3 },
  { SX + 17,  SY - 4 },
  { SX + 17,  SY - 5 },
  { SX + 17,  SY - 6 },

  { SX + 17,  SY - 3 },
  { SX + 18,  SY - 3 },
  { SX + 19,  SY - 3 },

  { SX + 20,  SY - 1 },
  { SX + 20,  SY - 2 },
  { SX + 19,  SY - 4 },
  { SX + 20,  SY - 5 },
  { SX + 20,  SY - 6 },

  // I
  { SX + 22, SY },
  { SX + 23, SY },
  { SX + 24, SY },

  { SX + 23, SY - 1 },
  { SX + 23, SY - 2 },
  { SX + 23, SY - 3 },
  { SX + 23, SY - 4 },
  { SX + 23, SY - 5 },
  { SX + 23, SY - 6 },

  { SX + 22, SY - 6 },
  { SX + 23, SY - 6 },
  { SX + 24, SY - 6 },

  // S
  { SX + 27, SY },
  { SX + 28, SY },
  { SX + 29, SY - 1 },

  { SX + 26, SY - 1 },
  { SX + 26, SY - 2 },

  { SX + 27, SY - 3 },
  { SX + 28, SY - 3 },

  { SX + 29, SY - 4 },
  { SX + 29, SY - 5 },

  { SX + 26, SY - 5 },
  { SX + 27, SY - 6 },
  { SX + 28, SY - 6 },

};

void tetrisTitle() {
  int len = sizeof( title ) / 2 / sizeof(int) ;

  for(int i=0;i< len;i++) {
    int c = rand() % 4 + 1;
    drawCell( title[i][0], title[i][1], c );
    this_thread::sleep_for(20);
  }

  this_thread::sleep_for(1000);

  for(int i=0;i<180;i++) {
  vexDisplayScroll( 0, -2 );
    this_thread::sleep_for(10);
  }  

  this_thread::sleep_for(500);
}