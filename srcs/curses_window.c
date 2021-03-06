#include "cursed.h"
#include "libft.h"
#define MAX_WIN_LEVEL   15

/*
** put a string in the center of a window at the specified "line"
** len : the string lenght to put
** note: the line has to fit into the window otherwise it will be truncated.
*/

void                curses_puts_center(
    struct curses_window *win,
    const int line,
    const char *text,
    size_t len)
{
    if (win->w < 2)
        return ;
    if (len > (size_t)win->w - 2)
        len = (size_t)win->w - 2;
    mvprintw(
        win->y + line,
        win->x + 1 + (win->w / 2) - ((int)len / 2),
        "%.*s",
        len,
        text);
}

void                curses_window_decorate(struct curses_window *win)
{
    if (!(win->flags & WIN_NOBORDER))
        curses_box(win->x, win->y, win->w, win->h);
    if (win->title)
        curses_puts_center(win, 1, win->title, ft_strlen(win->title));
}

#define CURSES_WINRET(win, ret, f) \
    if (f != NULL) \
    { \
        ret = f(win); \
        if (win->flags & WIN_QUIT) \
            return ret; \
    }

/*
** This is a main loop for window system, util the window is not exited this
** function will not return.
** to close the window the caller has to set a flag WIN_QUIT then the loop
** will return the same data as the user specified.
** to return pointers add a field in your structure provided by userdata
*/

int                 curses_new_window(struct curses_window *win)
{
    int             ret;
    int             key;

	if (!win->object)
		win->object = newwin(win->h, win->w, win->y, win->x);
    CURSES_WINRET(win, ret, win->init)
    do
    {
        key = 0;
        curses_window_decorate(win);
        CURSES_WINRET(win, ret, win->draw)
        if (!win->draw)
            refresh();
        if (!(win->flags & WIN_NOINPUT))
        {
            key = getch();
            if ((key == 'p') && (curses_window_level(win) < MAX_WIN_LEVEL))
                curses_window_info(win);
            if (win->input)
            {
                ret = win->input(win, key);
                if (win->flags & WIN_QUIT)
                    return (ret);
            }
            if ((win->flags & WIN_CONFIRM_CLOSE) && (key == 'q') &&
                    (!curses_confirm(win, "Quit ?", false)))
                key = 0;
        }
        else
            usleep(100);
    }
    while (((key != 'q') || (win->flags & WIN_NOQ)) && (!(win->flags & WIN_QUIT)));
	delwin(win->object);
    win->object = NULL;
    CURSES_WINRET(win, ret, win->quit)
    if ((win->parent) && (!(win->flags & NO_REFRESHPARENT)))
        curses_refresh_parents(win->parent);
    return (0);
}

/*
** this forces all parents of a window to re-draw, this should be used when
** a window is destroyed, because we need to re-draw what is behind but
** nothing says this will be the first parent, because window can be anywhere.
*/

void         curses_refresh_parents(struct curses_window *win)
{
    if (!win)
        return ;
    if (win->parent)
        curses_refresh_parents(win->parent);
    curses_window_decorate(win);
    if (win->draw)
        win->draw(win);
}

/*
** center the given window from it's parent coordonates and return a pointer
** to the window.
*/

struct curses_window    *curses_centerfrom_parent(struct curses_window *win,
    const int w, const int h)
{
    if (!win->parent)
        return (win);
    win->x = win->parent->x + (win->parent->w >> 1) - (w >> 1);
    win->y = win->parent->y + (win->parent->h >> 1) - (h >> 1);
    win->w = w;
    win->h = h;
    return (win);
}

/*
** return the amount of parents of "win"
*/

size_t			curses_window_level(const struct curses_window *win)
{
    size_t      level;

    level = 0;
    while (win)
    {
        level++;
        win = win->parent;
    }
    return (level);
}
