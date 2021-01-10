#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <list>
#include <typeinfo>
#include <ncurses.h>
using namespace std;

/****************************************************\
* Страшный код, однако при ближайшем рассмотрении не *
* такой уж и сложный.                                *
* По большей части идеи из Unity 3D                  *
\****************************************************/


class Object;

/**
 * Маленький класс для запоминания кого первым рисовать
*/
struct PriorityObj {
  // Кого рисовать
  Object* object = nullptr;
  // Приоритет
  int priority = 0;

  // Это сделано для сортировки, для другого не применимо
  bool operator<(const PriorityObj &other) {
    return priority > other.priority;
  }
};

/**
 * Компонент - как ClickListener, PlayerController и подобное
 * В конструкторе не обращаться к обьекту, он ещё не подключен. Для этого есть `start()`
*/
class Component {
  friend class Object;

 private:
  // Имя компонента, не всегда читаемое. Генерируется автоматически.
  string _name;
  // Обьект, к которому прикрепили компонент
  Object* _object;

 protected:
  // --- Методы, которые изменяют в своих наследованных компонентах. Тут они пустые.

  /**
   * Меняем что-то каждий кадр, например подправляем счётчик
  */
  virtual void update() { }
  /**
   * Выводим на экран картинку - это происходит после `update`. 
  */
  virtual void draw() { }
  /**
   * Запускается после добавки компонента, когда все настройки применены
  */
  virtual void start() {}
  /**
   * Тут запрашивается разрешения на отрисовку, используется только в `Render`
   * @todo Перенести в какое-то другое место, чтобы никто до него не добирался
  */
  virtual void renderRequest() {  }

 public:
  /**
   * Получить имя
  */
  string    name() const;
  /**
   * Получить родителя
  */
  Object* object() const { return _object; };
  /**
   * Получить тег родителя
  */
  string     tag() const;

  // @todo Добавить доступ к Transform - или не стоит?

  /**
   * Добыть какой-то другой компонент - например Transform
  */
  template <typename T>
  T* getComponent();

  // @todo Убрать эти штуки (если можно)
  void qUpdate() { update(); }
  void qOndraw() { draw(); }
  void qAfterSetup() { start(); }
  void qRenderRequest() { renderRequest(); }
  void setParent(Object* parent) { _object = parent; }

  // @todo Сделать это виртуальным
  void destroy() {
  }
};

/**
 * Компонент, в котором хранится позиция объекта в игровом мире
 * @todo Сделать относительной от родителей
 * @todo Написать класс Vector2
*/
class Transform : public Component {
 private:
  // Позиция по X
  short _x = 0;
  // Позиция по Y
  short _y = 0;

 public:
  
  Transform() {}
  Transform(short x, short y) { _x = x; _y = y; }
  void x(short newX) { _x = newX; }
  void y(short newY) { _y = newY; }
  short x() { return _x; }
  short y() { return _y; }
  void xy(short newX, short newY) { _x = newX; _y = newY; }
};

/**
 * Пока что пустая структура данных события
 * @todo Что оно тут делает (.-.)?
*/
struct EventData {
  // TODO: Maybe, add some properties for event data?
};

/**
 * Игровой обьект - как Player, Tree, Button
 * Самая страшная вещь в этом файле
*/
class Object {
  friend class Component;

 private:
  // Имя обьекта, его можно установить
  // @todo Реализовать поиск обьекта по имени
  string _label;
  // Список всех дочерних элементов обьекта
  vector<Object*> childs;
  // Список компонентов
  vector<Component*> components;
  // Кто родитель. У рута это nullptr
  Object* parent;
  
  // Тег
  string _tag;
  // Список рендера
  list<PriorityObj*> renders;
  // Рендер этого обьекта
  PriorityObj* render = nullptr;
  // Позиция обьекта
  Transform* _transform;
  
  // Найти компонент по имени
  Component* getComponent(string name) {
    auto it = find_if(components.begin(), components.end(), [name](const Component* item){return item->name() == name; });
    return *it;
  }

  // @todo Это довольно бестолково, надо убрать
  void removeComponentHook(Component* component) {
    removeComponent(component);
  }
  
  // @todo Удалить
  void removeObjectHook(Object* object) {
    auto ptr = find(childs.begin(), childs.end(), object);
    delete *ptr;
    childs.erase(ptr);
  }

 protected:

  /**
   * Добавить чей-то рендер
   */
  void addRenderer(PriorityObj* object) {
    renders.push_back(object);
  }

  /**
   * Удалить рендер
   */
  void removeRenderer(PriorityObj* object) {
    renders.remove(object);
  }

  /**
   * Чей-то приоритет поменялся, надо обновить
  */
  void changePriotity() {
    renders.sort();
  }

  /**
   * Поменяли свой рендер, меняем приоритет
   * @todo Разобраться, где же всё-таки меняется приоритет.
  */
  void setRenderPriority() {
    if(!render) return;
    parent->changePriotity();
  }

  /**
   * Изменить тег
  */
  void tag(string newTag) { _tag = newTag; }
  /**
   * Изменить название
  */
  void label(string newLabel) { _label = newLabel; }

 public:
  /**
   * Создаём обьект с неким названием
  */
  Object(string label) {
    render = nullptr;
    _label = label;
    _transform = addComponent<Transform>(0, 0);
  }

  /**
   * Выясняем родителя
   * @todo Может, добавить API для определения дочерних компонентов?
  */
  Object* _parent() { return parent; }

  /**
   * Включаем/выключаем отображение
   * @todo Почему оно публичное?
  */
  void rendered(bool v) {
    if( v && render != nullptr) return;
    if(!v && render == nullptr) return;
    if(!parent) return;
    if(v) {
      render = new PriorityObj;
      render->object = this;
      render->priority = 0;

      parent->addRenderer(render);
    } else {
      parent->removeRenderer(render);
      delete render;
      render = nullptr;
    }
  }

  /**
   * Выяснить тег
  */
  string tag() const { return _tag; }
  /**
   * Выяснить имя обьекта
  */
  string label() const {return _label; }

  /**
   * Выяснить компонент ответственный за позицию на экране
  */
  Transform* transform() const { return _transform; }

  /**
   * Добавить дочерний элемент
  */
  template<typename T, typename... Args, typename std::enable_if<std::is_base_of<Object, T>::value>::type* = nullptr>
  T* addChild(Args&& ...args) {
    T* ch = new T(forward<Args>(args)...);
    childs.push_back(ch);
    ch->parent = this;
    return ch;
  }

  /**
   * Самоуничтожиться
  */
  void destroy() {
    if(parent == nullptr) throw "Attempt to destroy the root";
    for(auto &i : childs)     i->destroy();
    for(auto &i : components) i->destroy();
    parent->removeObjectHook(this);
  }

  /**
   * Провести переобработку обьекта
  */
  void update() {
    for(auto &i : components) i->qUpdate();
    for(auto &i : childs) i->update();
  }

  /**
   * Нарисовать на экране
  */
  void draw() {
    if(render) for(auto &i : components) i->qOndraw();
    for(auto i : renders) {
      i->object->draw();
    }
  }

  /**
   * Получить некий компонент
  */
  template <typename T>
  T* getComponent() {
    return (T*) getComponent(typeid(T).name());
  }

  /**
   * Добавить компонент
  */
  template<typename T = Object, typename... Args>
  T* addComponent(Args&& ...args) {
    T* cmp = new T(forward<Args>(args)...);
    components.push_back(cmp);
    cmp->setParent(this);
    cmp->_name = typeid(T).name();
    cmp->qRenderRequest();
    cmp->qAfterSetup();
    return cmp;
  }
  
  // Удалить компонент
  // @todo Надо бы привести в соответствие с addComponent
  bool removeComponent(Component* component) {
    auto ptr = find(components.begin(), components.end(), component);
    if(ptr == components.end()) return false;
    delete *ptr;
    components.erase(ptr);
    return true;
  }
};


/**
 * Получить имя компонента
*/
string    Component::name() const { return _name; }

/**
 * Получить тег обьекта, к которому прикрепили компонент
*/
string     Component::tag() const { return _object->tag(); }

/**
 * Найти другой компонент
*/
template <typename T>
T* Component::getComponent() {
  return _object->getComponent<T>();
}

class SceneManager;

/**
 * Сцена
 * @todo Сцены можно переключать!
 * На экране может быть только одна сцена
*/
class Scene {
  friend class SceneManager;
 private:
  // Рут сцены
  Object root;
 protected:

  /**
   * Просто создаём сцену
  */
  Scene() :root("root") {}

  /**
   * Тут пишет создание сцену (добавление на неё игровых обьектов)
  */
  virtual void create() {
    // TODO: add demo content
  }

  /**
   * Обновляем сцену
  */
  virtual void update() {
  }

  /**
   * Получаем рут сцены
  */
  Object &base() { return root; }
};

class SceneManager {
  // Открытая сцена
  Scene* current;

 private:
  /** 
   * Создаём сцену
   * Эта функция замаскирована в `scenes.hpp` макросами
  */
  Scene* create(string);

  /**
   * Открыть сцену
  */
  template <typename T>
  void openScene() {
    delete current;
    erase();
    current = create(typeid(T).name());
    current->create();
  }

 public:
  /**
   * Создаём менеджера, открываем сцену по умолчанию
  */
  SceneManager() {
    current = create("");
    current->create();
  }

  /**
   * Обновляем сцену
  */
  void update() { 
    current->update();
    current->base().update();
  }

  /**
   * Рисуем сцену
  */
  void draw() {
    current->base().draw();
  }
};

/**
 * Некоторое количество хитрых макросов
*/
#define scenes Scene* SceneManager::create(string s) {
#define startScene(z) if(s == "" || s == typeid(z).name()) \
                           return new z; \
                         else
#define scene(z) if(s == typeid(z).name()) \
                           return new z; \
                         else
#define thatsall throw "No scene \"" + s + "\""; }
