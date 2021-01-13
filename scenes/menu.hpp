#include "lib/object.h"
#include "lib/components/sprite.hpp"
#include "lib/components/windowRender.hpp"
#include "lib/components/text.hpp"
#include "components/center.hpp"
#include <ncurses.h>

class Menu : public Scene {
  Transform* illustrationPos;

  void create() {
    auto illustration = root()->addChild<Object>("bg");
    illustration->addComponent<Sprite>("./sprites/mainmenu/main.xml");
    illustration->addComponent<Center>();

    /*auto btn = root()->addChild<Object>("btn");
    btn->getTransform()->setAbsolutePosition({5, 5});
    btn->addComponent<WindowRender>(10, 2);*/
  }

  void update() {
    /*unsigned x, y;
    getmaxyx(stdscr, y, x);
    illustrationPos->xy(x/2, y/2 - 5);*/
  }
};
