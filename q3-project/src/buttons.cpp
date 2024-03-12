/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       tetris.cpp                                             */
/*    Author:       james, mitchellg2026                                                     */
/*    Created:      Sun Mar 24 2019                                           */
/*    Modified:     Thu Mar 12 2024                                        */
/*    Description:  Implementation for buttons class                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"
#include "buttons.h"

using namespace vex;

button_ui::button_ui() {
  // register events
  _enabled = false;
  event::init( brain::_getIndex(), (1 << static_cast<uint32_t>(brain::tEventType::EVENT_LCD_PRESSED)) , _screenPress  , static_cast<void *>(this) );
  event::init( brain::_getIndex(), (1 << static_cast<uint32_t>(brain::tEventType::EVENT_LCD_RELEASED)), _screenRelease, static_cast<void *>(this) );
};
  
button_ui::~button_ui() {
};
    
void
button_ui::addButton( int x, int y, int width, int height, vex::color color, const char *text, bool (* callback)( int, bool, int ), int id, bool toggle ) {
  button b;

  b.index  = _buttons.empty() ? 0 : _buttons.back().index + 1;
  b.xpos   = x;
  b.ypos   = y;
  b.width  = width;
  b.height = height;
  b.color  = color;
  b.text   = text;
  b.state  = false;
  b.id     = id;
  b.toggle = toggle;
  b.callback = callback;

  _buttons.push_back(b);
}

void
button_ui::enable() {
  _enabled = true;
}

void
button_ui::disable() {
  _enabled = false;
}

void
button_ui::display() {
  _drawButtons();
}


void
button_ui::clear() {
  _brain.Screen.clearScreen();
}

void
button_ui::removeAll() {
  _brain.Screen.setPenColor( black );
  for ( auto &b: _buttons) {
    _brain.Screen.drawRectangle( b.xpos, b.ypos, b.width, b.height, black );
  }
  _buttons.clear();
}

int 
button_ui::_findButton(  button & b, int32_t xpos, int32_t ypos ) {
  if( xpos < b.xpos || xpos > (b.xpos + b.width) )
    return false;

  if( ypos < b.ypos || ypos > (b.ypos + b.height) )
    return false;
      
  return true;
}
       
void
button_ui::_drawButton( button & b, bool bHighlight ) {
  vex::color textColor( black );

  if( bHighlight ) {
    _brain.Screen.drawRectangle( b.xpos, b.ypos, b.width, b.height, vex::color( 0x808080 ) );
    textColor = vex::color( 0x808080 );
  }
  else {
    if( b.state == true ) {
      _brain.Screen.drawRectangle( b.xpos, b.ypos, b.width, b.height, b.color );
      textColor = b.color;
    }
    else
      _brain.Screen.drawRectangle( b.xpos, b.ypos, b.width, b.height, black );          
  }
  _brain.Screen.drawRectangle( b.xpos, b.ypos, b.width, b.height, vex::color::transparent );
  _brain.Screen.setFillColor( textColor );
  _brain.Screen.printAt( b.xpos + b.width/2 - _brain.Screen.getStringWidth(b.text)/2, b.ypos + b.height/2, b.text );
}

void
button_ui::_drawButtons() {
  _brain.Screen.setPenColor( white );
  _brain.Screen.setFillColor( black );
  for ( auto & b: _buttons) {
      _drawButton( b, false );
  }      
}

void
button_ui::_screenPress( void *arg ) {
  if( arg == NULL)
    return;
  
  button_ui *instance = static_cast<button_ui *>(arg);

  if(!instance->_enabled)
    return;

  int xpos = instance->_brain.Screen.xPosition();
  int ypos = instance->_brain.Screen.yPosition();
  
  for ( auto &b: instance->_buttons) {
      if( instance->_findButton( b, xpos, ypos ) ) {
        if( b.toggle == true ) 
          b.state = (b.state == true) ? false : true;
        else
          b.state = true;
        instance->_drawButton(b, true);
        b.callback( b.index, b.state, b.id );
        return;
      }
  }      
}

void 
button_ui::_screenRelease(  void *arg ) {
  if( arg == NULL)
    return;

  button_ui *instance = static_cast<button_ui *>(arg);

  if(!instance->_enabled)
    return;

  bool enabled = true;

  for ( auto &b: instance->_buttons) {
    if(b.toggle == false) {
      if( b.state == true ) {
        b.state = false;
        enabled = b.callback( b.index, b.state, b.id );
      }
    }
  }
  
  if( !enabled ) {
    instance->disable();
  }
  else {
    instance->_drawButtons();
  }
}