#include "cursed.h"
#include "libft.h"

#define CONFIRM_WIDTH       36
#define CONFIRM_HEIGHT      6
#define MARGIN              7

static void         curses_confirm_button(const int y, const int x,
    const char *button, const bool selected)
{
    int             pair;

    pair = COLOR_PAIR((selected == false) ? COLOR_DEFAULT : COLOR_SELECTED);
    attron(pair);
    mvprintw(y, x, "%s", button);
    attroff(pair);
}

static int          curses_confirm_input(struct curses_window *win, int key)
{
    int         *ret = win->userdata;

    if ((key == ARROW_LEFT) || (key == ARROW_RIGHT))
        *ret = !*ret;
    else if (key == '\n')
    {
        win->flags |= WIN_QUIT;
        return (*ret);
    }
    else if ((key == 'y') || (key == 'n'))
    {
        win->flags |= WIN_QUIT;
        return (key == 'y');
    }
    return (0);
}

static int          curses_confirm_draw(struct curses_window *win)
{
    const int ret = *((int *)win->userdata);
    const int line = win->y + (win->h >> 1);
    const int col = win->x + (win->w >> 1);

    curses_confirm_button(line, col - MARGIN, "Yes", ret);
    curses_confirm_button(line, col + MARGIN, "No", !ret);
    return (0);
}

/*
** Prompt a confirmation window then return the use choice,
** if the user quit before selecting a value the intial value will be returned
*/

__attribute((pure))
int                 curses_confirm(struct curses_window *win,
    const char *message, const int initial)
{
    struct curses_window    this;
    int                     ret = initial;
    const size_t            title_len = ft_strlen(message);
    size_t                  width;

    width = title_len + MARGIN;
    if (width < CONFIRM_WIDTH)
        width = CONFIRM_WIDTH;
    this = (struct curses_window) {
        .parent = win,
        .title = message,
        .input = &curses_confirm_input,
        .draw = &curses_confirm_draw,
        .userdata = &ret
    };
    curses_centerfrom_parent(&this, (int)width, CONFIRM_HEIGHT);
    return (curses_new_window(&this));
}
