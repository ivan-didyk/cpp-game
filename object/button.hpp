#include "lib/object.h"
#include "lib/components/text.hpp"
#include "lib/components/windowRender.hpp"
#include "lib/utf8.hpp"
#include "lib/components/eventSystem.hpp"
#include <string>

class Button : public Object {
  string t;
  unsigned width;
  EventSystem * s;
  Text* tt;
  int color = COLOR_GREEN;

  void create() {
    width = utf8size(t.c_str())+3;
    addComponent<WindowRender>(width, 2, color);
    s = addComponent<EventSystem>();
    auto btnText = addChild<Object>("btn-text");
    tt = btnText->addComponent<Text>(t, color);
    btnText->getTransform()->setRelativePosition({ 2, 1 });
  }

  void update() {
    Vector2 pos = Game::getMousePosition();
    if(Game::getMouseState() | BUTTON1_CLICKED) {
      if(pos.x >= getTransform()->getAbsolutePosition().x && 
         pos.x <= getTransform()->getAbsolutePosition().x + width &&
         pos.y >= getTransform()->getAbsolutePosition().y &&
         pos.y <= getTransform()->getAbsolutePosition().y + 2) {
           s->emit("click", nullptr);
         }
    }
  }
public:
  Button(string name, string txt, int __color = COLOR_GREEN) :Object(name) {
    t = txt;
    color = __color;
  }
};