#include "objectRewrite.h"

//    ___  _     _           _   
//   / _ \| |__ (_) ___  ___| |_ 
//  | | | | '_ \| |/ _ \/ __| __|
//  | |_| | |_) | |  __/ (__| |_ 
//   \___/|_.__// |\___|\___|\__|
//            |__/               

void Object::callUpdate() {
  for(auto &i : components) i->callUpdate();
  for(auto &i : children    ) i->callUpdate();
}

void Object::callDraw() {
  if(ownRender) ownRender->render->callDraw();
  for(auto &i : renders) i->object->callDraw();
}

void Object::removeChild(Object* child) {
  for(unsigned i = 0; i < children.size(); ++i) {
    if(children[i] == child) {
      children.erase(children.begin() + i);
      return;
    }
  }
}

void Object::removeComponent(Component* component) {
  for(unsigned i = 0; i < components.size(); ++i) {
    if(components[i] == component) {
      components.erase(components.begin() + i);
      return;
    }
  }
}

template<typename T>
T* Object::getComponent() const {
  for(auto &i : components) {
    if(i->id == typeid(T).name()) return (T*) i;
  }
  return nullptr; // Ничего не нашли
}

template<typename T>
vector<T*> Object::getComponents() const {
  vector<T*> results;
  for(auto &i : components) {
    if(i->id == typeid(T).name()) results.push_back((T*) i);
  }
  return results;
}

Object* Object::getParent() const {
  return parent;
}

vector<Object*> Object::getChildren() const {
  return children;
}

string Object::getTag() const {
  return tag;
}

string Object::getLabel() const {
  return label;
}

Transform* Object::getTransform() const {
  return transform;
}

template<
  typename T,
  typename... Args,
  typename std::enable_if<std::is_base_of<Object, T>::value>::type*
>
T* Object::addComponent(Args&& ...args) {
  T* cmp = new T(forward<Args>(args)...);
  if(is_base_of<Render, T>::value) { // Это рендер
    if(ownRender) throw "Два рендера к обьекту не крепятся";
    ownRender = new PriorityObj;
    ownRender->object = this;
    ownRender->render = cmp;
  }
  components.push_back(cmp);
  cmp->attachedObject = this;
  cmp->id = typeid(T).name();
  cmp->callStart();
  return cmp;
}

template<
  typename T,
  typename... Args,
  typename enable_if<is_base_of<Object, T>::value>::type*
>
T* Object::addChild(Args&& ...args) {
  T* ch = new T(forward<Args>(args)...);
  children.push_back(ch);
  ch->parent = this;
  return ch;
}

void Object::destroy() {
  for(auto &i : components) i->onDestroy();
  parent->removeChild(this);
}


//   ____                                             _   
//  / ___|___  _ __ ___  _ __   ___  _ __   ___ _ __ | |_ 
// | |   / _ \| '_ ` _ \| '_ \ / _ \| '_ \ / _ \ '_ \| __|
// | |__| (_) | | | | | | |_) | (_) | | | |  __/ | | | |_ 
//  \____\___/|_| |_| |_| .__/ \___/|_| |_|\___|_| |_|\__|
//                      |_|                               

void Component::callUpdate() {
  update();
}

void Component::callStart() {
  start();
}

Object* Component::getObject() const {
  return attachedObject;
}

string Component::getTag() const {
  return attachedObject->getTag();
}

template <typename T>
T* Component::getComponent() const {
  return attachedObject->getComponent<T>();
}

template <typename T>
vector<T*> Component::getComponents() const {
  return attachedObject->getComponents<T>();
}

void Component::destroy() {
  attachedObject->removeComponent(this);
}

//  _____                     __                      
// |_   _| __ __ _ _ __  ___ / _| ___  _ __ _ __ ___  
//   | || '__/ _` | '_ \/ __| |_ / _ \| '__| '_ ` _ \ 
//   | || | | (_| | | | \__ \  _| (_) | |  | | | | | |
//   |_||_|  \__,_|_| |_|___/_|  \___/|_|  |_| |_| |_|
                                                    

void Transform::moveChildren(Vector2 delta) {
  for(auto i : getObject()->getChildren()) {
    i->getTransform()->move(delta);
  }
}

void Transform::setAbsolutePosition(Vector2 pos) {
  Vector2 delta = pos - absolutePosition;
  move(delta);
  moveChildren(delta);
}

void Transform::setRelativePosition(Vector2 pos) {
  Vector2 delta = pos - relativePosition;
  move(delta);
  moveChildren(delta);
}

void Transform::setRelativePositionSelf(Vector2 pos) {
  move(pos - relativePosition);
}

void Transform::setAbsolutePositionSelf(Vector2 pos) {
  move(pos - absolutePosition);
}

void Transform::move(Vector2 delta) {
  absolutePosition += delta;
  relativePosition += delta;
}

Vector2 Transform::getAbsolutePosition() const {
  return absolutePosition;
}

Vector2 Transform::getRelativePosition() const {
  return relativePosition;
}


//  ____                _           
// |  _ \ ___ _ __   __| | ___ _ __ 
// | |_) / _ \ '_ \ / _` |/ _ \ '__|
// |  _ <  __/ | | | (_| |  __/ |   
// |_| \_\___|_| |_|\__,_|\___|_|   
//

void Render::callDraw() {
  draw();
}


//  ____                      
// / ___|  ___ ___ _ __   ___ 
// \___ \ / __/ _ \ '_ \ / _ \
//  ___) | (_|  __/ | | |  __/
// |____/ \___\___|_| |_|\___|
//

Object* Scene::root() const {
  return &sceneRoot;
}



//   ____                      
//  / ___| __ _ _ __ ___   ___ 
// | |  _ / _` | '_ ` _ \ / _ \
// | |_| | (_| | | | | | |  __/
//  \____|\__,_|_| |_| |_|\___|
                             
void Game::openScene(string name) {
  delete current;
  current = create(name);
}

void Game::update() {
  current->root()->callUpdate();
}

void Game::draw() {
  current->root()->callDraw();
}