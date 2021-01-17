#include "object.h"
#include <ncurses.h>

//    ___  _     _           _   
//   / _ \| |__ (_) ___  ___| |_ 
//  | | | | '_ \| |/ _ \/ __| __|
//  | |_| | |_) | |  __/ (__| |_ 
//   \___/|_.__// |\___|\___|\__|
//            |__/               

Object::Object(string name) {
  label = name;
  transform = addComponent<Transform>();
}

void Object::callUpdate() {
  children.sort(); // FIXME: Пока что бестолково
  for(auto &i : components) i->callUpdate();
  for(auto &i : children    ) i->callUpdate();
}

void Object::callDraw() {
  if(ownRender) ownRender->callDraw();
  for(auto &i : children) i->callDraw();
}

void Object::callCreate() {
  create();
}

void Object::removeChild(Object* child) {
  for(auto i = children.begin(); i != children.end(); ++i) {
    if(*i == child) {
      children.erase(i);
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

list<Object*> Object::getChildren() const {
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
  typename... Args
>
T* Object::addComponent(Args&& ...args) {
  T* cmp = new T(forward<Args>(args)...);
  if constexpr (is_base_of<Render, T>::value) { // Это рендер
    if(ownRender) throw "Два рендера к обьекту не крепятся";
    ownRender = cmp;
    parent->children.sort();
  }
  components.push_back(cmp);
  cmp->attachedObject = this;
  cmp->id = typeid(T).name();
  cmp->callStart();
  return cmp;
}

template<
  typename T,
  typename... Args
>
T* Object::addChild(Args&& ...args) {
  T* ch = new T(forward<Args>(args)...);
  children.push_back(ch);
  ch->transform->absolutePosition = transform->absolutePosition;
  ch->transform->relativePosition = { 0, 0 };
  ch->parent = this;
  ch->callCreate();
  children.sort();
  return ch;
}

void Object::destroy() {
  for(auto &i : components) i->onDestroy();
  parent->removeChild(this);
}


bool Object::operator<(Object &other) const {
  return other.priority > priority;
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

Transform* Component::getTransform() const {
  return attachedObject->getTransform();
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
    i->getTransform()->moveAbsolute(delta);
  }
}

void Transform::setAbsolutePosition(Vector2 pos) {
  Vector2 delta = pos - absolutePosition;
  moveChildren(delta);
  absolutePosition += delta;
  relativePosition += delta;
}

void Transform::setRelativePosition(Vector2 pos) {
  Vector2 delta = pos - relativePosition;
  moveChildren(delta);
  absolutePosition += delta;
  relativePosition += delta;
}

void Transform::moveAbsolute(Vector2 delta) {
  absolutePosition += delta;
  moveChildren(delta);
}

void Transform::move(Vector2 delta) {
  absolutePosition += delta;
  relativePosition += delta;
  moveChildren(delta);
}

Vector2 Transform::getAbsolutePosition() const {
  return absolutePosition;
}

Vector2 Transform::getRelativePosition() const {
  return relativePosition;
}

int Transform::x() const {
  return absolutePosition.x;
}

int Transform::y() const {
  return absolutePosition.y;
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

Object* Scene::root() {
  return &sceneRoot;
}



//   ____                      
//  / ___| __ _ _ __ ___   ___ 
// | |  _ / _` | '_ ` _ \ / _ \ 
// | |_| | (_| | | | | | |  __/
//  \____|\__,_|_| |_| |_|\___|
                             
void Game::openScene(string name) {
  delete game->current;
  erase();
  game->current = game->create(name);
  game->current->create();
}

void Game::update() {
  current->root()->callUpdate();
}

void Game::draw() {
  current->root()->callDraw();
}

Game::Game() {
  current = create("");
  current->create();
  game = this;
}

bool Game::getKey(int key) {
  return game->pressed.count(key);
}

Game* Game::game = nullptr;