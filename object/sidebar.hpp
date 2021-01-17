#include "lib/object.h"
#include "lib/components/text.hpp"
#include <string>

struct MenuItem {
  string label;
  bool isHeader;
  string scene;
};

static vector<MenuItem> menu = {
  {"Basic examples", true, ""},
  {"Transform", false, ""},
  {"Sprite", false, ""},
};

class Sidebar : public Object {
  void create() {
    for(unsigned i = 0; i < menu.size(); ++i) {
      auto item = addChild<Object>("menu-label-"+to_string(i));
      item->getTransform()->setRelativePosition({ 0, i });
      if(menu[i].isHeader) {
        item->addComponent<Text>(menu[i].label);
      } else {
        item->addComponent<Text>("- " + menu[i].label);
      }
    }
  }
public:
  Sidebar() :Object("sidebar") {

  }
};