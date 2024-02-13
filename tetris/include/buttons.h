#include "vex.h"

#ifndef _BUTTON_UI_
#define _BUTTON_UI_

namespace vex {
  class button_ui : public vex::brain  {
    private:
       typedef struct _button {
          int    index;
          int    xpos;
          int    ypos;
          int    width;
          int    height;
          int    id;
          bool   state;
          bool   toggle;
          vex::color color;
          bool   (* callback)(int, bool, int);
          const char   *text;
       } button;
       
        vex::brain _brain;
        bool       _enabled;
        
        std::vector<button_ui::button> _buttons;

        int _findButton(  button & b, int32_t xpos, int32_t ypos );
        void _drawButton( button & b, bool bHighlight );
        void _drawButtons();
        static void _screenPress( void *arg );
        static void _screenRelease(  void *arg );

    public:
      button_ui();
      ~button_ui();
          
      void addButton( int x, int y, int width, int height, vex::color color, const char *text, bool (* callback)( int, bool, int ), int id, bool toggle = false );
      void enable();
      void disable();      
      void display();
      void removeAll();
      void clear();
  };
}

#endif