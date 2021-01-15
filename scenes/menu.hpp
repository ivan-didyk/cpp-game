#include "lib/object.h"
#include "lib/components/sprite.hpp"
#include "lib/components/windowRender.hpp"
#include "lib/components/text.hpp"
#include "lib/components/dotRender.hpp"
#include "components/center.hpp"
#include <ncurses.h>

class Menu : public Scene {
  Transform* illustrationPos;

  void create() {

    auto container = root()->addChild<Object>("bg");
    //container->addComponent<DotRender>();
    container->addComponent<Center>();
    container->getTransform()->setAbsolutePosition({ 15, 15 });

    auto btn = container->addChild<Object>("btn");
    btn->getTransform()->setRelativePosition({-10, 10});
    btn->addComponent<WindowRender>(20, 2, COLOR_GREEN);
    //btn->addComponent<DotRender>();

    auto btnText = btn->addChild<Object>("btn-text");
    btnText->addComponent<Text>("Примеры");
    btnText->getTransform()->setRelativePosition({ 7, 1 });

    auto illustration = container->addChild<Object>("illo");
    //illustration->addComponent<DotRender>();
    illustration->addComponent<Sprite>("./sprites/mainmenu/main.xml");
  }

  void update() {
    /*unsigned x, y;
    getmaxyx(stdscr, y, x);
    illustrationPos->xy(x/2, y/2 - 5);*/
  }
};