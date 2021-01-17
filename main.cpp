#include "./lib/object.cpp"
#include "scenes.hpp"
#include <unistd.h>
#include <clocale>
#include <ncurses.h>

int main() {
  try {
    setlocale(LC_ALL, "en_US.UTF-8");
    initscr();
    use_default_colors();
    noecho();
    keypad(stdscr, true);
    
    start_color();
    init_pair(0, COLOR_BLACK, -1);
    init_pair(1, COLOR_RED, -1);
    init_pair(2, COLOR_GREEN, -1);
    init_pair(3, COLOR_YELLOW, -1);
    init_pair(4, COLOR_BLUE, -1);
    init_pair(5, COLOR_MAGENTA, -1);
    init_pair(6, COLOR_CYAN, -1);
    init_pair(7, COLOR_WHITE, -1);
    nodelay(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
  
    //erase();


    Game manager;
    int ch;
    MEVENT evt;



    curs_set(0);

    while (true) {
      manager.pressed.clear();
      manager.mouseEvents.clear();
      while((ch = getch()) != ERR)
        if(ch == KEY_MOUSE) {
          if(getmouse(&evt) == OK) {
            manager.mouseEvents.push_back(evt);
          }
        } else manager.pressed.insert(ch);

      manager.update();
      erase();
      manager.draw();
      refresh();
      flushinp();
      usleep(10000);
    }
    
    erase();
    curs_set(1);
    endwin();
  } catch(const char* e) {
    endwin();
    printf("[ERR] %s\n", e);
  } catch(...) {
    endwin();
    printf("[ERR] Unknown\n");
  }
  return 0;
}