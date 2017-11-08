#include <math.h>
#include "game.h"

double		vec_dot(t_vector v1, t_vector v2)
{
  return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

t_vector	vec_scalar_prod(t_vector vec, double a)
{
  t_vector	res;

  res.x = vec.x * a;
  res.y = vec.y * a;
  res.z = vec.z * a;
  return (res);
}

t_vector	vec_scalar_add(t_vector vec, double a)
{
  t_vector	res;

  res.x = vec.x + a;
  res.y = vec.y + a;
  res.z = vec.z + a;
  return (res);
}

t_vector	vec_add(t_vector v1, t_vector v2)
{
  v1.x += v2.x;
  v1.y += v2.y;
  v1.z += v2.z;
  return (v1);
}

t_vector	vec_sub(t_vector v1, t_vector v2)
{
  v1.x -= v2.x;
  v1.y -= v2.y;
  v1.z -= v2.z;
  return (v1);
}

t_vector	vec_set(double x, double y, double z)
{
  t_vector	res;

  res.x = x;
  res.y = y;
  res.z = z;
  return (res);
}

double		vec_norme(t_vector vector)
{
  return (sqrt(vec_dot(vector, vector)));
}

t_vector	vec_normalize(t_vector vector)
{
  return (vec_scalar_prod(vector, 1.0 / vec_norme(vector)));
}

t_vector	vec_cross_prod(t_vector v1, t_vector v2)
{
  t_vector	prod;

  prod.x = v1.y * v2.z - v1.z * v2.y;
  prod.y = v1.z * v2.x - v1.x * v2.z;
  prod.z= v1.x * v2.y - v1.y * v2.x;
  return (prod);
}

t_vector	vec_mat_prod(t_mat mat, t_vector vec)
{
  t_vector	res;

  res.x = vec_dot(mat.ux, vec);
  res.y = vec_dot(mat.uy, vec);
  res.z = vec_dot(mat.uz, vec);
  return (res);
}

t_mat		mat_set(t_vector ux, t_vector uy, t_vector uz)
{
  t_mat		mat;

  mat.ux = ux;
  mat.uy = uy;
  mat.uz = uz;
  return (mat);
}

t_mat		mat_transpose(t_mat src)
{
  t_mat dest;

  dest.ux = vec_set(src.ux.x, src.uy.x, src.uz.x);
  dest.uy = vec_set(src.ux.y, src.uy.y, src.uz.y);
  dest.uz = vec_set(src.ux.z, src.uy.z, src.uz.z);
  return (dest);
}

t_mat		mat_prod(t_mat m1, t_mat m2)
{
  t_mat		res;

  m2 = mat_transpose(m2);
  res.ux = vec_set(vec_dot(m1.ux, m2.ux), vec_dot(m1.ux, m2.uy), vec_dot(m1.ux, m2.uz));
  res.uy = vec_set(vec_dot(m1.uy, m2.ux), vec_dot(m1.uy, m2.uy), vec_dot(m1.uy, m2.uz));
  res.uz = vec_set(vec_dot(m1.uz, m2.ux), vec_dot(m1.uz, m2.uy), vec_dot(m1.uz, m2.uz));
  return (res);
}

t_mat		mat_id()
{
  t_mat		mat;

  mat.ux = vec_set(1, 0, 0);
  mat.uy = vec_set(0, 1, 0);
  mat.uz = vec_set(0, 0, 1);
  return (mat);
}

static t_mat	rot_x(double a)
{
  t_mat		mat;

  mat = mat_id();
  mat.uy.y = cos(a);
  mat.uz.y = sin(a);
  mat.uy.z = -mat.uz.y;
  mat.uz.z = mat.uy.y;
  return (mat);
}

static t_mat	rot_y(double a)
{
  t_mat		mat;

  mat = mat_id();
  mat.ux.x = cos(a);
  mat.ux.z = sin(a);
  mat.uz.x = -mat.ux.z;
  mat.uz.z = mat.ux.x;
  return (mat);
}

static t_mat	rot_z(double a)
{
  t_mat		mat;
  
  mat = mat_id();
  mat.ux.x = cos(a);
  mat.uy.x = sin(a);
  mat.ux.y = -mat.uy.x;
  mat.uy.y = mat.ux.x;
  return (mat);
}

t_mat		rot_set(t_vector angle)
{
  return  (mat_prod(mat_prod(rot_x(angle.x), rot_y(angle.y)), rot_z(angle.z)));
}
