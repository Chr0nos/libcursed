#ifndef CURSED_H
# define CURSED_H
# include <ncurses.h>

# define WIN_NOBORDER		(1u << 0)
# define WIN_QUIT			(1u << 1)
# define WIN_NOQ			(1u << 2)
# define WIN_CONFIRM_CLOSE	(1u << 3)
# define WIN_NOINPUT		(1u << 4)
# define NO_REFRESHPARENT	(1u << 5)

# define ARROW_UP      		65
# define ARROW_DOWN      	66
# define ARROW_RIGHT     	67
# define ARROW_LEFT     	68

# define COLOR_DEFAULT		0
# define COLOR_SELECTED		1
# define COLOR_WINBORDERS	2

struct curses_window {
	struct curses_window	*parent;
	const char				*title;
	int						x;
	int						y;
	int						w;
	int						h;
	int						(*init)(struct curses_window *);
	int						(*draw)(struct curses_window *);
	int						(*input)(struct curses_window *, int);
	int						(*quit)(struct curses_window *);
	size_t					flags;
	WINDOW					*object;
	void					*userdata;
};

/*
** Future curse lib functions
*/

int             		curses_confirm(struct curses_window *win,
    const char *message, const int initial);
int             		curses_new_window(struct curses_window *win);
void            		curses_window_info(struct curses_window *win);
void            		curses_window_decorate(struct curses_window *win);
size_t					curses_window_level(const struct curses_window *win);
struct curses_window	*curses_centerfrom_parent(struct curses_window *win,
    const int w, const int h);
void            		curses_puts_center(struct curses_window *win,
	const int line, const char *text, size_t len);
void  					curses_box(int x, int y, int w, int h);
void         			curses_refresh_parents(struct curses_window *win);

#endif
