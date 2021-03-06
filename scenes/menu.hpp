#include "lib/object.h"
#include "lib/components/sprite.hpp"
#include "lib/components/windowRender.hpp"
#include "lib/components/text.hpp"
#include "lib/components/dotRender.hpp"
#include "components/center.hpp"
#include "object/button.hpp"
#include <ncurses.h>

void exitScene(const EventData* eventData) {
  //printw("exitScene");
  Game::openScene("key-demo");
}

void exitSceneFish(const EventData* eventData) {
  //printw("exitScene");
  Game::openScene("fish");
}

class Menu : public Scene {
  Transform* illustrationPos;

  void create() {

    auto container = root()->addChild<Object>("bg");
    //container->addComponent<DotRender>();
    container->addComponent<Center>();
    container->getTransform()->setAbsolutePosition({ 15, 15 });

    auto btn = container->addChild<Button>("btn", "Посмотреть примеры");
    btn->getTransform()->setRelativePosition({ 10, 10 });
    btn->getComponent<EventSystem>()->listen("click", exitScene);

    auto btn2 = container->addChild<Button>("btn", "Рыба!");
    btn2->getTransform()->setRelativePosition({ -10, 10 });
    btn2->getComponent<EventSystem>()->listen("click", exitSceneFish);

    auto illustration = container->addChild<Object>("illo");
    //illustration->addComponent<DotRender>();
    illustration->addComponent<Sprite>("./sprites/mainmenu/main.xml");
  }
};