/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\okumap15                                         */
/*    Created:      Tue Sep 20 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;
enum Axis
{
  Left,
  Right,
  Up,
  Down
};

enum MarioState
{
  Small,
  Mushroom,
  FireFlower
};

class Vector2
{
  public:
  int X;
  int Y;
  public: Vector2(){}
  public: Vector2 (int x, int y)
  {
    X = x;
    Y = y;
  }
};

class Sprite
{
  public:
  color Drawing[16][16];
  public: Sprite(){}
  public: Sprite(color drawing[16][16])
  {
    for (int i = 0; i <= 15; i++) 
    {
      for (int j = 0; j <= 15; j++) 
      {
        Drawing[i][j] = drawing[i][j];
      }
    }
  }
  public: void Draw(Vector2 Pos)
  {
    for (int i = 0; i <= 15; i++) 
    {
      for (int j = 0; j <= 15; j++) 
      {
        Brain.Screen.setFillColor(Drawing[i][j]);
        Brain.Screen.setPenColor(Drawing[i][j]);
        if (Drawing[i][j] != transparent) {
          Brain.Screen.drawPixel(Pos.X+j,Pos.Y+i);
        }
      }
    }
  }
  public: void DrawUpscaled(Vector2 Pos, int Upscale)
  {
    for (int i = 0; i <= 15; i++) 
    {
      for (int j = 0; j <= 15; j++) 
      {
        Brain.Screen.setFillColor(Drawing[i][j]);
        Brain.Screen.setPenColor(Drawing[i][j]);
        for (int l = 0; l <= Upscale; l++) 
        {
          for (int k = 0; k <= Upscale; k++) 
          {
            if (Drawing[i][j] != transparent) 
            {
              Brain.Screen.drawPixel(Pos.X+(j*Upscale)+k,Pos.Y+(i*Upscale)+l);
            }
          }
        } 
      }
    }
  }
  public: void DrawFlipped(Vector2 Pos)
  {
    for (int i = 0; i <= 15; i++) 
    {
      int k = 15;
      for (int j = 0; j <= 15; j++) 
      {
        Brain.Screen.setFillColor(Drawing[i][k]);
        Brain.Screen.setPenColor(Drawing[i][k]);
        Brain.Screen.drawPixel(Pos.X+j,Pos.Y+i);
        k--;
      }
    }
  }
  public: void DrawUpscaleFlipped(Vector2 Pos, int Upscale)
  {
    for (int i = 0; i <= 15; i++) 
    {
      int m = 15;
      for (int j = 0; j <= 15; j++) 
      {
        
        Brain.Screen.setFillColor(Drawing[i][m]);
        Brain.Screen.setPenColor(Drawing[i][m]);
        for (int l = 0; l <= Upscale; l++) 
        {
          for (int k = 0; k <= Upscale; k++) 
          {
            if (Drawing[i][m] != transparent) 
            {
              Brain.Screen.drawPixel(Pos.X+(j*Upscale)+k,Pos.Y+(i*Upscale)+l);  
            }
            
          }
        }
        m--;
      }
    }
  }
};

class Rectangle
{
  public:
  int X;
  int Y;
  int Width;
  int Height;
  public: Rectangle(){}
  public: Rectangle(int x, int y, int WWidth, int HHeight)
  {
    X = x;
    Y = y;
    Width = WWidth;
    Height = HHeight;
  }
  
  public: Rectangle(Vector2 Pos, Vector2 Lengths)
  {
    X = Pos.X;
    Y = Pos.Y;
    Width = Lengths.X;
    Height = Lengths.Y;
  }
  
  public: int Left()
  {
    return(X);
  }
  public: int Right()
  {
    return(X + Width);
  }
  public: int Top()
  {
    return(Y);
  }
  public: int Bottom()
  {
    return(Y + Height);
  }
  public: int XMiddle()
  {
    return(X + Width/2);
  }
  public: int YMiddle()
  {
    return(Y + Height/2);
  }
  public: bool Intersects(Rectangle OtherRectangle)
  {
    
    if (Right() > OtherRectangle.Left() && Bottom() > OtherRectangle.Top() && Left() < OtherRectangle.Right() && Top() < OtherRectangle.Bottom())
    {
      return true;
    }
    
    else if (YMiddle() < OtherRectangle.Bottom() && YMiddle() > OtherRectangle.Top() && XMiddle() > OtherRectangle.Left() && XMiddle() < OtherRectangle.Right()) {
      return true;
    }
    
    return false;
  }
  public: bool EqIntersects(Rectangle OtherRectangle)
  {
    
    if (Right() >= OtherRectangle.Left() && Bottom() >= OtherRectangle.Top() && Left() <= OtherRectangle.Right() && Top() <= OtherRectangle.Bottom())
    {
      return true;
    }
    
    else if (YMiddle() < OtherRectangle.Bottom() && YMiddle() > OtherRectangle.Top() && XMiddle() > OtherRectangle.Left() && XMiddle() < OtherRectangle.Right()) {
      return true;
    }
    
    return false;
  }
  public: bool OnScreen()
  {
    if (X < -32 || X > 480) {
      return false;
    }
    else{
      return true;
    }
  }
};

class Object 
{
  
  public:
  Rectangle Hitbox;
  int XVel = 0;
  int YVel = 0;
  bool Alive = true;
  bool InAir = false;
  bool Jumping = false;
  bool OnScreen = true;
  public:  Object(){}
 
  public: Rectangle ReturnHitbox()
  {
    return Hitbox;
  }
  public: void MoveVel()
  {
    Hitbox.X += XVel;
    Hitbox.Y += YVel;
  }
};

int main() 
{


  vexcodeInit();
 #pragma region creating extra colors
  color brown(165,42,42);
  color Black(1,1,1);
  color tan(253,217,181);
  color Orange(204,112,0);
  color DarkBrown(78,53,36);
  color Lgrey(211,211,211);
  color Dgrey(169,169,169);
  #pragma endregion
  
 #pragma region creating sprite color arrays, this gonna be painful
  
  color MarioNeutralColors[16][16] = 
  {
    {transparent,transparent,transparent,transparent,transparent,red,red,red,red,red,transparent,transparent,transparent,transparent,transparent,transparent}, 
    {transparent,transparent,transparent,transparent,red,red,red,red,red,red,red,red,red,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,green,green,green,yellow,yellow,green,yellow,transparent,transparent,transparent,transparent,transparent}, 
    {transparent,transparent,transparent,green,yellow,green,yellow,yellow,yellow,green,yellow,yellow,yellow,transparent,transparent,transparent},
    {transparent,transparent,transparent,green,yellow,green,green,yellow,yellow,yellow,green,yellow,yellow,yellow,transparent,transparent}, 
    {transparent,transparent,transparent,green,green,yellow,yellow,yellow,yellow,green,green,green,green,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,transparent,yellow,yellow,yellow,yellow,yellow,yellow,yellow,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,green,green,red,green,green,green,transparent,transparent,transparent,transparent,transparent,transparent}, 
    {transparent,transparent,transparent,green,green,green,red,green,green,red,green,green,green,transparent,transparent,transparent},
    {transparent,transparent,green,green,green,green,red,red,red,red,green,green,green,green,transparent,transparent}, 
    {transparent,transparent,yellow,yellow,green,red,yellow,red,red,yellow,red,green,yellow,yellow,transparent,transparent}, 
    {transparent,transparent,yellow,yellow,yellow,red,red,red,red,red,red,yellow,yellow,yellow,transparent,transparent},
    {transparent,transparent,yellow,yellow,red,red,red,red,red,red,red,red,yellow,yellow,transparent,transparent}, 
    {transparent,transparent,transparent,transparent,red,red,red,transparent,transparent,red,red,red,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,green,green,green,transparent,transparent,transparent,transparent,green,green,green,transparent,transparent,transparent}, 
    {transparent,transparent,green,green,green,green,transparent,transparent,transparent,transparent,green,green,green,green,transparent,transparent}
  };
  color MarioDeadColors[16][16] = 
  {
    {transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,transparent,transparent,red,red,red,red,transparent,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,yellow,transparent,red,red,red,red,red,red,transparent,yellow,transparent,transparent,transparent},
    {transparent,yellow,yellow,yellow,green,yellow,green,yellow,yellow,green,yellow,green,yellow,yellow,yellow,transparent},
    {transparent,yellow,yellow,green,green,yellow,green,yellow,yellow,green,yellow,green,green,yellow,yellow,transparent},
    {transparent,yellow,yellow,green,green,green,yellow,yellow,yellow,yellow,green,green,green,yellow,yellow,transparent},
    {transparent,transparent,transparent,green,green,green,green,yellow,yellow,green,green,green,green,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,green,yellow,green,green,green,green,yellow,green,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,green,yellow,yellow,yellow,yellow,yellow,yellow,green,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,red,red,red,yellow,yellow,yellow,yellow,red,red,red,transparent,transparent,transparent},
    {transparent,transparent,green,green,red,red,green,green,green,green,red,red,green,green,transparent,transparent},
    {transparent,transparent,green,green,green,red,red,green,green,red,red,green,green,green,transparent,transparent},
    {transparent,transparent,green,green,green,red,yellow,red,red,yellow,red,green,green,green,transparent,transparent},
    {transparent,transparent,green,green,green,red,red,red,red,red,red,green,green,green,transparent,transparent},
    {transparent,transparent,transparent,green,green,red,red,red,red,red,red,green,green,transparent,transparent,transparent},
  };
  color JumpingColors[16][16] = 
  {
    {transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,yellow,yellow,yellow},
    {transparent,transparent,transparent,transparent,transparent,transparent,red,red,red,red,red,transparent,transparent,yellow,yellow,yellow},
    {transparent,transparent,transparent,transparent,transparent,red,red,red,red,red,red,red,red,red,yellow,yellow},
    {transparent,transparent,transparent,transparent,transparent,green,green,green,yellow,yellow,green,yellow,transparent,red,red,red},
    {transparent,transparent,transparent,transparent,green,yellow,green,yellow,yellow,yellow,green,yellow,yellow,red,red,red},
    {transparent,transparent,transparent,transparent,green,yellow,green,green,yellow,yellow,yellow,green,yellow,yellow,yellow,red},
    {transparent,transparent,transparent,transparent,green,green,yellow,yellow,yellow,yellow,green,green,green,green,green,transparent},
    {transparent,transparent,transparent,transparent,transparent,transparent,yellow,yellow,yellow,yellow,yellow,yellow,yellow,green,transparent,transparent},
    {transparent,transparent,green,green,green,green,green,red,green,green,green,red,green,transparent,transparent,transparent},
    {transparent,green,green,green,green,green,green,green,red,green,green,green,red,transparent,transparent,green},
    {yellow,yellow,green,green,green,green,green,green,red,red,red,red,red,transparent,transparent,green},
    {yellow,yellow,yellow,transparent,red,red,green,red,red,yellow,red,red,yellow,red,green,green},
    {transparent,yellow,transparent,green,red,red,red,red,red,red,red,red,red,red,green,green},
    {transparent,transparent,green,green,green,red,red,red,red,red,red,red,red,red,green,green},
    {transparent,green,green,green,red,red,red,red,red,red,red,transparent,transparent,transparent,transparent,transparent},
    {transparent,green,transparent,transparent,red,red,red,red,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent},
  };

  color MarioRunningOneColors[16][16]
  {
    {transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,transparent,red,red,red,red,red,transparent,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,red,red,red,red,red,red,red,red,red,transparent,transparent,transparent,},
    {transparent,transparent,transparent,transparent,green,green,green,yellow,yellow,green,yellow,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,green,yellow,green,yellow,yellow,yellow,green,yellow,yellow,yellow,transparent,transparent,transparent},
    {transparent,transparent,transparent,green,yellow,green,green,yellow,yellow,yellow,green,yellow,yellow,yellow,transparent,transparent},
    {transparent,transparent,transparent,green,green,yellow,yellow,yellow,yellow,green,green,green,green,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,transparent,yellow,yellow,yellow,yellow,yellow,yellow,yellow,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,green,green,green,red,green,green,transparent,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,green,green,green,green,red,red,green,green,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,green,green,green,red,red,yellow,red,red,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,green,green,green,green,red,red,red,red,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,green,green,green,yellow,yellow,red,red,red,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,red,green,yellow,yellow,red,red,transparent,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,transparent,red,red,red,green,green,green,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,transparent,green,green,green,green,transparent,transparent,transparent,transparent,transparent,transparent,transparent},
  };
  color MarioRunningTwoColors[16][16]
  {
    {transparent,transparent,transparent,transparent,transparent,red,red,red,red,red,transparent,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,red,red,red,red,red,red,red,red,red,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,green,green,green,yellow,yellow,green,yellow,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,green,yellow,green,yellow,yellow,yellow,green,yellow,yellow,yellow,transparent,transparent,transparent},
    {transparent,transparent,transparent,green,yellow,green,green,yellow,yellow,yellow,green,yellow,yellow,yellow,transparent,transparent},
    {transparent,transparent,transparent,green,green,yellow,yellow,yellow,yellow,green,green,green,green,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,transparent,yellow,yellow,yellow,yellow,yellow,yellow,yellow,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,transparent,transparent,green,green,red,red,green,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,transparent,green,green,green,green,red,green,yellow,yellow,transparent,transparent,transparent},
    {transparent,transparent,transparent,yellow,yellow,green,green,green,green,green,green,yellow,yellow,yellow,transparent,transparent},
    {transparent,transparent,yellow,yellow,yellow,red,green,green,green,green,green,yellow,yellow,transparent,transparent,transparent},
    {transparent,transparent,transparent,green,green,red,red,red,red,red,red,red,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,green,red,red,red,red,red,red,red,red,transparent,transparent,transparent,transparent},
    {transparent,transparent,green,green,red,red,transparent,transparent,red,red,red,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,green,transparent,transparent,transparent,transparent,green,green,green,transparent,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,green,green,green,transparent,transparent,transparent,transparent,transparent},
  };
  color MarioRunningThreeColors[16][16]
  {
    {transparent,transparent,transparent,transparent,transparent,red,red,red,red,red,transparent,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,red,red,red,red,red,red,red,red,red,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,green,green,green,yellow,yellow,green,yellow,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,green,yellow,green,yellow,yellow,yellow,green,yellow,yellow,yellow,transparent,transparent,transparent},
    {transparent,transparent,transparent,green,yellow,green,green,yellow,yellow,yellow,green,yellow,yellow,yellow,transparent,transparent,},
    {transparent,transparent,transparent,green,green,yellow,yellow,yellow,yellow,green,green,green,green,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,transparent,yellow,yellow,yellow,yellow,yellow,yellow,yellow,transparent,transparent,transparent,transparent},
    {transparent,transparent,green,green,green,green,red,green,green,green,red,transparent,transparent,transparent,transparent,transparent},
    {yellow,yellow,green,green,green,green,red,red,green,green,green,red,green,yellow,yellow,yellow},
    {yellow,yellow,yellow,transparent,green,green,red,red,red,red,red,red,green,green,yellow,yellow},
    {yellow,yellow,transparent,transparent,red,red,red,yellow,red,red,red,yellow,transparent,transparent,green,transparent},
    {transparent,transparent,transparent,red,red,red,red,red,red,red,red,red,red,green,green,transparent},
    {transparent,transparent,red,red,red,red,red,red,red,red,red,red,red,green,green,transparent},
    {transparent,green,green,red,red,red,transparent,transparent,transparent,transparent,red,red,red,green,green,transparent},
    {transparent,green,green,green,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,green,green,green,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent},
  };

  //Brick Block Sprite Colors
  //brown
  color BrownBrickBlockColors[16][16]
  {
    {brown,brown,brown,brown,brown,brown,brown,Black,brown,brown,brown,brown,brown,brown,brown,Black},
    {brown,brown,brown,brown,brown,brown,brown,Black,brown,brown,brown,brown,brown,brown,brown,Black},
    {brown,brown,brown,brown,brown,brown,brown,Black,brown,brown,brown,brown,brown,brown,brown,Black},
    {Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black},
    {brown,brown,brown,Black,brown,brown,brown,brown,brown,brown,brown,Black,brown,brown,brown,brown},
    {brown,brown,brown,Black,brown,brown,brown,brown,brown,brown,brown,Black,brown,brown,brown,brown},
    {brown,brown,brown,Black,brown,brown,brown,brown,brown,brown,brown,Black,brown,brown,brown,brown},
    {Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black},
    {brown,brown,brown,brown,brown,brown,brown,Black,brown,brown,brown,brown,brown,brown,brown,Black},
    {brown,brown,brown,brown,brown,brown,brown,Black,brown,brown,brown,brown,brown,brown,brown,Black},
    {brown,brown,brown,brown,brown,brown,brown,Black,brown,brown,brown,brown,brown,brown,brown,Black},
    {Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black},
    {brown,brown,brown,Black,brown,brown,brown,brown,brown,brown,brown,Black,brown,brown,brown,brown},
    {brown,brown,brown,Black,brown,brown,brown,brown,brown,brown,brown,Black,brown,brown,brown,brown},
    {brown,brown,brown,Black,brown,brown,brown,brown,brown,brown,brown,Black,brown,brown,brown,brown},
    {Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black},
    
  };
  //blue
  color BlueBrickBlockColors[16][16]
  {
    {blue,blue,blue,blue,blue,blue,blue,Black,blue,blue,blue,blue,blue,blue,blue,Black},
    {blue,blue,blue,blue,blue,blue,blue,Black,blue,blue,blue,blue,blue,blue,blue,Black},
    {blue,blue,blue,blue,blue,blue,blue,Black,blue,blue,blue,blue,blue,blue,blue,Black},
    {Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black},
    {blue,blue,blue,Black,blue,blue,blue,blue,blue,blue,blue,Black,blue,blue,blue,blue},
    {blue,blue,blue,Black,blue,blue,blue,blue,blue,blue,blue,Black,blue,blue,blue,blue},
    {blue,blue,blue,Black,blue,blue,blue,blue,blue,blue,blue,Black,blue,blue,blue,blue},
    {Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black},
    {blue,blue,blue,blue,blue,blue,blue,Black,blue,blue,blue,blue,blue,blue,blue,Black},
    {blue,blue,blue,blue,blue,blue,blue,Black,blue,blue,blue,blue,blue,blue,blue,Black},
    {blue,blue,blue,blue,blue,blue,blue,Black,blue,blue,blue,blue,blue,blue,blue,Black},
    {Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black},
    {blue,blue,blue,Black,blue,blue,blue,blue,blue,blue,blue,Black,blue,blue,blue,blue},
    {blue,blue,blue,Black,blue,blue,blue,blue,blue,blue,blue,Black,blue,blue,blue,blue},
    {blue,blue,blue,Black,blue,blue,blue,blue,blue,blue,blue,Black,blue,blue,blue,blue},
    {Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black},
  };
  //Creating goomba colors
  color BrownGoombaLFColors[16][16]
  {
    {transparent,transparent,transparent,transparent,transparent,transparent,brown,brown,brown,brown,transparent,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,transparent,brown,brown,brown,brown,brown,brown,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,brown,brown,brown,brown,brown,brown,brown,brown,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,transparent,transparent,transparent},
    {transparent,transparent,brown,Black,Black,brown,brown,brown,brown,brown,brown,Black,Black,brown,transparent,transparent},
    {transparent,brown,brown,brown,white,Black,brown,brown,brown,brown,Black,white,brown,brown,brown},
    {transparent,brown,brown,brown,white,Black,Black,Black,Black,Black,Black,white,brown,brown,brown},
    {brown,brown,brown,brown,white,Black,white,brown,brown,white,Black,white,brown,brown,brown,brown},
    {brown,brown,brown,brown,white,white,white,brown,brown,white,white,white,brown,brown,brown,brown},
    {brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown},
    {transparent,brown,brown,brown,brown,white,white,white,white,white,white,brown,brown,brown,brown,transparent},
    {transparent,transparent,transparent,transparent,white,white,white,white,white,white,white,white,transparent,transparent,transparent,transparent},
    {transparent,transparent,Black,Black,white,white,white,white,white,white,white,white,transparent,transparent,transparent,transparent},
    {transparent,Black,Black,Black,Black,Black,white,white,white,white,white,Black,Black,transparent,transparent,transparent},
    {transparent,Black,Black,Black,Black,Black,Black,white,white,white,Black,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,Black,Black,Black,Black,Black,transparent,transparent,Black,Black,Black,transparent,transparent}
  };
  
  //blue goomba Colors
  color BlueGoombaLFColors[16][16]
  {
    {transparent,transparent,transparent,transparent,transparent,transparent,blue,blue,blue,blue,transparent,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,transparent,blue,blue,blue,blue,blue,blue,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,blue,blue,blue,blue,blue,blue,blue,blue,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,blue,blue,blue,blue,blue,blue,blue,blue,blue,blue,transparent,transparent,transparent},
    {transparent,transparent,blue,Black,Black,blue,blue,blue,blue,blue,blue,Black,Black,blue,transparent,transparent},
    {transparent,blue,blue,blue,white,Black,blue,blue,blue,blue,Black,white,blue,blue,blue},
    {transparent,blue,blue,blue,white,Black,Black,Black,Black,Black,Black,white,blue,blue,blue},
    {blue,blue,blue,blue,white,Black,white,blue,blue,white,Black,white,blue,blue,blue,blue},
    {blue,blue,blue,blue,white,white,white,blue,blue,white,white,white,blue,blue,blue,blue},
    {blue,blue,blue,blue,blue,blue,blue,blue,blue,blue,blue,blue,blue,blue,blue,blue},
    {transparent,blue,blue,blue,blue,white,white,white,white,white,white,blue,blue,blue,blue,transparent},
    {transparent,transparent,transparent,transparent,white,white,white,white,white,white,white,white,transparent,transparent,transparent,transparent},
    {transparent,transparent,Black,Black,white,white,white,white,white,white,white,white,transparent,transparent,transparent,transparent},
    {transparent,Black,Black,Black,Black,Black,white,white,white,white,white,Black,Black,transparent,transparent,transparent},
    {transparent,Black,Black,Black,Black,Black,Black,white,white,white,Black,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,Black,Black,Black,Black,Black,transparent,transparent,Black,Black,Black,transparent,transparent}
  };
  
  //Ground Tile Colors
  color BrownGroundColors[16][16]
  {
    {Orange,tan,tan,tan,tan,tan,tan,tan,tan,Black,Orange,tan,tan,tan,tan,Orange},
    {tan,Orange,Orange,Orange,Orange,Orange,Orange,Orange,Orange,Black,tan,Orange,Orange,Orange,Orange,Black},
    {tan,Orange,Orange,Orange,Orange,Orange,Orange,Orange,Orange,Black,tan,Orange,Orange,Orange,Orange,Black},
    {tan,Orange,Orange,Orange,Orange,Orange,Orange,Orange,Orange,Black,tan,Orange,Orange,Orange,Orange,Black},
    {tan,Orange,Orange,Orange,Orange,Orange,Orange,Orange,Orange,Black,tan,Black,Orange,Orange,Orange,Black},
    {tan,Orange,Orange,Orange,Orange,Orange,Orange,Orange,Orange,Black,Orange,Black,Black,Black,Black,Orange},
    {tan,Orange,Orange,Orange,Orange,Orange,Orange,Orange,Orange,Black,tan,tan,tan,tan,tan,Black},
    {tan,Orange,Orange,Orange,Orange,Orange,Orange,Orange,Orange,Black,tan,Orange,Orange,Orange,Orange,Black},
    {tan,Orange,Orange,Orange,Orange,Orange,Orange,Orange,Orange,Black,tan,Orange,Orange,Orange,Orange,Black},
    {tan,Orange,Orange,Orange,Orange,Orange,Orange,Orange,Orange,Black,tan,Orange,Orange,Orange,Orange,Black},
    {Black,Black,Orange,Orange,Orange,Orange,Orange,Orange,Black,tan,Orange,Orange,Orange,Orange,Orange,Black},
    {tan,tan,Black,Black,Orange,Orange,Orange,Orange,Black,tan,Orange,Orange,Orange,Orange,Orange,Black},
    {tan,Orange,tan,tan,Black,Black,Black,Black,tan,Orange,Orange,Orange,Orange,Orange,Orange,Black},
    {tan,Orange,Orange,Orange,tan,tan,tan,Black,tan,Orange,Orange,Orange,Orange,Orange,Orange,Black},
    {tan,Orange,Orange,Orange,Orange,Orange,Orange,Black,tan,Orange,Orange,Orange,Orange,Orange,Black,Black},
    {Orange,Black,Black,Black,Black,Black,Black,Orange,tan,Black,Black,Black,Black,Black,Black,Orange},
  };
  //underground
  color BlueGroundColors[16][16]
  {
    {blue,cyan,cyan,cyan,cyan,cyan,cyan,cyan,cyan,Black,blue,cyan,cyan,cyan,cyan,blue},
    {cyan,blue,blue,blue,blue,blue,blue,blue,blue,Black,cyan,blue,blue,blue,blue,Black},
    {cyan,blue,blue,blue,blue,blue,blue,blue,blue,Black,cyan,blue,blue,blue,blue,Black},
    {cyan,blue,blue,blue,blue,blue,blue,blue,blue,Black,cyan,blue,blue,blue,blue,Black},
    {cyan,blue,blue,blue,blue,blue,blue,blue,blue,Black,cyan,Black,blue,blue,blue,Black},
    {cyan,blue,blue,blue,blue,blue,blue,blue,blue,Black,blue,Black,Black,Black,Black,blue},
    {cyan,blue,blue,blue,blue,blue,blue,blue,blue,Black,cyan,cyan,cyan,cyan,cyan,Black},
    {cyan,blue,blue,blue,blue,blue,blue,blue,blue,Black,cyan,blue,blue,blue,blue,Black},
    {cyan,blue,blue,blue,blue,blue,blue,blue,blue,Black,cyan,blue,blue,blue,blue,Black},
    {cyan,blue,blue,blue,blue,blue,blue,blue,blue,Black,cyan,blue,blue,blue,blue,Black},
    {Black,Black,blue,blue,blue,blue,blue,blue,Black,cyan,blue,blue,blue,blue,blue,Black},
    {cyan,cyan,Black,Black,blue,blue,blue,blue,Black,cyan,blue,blue,blue,blue,blue,Black},
    {cyan,blue,cyan,cyan,Black,Black,Black,Black,cyan,blue,blue,blue,blue,blue,blue,Black},
    {cyan,blue,blue,blue,cyan,cyan,cyan,Black,cyan,blue,blue,blue,blue,blue,blue,Black},
    {cyan,blue,blue,blue,blue,blue,blue,Black,cyan,blue,blue,blue,blue,blue,Black,Black},
    {blue,Black,Black,Black,Black,Black,Black,blue,cyan,Black,Black,Black,Black,Black,Black,blue},
  };
  //castle 
  color CastleGroundColors[16][16]
  {
    {Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black},
    {white,white,white,Black,white,white,white,white,white,white,white,Black,white,white,white,white},
    {Lgrey,Dgrey,Dgrey,Black,white,Lgrey,Lgrey,Lgrey,Lgrey,Lgrey,Dgrey,Black,white,Lgrey,Lgrey,Lgrey},
    {Lgrey,Dgrey,Dgrey,Black,white,Lgrey,Lgrey,Lgrey,Lgrey,Lgrey,Dgrey,Black,white,Lgrey,Lgrey,Lgrey},
    {Lgrey,Dgrey,Dgrey,Black,white,Lgrey,Lgrey,Lgrey,Lgrey,Lgrey,Dgrey,Black,white,Lgrey,Lgrey,Lgrey},
    {Lgrey,Dgrey,Dgrey,Black,white,Lgrey,Lgrey,Lgrey,Lgrey,Lgrey,Dgrey,Black,white,Lgrey,Lgrey,Lgrey},
    {Lgrey,Dgrey,Dgrey,Black,white,Lgrey,Lgrey,Lgrey,Lgrey,Lgrey,Dgrey,Black,white,Lgrey,Lgrey,Lgrey},
    {Lgrey,Lgrey,Lgrey,Black,white,Lgrey,Lgrey,Lgrey,Lgrey,Lgrey,Lgrey,Black,white,Lgrey,Lgrey,Black},
    {Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black},
    {white,white,white,white,white,white,white,Black,white,white,white,white,white,white,white,Black},
    {white,Lgrey,Lgrey,Lgrey,Lgrey,Lgrey,Dgrey,Black,white,Lgrey,Lgrey,Lgrey,Lgrey,Lgrey,Dgrey,Black,},
    {white,Lgrey,Lgrey,Lgrey,Lgrey,Lgrey,Dgrey,Black,white,Lgrey,Lgrey,Lgrey,Lgrey,Lgrey,Dgrey,Black,},
    {white,Lgrey,Lgrey,Lgrey,Lgrey,Lgrey,Dgrey,Black,white,Lgrey,Lgrey,Lgrey,Lgrey,Lgrey,Dgrey,Black,},
    {white,Lgrey,Lgrey,Lgrey,Lgrey,Lgrey,Dgrey,Black,white,Lgrey,Lgrey,Lgrey,Lgrey,Lgrey,Dgrey,Black,},
    {white,Lgrey,Lgrey,Lgrey,Lgrey,Lgrey,Dgrey,Black,white,Lgrey,Lgrey,Lgrey,Lgrey,Lgrey,Dgrey,Black,},
    {white,Dgrey,Dgrey,Dgrey,Dgrey,Dgrey,Dgrey,Black,white,Dgrey,Dgrey,Dgrey,Dgrey,Dgrey,Dgrey,Black,},
  };
  //?Block Colors
  color CoinBlockColors[16][16]
  {
    {Black,Orange,Orange,Orange,Orange,Orange,Orange,Orange,Orange,Orange,Orange,Orange,Orange,Orange,Orange,Black},
    {Orange,yellow,yellow,yellow,yellow,yellow,yellow,yellow,yellow,yellow,yellow,yellow,yellow,yellow,yellow,Black},
    {Orange,yellow,Black,yellow,yellow,yellow,yellow,yellow,yellow,yellow,yellow,yellow,yellow,Black,yellow,Black},
    {Orange,yellow,yellow,yellow,yellow,Orange,Orange,Orange,Orange,Orange,yellow,yellow,yellow,yellow,yellow,Black},
    {Orange,yellow,yellow,yellow,Orange,Orange,Black,Black,Black,Orange,Orange,yellow,yellow,yellow,yellow,Black},
    {Orange,yellow,yellow,yellow,Orange,Orange,Black,yellow,yellow,Orange,Orange,Black,yellow,yellow,yellow,Black},
    {Orange,yellow,yellow,yellow,Orange,Orange,Black,yellow,yellow,Orange,Orange,Black,yellow,yellow,yellow,Black},
    {Orange,yellow,yellow,yellow,yellow,Black,Black,yellow,Orange,Orange,Orange,Black,yellow,yellow,yellow,Black},
    {Orange,yellow,yellow,yellow,yellow,yellow,yellow,Orange,Orange,Black,Black,Black,yellow,yellow,yellow,Black}, 
    {Orange,yellow,yellow,yellow,yellow,yellow,yellow,Orange,Orange,Black,yellow,yellow,yellow,yellow,yellow,Black},
    {Orange,yellow,yellow,yellow,yellow,yellow,yellow,yellow,Black,Black,yellow,yellow,yellow,yellow,yellow,Black},
    {Orange,yellow,yellow,yellow,yellow,yellow,yellow,Orange,Orange,yellow,yellow,yellow,yellow,yellow,yellow,Black},
    {Orange,yellow,yellow,yellow,yellow,yellow,yellow,Orange,Orange,Black,yellow,yellow,yellow,yellow,yellow,Black},
    {Orange,yellow,Black,yellow,yellow,yellow,yellow,yellow,Black,Black,yellow,yellow,yellow,Black,yellow,Black},
    {Orange,yellow,yellow,yellow,yellow,yellow,yellow,yellow,yellow,yellow,yellow,yellow,yellow,yellow,yellow,Black},
    {Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black},
  };
  color HitCoinBlockColors[16][16]{
    {Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black},
    {Black,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,Black},
    {Black,brown,Black,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,Black,brown,Black},
    {Black,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,Black},
    {Black,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,Black},
    {Black,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,Black},
    {Black,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,Black},
    {Black,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,Black},
    {Black,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,Black},
    {Black,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,Black},
    {Black,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,Black},
    {Black,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,Black},
    {Black,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,Black},
    {Black,brown,Black,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,Black,brown,Black},
    {Black,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,brown,Black},
    {Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black},
  };
  //hard Block Colors
  color BrownHardBlockColors[16][16]
  {
    {brown,tan,tan,tan,tan,tan,tan,tan,tan,tan,tan,tan,tan,tan,tan,Black},
    {tan,brown,tan,tan,tan,tan,tan,tan,tan,tan,tan,tan,tan,tan,Black,Black},
    {tan,tan,brown,tan,tan,tan,tan,tan,tan,tan,tan,tan,tan,Black,Black,Black},
    {tan,tan,tan,brown,tan,tan,tan,tan,tan,tan,tan,tan,Black,Black,Black,Black},
    {tan,tan,tan,tan,brown,brown,brown,brown,brown,brown,brown,brown,Black,Black,Black,Black},
    {tan,tan,tan,tan,brown,brown,brown,brown,brown,brown,brown,brown,Black,Black,Black,Black},
    {tan,tan,tan,tan,brown,brown,brown,brown,brown,brown,brown,brown,Black,Black,Black,Black},
    {tan,tan,tan,tan,brown,brown,brown,brown,brown,brown,brown,brown,Black,Black,Black,Black},
    {tan,tan,tan,tan,brown,brown,brown,brown,brown,brown,brown,brown,Black,Black,Black,Black},
    {tan,tan,tan,tan,brown,brown,brown,brown,brown,brown,brown,brown,Black,Black,Black,Black},
    {tan,tan,tan,tan,brown,brown,brown,brown,brown,brown,brown,brown,Black,Black,Black,Black},
    {tan,tan,tan,tan,brown,brown,brown,brown,brown,brown,brown,brown,Black,Black,Black,Black},
    {tan,tan,tan,Black,Black,Black,Black,Black,Black,Black,Black,Black,brown,Black,Black,Black},
    {tan,tan,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,brown,Black,Black},
    {tan,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,brown,Black},
    {Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,brown},
  };
  
  //cloud colors
  color CloudColors[16][16]
  {
    {transparent,transparent,transparent,Black,Black,Black,Black,Black,Black,Black,Black,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,Black,white,white,white,white,white,white,white,white,white,cyan,Black,transparent,transparent},
    {transparent,Black,white,white,white,white,white,white,white,white,white,white,white,cyan,Black,transparent},
    {transparent,Black,white,white,white,white,white,white,white,white,white,white,white,cyan,Black,transparent},
    {transparent,Black,white,white,white,white,white,white,white,white,white,white,white,cyan,Black,transparent},
    {Black,white,white,white,white,white,Black,white,white,Black,white,white,white,white,cyan,Black},
    {Black,white,white,white,white,white,Black,white,white,Black,white,white,white,white,cyan,Black},
    {Black,white,white,white,white,white,Black,white,white,Black,white,white,white,white,cyan,Black},
    {Black,white,white,white,white,white,white,white,white,white,white,white,white,white,cyan,Black},
    {Black,white,white,white,white,white,white,white,white,white,white,white,white,white,cyan,Black},
    {Black,white,white,white,white,Black,white,white,white,white,Black,white,white,cyan,cyan,Black},
    {transparent,Black,white,white,white,white,Black,Black,Black,Black,white,white,white,cyan,Black,transparent},
    {transparent,Black,white,white,white,white,white,white,white,white,white,white,white,cyan,Black,transparent},
    {transparent,Black,cyan,white,white,white,white,cyan,cyan,white,white,white,cyan,cyan,Black,transparent},
    {transparent,transparent,Black,cyan,cyan,cyan,cyan,Black,Black,cyan,cyan,cyan,cyan,Black,transparent,transparent},
    {transparent,transparent,transparent,Black,Black,Black,Black,transparent,transparent,Black,Black,Black,Black,transparent,transparent,transparent},
  };
  //coin colors
  color BrightCoinColors[16][16]{
    {transparent,transparent,transparent,transparent,transparent,tan,tan,tan,tan,Black,Black,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,tan,tan,tan,tan,tan,tan,Black,Black,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,tan,tan,brown,brown,tan,tan,Black,Black,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,tan,tan,brown,tan,tan,Black,tan,tan,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,transparent,tan,tan,brown,tan,tan,Black,tan,tan,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,transparent,tan,tan,brown,tan,tan,Black,tan,tan,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,transparent,tan,tan,brown,tan,tan,Black,tan,tan,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,transparent,tan,tan,brown,tan,tan,Black,tan,tan,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,transparent,tan,tan,brown,tan,tan,Black,tan,tan,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,transparent,tan,tan,brown,tan,tan,Black,tan,tan,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,transparent,tan,tan,brown,tan,tan,Black,tan,tan,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,transparent,tan,tan,brown,tan,tan,Black,tan,tan,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,transparent,tan,tan,brown,tan,tan,Black,tan,tan,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,tan,tan,Black,Black,tan,tan,Black,Black,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,tan,tan,tan,tan,tan,tan,Black,Black,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,transparent,tan,tan,tan,tan,Black,Black,transparent,transparent,transparent,transparent,transparent},
  };
  color MidCoinColors[16][16]{
    {transparent,transparent,transparent,transparent,transparent,brown,brown,brown,brown,Black,Black,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,brown,brown,brown,brown,brown,brown,Black,Black,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,brown,brown,brown,brown,brown,brown,Black,Black,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,brown,brown,brown,brown,brown,Black,brown,brown,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,transparent,brown,brown,brown,brown,brown,Black,brown,brown,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,transparent,brown,brown,brown,brown,brown,Black,brown,brown,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,transparent,brown,brown,brown,brown,brown,Black,brown,brown,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,transparent,brown,brown,brown,brown,brown,Black,brown,brown,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,transparent,brown,brown,brown,brown,brown,Black,brown,brown,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,transparent,brown,brown,brown,brown,brown,Black,brown,brown,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,transparent,brown,brown,brown,brown,brown,Black,brown,brown,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,transparent,brown,brown,brown,brown,brown,Black,brown,brown,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,transparent,brown,brown,brown,brown,brown,Black,brown,brown,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,brown,brown,Black,Black,brown,brown,Black,Black,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,brown,brown,brown,brown,brown,brown,Black,Black,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,transparent,brown,brown,brown,brown,Black,Black,transparent,transparent,transparent,transparent,transparent},
  };
  color DarkCoinColors[16][16]{
    {transparent,transparent,transparent,transparent,transparent,DarkBrown,DarkBrown,DarkBrown,DarkBrown,Black,Black,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,DarkBrown,DarkBrown,DarkBrown,DarkBrown,DarkBrown,DarkBrown,Black,Black,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,DarkBrown,DarkBrown,brown,brown,DarkBrown,DarkBrown,Black,Black,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,DarkBrown,DarkBrown,brown,DarkBrown,DarkBrown,Black,DarkBrown,DarkBrown,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,transparent,DarkBrown,DarkBrown,brown,DarkBrown,DarkBrown,Black,DarkBrown,DarkBrown,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,transparent,DarkBrown,DarkBrown,brown,DarkBrown,DarkBrown,Black,DarkBrown,DarkBrown,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,transparent,DarkBrown,DarkBrown,brown,DarkBrown,DarkBrown,Black,DarkBrown,DarkBrown,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,transparent,DarkBrown,DarkBrown,brown,DarkBrown,DarkBrown,Black,DarkBrown,DarkBrown,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,transparent,DarkBrown,DarkBrown,brown,DarkBrown,DarkBrown,Black,DarkBrown,DarkBrown,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,transparent,DarkBrown,DarkBrown,brown,DarkBrown,DarkBrown,Black,DarkBrown,DarkBrown,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,transparent,DarkBrown,DarkBrown,brown,DarkBrown,DarkBrown,Black,DarkBrown,DarkBrown,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,transparent,DarkBrown,DarkBrown,brown,DarkBrown,DarkBrown,Black,DarkBrown,DarkBrown,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,transparent,DarkBrown,DarkBrown,brown,DarkBrown,DarkBrown,Black,DarkBrown,DarkBrown,Black,Black,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,DarkBrown,DarkBrown,Black,Black,DarkBrown,DarkBrown,Black,Black,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,DarkBrown,DarkBrown,DarkBrown,DarkBrown,DarkBrown,DarkBrown,Black,Black,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,transparent,DarkBrown,DarkBrown,DarkBrown,DarkBrown,Black,Black,transparent,transparent,transparent,transparent,transparent},
  };
  //small coin colors
  color SmallBrightCoinColors[16][16]
  {
    {transparent,tan,tan,black,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent},
    {tan,tan,tan,tan,black,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent},
    {tan,tan,tan,tan,black,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent},
    {tan,tan,tan,tan,black,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent},
    {tan,tan,tan,tan,black,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent},
    {tan,tan,tan,tan,black,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent},
    {black,tan,tan,black,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent},
    {transparent,black,black,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent},
    {transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent,transparent},
  };
#pragma endregion
 #pragma region Creating sprites
  //MarioSprites
  Sprite Spr_MarioNeutral(MarioNeutralColors);
  Sprite Spr_MarioDead(MarioDeadColors);
  Sprite Spr_MarioJumping(JumpingColors);
  Sprite Spr_MarioRunningOne(MarioRunningOneColors);
  Sprite Spr_MarioRunningTwo(MarioRunningTwoColors);
  Sprite Spr_MarioRunningThree(MarioRunningThreeColors);
  Sprite Spr_MarioCurrent(MarioNeutralColors);
  //Ground sprites
  Sprite Spr_BrownGroundBlock(BrownGroundColors);
  Sprite Spr_BlueGroundBlock(BlueGroundColors);
  Sprite Spr_CastleGroundBlock(CastleGroundColors);
  //Brick Block Sprite
  Sprite Spr_BrownBrickBlock(BrownBrickBlockColors);
  Sprite Spr_BlueBrickBlock(BlueBrickBlockColors);
  //coin block Sprites
  Sprite Spr_CoinBlock(CoinBlockColors);
  Sprite Spr_HitCoinBlock(HitCoinBlockColors);
  //Cloud Sprite 
  Sprite Spr_Cloud(CloudColors);
  //Goomba Sprites
  Sprite Spr_BrownGoombaLF(BrownGoombaLFColors); //LF = left foot, RF = right foot
  Sprite Spr_BlueGoombaLF(BlueGoombaLFColors);
  
  //Coin Sprites
  Sprite Spr_BrightCoin(BrightCoinColors);
  Sprite Spr_MidCoin(MidCoinColors);
  Sprite Spr_DarkCoin(DarkCoinColors);
  Sprite Spr_CurrCoin(BrightCoinColors);
  #pragma endregion
  //
  //setting Variables
  //
  
  int CoinTick = 0; //tick used for coin Drawing
  int CoinCount = 0; //coin counter
  int ScreenShift = 0; //shifts screen n stuff
  int MarioRunningTick = 0; //tick used for mario run Drawing
  int GoombaWalkTick = 0;
  int asthetics = 1; //used to change astheitcs, 1 = above ground, 2 = underground, 3 = castle
  
  //
  //objects

  //mario
  Object Mario;
  Mario.Hitbox = Rectangle(0,239,32,32);
  Axis MarioFacing = Right;
  
  bool MarioVelocityGoingUp = false;
  //brick block stuff
  

  #pragma region Creating Objects
  //ground block stuff
  Object GroundBlocks[15];
  for (int i = 0; i < (sizeof(GroundBlocks)/sizeof(Object)); i++) 
  {
    if (i < 5) {
      GroundBlocks[i].Hitbox = Rectangle(100+i*32,175,32,32);
    }
    else {
      GroundBlocks[i].Hitbox = Rectangle(200+i*32,175,32,32);
    }
    GroundBlocks[i].OnScreen = GroundBlocks[i].Hitbox.OnScreen();
  }
  //wall blocks
  Object WallBlocks[15];
  for (int i = 0; i < sizeof(WallBlocks)/sizeof(Object); i++) {
    if (i < 5) {
      WallBlocks[i].Hitbox = Rectangle(100+i*32,207,32,32);
    }
    else {
      WallBlocks[i].Hitbox = Rectangle(200+i*32,207,32,32);
    }
    WallBlocks[i].OnScreen = WallBlocks[i].Hitbox.OnScreen();
    
  }
  //hard blocks
  //brick blocks
  Object BrickBlocks[3];
  for (int i = 0; i < sizeof(BrickBlocks)/sizeof(Object); i++) 
  {
    BrickBlocks[i].Hitbox = Rectangle(264+i*32,82,32,32);
    BrickBlocks[i].OnScreen = BrickBlocks[i].Hitbox.OnScreen();
  }
  //Coin blocks
  Object CoinBlocks[3];
  for (int i = 0; i < sizeof(CoinBlocks)/sizeof(Object); i++) 
  {
    CoinBlocks[i].Hitbox = Rectangle(168+i*32,82,32,32);
    CoinBlocks[i].OnScreen = CoinBlocks[i].Hitbox.OnScreen();
  }
  //Cloud Blocks
  Object CloudBlocks[0];
  for (int i = 0; i < sizeof(CloudBlocks)/sizeof(Object); i++) {
    CloudBlocks[i].Hitbox = Rectangle(i*32,200,32,32);
    CloudBlocks[i].OnScreen = CloudBlocks[i].Hitbox.OnScreen();
  }
  //goomba stuff
  Object Goombas[3];
  for (int i = 0; i < sizeof(Goombas)/sizeof(Object); i++) {
    Goombas[i].Hitbox = Rectangle(500+i*75,100,32,32);
    Goombas[i].XVel = -2;
    Goombas[i].OnScreen = Goombas[i].Hitbox.OnScreen();
  }
  //coin stuff
  Object Coins[6];
  for (int i = 0; i < sizeof(Coins)/sizeof(Object); i++) 
  {
    Coins[i].Hitbox = Rectangle(168+i*32,50,32,32);
    Coins[i].OnScreen = Coins[i].Hitbox.OnScreen();
  }
#pragma endregion 
  /*Temporary Code
  
  */
  
  while (Mario.Alive) //(set to if alive later)
  {
    //
    //Mario Movement
    //
    #pragma region Mario Movement
    
    bool Moving = false;
    if (Controller1.Axis4.position() >= 10) //right
    {
      MarioRunningTick++;
      if (MarioRunningTick >= 16) 
      {
        MarioRunningTick = 0;
      }
      MarioFacing = Right;
      Moving = true;
      if (Mario.Hitbox.X > 300) {
        if (Controller1.ButtonY.pressing()) 
        {
          MarioRunningTick+=2;
          ScreenShift+=6;
        }
        else 
        {
          MarioRunningTick++;
          ScreenShift+=4;
        }
      }
      else
      {
        if (Controller1.ButtonY.pressing()) 
        {
          MarioRunningTick+=2;
          Mario.Hitbox.X+=6;
        }
        else 
        {
          MarioRunningTick++;
          Mario.Hitbox.X += 4;
        }
      }
      //screen scrolling (oh no)
      #pragma region ScreenShift
      if (ScreenShift > 0) {
        for (int i = 0; i < sizeof(GroundBlocks)/sizeof(Object); i++) {
          GroundBlocks[i].Hitbox.X -= ScreenShift;
          GroundBlocks[i].OnScreen = GroundBlocks[i].Hitbox.OnScreen();
        }
        for (int i = 0; i < sizeof(WallBlocks)/sizeof(Object); i++) {
          WallBlocks[i].Hitbox.X -= ScreenShift;
          WallBlocks[i].OnScreen = WallBlocks[i].Hitbox.OnScreen();
        }
        for (int i = 0; i < sizeof(Coins)/sizeof(Object); i++) {
          Coins[i].Hitbox.X -= ScreenShift;
          Coins[i].OnScreen = Coins[i].Hitbox.OnScreen();
          
        }
        for (int i = 0; i < sizeof(BrickBlocks)/sizeof(Object); i++) {
          BrickBlocks[i].Hitbox.X -= ScreenShift;
          BrickBlocks[i].OnScreen = BrickBlocks[i].Hitbox.OnScreen();
        }
        for (int i = 0; i < sizeof(CoinBlocks)/sizeof(Object); i++) {
          CoinBlocks[i].Hitbox.X -= ScreenShift;
          CoinBlocks[i].OnScreen = CoinBlocks[i].Hitbox.OnScreen();
        }
        for (int i = 0; i < sizeof(CloudBlocks)/sizeof(Object); i++) {
          CloudBlocks[i].Hitbox.X -= ScreenShift;
          CloudBlocks[i].OnScreen = CloudBlocks[i].Hitbox.OnScreen();
        }
        for (int i = 0; i < sizeof(Goombas)/sizeof(Object); i++) {
          Goombas[i].Hitbox.X -= ScreenShift;
          Goombas[i].OnScreen = Goombas[i].Hitbox.OnScreen();
        }
        ScreenShift = 0;
      }
      #pragma endregion
    }
    else if (Controller1.Axis4.position() <= -10) //left
    {
      MarioRunningTick++;
      if (MarioRunningTick >= 16) 
      {
        MarioRunningTick = 0;
      }
      if (Controller1.ButtonY.pressing()) 
      {
        MarioRunningTick+=2;
        Mario.Hitbox.X-=6;
      }
      else 
      {
        MarioRunningTick++;
        Mario.Hitbox.X -= 4;
      }
      MarioFacing = Left;
      Moving = true;
    }
    else 
    {
      MarioRunningTick = 0;
      Spr_MarioCurrent = Spr_MarioNeutral;
    }
    if (!Mario.Jumping && (Controller1.ButtonA.pressing() || Controller1.ButtonB.pressing())) //time for Mario.Jumping and gravity! (Oh no)
    {
      Mario.Jumping = true;
      Mario.YVel = 3;
      MarioVelocityGoingUp = true;
    }
    if (Moving) 
    {
      //seting mario sprites
      if (!Mario.Jumping) 
      {
        if (MarioRunningTick < 4) 
        {
          Spr_MarioCurrent = Spr_MarioRunningOne;
        }
        else if (MarioRunningTick < 8) 
        {
          Spr_MarioCurrent = Spr_MarioRunningTwo;
        }
        else if (MarioRunningTick < 12) 
        {
          Spr_MarioCurrent = Spr_MarioRunningOne;
        }
        else 
        {
          Spr_MarioCurrent = Spr_MarioRunningThree;
        }
      }
      //Wall Colision
      for (int i = 0; i < sizeof(WallBlocks)/sizeof(Object); i++) 
      {
        if (WallBlocks[i].OnScreen && Mario.Hitbox.Intersects(WallBlocks[i].Hitbox)) 
        {
          if (Mario.Hitbox.XMiddle() < WallBlocks[i].Hitbox.XMiddle()) 
          {
            Mario.Hitbox.X = WallBlocks[i].Hitbox.Left()-33;
          }
          else
          {
            Mario.Hitbox.X = WallBlocks[i].Hitbox.Right()+1;
          }
        }
      }
      //Bouncing Mario Back Onto Screen
      if (Mario.Hitbox.X < 0) 
      {
        Mario.Hitbox.X = 0;  
      }
      else if(Mario.Hitbox.X+Mario.Hitbox.Width > 479)
      {
        Mario.Hitbox.X = 479-Mario.Hitbox.Width;
      }
      //Mario Brick Fixing
    }
    #pragma endregion
    if (Mario.Hitbox.Y+Mario.Hitbox.Height > 239)  
    {
      Mario.Jumping = false;
      Mario.Hitbox.Y = 239-Mario.Hitbox.Height;
      Mario.YVel = 0;
    }
    #pragma region MarioGravity 
    if (Mario.Jumping) 
    {
      Spr_MarioCurrent = Spr_MarioJumping;
      Mario.Hitbox.Y-=Mario.YVel;
      if(MarioVelocityGoingUp)
      {
        Mario.YVel+=2;
        if (Mario.YVel >= 15) 
        {
          MarioVelocityGoingUp = false;
        }
      }
      else 
      {
        Mario.YVel-=2;
      }
      if (Mario.YVel < -16) {
        Mario.YVel = -15;
      }
      for (int i = 0; i < (sizeof(GroundBlocks)/sizeof(Object)); i++) 
      {
        if (Mario.Hitbox.Left() < GroundBlocks[i].Hitbox.Right() && Mario.Hitbox.Right() > GroundBlocks[i].Hitbox.Left()) //if mario is in x pos within a block
        {
          if (Mario.Hitbox.Bottom() > GroundBlocks[i].Hitbox.Top() && Mario.Hitbox.Bottom() < GroundBlocks[i].Hitbox.YMiddle()) //landing on top of a block
          {
            Mario.Jumping = false;
            Mario.Hitbox.Y = GroundBlocks[i].Hitbox.Top() - Mario.Hitbox.Height;
            Mario.YVel = 0;
          }
          else if (Mario.Hitbox.Top() > GroundBlocks[i].Hitbox.YMiddle() && Mario.Hitbox.Top() < GroundBlocks[i].Hitbox.Bottom()) //hitting bottom of a block
          {
            if(Mario.YVel > 0)
            {
              Mario.YVel = 0;
            }
            MarioVelocityGoingUp = false;
          }
          

          
        }
      }
      for (int i = 0; i < sizeof(BrickBlocks)/sizeof(Object); i++) 
      {
        if (Mario.Hitbox.Left() < BrickBlocks[i].Hitbox.Right() && Mario.Hitbox.Right() > BrickBlocks[i].Hitbox.Left()) //if mario is in x pos within a block
        {
          if (Mario.Hitbox.Bottom() > BrickBlocks[i].Hitbox.Top() && Mario.Hitbox.Bottom() < BrickBlocks[i].Hitbox.YMiddle()) //landing on top of a block
          {
            Mario.InAir = false;
            Mario.Jumping = false;
            Mario.Hitbox.Y = BrickBlocks[i].Hitbox.Top() - Mario.Hitbox.Height;
            Mario.YVel = 0;
             
          }
          else if (Mario.Hitbox.Top() > BrickBlocks[i].Hitbox.YMiddle() && Mario.Hitbox.Top() < BrickBlocks[i].Hitbox.Bottom()) //hitting bottom of a block
          {
            if(Mario.YVel > 0)
            {
              Mario.YVel = 0;
            }
            MarioVelocityGoingUp = false;
            if (!BrickBlocks[i].InAir) {
              BrickBlocks[i].InAir = true;
            }
          }
          
        }
      }
      for (int i = 0; i < sizeof(CoinBlocks)/sizeof(Object); i++) 
      {
        if (Mario.Hitbox.Left() < CoinBlocks[i].Hitbox.Right() && Mario.Hitbox.Right() > CoinBlocks[i].Hitbox.Left()) //if mario is in x pos within a block
        {
          if (Mario.Hitbox.Bottom() > CoinBlocks[i].Hitbox.Top() && Mario.Hitbox.Bottom() < CoinBlocks[i].Hitbox.YMiddle()) //landing on top of a block
          {
            Mario.InAir = false;
            Mario.Jumping = false;
            Mario.Hitbox.Y = CoinBlocks[i].Hitbox.Top() - Mario.Hitbox.Height;
            Mario.YVel = 0;
          }
          else if (Mario.Hitbox.Top() > CoinBlocks[i].Hitbox.YMiddle() && Mario.Hitbox.Top() < CoinBlocks[i].Hitbox.Bottom()) //hitting bottom of a block
          {
            if(Mario.YVel > 0)
            {
              Mario.YVel = 0;
            }
            MarioVelocityGoingUp = false;
            if (CoinBlocks[i].Alive && !CoinBlocks[i].InAir) {
              CoinBlocks[i].InAir = true;
              CoinCount++;
            }
          }
          
        }
      }
    
      for (int i = 0; i < (sizeof(CloudBlocks)/sizeof(Object)); i++) 
      {
        if (!MarioVelocityGoingUp && Mario.Hitbox.Left() < CloudBlocks[i].Hitbox.Right() && Mario.Hitbox.Right() > CloudBlocks[i].Hitbox.Left()) //if mario is in x pos within a block
        {
        if (Mario.Hitbox.Bottom() > CloudBlocks[i].Hitbox.Top() && Mario.Hitbox.Bottom() < CloudBlocks[i].Hitbox.YMiddle()) //landing on top of a block
          { 
          Mario.Jumping = false;
          Mario.Hitbox.Y = CloudBlocks[i].Hitbox.Top() - Mario.Hitbox.Height;
          Mario.YVel = 0;
          }
                
        }
      }
    }
    else if (Mario.Hitbox.Bottom() < 239) //falling logic
    {
      Mario.InAir = true;
      for (int i = 0; i < (sizeof(GroundBlocks)/sizeof(Object)); i++) 
      {
        if (Mario.Hitbox.Left() < GroundBlocks[i].Hitbox.Right() && Mario.Hitbox.Right() > GroundBlocks[i].Hitbox.Left()) //if mario is in x pos within a block
        {
          if (Mario.Hitbox.Bottom() == GroundBlocks[i].Hitbox.Top()) 
          {
            Mario.Hitbox.Y = GroundBlocks[i].Hitbox.Top()-32;
            Mario.InAir = false;  
          }
        }
      }
      for (int i = 0; i < (sizeof(BrickBlocks)/sizeof(Object)); i++) 
      {
        if (BrickBlocks[i].OnScreen && Mario.Hitbox.Left() < BrickBlocks[i].Hitbox.Right() && Mario.Hitbox.Right() > BrickBlocks[i].Hitbox.Left()) //if mario is in x pos within a block
        {
          if (Mario.Hitbox.Bottom() == BrickBlocks[i].Hitbox.Top()) 
          {
            Mario.Hitbox.Y = BrickBlocks[i].Hitbox.Top()-32;
            Mario.InAir = false;  
          }
        }
      }
      for (int i = 0; i < (sizeof(CoinBlocks)/sizeof(Object)); i++) 
      {
        if (CoinBlocks[i].OnScreen && Mario.Hitbox.Left() < CoinBlocks[i].Hitbox.Right() && Mario.Hitbox.Right() > CoinBlocks[i].Hitbox.Left()) //if mario is in x pos within a block
        {
          if (Mario.Hitbox.Bottom() == CoinBlocks[i].Hitbox.Top()) 
          {
            Mario.Hitbox.Y = CoinBlocks[i].Hitbox.Top()-32;
            Mario.InAir = false;  
          }
        }
      }
      for (int i = 0; i < (sizeof(CloudBlocks)/sizeof(Object)); i++) 
      {
        if (CloudBlocks[i].OnScreen && Mario.Hitbox.Left() < CloudBlocks[i].Hitbox.Right() && Mario.Hitbox.Right() > CloudBlocks[i].Hitbox.Left()) //if mario is in x pos within a block
        {
          if (Mario.Hitbox.Bottom() == CloudBlocks[i].Hitbox.Top()) 
          {
            Mario.Hitbox.Y = CloudBlocks[i].Hitbox.Top()-32;
            Mario.InAir = false;  
          }
        }
      }
      if (Mario.InAir) 
      {
        Mario.YVel = 0;
        Mario.Jumping = true;
        MarioVelocityGoingUp = false;
      }
    }
    #pragma endregion
    
    //BrickBlock Bounce Logic
    for (int i = 0; i < sizeof(BrickBlocks)/sizeof(Object); i++) {
      if (BrickBlocks[i].InAir) {
        if (BrickBlocks[i].YVel != 0) {
          BrickBlocks[i].YVel += 2;
        }
        else {
          BrickBlocks[i].YVel = -7;
        }
        if (BrickBlocks[i].YVel == 9) {
          BrickBlocks[i].InAir = false;
          BrickBlocks[i].YVel = 0;
        }
        else{
          BrickBlocks[i].MoveVel();
        }
      }   
    }
    //CoinBlock Bounce Logic
    for (int i = 0; i < sizeof(CoinBlocks)/sizeof(Object); i++) {
      if (CoinBlocks[i].InAir) {
        if (CoinBlocks[i].YVel != 0) {
          CoinBlocks[i].YVel += 2;
        }
        else {
          CoinBlocks[i].YVel = -7;
        }
        if (CoinBlocks[i].YVel == 9) {
          CoinBlocks[i].InAir = false;
          CoinBlocks[i].Alive = false;
          CoinBlocks[i].YVel = 0;
        }
        else{
          CoinBlocks[i].MoveVel();
        }
      }
      
    }
    //Coin logic
    for (int i = 0; i < sizeof(Coins)/sizeof(Object); i++) 
    {
      if (Coins[i].OnScreen && Coins[i].Alive) 
      {
        if (Mario.Hitbox.Intersects(Coins[i].Hitbox)) {
          CoinCount++;
          Coins[i].Alive = false;
        }
        else 
        {
          for (int j = 0; j < sizeof(BrickBlocks)/sizeof(Object); j++) 
          {
            if (BrickBlocks[j].InAir && BrickBlocks[j].Hitbox.Intersects(Coins[i].Hitbox)) 
            {
              CoinCount++;
              Coins[i].Alive = false;
            }
          }
          for (int j = 0; j < sizeof(CoinBlocks)/sizeof(Object); j++) 
          {
            if (CoinBlocks[j].InAir && CoinBlocks[j].Hitbox.Intersects(Coins[i].Hitbox)) 
            {
              CoinCount++;
              Coins[i].Alive = false;
            }
          }
        }
      }
    }
    //
    //goomba logic
    //
    for (int i = 0; i < sizeof(Goombas)/sizeof(Object); i++) 
    {
      if (Goombas[i].OnScreen && Goombas[i].Alive) 
      {
        //If hitting Mario
        if (Mario.Hitbox.Intersects(Goombas[i].Hitbox)) 
        {
          if (Mario.Hitbox.Bottom() < Goombas[i].Hitbox.YMiddle()) 
          {
            MarioVelocityGoingUp = true;
            Mario.YVel = 13;
            Goombas[i].Alive = false;
          } 
          else 
          {
            Mario.Alive = false;
          }
        }
        //Block Checks
        bool GoombaInAirTemp = true;
        for (int j = 0; j < (sizeof(GroundBlocks)/sizeof(Object)); j++) 
        {
          if (GroundBlocks[j].OnScreen && GroundBlocks[j].Hitbox.EqIntersects(Goombas[i].Hitbox)) 
          {
            
            if (Goombas[i].Hitbox.Bottom() < GroundBlocks[j].Hitbox.YMiddle()) 
            {
              
              if (Goombas[i].InAir) 
              {
                Goombas[i].InAir = false;
                Goombas[i].YVel = 0;
                Goombas[i].Hitbox.Y = GroundBlocks[j].Hitbox.Top()-32;
                GoombaInAirTemp = false;
              }
              else
              {
                GoombaInAirTemp = false;
              }
            }
            else
            {
              if (Goombas[i].XVel == -2) 
              {
                Goombas[i].XVel = 2;
              }
              else
              {
                Goombas[i].XVel = -2;
              }
            }
          }
        }
        for (int j = 0; j < (sizeof(BrickBlocks)/sizeof(Object)); j++) 
        {
          if (BrickBlocks[j].OnScreen && BrickBlocks[j].Hitbox.EqIntersects(Goombas[i].Hitbox)) 
          {
            if (Goombas[i].Hitbox.Bottom() < BrickBlocks[j].Hitbox.YMiddle()) 
            {
              if (Goombas[i].InAir) 
              {
                Goombas[i].InAir = false;
                Goombas[i].YVel = 0;
                Goombas[i].Hitbox.Y = BrickBlocks[j].Hitbox.Top()-32;
                GoombaInAirTemp = false;
              }
              else
              {
                GoombaInAirTemp = false;
              }
            }
            else
            {
              if (Goombas[i].XVel == -2) 
              {
                Goombas[i].XVel = 2;
              }
              else
              {
                Goombas[i].XVel = -2;
              }
            }
          }
        }
        for (int j = 0; j < (sizeof(CoinBlocks)/sizeof(Object)); j++) 
        {
          if (CoinBlocks[j].OnScreen && CoinBlocks[j].Hitbox.EqIntersects(Goombas[i].Hitbox)) 
          {
            if (Goombas[i].Hitbox.Bottom() < CoinBlocks[j].Hitbox.YMiddle()) 
            {
              if (Goombas[i].InAir) 
              {
                Goombas[i].InAir = false;
                Goombas[i].YVel = 0;
                Goombas[i].Hitbox.Y = CoinBlocks[j].Hitbox.Top()-32;
                GoombaInAirTemp = false;
              }
              else
              {
                GoombaInAirTemp = false;
              }
            }
            else
            {
              if (Goombas[i].XVel == -2) 
              {
                Goombas[i].XVel = 2;
              }
              else
              {
                Goombas[i].XVel = -2;
              }
            }
          }
        }
        for (int j = 0; j < (sizeof(CloudBlocks)/sizeof(Object)); j++) 
        {
          if (CloudBlocks[j].OnScreen && CloudBlocks[j].Hitbox.EqIntersects(Goombas[i].Hitbox)) 
          {
            if (Goombas[i].Hitbox.Bottom() < CloudBlocks[j].Hitbox.YMiddle()) 
            {
              if (Goombas[i].InAir) 
              {
                Goombas[i].InAir = false;
                Goombas[i].YVel = 0;
                Goombas[i].Hitbox.Y = CloudBlocks[j].Hitbox.Top()-32;
                GoombaInAirTemp = false;
              }
              else
              {
                GoombaInAirTemp = false;
              }
            }
          }
        }
        for (int i = 0; i < sizeof(WallBlocks)/sizeof(Object); i++) 
        {
          if (WallBlocks[i].Hitbox.Intersects(Goombas[i].Hitbox)) 
          {
            if (Goombas[i].XVel == -2) 
            {
              Goombas[i].XVel = 2;
            }
            else
            {
              Goombas[i].XVel = -2;
            }
          }
        }
        Goombas[i].InAir = GoombaInAirTemp;
        if (Goombas[i].InAir) 
        {
          Goombas[i].YVel += 2;
        }
        //Movement
        Goombas[i].MoveVel();
      }
    }
    //
    //controller screen printing (debugging) WILL Cause lag
    /*
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1,1);
    Controller1.Screen.print(Mario.YVel);
    
    
    */
    //Drawing (loaded Top -> Bottom)
    //
    #pragma region Drawing
    Brain.Screen.render();
    #pragma region Drawing Background
    if(asthetics == 1)
    {
      Brain.Screen.setPenColor(cyan);
      Brain.Screen.setFillColor(cyan);
    }
    else if(asthetics == 2 || asthetics == 3)
    {
      Brain.Screen.setPenColor(black);
      Brain.Screen.setFillColor(black);
    }
    Brain.Screen.drawRectangle(0,0,479,239);
    #pragma endregion
    #pragma region Drawing Coins
    CoinTick++;
    if (CoinTick > 14) 
    {
      CoinTick = 0;
    }
    if (CoinTick == 0) {
      Spr_CurrCoin = Spr_BrightCoin;
    }
    else if (CoinTick == 10 || CoinTick == 14) {
      Spr_CurrCoin = Spr_MidCoin;
    }
    else if (CoinTick == 12) {
      Spr_CurrCoin = Spr_DarkCoin;
    }
    for(int i = 0; i < sizeof(Coins)/sizeof(Object); i++) 
    {
      if (Coins[i].OnScreen && Coins[i].Alive) 
      {
        Spr_CurrCoin.DrawUpscaled(Vector2(Coins[i].Hitbox.Left(),Coins[i].Hitbox.Top()),2);
      }
    }
    #pragma endregion
    #pragma region Drawing GroundBlocks
    if(asthetics == 1)
    {
      for (int i = 0; i < sizeof(GroundBlocks)/sizeof(Object); i++) 
      {
        if (GroundBlocks[i].OnScreen) {
          Spr_BrownGroundBlock.DrawUpscaled(Vector2(GroundBlocks[i].Hitbox.Left(),GroundBlocks[i].Hitbox.Top()),2);
        }
      }
    }
    else if (asthetics == 2)
    {
      for (int i = 0; i < sizeof(GroundBlocks)/sizeof(Object); i++) 
      {
        if (GroundBlocks[i].OnScreen) {
          Spr_BlueGroundBlock.DrawUpscaled(Vector2(GroundBlocks[i].Hitbox.Left(),GroundBlocks[i].Hitbox.Top()),2);
        }
      }
    }
    else if(asthetics == 3)
    {
      for (int i = 0; i < sizeof(GroundBlocks)/sizeof(Object); i++) 
      {
        if (GroundBlocks[i].OnScreen) {
          Spr_CastleGroundBlock.DrawUpscaled(Vector2(GroundBlocks[i].Hitbox.Left(),GroundBlocks[i].Hitbox.Top()),2);
        }
      }
    }
    #pragma endregion
    #pragma region Drawing BrickBlocks
    
    if(asthetics == 2)
    {
      for (int i = 0; i < sizeof(BrickBlocks)/sizeof(Object); i++) 
      {
        if (BrickBlocks[i].OnScreen) {
          Spr_BlueBrickBlock.DrawUpscaled(Vector2(BrickBlocks[i].Hitbox.Left(),BrickBlocks[i].Hitbox.Top()),2);
        }
      }
    }
    else
    {
      for (int i = 0; i < sizeof(BrickBlocks)/sizeof(Object); i++) 
      {
        if (BrickBlocks[i].OnScreen) {
          Spr_BrownBrickBlock.DrawUpscaled(Vector2(BrickBlocks[i].Hitbox.Left(),BrickBlocks[i].Hitbox.Top()),2);
        }
      }
    }
    #pragma endregion 
    //Drawing CloudBlocks
    for (int i = 0; i < sizeof(CloudBlocks)/sizeof(Object); i++) 
    {
      if (CloudBlocks[i].OnScreen) {
        Spr_Cloud.DrawUpscaled(Vector2(CloudBlocks[i].Hitbox.Left(),CloudBlocks[i].Hitbox.Top()),2);
      }
      
    }
    //Drawing CoinBlocks
    for (int i = 0; i < sizeof(CoinBlocks)/sizeof(Object); i++) 
    {
      if (CoinBlocks[i].OnScreen) {
        if (CoinBlocks[i].Alive) {
          Spr_CoinBlock.DrawUpscaled(Vector2(CoinBlocks[i].Hitbox.Left(),CoinBlocks[i].Hitbox.Top()),2);
        }
        else {
          Spr_HitCoinBlock.DrawUpscaled(Vector2(CoinBlocks[i].Hitbox.Left(),CoinBlocks[i].Hitbox.Top()),2); //later use dead coin block sprite 
        }
        
      }
    }
    
    
    

    #pragma region drawing wall blocks
    for (int i = 0; i < sizeof(WallBlocks)/sizeof(Object); i++) 
    {
      if(asthetics == 1)
      {
        for (int i = 0; i < sizeof(WallBlocks)/sizeof(Object); i++) 
        {
          if (WallBlocks[i].OnScreen) {
            Spr_BrownGroundBlock.DrawUpscaled(Vector2(WallBlocks[i].Hitbox.Left(),WallBlocks[i].Hitbox.Top()),2);
          }
        }
      }
      else if (asthetics == 2)
      {
        for (int i = 0; i < sizeof(WallBlocks)/sizeof(Object); i++) 
        {
          if (WallBlocks[i].OnScreen) {
            Spr_BlueGroundBlock.DrawUpscaled(Vector2(WallBlocks[i].Hitbox.Left(),WallBlocks[i].Hitbox.Top()),2);
          }
        }
      }
      else if(asthetics == 3)
      {
        for (int i = 0; i < sizeof(WallBlocks)/sizeof(Object); i++) 
        {
          if (WallBlocks[i].OnScreen) {
            Spr_CastleGroundBlock.DrawUpscaled(Vector2(WallBlocks[i].Hitbox.Left(),WallBlocks[i].Hitbox.Top()),2);
          }
        }
      }
    }
    #pragma endregion 
    #pragma region Drawing Goombas
    GoombaWalkTick++;
    if (GoombaWalkTick > 10) {
      GoombaWalkTick = 0;
    }
    
    if(asthetics == 2)
    {
      for (int i = 0; i < sizeof(Goombas)/sizeof(Object); i++) 
      {
        if (Goombas[i].OnScreen && Goombas[i].Alive) 
        {
          if (GoombaWalkTick < 6) {
            Spr_BlueGoombaLF.DrawUpscaled(Vector2(Goombas[i].Hitbox.Left(),Goombas[i].Hitbox.Top()),2);
          }
          else
          {
            Spr_BlueGoombaLF.DrawUpscaleFlipped(Vector2(Goombas[i].Hitbox.Left(),Goombas[i].Hitbox.Top()),2);
          }
        }
      }
    }
    else
    {
      for (int i = 0; i < sizeof(Goombas)/sizeof(Object); i++) 
      {
        if (Goombas[i].OnScreen && Goombas[i].Alive) 
        {
          if (GoombaWalkTick < 6) {
            Spr_BrownGoombaLF.DrawUpscaled(Vector2(Goombas[i].Hitbox.Left(),Goombas[i].Hitbox.Top()),2);
          }
          else
          {
            Spr_BrownGoombaLF.DrawUpscaleFlipped(Vector2(Goombas[i].Hitbox.Left(),Goombas[i].Hitbox.Top()),2);
          }
        }
      }
    }
    #pragma endregion
    //Drawing Mario
    if (MarioFacing == Left) 
    {
      Spr_MarioCurrent.DrawUpscaleFlipped(Vector2(Mario.Hitbox.X,Mario.Hitbox.Y),2);  
    }
    else if (MarioFacing == Right) 
    {
      Spr_MarioCurrent.DrawUpscaled(Vector2(Mario.Hitbox.X,Mario.Hitbox.Y),2);  
    }
    #pragma endregion
    wait(0.02,seconds); //delay
  }
  //
  //MarioDeath
  //
  Spr_MarioCurrent = Spr_MarioDead;
  Mario.YVel = -8;
  while (Mario.Hitbox.Top() < 239) 
  {
    Brain.Screen.render();
    Brain.Screen.setPenColor(cyan);
    Brain.Screen.setFillColor(cyan);
    Brain.Screen.drawRectangle(0,0,479,239);
    Mario.YVel+=2;
    Mario.Hitbox.Y += Mario.YVel;
    Spr_MarioCurrent.DrawUpscaled(Vector2(Mario.Hitbox.X,Mario.Hitbox.Y),2);
    wait(0.1,seconds);
  }
  vexSystemExitRequest();
}