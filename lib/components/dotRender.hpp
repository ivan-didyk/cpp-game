#include "lib/object.h"

/**
 * A text rendering stuff
 * @todo Add FoRmAtTeD text!
 */
class DotRender : public Render {
  Transform* transform;

  void start() {
    transform = getComponent<Transform>();
  }

  void draw() {
    mvprintw(transform->y(),   transform->x(), "■ %s", this->getObject()->getLabel().c_str());
    mvprintw(transform->y()+1, transform->x(), "  abs %d %d", getTransform()->y(), getTransform()->y());
    mvprintw(transform->y()+2, transform->x(), "  rel %d %d", getTransform()->getRelativePosition().x, getTransform()->getRelativePosition().y);
  }
};