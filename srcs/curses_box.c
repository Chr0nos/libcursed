#include "cursed.h"
#define CORNER_TOP_LEFT     'A'
#define CORNER_TOP_RIGHT    'B'
#define CORNER_BOT_LEFT     'C'
#define CORNER_BOT_RIGHT    'D'
#define BOX_BORDER      '|'

/*
** just draw an empty rectangle on the screen
** if "x" or "y" is negative then the center of the screen will be choosed
*/

void  curses_box(int x, int y, int w, int h)
{
    int     line;
    int     col;
	const int	pair = COLOR_PAIR(COLOR_WINBORDERS);

	attron(pair);
    if (x < 0)
        x = (LINES >> 1) - (w >> 1);
    if (y < 0)
        y = (COLS >> 1) - (h >> 1);
    line = y + h;
    col = x + w - 2;
    mvprintw(y, x, "%c", CORNER_TOP_LEFT);
    mvprintw(y, col + 1, "%c", CORNER_TOP_RIGHT);
    mvprintw(line, x, "%c", CORNER_BOT_LEFT);
    mvprintw(line, col + 1, "%c", CORNER_BOT_RIGHT);
    while (col > x)
    {
        mvprintw(y, col, "-");
        mvprintw(line, col, "-");
        col--;
    }
    line--;
    while (line > y)
    {
        mvprintw(line, x, "%c%-*s%c", BOX_BORDER, w - 2, "", BOX_BORDER);
        line--;
    }
	attroff(pair);
}
