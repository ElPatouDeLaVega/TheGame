#include "game.h"
#define _GNU_SOURCE

t_coord		*ft_getcoord(t_vector *vmap, int size)
{
  int		i;
  t_coord	*cmap;

  if (!(cmap = malloc(sizeof(t_coord) * size)))
    return (0);
  i = -1;
  while (++i < size)
    {
      cmap[i].x = vmap[i].x + vmap[i].z / 2;
      cmap[i].y = vmap[i].y + vmap[i].z / 2;      
    }
  return (cmap);
}



void		ft_tracemap(Display *dpy, Window win, GC gc, GC line, t_vector *map, t_coord s_map)
{
  int		i;
  int		f_size;
  XPoint	pol[3];

  f_size = s_map.x * s_map.y;
  i = -1;
  while (++i < f_size - s_map.x - 1)
    {
      if (i % s_map.x != s_map.x - 1)
	{
	  pol[0].x = map[i].x;
	  pol[0].y = map[i].y;
	  pol[1].x = map[i + 1].x;
	  pol[1].y = map[i + 1].y;
	  pol[2].x = map[i + s_map.x].x;
	  pol[2].y = map[i + s_map.x].y;
	  XFillPolygon(dpy, win, gc, pol, 3, Convex, CoordModeOrigin);
	  pol[0].x = map[i + s_map.x + 1].x;
	  pol[0].y = map[i + s_map.x + 1].y;
	  pol[1].x = map[i + 1].x;
	  pol[1].y = map[i + 1].y;
	  pol[2].x = map[i + s_map.x].x;
	  pol[2].y = map[i + s_map.x].y;
	  XFillPolygon(dpy, win, gc, pol, 3, Convex, CoordModeOrigin);
	}
    }
  usleep(0.001);
  i = -1;
  while (++i < f_size - 1)
    {
      if (i % s_map.x != s_map.x - 1 && map[i].z < 0 && map[i + 1].z < 0)
	XDrawLine(dpy, win, line,
		  map[i].x,//x1
		  map[i].y,//y1
		  map[i + 1].x,//x2
		  map[i + 1].y);//y2
    }
  i = -1;
  while (++i < f_size - s_map.x && map[i].z < 0 && map[i + s_map.x].z < 0)
    {
	XDrawLine(dpy, win, line,
		  map[i].x,
		  map[i].y,
		  map[i + s_map.x].x,
		  map[i + s_map.x].y);
		  }
}

t_vector	*ft_getz(t_vector *map, char *path)
{
  char		buffer[15];
  int		fd;
  int		i, x, y, s;
  char		*str_map;

  buffer[14] = 0;
  fd = open(path, O_RDONLY);
  read(fd, buffer, 14);
  x = atoi(buffer);
  y = atoi(buffer + 5);
  s = atoi(buffer + 10);
  //  printf("%s\n", buffer);
  //  printf("%d x %d : %d\n", x, y, s);
  if (!(str_map = malloc(sizeof(char) * (s + 1))))
    return (map);
  str_map[s] = 0;
  read(fd, str_map, s);
  printf("%s\n", str_map);
  while (++i < x * y)
    {
      map[i].z = atoi(str_map + (i * 3)) * 5;
      //printf("%f : %f : %f\n", map[i].x, map[i].y, map[i].z);
    }
  return (map);
}

t_vector	*ft_getvectxy(t_coord s_map, t_coord s_win)
{
  t_vector	*map;
  int		i;
  int		f_size;
  t_coord	delta;

  f_size = s_map.x * s_map.y;
  if (!(map = malloc(sizeof(t_vector) * f_size)))
    return (0);
  delta.x = (s_win.x - 100) / s_map.x;
  delta.y = (s_win.y - 100) / s_map.y;
  i = -1;
  while (++i < f_size)
    {
      map[i].x = (i / s_map.x + 1) * delta.x;
      map[i].y = (i % s_map.x + 1) * delta.y;
      map[i].z = 0;
      printf("%d)\t%f : %f\n", i, map[i].x, map[i].y);
    }
  return (map);
}

t_map		map_get(char *path, t_coord win_size)
{
  t_map		map;
  FILE		*fp;
  char		*line = NULL;
  size_t	len = 0;
  ssize_t	read;
  t_coord	min;
  t_coord	max;
  int		i;
  int		size;
  int		l;
  
  if ((fp = fopen(path, "r")) == NULL)
    exit(0);
  read = getline(&line, &len, fp);
  printf("%d\n", map.size.x = atoi(line + 2));
  read = getline(&line, &len, fp);
  printf("%d\n", map.size.y = atoi(line + 2));
  size = map.size.x * map.size.y;
  if (!(map.orig = malloc(sizeof(t_vector) * size)))
    exit(0);
  if (!(map.relat = malloc(sizeof(t_vector) * size)))
    exit(0);
  min.x = win_size.x / 2;
  min.y = win_size.y / 2;
  max.x = win_size.x;
  max.y = win_size.y; 
  i = -1;
  while (++i < size)
    {
      if (i % map.size.x == 0)
	{
	  read = getline(&line, &len, fp);
	  l = 0;
	}
      map.orig[i].x =  i % map.size.x * ((max.x - min.x) / map.size.x);
      map.orig[i].y =  i / map.size.x * ((max.x - min.x) / map.size.x);
      map.orig[i].z = atoi(line + l) * 7;
      l += 3;
    }
  return (map);
}

int		main(int ac, char **av)
{
  /* Base X11 Var */
  Display	*dpy;
  int		screen;
  Drawable	win;
  Window	childwin;
  XEvent	event;

  /* X11 Context var */
  int		x, y, width, height;
  unsigned int	border_width, depth;
  Window	root_win;
  GC		gc_green;
  GC		rgb[3];

  /* Text Variables */
  XFontStruct	*font;
  XTextItem	ti[1];

  /* Fdf Variables */
  t_coord	s_win, s_map;
  t_map		mmap;
  t_vector	*map;
  t_cam		cam;
  //t_coord	*cmap;

  cam.pos = vec_set(0, 0, 20);
  cam.axis = mat_id();
  cam.axis = rot_set(vec_set(0,1,0));
  s_win.x = 1280;
  s_win.y = 720;
  s_map.x = 10;
  s_map.y = 10;
  if (!(map = ft_getvectxy(s_map, s_win)))
    return (0);
  map = ft_getz(map, "map");
  mmap.orig = map;
  mmap.size = s_map;
  dpy = XOpenDisplay(NULL);
  
  if (dpy == NULL)
    {
      printf("Cannot open Display\n");
      return (0);
    }
  else
    printf("Connexion OK\n");
  
  screen = DefaultScreen(dpy);
  
  /* parent window */
  win = XCreateSimpleWindow(dpy, RootWindow(dpy, screen),
			    0, 0, //origin
			    1280, 720, //size
			    1, //border line
			    WhitePixel(dpy, screen), //border color
			    BlackPixel(dpy, screen) //window color
			    );
  XSelectInput(dpy, win, ExposureMask | KeyPressMask);
  XMapWindow(dpy, win);

  XGetGeometry(dpy, win, &root_win, &x, &y, &width, &height, &border_width, &depth);
  gc_green = get_color(dpy, screen, "rgb:00/00/00", "rgb:2f/f0/00");
  rgb[0] = get_color(dpy, screen, "rgb:00/00/00", "rgb:ff/00/00");
  rgb[1] = get_color(dpy, screen, "rgb:00/00/00", "rgb:00/ff/00");
  rgb[2] = get_color(dpy, screen, "rgb:00/00/00", "rgb:00/00/ff");

  /*	polygon		*/
  Drawable	d;
  XPoint	points[3];
  int		npoint;
  int		shape;
  int		mode;

  points[0].x = (short)10;
  points[0].y = (short)10;
  points[1].x = (short)10;
  points[1].y = (short)30;
  points[2].x = (short)30;
  points[2].y = (short)10;
  
  mmap = map_get("map2", s_win);
  
  cam.pos.x = 119.266142;
  cam.pos.y = 371.206638;
  cam.pos.z = 432.096049;
  cam.axis.ux.x = 0.995950;
  cam.axis.ux.y = 0.035649;
  cam.axis.ux.z = 0.082545;
  cam.axis.uy.x = -0.000700;
  cam.axis.uy.y = 0.921094;
  cam.axis.uy.z = -0.389341;
  cam.axis.uz.x = -0.089912;
  cam.axis.uz.y = 0.387706;
  cam.axis.uz.z = 0.917388;
  
  //if (!(mmap.relat = malloc(sizeof(t_vector) * mmap.size.x * mmap.size.y)))
  //return (0);
  screen_projection(mmap, cam, s_win);
  double mescouilles;
  
  while (1)
    {
      XNextEvent(dpy, &event);
      if (event.type == KeyPress)
	{
	  if ((event.xkey.keycode == K_LF || event.xkey.keycode == K_RT) ||
	      (event.xkey.keycode == K_UP || event.xkey.keycode == K_DW) ||
	      (event.xkey.keycode == K_SPC || event.xkey.keycode == K_SHIFT) ||
	      (event.xkey.keycode >= K_A && event.xkey.keycode <= K_E) ||
	      (event.xkey.keycode >= K_Q && event.xkey.keycode <= K_D))
	    {
	      if (event.xkey.keycode == K_UP || event.xkey.keycode == K_DW)
		cam.pos = cam_move(cam, (int)event.xkey.keycode - 114);
	      else if (event.xkey.keycode == K_A || event.xkey.keycode == K_E)
		{
		  mescouilles = (int)event.xkey.keycode - K_Z;
		  //printf("%f\n", mescouilles * 0.1);
		  cam.axis = mat_prod(cam.axis, rot_set(vec_set(0, 0, mescouilles * 0.1)));
		}
	      else if (event.xkey.keycode == K_Q || event.xkey.keycode == K_D)
		{
		  mescouilles = (int)event.xkey.keycode - K_S;
		  //printf("%f\n", mescouilles * 0.1);
		  cam.axis = mat_prod(cam.axis, rot_set(vec_set(mescouilles * 0.1, 0, 0)));
		}
	      else if (event.xkey.keycode == K_Z || event.xkey.keycode == K_S)
		{
		  mescouilles = (event.xkey.keycode == K_Z) ? 1 : -1;
		  //printf("%f\n", mescouilles * 0.1);
		  cam.axis = mat_prod(cam.axis, rot_set(vec_set(0, mescouilles * 0.1, 0)));
		}
	      else if (event.xkey.keycode == K_LF || event.xkey.keycode == K_RT)
		{
		  mescouilles = (event.xkey.keycode == K_RT) ? 1 : -1;
		  //printf("%f\n", mescouilles);
		  cam.pos = cam_straf(cam, mescouilles);
		}
	      else if (event.xkey.keycode == K_SPC || event.xkey.keycode == K_SHIFT)
		{
		  mescouilles = (event.xkey.keycode == K_SPC) ? 1 : -1;
		  //printf("%f\n", mescouilles);
		  cam.pos = vec_add(vec_scalar_prod(cam.axis.uy, mescouilles * 20), cam.pos);
		}
	      screen_projection(mmap, cam, s_win);
	      XClearArea(dpy, win, 0, 0, width, height, 1);
	      display_axis(cam, dpy, win, rgb);
	      ft_tracemap(dpy, win, rgb[0], gc_green, mmap.relat, mmap.size);
	    }
	  if (event.xkey.keycode == 9)
	    {
	      printf("This is the end\n");
	      exit(0);
	      return (0);
	    }
	  if (event.xkey.keycode == K_I)
	    printf("pos : %f:%f:%f\nmat : \t%f:%f:%f\n\t%f:%f:%f\n\t%f:%f:%f\n", cam.pos.x, cam.pos.y,cam.pos.z, cam.axis.ux.x, cam.axis.ux.y, cam.axis.ux.z, cam.axis.uy.x, cam.axis.uy.y, cam.axis.uy.z, cam.axis.uz.x, cam.axis.uz.y, cam.axis.uz.z);
	  //printf("key %d pressed\n", event.xkey.keycode);
	}
      if (event.xany.window == win)
	{
	  if (event.type == Expose)
	    {
	      ft_tracemap(dpy, win, rgb[0], gc_green, mmap.relat, mmap.size);
	      XGetGeometry(dpy, win, &root_win, &x, &y, &width, &height, &border_width, &depth);
	    }
	}
    }  
  return (0);
}
