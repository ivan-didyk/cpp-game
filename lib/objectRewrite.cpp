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

