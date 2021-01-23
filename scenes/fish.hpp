#include "lib/object.h"
#include "lib/components/sprite.hpp"
#include "components/fish-control.hpp"
#include "components/plankton.hpp"
#include "components/energy-bar.hpp"
#include "lib/components/text.hpp"

static const unsigned foodAmount = 70;

class FishGame : public Scene {
  unsigned stats;
  Sprite* ss;
  Text* score;
  EnergyBar* energyBar;
  unsigned eaten = 1000;

  void create() {
    auto fish = root()->addChild<Object>("fish");
    ss = fish->addComponent<Sprite>("sprites/fish.xml");
    fish->addComponent<FishController>(&eaten);

    auto bar = root()->addChild<Object>("bar");
    energyBar = bar->addComponent<EnergyBar>();
    bar->setPriority(-100);

    stats = foodAmount;
    for(unsigned int i = 0; i < foodAmount; ++i) {
      auto pl = root()->addChild<Object>("pl");
      pl->addComponent<Plankton>(ss, &stats, &eaten, i*8+11);
      pl->getTransform()->setAbsolutePosition({
        rand() % Game::getScreenSize().x,
        rand() % Game::getScreenSize().y });
    }
  }

  void update() {
    energyBar->setEnergy(eaten / 2000.0);
    while(stats < foodAmount) {
      auto pl = root()->addChild<Object>("pl");
      pl->addComponent<Plankton>(ss, &stats, &eaten);
      pl->getTransform()->setAbsolutePosition({ rand() % Game::getScreenSize().x, rand() % Game::getScreenSize().y });
      ++stats;
    }
    if(eaten > 0) --eaten;
  }
};