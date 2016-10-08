#ifndef NOISE_H
# define NOISE_H

# include <stdlib.h>
# include <string.h>
# include <time.h>
# include <stdio.h>

/** 2d noise */
typedef struct	s_noise2 {
	unsigned int	seed;
	unsigned char	p[512];
	unsigned char	pmod12[512];
}				t_noise2;

t_noise2 * 	noise2New(void);
void 		noise2Seed(t_noise2 * noise, unsigned int seed);
void 		noise2Delete(t_noise2 * noise);
float		noise2(t_noise2 * noise, float x, float y);

/** 3d noise */
typedef struct	s_noise3 {
	unsigned int	seed;
	unsigned char	p[512];
}				t_noise3;

t_noise3 *	noise3New(void);
void		noise3Seed(t_noise3 * noise, unsigned int seed);
void 		noise3Delete(t_noise3 * noise);
float		noise3(t_noise3 * noise, float x, float y, float z);

#endif