#include "./lib/object.cpp"
#include "scenes.hpp"
#include <unistd.h>
#include <clocale>
#include <ncurses.h>

void getBuff(set<int> & v) {
  int ch;
  v.clear();
  while((ch = getch()) != ERR) v.insert(ch);
}

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
  
    //erase();


    Game manager;


    curs_set(0);

    while (true) {
      getBuff(manager.pressed);
      manager.update();
      erase();
      manager.draw();
      refresh();
      flushinp();
      usleep(200000);
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