#include "lib/object.h"
#include "lib/components/sprite.hpp"
#include "stdlib.h"

class EnemyFishController : public Component {
  bool movingRight;
  unsigned wait = 0;
  unsigned skipFrames = 0;
  Sprite* sprite;

  void start() {
    sprite = getComponent<Sprite>();
    sprite->startPlaying();
  }

  void restartFish() {
    wait = (rand() % 50) + 50;
    movingRight = rand() & 1;
    if(movingRight) {
      getTransform()->setAbsolutePosition({
        -13,
        rand() % (Game::getScreenSize().y + 3)
      });
      sprite->setTrack("right");
    } else {
      getTransform()->setAbsolutePosition({
        Game::getScreenSize().x + 1,
        rand() % (Game::getScreenSize().y + 3)
      });
      sprite->setTrack("left");
    }
  }

  void update() {
    if(skipFrames) {
      --skipFrames;
      return;
    }
    skipFrames = 2;
    if(wait) --wait;
    else {
      if(getTransform()->x() < -14 || getTransform()->x() > Game::getScreenSize().x+2)
        restartFish();
      else
        getTransform()->move({ movingRight ? 1 : -1, 0 });
    }
  }
};