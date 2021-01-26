#include "lib/object.h"
#include "lib/sceneList.hpp"

#include "scenes/menu.hpp"
#include "scenes/keyDemo.hpp"

#include "scenes/fish.hpp"
#include "scenes/fish-died.hpp"

scenes
  startScene("menu"     ,     Menu)
  scene     ("fish"     , FishGame)
  scene     ("key-demo" ,  KeyDemo)
  scene     ("fish-died", FishDied)
thatsall