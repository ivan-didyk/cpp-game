#include "lib/object.h"
#include "lib/components/sprite.hpp"
#include "stdlib.h"

static Vector2 moves[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

class Plankton : public Render {
  Sprite* fish;
  unsigned* stats;
  unsigned* score;

  bool isSpecial = false;

  unsigned rl = 0;

  unsigned ttl = 261;

  void draw() {
    if(isSpecial) {
      attron(COLOR_PAIR(COLOR_CYAN));
      mvaddch(getTransform()->y(), getTransform()->x(), '*');
      attroff(COLOR_PAIR(COLOR_YELLOW));
    } else {
      attron(COLOR_PAIR(COLOR_GREEN) | A_DIM);
      mvaddch(getTransform()->y(), getTransform()->x(), '+');
      attroff(COLOR_PAIR(COLOR_GREEN) | A_DIM);
    }
  }

  void update() {
    if(fish->checkCollision(getTransform()->getAbsolutePosition())) {
      --(*stats);
      if(*score < 2000) (*score) += (isSpecial ? 150 : 30);
      getObject()->destroy();
      return;
    }

    if(--ttl == 0) {
      --(*stats);
      getObject()->destroy();
      return;
    }

    rl++;
    if(rl < 10) return;
    rl = 0;
    Vector2 move = moves[rand() % 4];
    Vector2 nextPos = getTransform()->getAbsolutePosition() + move;
    if(nextPos.x < 0                       || 
       nextPos.y < 0                       || 
       nextPos.x > Game::getScreenSize().x || 
       nextPos.y > Game::getScreenSize().y - 1) return;
    
    getTransform()->move(move);
  }
 public:
  Plankton(Sprite* __fish, unsigned *__stats, unsigned *__score) {
    fish = __fish;
    stats = __stats;
    isSpecial = (rand() % 30) == 0;
    score = __score;
  }

  Plankton(Sprite* __fish, unsigned *__stats, unsigned *__score, unsigned __ttl) {
    fish = __fish;
    stats = __stats;
    isSpecial = (rand() % 50) == 0;
    score = __score;
    ttl = __ttl;
  }
};