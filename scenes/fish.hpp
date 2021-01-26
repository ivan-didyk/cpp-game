#include "lib/object.h"
#include "lib/components/sprite.hpp"
#include "components/fish-control.hpp"
#include "components/plankton.hpp"
#include "components/energy-bar.hpp"
#include "components/enemy-fish-controller.hpp"
#include "lib/components/text.hpp"
#include "globals.hpp"

static const unsigned foodAmount = 70;

class FishGame : public Scene {
  unsigned stats;
  Sprite* ss;
  Sprite* en;
  Text* score;
  EnergyBar* energyBar;
  unsigned eaten = 1000;
  unsigned dist = 0;
  unsigned reload = 10;

  void create() {
    auto fish = root()->addChild<Object>("fish");
    ss = fish->addComponent<Sprite>("sprites/fish.xml");
    fish->addComponent<FishController>(&eaten);

    auto fishEnemy = root()->addChild<Object>("fish-enemy");
    en = fishEnemy->addComponent<Sprite>("sprites/fish-enemy.xml");
    fishEnemy->addComponent<EnemyFishController>();
    fishEnemy->getTransform()->setAbsolutePosition({ 1000, 1000 });

    auto bar = root()->addChild<Object>("bar");
    energyBar = bar->addComponent<EnergyBar>();
    bar->setPriority(-100);

    auto scoreText = root()->addChild<Object>("score");
    score = scoreText->addComponent<Text>("0");
    scoreText->setPriority(-100);
    scoreText->getTransform()->setAbsolutePosition({ 2, 0 });

    auto clock = root()->addChild<Object>("clock");
    clock->addComponent<Sprite>("sprites/clock.xml")->startPlaying();

    stats = foodAmount;
    for(unsigned int i = 0; i < foodAmount; ++i) {
      auto pl = root()->addChild<Object>("pl");
      pl->addComponent<Plankton>(ss, en, &stats, &eaten, i*8+11);
      pl->getTransform()->setAbsolutePosition({
        rand() % Game::getScreenSize().x,
        rand() % Game::getScreenSize().y });
    }
  }

  void update() {
    if(reload) --reload;
    else {
      reload = 10;
      ++dist;
      score->text(to_string(dist));
    }
    energyBar->setEnergy(eaten / 2000.0);
    while(stats < foodAmount) {
      auto pl = root()->addChild<Object>("pl");
      pl->addComponent<Plankton>(ss, en, &stats, &eaten);
      pl->getTransform()->setAbsolutePosition({ rand() % Game::getScreenSize().x, rand() % Game::getScreenSize().y });
      ++stats;
    }
    if(ss->checkCollision(en)) {
      gameScore = dist;
      Game::openScene("fish-died");
    }
    if(eaten > 0) --eaten;
    else {
      gameScore = dist;
      Game::openScene("fish-died");
    }
  }
};