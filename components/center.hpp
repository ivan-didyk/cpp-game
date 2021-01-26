#pragma once
#include "lib/object.h"

/******************************\
* Крепим обьект в центр экрана *
\******************************/

class Center : public Component {
  void update() {
    getTransform()->setAbsolutePosition({ Game::getScreenSize().x / 2, Game::getScreenSize().y / 2 });
  }
};