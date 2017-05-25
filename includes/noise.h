#ifndef NOISE_H
# define NOISE_H

# include <stdlib.h>
# include <string.h>
# include <time.h>
# include <stdio.h>

/** noise */
typedef struct	s_noise {
	long long unsigned int	seed;
	unsigned char			p[512];
}				t_noise;

/** noise data structure */
t_noise * 	noiseNew(void);
void 		noiseSeed(t_noise * noise, long long unsigned int seed);
void 		noiseDelete(t_noise * noise);

/** a simple function which gives a 'pseudo-random' integer from the passed one */
unsigned int noiseNextInt(long long unsigned int * seed);

/** simplex noise 2D */
float	snoise2(t_noise * noise, float x, float y);

/** simplex noise 3D */
float	snoise3(t_noise * noise, float x, float y, float z);

/** perlin noise 2D */
float	pnoise2(t_noise * noise, float x, float y);

#endif