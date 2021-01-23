#include "lib/object.h"

class EnergyBar : public Render {
  double energy = 0.5;

  void draw() {
    mvaddch(Game::getScreenSize().y-1, Game::getScreenSize().x-1, 'o');
    mvaddch(Game::getScreenSize().y-1, 0, 'o');
    unsigned w = (Game::getScreenSize().x - 3);
    unsigned l = w * min(energy, 1.0);
    unsigned i = 0;
    int attr = (
      energy < 0.2 ? COLOR_MAGENTA : (
      energy < 0.4 ? COLOR_RED : (
      energy < 0.6 ? COLOR_GREEN : (
      energy < 0.8 ? COLOR_BLUE : COLOR_CYAN))));
    attron(COLOR_PAIR(attr));
    for(; i < l; i++) {
      addch('=');
    }
    attroff(COLOR_PAIR(attr));
    attron(COLOR_PAIR(COLOR_WHITE) | A_DIM);
    for(; i < w; i++) addch('-');
    attroff(COLOR_PAIR(COLOR_WHITE) | A_DIM);
  }

public:
  void setEnergy(double __energy) {
    energy = __energy;
  }

  double getEnergy() const {
    return energy;
  }
};