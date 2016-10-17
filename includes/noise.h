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

t_noise * 	noiseNew(void);
void 		noiseSeed(t_noise * noise, long long unsigned int seed);
void 		noiseDelete(t_noise * noise);

float		noise2(t_noise * noise, float x, float y);
float		noise3(t_noise * noise, float x, float y, float z);

unsigned int	noiseNextInt(long long unsigned int * seed);

#endif