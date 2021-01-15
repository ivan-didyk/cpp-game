#include "lib/object.h"
#include <ncurses.h>

struct WindowBorder {
  string 	ls, rs, ts, bs, 
	 	      tl, tr, bl, br;
};

struct Window {
  unsigned startx, starty;
  unsigned width, height;
  WindowBorder border;
  int color;
};

void create_box(Window &win, bool flag) {
  unsigned i, j;
	unsigned x, y, w, h;

	x = win.startx;
	y = win.starty;
	w = win.width;
	h = win.height;

	if(flag == true){
     attron(COLOR_PAIR(win.color));
    mvprintw(y    , x    , "%s", win.border.tl.c_str());
    mvprintw(y    , x + w, "%s", win.border.tr.c_str());
    mvprintw(y + h, x    , "%s", win.border.bl.c_str());
    mvprintw(y + h, x + w, "%s", win.border.br.c_str());

    for(i = y + 1; i < y + h; ++i) {
      mvprintw(i, x    , "%s", win.border.ls.c_str());
      mvprintw(i, x + w, "%s", win.border.rs.c_str());
    }

    for(i = x + 1; i < x + w; ++i) {
      mvprintw(y  , i, "%s", win.border.ts.c_str());
      mvprintw(y+h, i, "%s", win.border.bs.c_str());
    }
     attroff(COLOR_PAIR(win.color));
	}
	else
		for(j = y; j <= y + h; ++j)
			for(i = x; i <= x + w; ++i)
				mvaddch(j, i, ' ');
}

class WindowRender : public Render {
  Window win;
  Transform *transform;

  void start() {
    transform = getComponent<Transform>();
    win.border.ls = "┃";
    win.border.rs = "┃";
    win.border.ts = "━";
    win.border.bs = "━";
    win.border.tl = "┏";
    win.border.tr = "┓";
    win.border.bl = "┗";
    win.border.br = "┛";
  }

  void update() {
    win.startx = transform->x();
    win.starty = transform->y();
  }

  void draw() {
    create_box(win, true);
  }

  void setWidth(unsigned width) { win.width = width; }
  void setHeight(unsigned height) { win.height = height; }

 public:
  WindowRender(unsigned width, unsigned height, int color) {
    win.width  = width;
    win.height = height;
    win.color = color;
  }
};