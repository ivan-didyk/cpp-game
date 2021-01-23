#include "lib/object.h"
#include "lib/components/sprite.hpp"

/********************************\
* Бегаем туда/сюда клавишами,    *
* за экран не выбегаем           *
\********************************/

class FishController : public Component {
  
  Sprite* sprite;
  unsigned* score;

  void start() {
    sprite = getComponent<Sprite>();
    sprite->startPlaying();
    sprite->setTrack("right");
  }

  void update() {
    if(Game::getKey(KEY_UP) && getTransform()->getAbsolutePosition().y > 0) {
      getTransform()->setRelativePosition(getTransform()->getRelativePosition() + Vector2(0, -1));
      if(*score) (*score)--;
    }
    if(Game::getKey(KEY_DOWN) && getTransform()->getAbsolutePosition().y < Game::getScreenSize().y - 4) {
      getTransform()->setRelativePosition(getTransform()->getRelativePosition() + Vector2(0,  1));
      if(*score) (*score)--;
    }
    if(Game::getKey(KEY_RIGHT) && getTransform()->getAbsolutePosition().x < Game::getScreenSize().x - 12) {
      sprite->setTrack("right");
      getTransform()->setRelativePosition(getTransform()->getRelativePosition() + Vector2(2,  0));
      if(*score) (*score)--;
    }
    if(Game::getKey(KEY_LEFT) && getTransform()->getAbsolutePosition().x > 0) {
      sprite->setTrack("left");
      getTransform()->setRelativePosition(getTransform()->getRelativePosition() + Vector2(-2,  0));
      if(*score) (*score)--;
    }
  }

public:
  FishController(unsigned *__score) {
    score = __score;
  }
};