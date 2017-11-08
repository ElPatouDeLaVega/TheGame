#include "game.h"

GC		get_color(Display *dpy, int screen, char *bg, char *fg)
{
  Colormap	colormap;
  XColor	xcolor[2];
  XGCValues	gcv;
  int		i;

  i = -1;
  colormap = DefaultColormap(dpy, screen);
  while (++i < 2)
    {
      XParseColor(dpy, colormap, (i == 0) ? bg : fg, &xcolor[i]);
      XAllocColor(dpy, colormap, &xcolor[i]);
    }
  gcv.background = xcolor[0].pixel;
  gcv.foreground = xcolor[1].pixel;
  return (XCreateGC(dpy, RootWindow(dpy, screen), GCForeground | GCBackground, &gcv));
}

t_vector	cam_straf(t_cam cam, int dir)
{
  return ((vec_add(vec_scalar_prod(cam.axis.ux, dir * 20), cam.pos)));
}

t_vector	cam_move(t_cam cam, int dir)
{
  return (vec_add(vec_scalar_prod(cam.axis.uz, dir * 20), cam.pos));
}

void		display_axis(t_cam cam, Display *dpy, Window win, GC *rgb)
{
  t_vector	orig;

  orig = vec_set(50, 50, 0);
  cam.axis.ux = vec_add(orig, vec_scalar_prod(cam.axis.ux, 20));
  cam.axis.uy = vec_add(orig, vec_scalar_prod(cam.axis.uy, 20));
  cam.axis.uz = vec_add(orig, vec_scalar_prod(cam.axis.uz, 20));
  XDrawLine(dpy, win, rgb[0],
	    /*x1*/orig.x,
	    /*y1*/orig.y,
	    /*x2*/cam.axis.ux.x,
	    /*y2*/cam.axis.ux.y);
  XDrawLine(dpy, win, rgb[1],
	    /*x1*/orig.x,
	    /*y1*/orig.y,
	    /*x2*/cam.axis.uy.x,
	    /*y2*/cam.axis.uy.y);
  XDrawLine(dpy, win, rgb[2],
	    /*x1*/orig.x,
	    /*y1*/orig.y,
	    /*x2*/cam.axis.uz.x,
	    /*y2*/cam.axis.uz.y);
}
  

void		screen_projection(t_map map, t_cam cam, t_coord s_win)
{
  int		i;
  int		size;

  size = map.size.x * map.size.y;
  i = -1;
  while (++i < size)
    {
      map.relat[i] = vec_mat_prod(cam.axis, vec_sub(map.orig[i], cam.pos));
      map.relat[i].x /= -map.relat[i].z / 1000;
      map.relat[i].y /= -map.relat[i].z / 1000;
      map.relat[i] = vec_add(map.relat[i], vec_set(s_win.x / 2, s_win.y/2, 50));
      //printf("%d)\t%f : %f : %f\n", i, map.relat[i].x, map.relat[i].y, map.relat[i].z);
    }
}
