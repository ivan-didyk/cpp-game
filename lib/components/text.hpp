#pragma once
#include "lib/object.h"

/**
 * A text rendering stuff
 * @todo Add FoRmAtTeD text!
 */
class Text : public Render {
  string txt;

  void draw() {
    mvprintw(getTransform()->y(), getTransform()->x(), "%s", txt.c_str());
  }
 public:
  Text() { txt="Text..."; }
  Text(string t) { txt=t; }
  void text(string newText) { txt=newText; }
  string text() { return txt; }
};