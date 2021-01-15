# Движок для терминальных игр под Linux

## Что все эти папки и файлы значат?
- `lib` - файлы движка
  - `object.hpp` - Основной файл движка, где находится почти вся логика 
  - `sceneList.hpp` - Файл с макросами для описания списка сцен
  - `components` - папка с основными компонентами
  - `rapidxml` - XML парсер, мощная штука
- `scenes` - папка со сценами. Там можно создавать свои сцены
- `sprites` - папка с картинками в XML формате
- `anims.txt` - Файлик с разной ASCII графикой, придумываемой мной для игры.
- `main.cpp` - Основной файл, **при дополнении игры не менять.**
- `scenes.hpp` - Файл со списком сцен

## Что из себя представляют XML спрайты?
Вот пример (это ёлка):
```xml
<!-- Основной тег -->
<sprite>
  <!-- Анимация "default" -->
  <animation name="default">
    <!-- Единственный кадр -->
    <frame>
      <!-- Картинка по рядам -->
      <row color="  g  " chars="  .  "></row>
      <row color=" Ggg " chars=" /|\ "></row>
      <row color="Ggggg" chars="//|\\"></row>
      <row color="GGggg" chars="//|\\"></row>
      <row color="Ggggg" chars="//|\\"></row>
      <row color="  y  " chars="  |  "></row>
    </frame>
  </animation>
  <!-- Тег анимации "decorated" - на ёлке висит гирлянда -->
  <animation name="decorated">
    <!-- Первый кадр -->
    <frame>
      <!-- chars - это рисуемые символы -->
      <row color="  g  " chars="  .  "></row>
      <row color=" Ggg " chars=" /|\ "></row>
      <row color="GRggg" chars="/*|\\"></row>
      <row color="GGgLg" chars="//|*\"></row>
      <row color="GYggg" chars="/*|\\"></row>
      <row color="  y  " chars="  |  "></row>
    </frame>
    <!-- Второй -->
    <frame>
      <!-- color - закодированный символы. Пробел - прозрачный цвет -->
      <row color="  g  " chars="  .  "></row>
      <row color=" Ggg " chars=" /|\ "></row>
      <row color="GLggg" chars="/*|\\"></row>
      <row color="GGgYg" chars="//|*\"></row>
      <row color="GRggg" chars="/*|\\"></row>
      <row color="  y  " chars="  |  "></row>
    </frame>
    <!-- Третий -->
    <frame>
      <row color="  g  " chars="  .  "></row>
      <row color=" Ggg " chars=" /|\ "></row>
      <row color="GYggg" chars="/*|\\"></row>
      <row color="GGgRg" chars="//|*\"></row>
      <row color="GLggg" chars="/*|\\"></row>
      <row color="  y  " chars="  |  "></row>
    </frame>
  </animation>
</sprite>
```
Вот список цветов:

|       | Нормальные     |       | Яркие                |
|-------|----------------|-------|----------------------|
| код   | имя            | код   | имя                  |
|   b   | Чёрный         |   B   | Тёмно-серый          |
|   r   | Красный        |   R   | Светлый красный      |
|   g   | Зелёный        |   G   | Светлый зелёный      |
|   y   | Жёлтый         |   Y   | Светлый жёлтый       |
|   l   | Синий          |   L   | Светлый синий        |
|   p   | Фиолетовый     |   P   | Светлый фиолетовый   |    
|   c   | Сине-зелёный   |   C   | Сине-зелёный светлый |
|   w   | Серый светлый  |   W   | Белый                |

## Как расширять игру?

### Пишем компоненты

Создаём свой файл в `components/` (например `pechka.hpp`) и пишем код:
```cpp
#include "lib/object.h" // Подключаем основную библиотеку

// Создаём класс своего компонента
class Pechka : public Render {
  unsigned depth = 0;

  // Запустится когда компонент добавят
  void start () { printw("Топим печку\n"); }
  // Один раз в кадр, обновляем переменные и делаем расчёты
  void update () { depth++; }
  // Отображаем компонент
  void draw () { printw("Печка натопилась на %u метров\n", depth); }
}
```
| ❌ | Важно: `cout` нельзя использовать в игре. Это всё сломает |
| -- | --- |  
|    |   Если вам ныжно что-то выводить, читать [тут](https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/index.html)  |

Если компонент не должен отображаться, то наследуется он от `Component` и метод `draw` не обьявляется.

### Создаём сцену
Делаем файл сцены в `/scenes` (назовём его `izba.hpp`)
Пишем там код:
```cpp
#include "lib/object.hpp" // Всё то же

#include "components/pechka.hpp" // Подключаем свой компонент

// Сцена
class Izba : Scene {
  // Запускается при создании сцены
  void create() {
    // Доюавляем обьект на сцену
    // base() это корневой компонент сцены
    // В скобках название обьекта в игровом мире
    auto pechka = base().addChild<Object>("Pechka");
    // Добавляем компонент
    pechka->addComponent<Pechka>();
  }
}
```

Теперь регистрируем сцену в `scenes.hpp`
```cpp
#include "lib/object.h"
#include "lib/sceneList.hpp"

#include "scenes/izba.hpp"

scenes
  // Начальная сцена
  startScene(Izba)
  // Обычная сцена
  // scene(Les)
  // scene(Kabinet)
thatsall
```

Должно заработать.

## Как запускать?

### Если вы на `Repl.it`

Напишите в терминал это:
```shell
clang -std=c++11 -lncurses main.cpp -o main -I `pwd`
```