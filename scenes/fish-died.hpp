#include "lib/object.h"
#include "lib/components/sprite.hpp"
#include "lib/components/text.hpp"
#include "components/center.hpp"
#include "globals.hpp"

void playAgain(const EventData* eventData) {
  //printw("exitScene");
  Game::openScene("fish");
}

void exitGame(const EventData* eventData) {
  //printw("exitScene");
  Game::exitc();
}


class FishDied : public Scene {
  void create() {
    auto container = root()->addChild<Object>("bg");
    container->addComponent<Center>();

    auto illustration = container->addChild<Object>("illo");
    illustration->addComponent<Sprite>("./sprites/end.xml");

    auto text = container->addChild<Object>("text");
    string score = to_string(gameScore);

    if(gameScore < 10)        score += " - Совсем новичок";
    else if(gameScore < 50)   score += " - Не совсем новичок";
    else if(gameScore < 100)  score += " - Средненький";
    else if(gameScore < 500)  score += " - Любитель";
    else if(gameScore < 1000) score += " - Профессиональная рыба";
    else                      score += " - Рыба с опытом";

    text->addComponent<Text>(score, COLOR_MAGENTA);
    text->getTransform()->setAbsolutePosition({ -utf8size(score.c_str())/2+1, 6 });

    auto btn = container->addChild<Button>("btn", "Дайте мне ещё раз");
    btn->getTransform()->setRelativePosition({ -9, 8 });
    btn->getComponent<EventSystem>()->listen("click", playAgain);

    auto btnExit = container->addChild<Button>("btn", "   Мне надоело   ", COLOR_RED);
    btnExit->getTransform()->setRelativePosition({ -9, 11 });
    btnExit->getComponent<EventSystem>()->listen("click", exitGame);
  }
};