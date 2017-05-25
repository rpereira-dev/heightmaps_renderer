#include "noise.h"

//SIMPLEX NOISE IMPLEMENTATION


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

t_noise * noiseNew(void) {
	t_noise * noise = (t_noise *)malloc(sizeof(t_noise));
	if (noise == NULL) {
		return (NULL);
	}
	noiseSeed(noise, time(NULL));
	return (noise);
}


unsigned int noiseNextInt(long long unsigned int * seed) {
	*seed = 6364136223846793005ULL * *seed + 1;
	unsigned int x = *seed >> 32;
	x ^= x >> 11;
	x ^= (x << 7) & 0x9D2C5680;
	x ^= (x << 15) & 0xEFC60000;
	x ^= x >> 18;
	return (x);
}

void noiseSeed(t_noise * noise, long long unsigned int seed) {
	noise->seed = seed;

	//copy default permutation
	memcpy(noise->p, default_permutation, sizeof(default_permutation));

	int swap_from, swap_to;
	unsigned char tmp;

	//process permutations
	int i;
	for (i = 0 ; i < 512; i++) {

		//generate next pemrutation
		swap_from	= noiseNextInt(&seed) & 255; //mod 256
		swap_to		= noiseNextInt(&seed) & 255; //mod 256

		//do the swap
		tmp 				= noise->p[swap_from];
		noise->p[swap_from]	= noise->p[swap_to];
		noise->p[swap_to] 	= tmp;
	}
}

void noiseDelete(t_noise * noise) {
	free(noise);
}

static float grad2(int hash, float x, float y) {
	switch(hash & 0x8) {
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

float snoise2(t_noise * noise, float x, float y) {
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
    int gi0 = noise->p[ii + noise->p[jj]];
    int gi1 = noise->p[ii + i1 + noise->p[jj + j1]];
    int gi2 = noise->p[ii + 1 + noise->p[jj + 1]];

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
        case 0xC: return  y + x;
        case 0xD: return -y + z;
        case 0xE: return  y - x;
        case 0xF: return -y - z;
		default: return 0;
    }
}


float snoise3(t_noise * noise, float x, float y, float z) {
	static float F3 = 1.0f / 3.0f;
	static float G3 = 1.0f / 6.0f;

	float n0, n1, n2, n3;

	float s = (x + y + z) * F3;
	int i = fastfloor(x + s);
	int j = fastfloor(y + s);
	int k = fastfloor(z + s);

	float t = ( i + j + k ) * G3;

	float X0 = i - t;
	float Y0 = j - t;
	float Z0 = k - t;

	float x0 = x - X0;
	float y0 = y - Y0;
	float z0 = z - Z0;

	int i1, j1, k1;
	int i2, j2, k2;
	if (x0 >= y0) {
		if (y0 >= z0) {
			i1 = 1;
			j1 = 0;
			k1 = 0;
			i2 = 1;
			j2 = 1;
			k2 = 0;
		} else if (x0 >= z0) {
            i1 = 1;
            j1 = 0;
            k1 = 0;
            i2 = 1;
            j2 = 0;
            k2 = 1;
        } else {
            i1 = 0;
            j1 = 0;
            k1 = 1;
            i2 = 1;
            j2 = 0;
            k2 = 1;
        }
    } else {
    	if (y0 < z0) {
            i1 = 0;
            j1 = 0;
            k1 = 1;
            i2 = 0;
            j2 = 1;
            k2 = 1;
        } else if (x0 < z0) {
            i1 = 0;
            j1 = 1;
            k1 = 0;
            i2 = 0;
            j2 = 1;
            k2 = 1;
        } else {
            i1 = 0;
            j1 = 1;
            k1 = 0;
            i2 = 1;
            j2 = 1;
            k2 = 0;
        }
    }

    float x1 = x0 - i1 + G3;
    float y1 = y0 - j1 + G3;
    float z1 = z0 - k1 + G3;

    float x2 = x0 - i2 + 2.0f * G3;
    float y2 = y0 - j2 + 2.0f * G3;
    float z2 = z0 - k2 + 2.0f * G3;

    float x3 = x0 - 1.0f + 3.0f * G3;
    float y3 = y0 - 1.0f + 3.0f * G3;
    float z3 = z0 - 1.0f + 3.0f * G3;

    int ii = i & 255;
    int jj = j & 255;
    int kk = k & 255;

    int gi0 = noise->p[ii +  0 + noise->p[jj +  0 + noise->p[kk +  0]]];
    int gi1 = noise->p[ii + i1 + noise->p[jj + j1 + noise->p[kk + k1]]];
    int gi2 = noise->p[ii + i2 + noise->p[jj + j2 + noise->p[kk + k2]]];
    int gi3 = noise->p[ii +  1 + noise->p[jj +  1 + noise->p[kk +  1]]];

    float t0 = 0.6f - x0 * x0 - y0 * y0 - z0 * z0;
    if (t0 < 0) {
    	n0 = 0.0f;
    } else {
    	t0 *= t0;
    	n0 = t0 * t0 * grad3(gi0, x0, y0, z0);
    }

    float t1 = 0.6f - x1 * x1 - y1 * y1 - z1 * z1;
    if (t1 < 0) {
    	n1 = 0.0f;
    } else {
        t1 *= t1;
        n1 = t1 * t1 * grad3(gi1, x1, y1, z1);
    }

    float t2 = 0.6f - x2 * x2 - y2 * y2 - z2 * z2;
    if( t2 < 0 ) {
    	n2 = 0.0f;
    } else {
    	t2 *= t2;
    	n2 = t2 * t2 * grad3(gi2, x2, y2, z2);
    }

    float t3 = 0.6f - x3 * x3 - y3 * y3 - z3 * z3;
    if (t3 < 0) {
    	n3 = 0.0f;
    } else {
    	t3 *= t3;
    	n3 = t3 * t3 * grad3(gi3, x3, y3, z3);
    }
    
    return (32.0f * (n0 + n1 + n2 + n3));
}


//PERLIN NOISE IMPLEMENTATION
static int phash(t_noise * noise, int x, int y) {
    return (noise->p[(noise->p[y % 256] + x) % 256]);
}

static float lin_inter(float x, float y, float s) {
    return (x + s * (y - x));
}

static float smooth_inter(float x, float y, float s) {
    return (lin_inter(x, y, s * s * (3-2*s)));
}

float pnoise2(t_noise * noise, float x, float y) {
    int x_i = (int)x;
    int y_i = (int)y;
    float x_dec = x - x_i;
    float y_dec = y - y_i;
    int s = phash(noise, x_i, y_i);
    int t = phash(noise, x_i + 1, y_i);
    int u = phash(noise, x_i, y_i + 1);
    int v = phash(noise, x_i +  1, y_i + 1);
    float low = smooth_inter(s, t, x_dec);
    float high = smooth_inter(u, v, x_dec);
    float n = smooth_inter(low, high, y_dec) / 256.0f;
    return (n * 2.0f - 1);
}
