#include "scenes.hpp"
#include <unistd.h>
#include <clocale>
#include <ncurses.h>

int main() {
  try {
    std::setlocale(LC_ALL, "en_US.UTF-8");
    initscr();
    use_default_colors();
    noecho();
    
    start_color();
    init_pair(8, COLOR_BLACK, -1);
    init_pair(1, COLOR_RED, -1);
    init_pair(2, COLOR_GREEN, -1);
    init_pair(3, COLOR_YELLOW, -1);
    init_pair(4, COLOR_BLUE, -1);
    init_pair(5, COLOR_MAGENTA, -1);
    init_pair(6, COLOR_CYAN, -1);
    init_pair(7, COLOR_WHITE, -1);
    halfdelay(100);
  
    //erase();


    SceneManager manager;


    curs_set(0);
    while (true) {
      manager.update();
      erase();
      manager.draw();
      refresh();
      usleep(1000000);
    }
    
    erase();
    curs_set(1);
    endwin();
  } catch(const char* e) {
    endwin();
    printf("[ERR] %s\n", e);
  } catch(...) {
    endwin();
  }
  return 0;
}