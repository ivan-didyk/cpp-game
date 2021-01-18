#pragma once
#include "lib/object.h"

/**
 * A text rendering stuff
 * @todo Add FoRmAtTeD text!
 */
class Text : public Render {
  string txt;
  int color = COLOR_WHITE;

  void draw() {
    attron(COLOR_PAIR(color));
    mvprintw(getTransform()->y(), getTransform()->x(), "%s", txt.c_str());
    attroff(COLOR_PAIR(color));
  }
 public:
  Text() { txt="Text..."; }
  Text(string t) { txt=t; }
  Text(string t, int clr) { txt = t; color = clr; }
  void text(string newText) { txt=newText; }
  string text() { return txt; }
};