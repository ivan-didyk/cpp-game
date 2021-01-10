#pragma once
#include "lib/object.hpp"
using namespace std;

class Render : public Component {
 public:

  void renderRequest() {
    object()->rendered(true);
  }
};