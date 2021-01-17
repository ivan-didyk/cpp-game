#pragma once
#include <string>
#include <vector>
#include <typeinfo>
#include <algorithm>
#include <list>
#include <type_traits>
#include <set>
#include <ncurses.h>

using namespace std;

/****************************************************\
* Страшный код, однако при ближайшем рассмотрении не *
* такой уж и сложный.                                *
* По большей части идеи из Unity 3D                  *
\****************************************************/

int main();

// Обьявляем имена классов
class Object   ; 
class Component; 
class Transform;
class Render   ; 
class Scene    ; 
class Game     ;

/**
 * Точка в огромном мире
*/
struct Vector2 {
  int x = 0, y = 0;

  Vector2(int X, int Y) : x(X), y(Y) {}
  Vector2(int d)        : x(d), y(d) {}
  Vector2()             : x(0), y(0) {}

  Vector2 operator-(Vector2 other) const {
    return Vector2(x - other.x, y - other.y);
  }

  Vector2 operator+(Vector2 other) const {
    return Vector2(x + other.x, y + other.y);
  }

  Vector2& operator+=(Vector2 other) {
    x += other.x;
    y += other.y;
    return *this;
  }
};

/**
 * Маленький класс для запоминания кого первым рисовать
*/
struct PriorityObj {
  // Кого рисовать
  Object* object = nullptr;
  // Какой компонент рисует
  Render* render = nullptr;
  // Приоритет
  int priority = 0;

  // Это сделано для сортировки, для другого не применимо
  bool operator<(const PriorityObj &other) {
    return priority > other.priority;
  }
};



/**
 * Игровой обьект - как Player, Tree, Button
*/
class Object {
  friend class Game;
  friend class Render;
  friend class Component;
 private:
  // Имя обьекта, его можно установить
  string label;
  // Список всех дочерних элементов обьекта
  string tag;
  // Список всех дочерних элементов обьекта
  list<Object*> children;
  // Список компонентов
  vector<Component*> components;
  // Ссылка на компонент Transform
  Transform* transform;
  // Родитель
  Object* parent;
  // Свой рендер
  Render* ownRender = nullptr;
  // Приоритет отрисовки
  int priority;

  /**
   * Запускаем обновление по всему обьекту
  */
  void callUpdate();
  /**
   * Запускаем отрисовку
  */
  void callDraw();

  void callCreate();

  void removeChild(Object*);
  void removeComponent(Component*);
 protected:
  virtual void create() {}
 public:
  /**
   * Создаём обьект с неким именем
  */
  Object(string);
  /**
   * Ищем компонент типа `T` и возвращаем его
  */
  template <typename T>
  T* getComponent() const;

  /**
   * Ищем ВСЕ компоненты типа `T` и возвращаем их в векторе
  */
  template<typename T>
  vector<T*> getComponents() const;

  /**
   * Узнаём родителя
  */
  Object* getParent() const;

  /**
   * Получаем список дочерних элементов
  */
  list<Object*> getChildren() const;

  /**
   * Выяснить тег обьекта
  */
  string getTag() const;

  /**
   * Имя обьекта
  */
  string getLabel() const;

  /**
   * Найти компонент ответственный за позицию
   * Такой компонент у обьекта обязательно присутствует
  */
  Transform* getTransform() const;

  /**
   * Выяснить приоритет отрисовки обьекта
  */
  int getPriority() const;

  /**
   * Установить приоритет отрисовки
  */
  void setPriority(int newPriority);

  /**
   * Добавить компонент `T`
   * 
   * Если вам потом что-то не понравилось, и вы решили удалить этот компонент, 
   * это делается через `.getComponent<T>.destroy()`
   * 
   * Подсказки параметров IntelliSense тут могут бестолково работать
   * (из-за Args...), помните что все аргументы уйдут к конструктору компонента
  */
  template<
    typename T,
    typename... Args
  >
  T* addComponent(Args&& ...args);

  /**
   * Добавить дочерний обьект `T` (в большинстве случаев `T = Object`)
   * @todo Сделать нормально работающий механизм наследования от `Object`
  */
  template<
    typename T,
    typename... Args
  >
  T* addChild(Args&& ...args);

  /**
   * Самоуничтожиться (((
  */
  void destroy();

  bool operator<(Object&) const;
};



/**
 * Компонент - как ClickListener, PlayerController и подобное
 * В конструкторе не обращаться к обьекту, он ещё не подключен. Для этого есть `start()`
*/
class Component {
  friend class Object;
 private:
  // Имя компонента, не всегда читаемое. Генерируется автоматически.
  // Используется для поиска компонентов
  string id;
  // Обьект, к которому прикрепили компонент
  Object* attachedObject;

  /**
   * Запускаем виртуальные методы компонента
  */
  void callUpdate();
  void callStart();
 protected:
  /**
   * Меняем что-то каждий кадр, например подправляем счётчик
  */
  virtual void update() { }
  /**
   * Запускается после добавки компонента, когда все настройки применены
  */
  virtual void start() {}
  /**
   * Когда компонент удаляют
  */
  virtual void onDestroy() {}
 public:
  /**
   * Получить обьект, к которому прикрепили компонент
  */
  Object* getObject() const;
  /**
   * Получить тег обьекта
  */
  string getTag() const;

  /**
   * Получить компонент с позицией обьекта
  */
  Transform* getTransform() const;

  /**
   * Ищем компонент типа `T` и возвращаем его
  */
  template <typename T>
  T* getComponent() const;

  /**
   * Ищем ВСЕ компоненты типа `T` и возвращаем их в векторе
  */
  template <typename T>
  vector<T*> getComponents() const;

  /**
   * Уничтожить себя
  */
 void destroy();
};

/**
 * Компонент ответственный за позицию
*/
class Transform : public Component {
  friend class Object;
 private:
  // Поиция относительно родителя
  Vector2 relativePosition;
  // Позиция относительно начала координат
  Vector2 absolutePosition;

  /**
   * Передвинуть детей
  */
  void moveChildren(Vector2);
 public:
  Transform() {}

  /**
   * Передвигаем обьект в некие абсолютные координаты
  */
  void setAbsolutePosition(Vector2);
  /**
   * Задать позицию относительную родителю
  */
  void setRelativePosition(Vector2);
  /**
   * Получаем абсолютную позицию
  */
  Vector2 getAbsolutePosition() const;
  /**
   * Получаем локальную позицию
  */
  Vector2 getRelativePosition() const;
  /**
   * Передвинуться на сколько-то
  */
  void move(Vector2);

  /**
   * Передвинуться вместе с родителем
  */
  void moveAbsolute(Vector2);

  /**
   * Получить абсолютную координату по X
   * @deprecated
  */
  int x() const;

  /**
   * Получить абсолютную координату по Y
   * @deprecated
  */
  int y() const;
};

/**
 * Компонент рендера, от него наследуем всё отображающееся
 * На обьект может быть всего один
*/
class Render : public Component {
  friend class Object;
 private:
  void callDraw();
 protected:
  /**
   * Выводим на экран картинку - это происходит после `update`. 
  */
  virtual void draw() { }
};

/**
 * Сцена
 * На экране может быть только одна сцена
*/
class Scene {
  friend class Game;
 private:
  // Рут сцены
  Object sceneRoot;
 protected:
  /**
   * Тут пишет создание сцену (добавление на неё игровых обьектов)
  */
  virtual void create() {}

  /**
   * Получаем рут сцены
  */
  Object* root();

  /**
   * Просто создаём сцену
  */
  Scene() :sceneRoot("root") {}
};

/**
 * Основной класс контроллера за игрой
 * Ничего с ним не делать, кроме статических методов
*/
class Game {
  friend int main();
 private:
  // Сcылка на единственный экземпляр
  static Game* game;
  // Сейчас открытая сцена
  Scene* current;
  // Всё что произошло на клавиатуре
  set<int> pressed;
  // Последнее событие с мышки
  vector<MEVENT> mouseEvents;
  /** 
   * Создаём сцену
   * Эта функция замаскирована в `scenes.hpp` макросами
  */
  Scene* create(string);

  /**
   * Создаём менеджера, открываем сцену по умолчанию
  */
  Game();
  /**
   * Обновляем сцену
  */
  void update();
  /**
   * Рисуем сцену
  */
  void draw();
 public:
  /**
   * Определить, нажата ли клавиша
  */
  static bool getKey(int);

  /**
   * Открыть сцену
  */
  static void openScene(string);
};