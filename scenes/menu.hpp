#include "lib/object.hpp"
#include "lib/components/transform.hpp"
#include "lib/components/sprite.hpp"
#include "lib/components/windowRender.hpp"
#include "lib/components/text.hpp"
#include <ncurses.h>

class Menu : public Scene {
  Transform* illustrationPos;

  void create() {
    /*auto illustration = base().addChild<Object>("bg");
    illustrationPos = illustration->addComponent<Transform>(10, 10);
    illustration->addComponent<Sprite>("./sprites/mainmenu/main.xml");*/

    auto btn = base().addChild<Object>("btn");
    btn->addComponent<Transform>(5, 5);
    btn->addComponent<WindowRender>(10, 2);

    auto btnText = btn->addChild<Object>("text");
    text->addComponent
  }

  void update() {
    /*unsigned x, y;
    getmaxyx(stdscr, y, x);
    illustrationPos->xy(x/2, y/2 - 5);*/
  }
};