#include "lib/object.h"
#include "lib/sceneList.hpp"

#include "scenes/menu.hpp"
#include "scenes/keyDemo.hpp"
#include "scenes/fish.hpp"

scenes
  startScene("menu"    ,     Menu)
  scene     ("fish"    , FishGame)
  scene     ("key-demo",  KeyDemo)
thatsall

