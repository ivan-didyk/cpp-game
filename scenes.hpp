#include "lib/object.h"
#include "lib/sceneList.hpp"

#include "scenes/menu.hpp"
#include "scenes/keyDemo.hpp"

scenes
  startScene("menu"    , Menu   )
  scene     ("key-demo", KeyDemo)
thatsall

