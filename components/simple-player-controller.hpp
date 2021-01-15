#include "lib/object.h"
#include <ncurses.h>

class PlayerController : public Component {
public:
  void update() {
    if(Game::getKey(KEY_UP)) {
      getTransform()->setRelativePosition(getTransform()->getRelativePosition() + Vector2(0, -1));
    } else if(Game::getKey(KEY_DOWN)) {
      getTransform()->setRelativePosition(getTransform()->getRelativePosition() + Vector2(0,  1));
    } else if(Game::getKey(KEY_RIGHT)) {
      getTransform()->setRelativePosition(getTransform()->getRelativePosition() + Vector2(2,  0));
    } else if(Game::getKey(KEY_LEFT)) {
      getTransform()->setRelativePosition(getTransform()->getRelativePosition() + Vector2(-2,  0));
    }
  }
};