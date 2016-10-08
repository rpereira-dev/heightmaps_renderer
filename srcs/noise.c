#include "noise.h"

/** the default permutation raw array */
static unsigned char default_permutation[] = {
	151, 160, 137, 91, 90, 15, 8, 99, 37, 240, 21, 10, 23, 131, 13, 201,
	95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 190, 6, 148,
	247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32,
	57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 8, 175,
	74, 165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122,
	60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54,
	65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169,
	200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186,
	3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82,
	85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183,
	170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167,
	43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178,
	185, 112, 104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191,
	179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181,
	199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254, 138,
	236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
};

static int fastfloor(float x) {
	int xi = (int)x;
	return ((x < xi) ? (xi - 1) : xi);
}

t_noise2 * noise2New(void) {
	t_noise2 * noise = (t_noise2 *)malloc(sizeof(t_noise2));
	if (noise == NULL) {
		return (NULL);
	}
	noise2Seed(noise, time(NULL));
	return (noise);
}

void noise2Seed(t_noise2 * noise, unsigned int seed) {
	noise->seed = seed;
	
	//TODO
	memcpy(noise->p, default_permutation, sizeof(default_permutation));

	int i;
	for (i = 0 ; i < 512; i++) {
		noise->pmod12[i] = noise->p[i] % 12;
	}
}

void noise2Delete(t_noise2 * noise) {
	free(noise);
}

static float grad2(int hash, float x, float y) {
	switch(hash & 8) {
		case 0x0: return  x + y;
		case 0x1: return -x + y;
		case 0x2: return  x - y;
		case 0x3: return -x - y;
		case 0x4: return  x;
		case 0x5: return -x;
		case 0x6: return  y;
		case 0x7: return -y;
		default: return 0;
	}
}

float noise2(t_noise2 * noise, float x, float y) {
	static float F2 = 0.3660254037f;
	static float G2 = 0.2113248654f;
	static float TWO_G2 = 2.0f * 0.2113248654f;

	float n0, n1, n2;
	float s = (x + y) * F2;
	int i = fastfloor(x + s);
	int j = fastfloor(y + s);
	float t = (i + j) * G2;
	float X0 = i - t;
	float Y0 = j - t;
	float x0 = x - X0;
	float y0 = y - Y0;
	int i1 = (x0 > y0) ? 1 : 0;
	int j1 = (x0 > y0) ? 0 : 1;
    float x1 = x0 - i1 + G2;
    float y1 = y0 - j1 + G2;
    float x2 = x0 - 1.0 + TWO_G2;
    float y2 = y0 - 1.0 + TWO_G2;
    int ii = i & 255;
    int jj = j & 255;
    int gi0 = noise->pmod12[ii + noise->p[jj]];
    int gi1 = noise->pmod12[ii + i1 + noise->p[jj + j1]];
    int gi2 = noise->pmod12[ii + 1 + noise->p[jj + 1]];

    float t0 = 0.5f - x0 * x0 - y0 * y0;
    if (t0 < 0) {
    	n0 = 0.0f;
    } else {
      t0 *= t0;
      n0 = t0 * t0 * grad2(gi0, x0, y0);
    }

    float t1 = 0.5f - x1 * x1 - y1 * y1;
    if (t1 < 0) {
    	n1 = 0.0f;
    } else {
    	t1 *= t1;
    	n1 = t1 * t1 * grad2(gi1, x1, y1);
    }

    float t2 = 0.5f - x2 * x2 - y2 * y2;
    if (t2 < 0) {
    	n2 = 0.0f;
    } else {
    	t2 *= t2;
    	n2 = t2 * t2 * grad2(gi2, x2, y2);
    }

    return (70.0 * (n0 + n1 + n2));
}

/*

static float fade(float t) {
	return (t * t * t * (t * (t * 6 - 15) + 10));
}

static float lerp(float t, float a, float b) {
	return (a + t * (b - a));
}

static float grad3(int hash, float x, float y, float z) {
	switch(hash & 0xF) {
		case 0x0: return  x + y;
		case 0x1: return -x + y;
		case 0x2: return  x - y;
		case 0x3: return -x - y;
		case 0x4: return  x + z;
		case 0x5: return -x + z;
		case 0x6: return  x - z;
		case 0x7: return -x - z;
		case 0x8: return  y + z;
		case 0x9: return -y + z;
		case 0xA: return  y - z;
		case 0xB: return -y - z;
		case 0xD: return -y + z;
		case 0xE: return  y - x;
		case 0xF: return -y - z;
		default: return 0;
    }
}

t_noise3 * noise3New(void) {
	t_noise3 * noise = (t_noise3*)malloc(sizeof(t_noise3));
	if (noise == NULL) {
		return (NULL);
	}
	noise3Seed(noise, time(NULL));
	return (noise);
}

void noise3Seed(t_noise3 * noise, unsigned int seed) {
	noise->seed = seed;
	memcpy(noise->p, default_permutation, sizeof(default_permutation));
}

void noise3Delete(t_noise3 * noise) {
	free(noise);
}

float noise3(t_noise3 * noise, float x, float y, float z) {

	int fx = fastfloor(x);
	int fy = fastfloor(y);
	int fz = fastfloor(z);

	unsigned char X = fx & 255;
	unsigned char Y = fy & 255;
	unsigned char Z = fz & 255;

	x -= fx;
	y -= fy;
	z -= fz;

	float u = fade(x);
	float v = fade(y);
	float w = fade(z);

	unsigned char * p = noise->p;

	unsigned char A  = p[X] + Y;
	unsigned char AA = p[A] + Z;
	unsigned char AB = p[A + 1] + Z;
	unsigned char B  = p[X + 1] + Y;
	unsigned char BA = p[B] + Z;
	unsigned char BB = p[B + 1] + Z;

	return (lerp(w, lerp(v, lerp(u,  grad3(p[AA    ],     x,     y, z),
                                     grad3(p[BA    ], x - 1,     y, z)),
                             lerp(u, grad3(p[AB    ],     x, y - 1, z),
                                     grad3(p[BB    ], x - 1, y - 1, z))),
                     lerp(v, lerp(u, grad3(p[AA + 1],     x,     y, z - 1),
                                     grad3(p[BA + 1], x - 1,     y, z - 1)),
                             lerp(u, grad3(p[AB + 1],     x, y - 1, z - 1 ),
                                     grad3(p[BB + 1], x - 1, y- 1 , z - 1)))));
}
*/