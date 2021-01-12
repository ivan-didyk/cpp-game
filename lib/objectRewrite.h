#include <string>
#include <vector>
#include <typeinfo>
#include <algorithm>
#include <list>

using namespace std;

/****************************************************\
* Страшный код, однако при ближайшем рассмотрении не *
* такой уж и сложный.                                *
* По большей части идеи из Unity 3D                  *
\****************************************************/

/**
 * Точка в огромном мире
*/
struct Vector2 {
  int x = 0, y = 0;
};

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

// Обьявляем имена классов
class Object   ; 
class Component; 
class Transform;
class Render   ; 
class Scene    ; 
class Game     ;


/**
 * Игровой обьект - как Player, Tree, Button
*/
class Object {
  friend class Game;
  friend class Render;
 private:
  // Имя обьекта, его можно установить
  string label;
  // Список всех дочерних элементов обьекта
  string tag;
  // Список всех дочерних элементов обьекта
  vector<Object*> childs;
  // Список компонентов
  vector<Component*> components;
  // Ссылка на компонент Transform
  Transform* transform;
  // Рендеры дочерних элементов
  list<PriorityObj*> renders;
  // Компонент с рендером

  /**
   * Запускаем обновление по всему обьекту
  */
  void callUpdate();
  /**
   * Запускаем отрисовку
  */
  void callDraw();
 protected:
  // @todo
 public:
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
  vector<Object*> getChildren() const;

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
    typename... Args,
    typename std::enable_if<std::is_base_of<Object, T>::value>::type* = nullptr
  >
  T* addComponent(Args&& ...args);

  /**
   * Добавить дочерний обьект `T` (в большинстве случаев `T = Object`)
   * @todo Сделать нормально работающий механизм наследования от `Object`
  */
  template<
    typename T,
    typename... Args,
    typename enable_if<is_base_of<Object, T>::value>::type* = nullptr
  >
  T* addChild(Args&& ...args);

  /**
   * Самоуничтожиться (((
  */
  void destroy();
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
   * Уничтожить себя
  */
 void destroy();
};

/**
 * Компонент ответственный за позицию
*/
class Transform : public Component {
 private:
  Vector2 relativePosition;
  Vector2 absolutePosition;
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
   * То же самое что `setAbsolutePosition`,
   * но подобьекты не перемещаются
  */
  void setAbsolutePositionSelf(Vector2);
  /**
   * То же самое что `setRelativePosition`,
   * но подобьекты не перемещаются
  */
  void setRelativePositionSelf(Vector2);
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
 public:
  Render();
  ~Render();
};

/**
 * Сцена
 * На экране может быть только одна сцена
*/
class Scene {
  friend class SceneManager;
 private:
  // Рут сцены
  Object sceneRoot;
 protected:
  /**
   * Тут пишет создание сцену (добавление на неё игровых обьектов)
  */
  virtual void create();

  /**
   * Получаем рут сцены
  */
  Object* root();
 public:
  /**
   * Просто создаём сцену
  */
  Scene() :root("root") {}
};

/**
 * Основной класс контроллера за игрой
 * Ничего с ним не делать, кроме статических методов
*/
class Game {
 private:
  // Сcылка на единственный экземпляр
  static Game* game = nullptr;
  // Сейчас открытая сцена
  Scene* current;

  /** 
   * Создаём сцену
   * Эта функция замаскирована в `scenes.hpp` макросами
  */
  Scene* create(string);

  /**
   * Открыть сцену
  */
  void openScene(string);

 public:
  /**
   * Создаём менеджера, открываем сцену по умолчанию
  */
  SceneManager();
  /**
   * Обновляем сцену
  */
  void update();
  /**
   * Рисуем сцену
  */
  void draw();
};