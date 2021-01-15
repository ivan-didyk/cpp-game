#include "lib/object.h"
#include "lib/components/text.hpp"
#include "components/simple-player-controller.hpp"

class KeyDemo : public Scene {
  void create() {
    auto player = root()->addChild<Object>("player");
    player->getTransform()->setRelativePosition({10, 5});
    auto playSprite = player->addComponent<Sprite>("sprites/ufo.xml");
    playSprite->startPlaying();
    player->addComponent<PlayerController>();
  }
};