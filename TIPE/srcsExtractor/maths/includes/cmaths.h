#ifndef CMATHS_H
# define CMATHS_H

# include <math.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>

# ifndef DEG_TO_RAD
#  define DEG_TO_RAD(X) (X * 0.01745329251f)
# endif

# ifndef RAD_TO_DEG
#  define RAD_TO_DEG(X) (X * 57.2957795131f)
# endif

# ifndef MAX
#  define MAX(X, Y) (X > Y ? X : Y)
# endif

# ifndef MIN
#  define MIN(X, Y) (X < Y ? X : Y)
# endif

# ifndef ABS
#  define ABS(X) (X < 0 ? -X : X)
# endif

int	cmaths_init(void);
int	cmaths_deinit(void);

float	acos_f(float x);
float	asin_f(float x);
float 	atan_f(float x);

float	sin_f(float x);
float	cos_f(float x);
float	tan_f(float x);

float	sqrt_f(float x);

#endif