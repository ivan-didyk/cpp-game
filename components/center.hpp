#include "lib/object.h"

class Center : public Component {
  void update() {
    unsigned x, y;
    getmaxyx(stdscr, y, x);
    getTransform()->setAbsolutePosition({ x/2, y/2 });
  }
};