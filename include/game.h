#ifndef GAME_H
# define GAME_H

# include <stdio.h>
# include <stdlib.h>
# include <X11/Xlib.h>
# include <X11/Xutil.h>
# include <X11/Xos.h>
# include <X11/Xatom.h>

# define K_I 31
# define K_A 24
# define K_Z 25
# define K_E 26
# define K_Q 38
# define K_S 39
# define K_D 40
# define K_UP 111
# define K_DW 116
# define K_LF 113
# define K_RT 114
# define K_SHIFT 94
# define K_SPC 65

typedef struct  s_coord
{
  int		x;
  int		y;
}		t_coord;

typedef struct  s_vector
{
  double	x;
  double	y;
  double	z;
}		t_vector;

typedef	struct	s_mat
{
  t_vector	ux;
  t_vector	uy;
  t_vector	uz;
}		t_mat;

typedef	struct	s_win
{
  Display	*dpy;
  int		screen;
  Window	win;
  XEvent	event;
}		t_win;

typedef	struct	s_map
{
  t_coord	size;
  t_vector	*orig;
  t_vector	*relat;
}		t_map;

typedef	struct	s_cam
{
  t_vector	pos;
  t_mat		axis;
}		t_cam;

/*	gmath.c		*/

double          vec_dot(t_vector v1, t_vector v2);
t_vector        vec_scalar_prod(t_vector vec, double a);
t_vector        vec_scalar_add(t_vector vec, double a);
t_vector        vec_add(t_vector v1, t_vector v2);
t_vector        vec_sub(t_vector v1, t_vector v2);
t_vector        vec_set(double x, double y, double z);
double          vec_norme(t_vector vector);
t_vector        vec_normalize(t_vector vector);
t_vector        vec_cross_prod(t_vector v1, t_vector v2);
t_vector        vec_mat_prod(t_mat mat, t_vector vec);
t_mat           mat_set(t_vector ux, t_vector uy, t_vector uz);
t_mat           mat_transpose(t_mat src);
t_mat           mat_prod(t_mat m1, t_mat m2);
t_mat           mat_id();
t_mat           rot_set(t_vector angle);

/*	game.c		*/
void            screen_projection(t_map map, t_cam cam, t_coord s_win);
t_vector        cam_move(t_cam cam, int dir);
t_vector        cam_straf(t_cam cam, int dir);
GC              get_color(Display *dpy, int screen, char *bg, char *fg);
void		display_axis(t_cam cam, Display *dpy, Window win, GC *rgb);

#endif /* GAME_H_ */
