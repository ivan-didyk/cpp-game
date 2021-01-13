#include "lib/object.h"

/**
 * A text rendering stuff
 * @todo Add FoRmAtTeD text!
 */
class Text : public Render {
  Transform* transform;
  string txt;

  void start() {
    transform = getComponent<Transform>();
  }

  void draw() {
    move(transform->y(), transform->x());
    printw("%s", txt.c_str());
  }
 public:
  Text() { txt="Text..."; }
  Text(string t) { txt=t; }
  void text(string newText) { txt=newText; }
  string text() { return txt; }
};