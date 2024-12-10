#include <stdio.h>

#define D 20
#define N 10
#define Nu 1
#define lambda 10

float M[N][Nu];
float MP[N][D - 1];
float K[Nu][N];
float dU[1][Nu];
float dUP[D - 1][1];
float Lambda[Nu][Nu];
float MTM_Lambda[Nu][Nu];
float MTM_Lambda_inv[Nu][Nu];

float S[] = {
    0.0000f, 0.0000f, 0.0000f, 0.0000f, 0.0000f, 0.0000f, 0.0000f, 0.0000f, 0.0000f, 0.0000f,
    0.0000f, 0.0000f, 0.0010f, 0.0031f, 0.0057f, 0.0086f, 0.0115f, 0.0144f, 0.0174f, 0.0203f,
    0.0231f, 0.0260f, 0.0288f, 0.0316f, 0.0343f, 0.0370f, 0.0397f, 0.0423f, 0.0450f, 0.0476f,
    0.0501f, 0.0526f, 0.0552f, 0.0576f, 0.0601f, 0.0625f, 0.0649f, 0.0672f, 0.0696f, 0.0719f,
    0.0742f, 0.0764f, 0.0787f, 0.0810f, 0.0830f, 0.0852f, 0.0873f, 0.0894f, 0.0915f, 0.0936f,
    0.0956f, 0.0976f, 0.0996f, 0.1016f, 0.1035f, 0.1054f, 0.1073f, 0.1092f, 0.1111f, 0.1129f,
    0.1147f, 0.1165f, 0.1183f, 0.1200f, 0.1218f, 0.1235f, 0.1252f, 0.1268f, 0.1285f, 0.1301f,
    0.1318f, 0.1334f, 0.1349f, 0.1365f, 0.1380f, 0.1396f, 0.1411f, 0.1426f, 0.1440f, 0.1455f,
    0.1469f, 0.1484f, 0.1498f, 0.1512f, 0.1525f, 0.1539f, 0.1552f, 0.1566f, 0.1579f, 0.1592f,
    0.1605f, 0.1617f, 0.1630f, 0.1642f, 0.1655f, 0.1667f, 0.1679f, 0.1691f, 0.1702f, 0.1714f,
    0.1725f, 0.1737f, 0.1748f, 0.1759f, 0.1770f, 0.1781f, 0.1791f, 0.1802f, 0.1812f, 0.1823f,
    0.1833f, 0.1843f, 0.1853f, 0.1863f, 0.1873f, 0.1882f, 0.1892f, 0.1901f, 0.1911f, 0.1920f,
    0.1929f, 0.1938f, 0.1947f, 0.1956f, 0.1964f, 0.1973f, 0.1981f, 0.1990f, 0.1998f, 0.2006f,
    0.2014f, 0.2022f, 0.2030f, 0.2038f, 0.2046f, 0.2053f, 0.2061f, 0.2069f, 0.2076f, 0.2083f,
    0.2090f, 0.2098f, 0.2105f, 0.2112f, 0.2119f, 0.2125f, 0.2132f, 0.2139f, 0.2145f, 0.2152f,
    0.2158f, 0.2165f, 0.2171f, 0.2177f, 0.2183f, 0.2190f, 0.2196f, 0.2202f, 0.2207f, 0.2213f,
    0.2219f, 0.2225f, 0.2230f, 0.2236f, 0.2241f, 0.2247f, 0.2252f, 0.2257f, 0.2263f, 0.2268f,
    0.2273f, 0.2278f, 0.2283f, 0.2288f, 0.2293f, 0.2298f, 0.2302f, 0.2307f, 0.2312f, 0.2316f,
    0.2321f, 0.2326f, 0.2330f, 0.2334f, 0.2339f, 0.2343f, 0.2347f, 0.2352f, 0.2356f, 0.2360f,
    0.2364f, 0.2368f, 0.2372f, 0.2376f, 0.2380f, 0.2384f, 0.2387f, 0.2391f, 0.2395f, 0.2398f,
    0.2402f, 0.2406f, 0.2409f, 0.2413f, 0.2416f, 0.2420f, 0.2423f, 0.2426f, 0.2430f, 0.2433f,
    0.2436f, 0.2439f, 0.2443f, 0.2446f, 0.2449f, 0.2452f, 0.2455f, 0.2458f, 0.2461f, 0.2464f,
    0.2467f, 0.2470f, 0.2472f, 0.2475f, 0.2478f, 0.2481f, 0.2483f, 0.2486f, 0.2488f, 0.2491f,
    0.2494f, 0.2496f, 0.2499f, 0.2501f, 0.2504f, 0.2506f, 0.2508f, 0.2511f, 0.2513f, 0.2515f,
    0.2518f, 0.2520f, 0.2522f, 0.2524f, 0.2526f, 0.2529f, 0.2531f, 0.2533f, 0.2535f, 0.2537f,
    0.2540f, 0.2541f, 0.2543f, 0.2545f, 0.2547f, 0.2550f, 0.2551f, 0.2553f, 0.2555f, 0.2557f,
    0.2560f, 0.2562f, 0.2564f, 0.2565f, 0.2567f, 0.2569f, 0.2571f, 0.2572f, 0.2574f, 0.2576f,
    0.2577f, 0.2579f, 0.2580f, 0.2582f, 0.2583f, 0.2585f, 0.2586f, 0.2588f, 0.2589f, 0.2591f,
    0.2592f, 0.2593f, 0.2595f, 0.2596f, 0.2597f, 0.2599f, 0.2600f, 0.2601f, 0.2603f, 0.2604f,
    0.2605f, 0.2606f, 0.2608f, 0.2609f, 0.2610f, 0.2611f, 0.2613f, 0.2614f, 0.2615f, 0.2616f,
    0.2617f, 0.2618f, 0.2620f, 0.2621f, 0.2622f, 0.2623f, 0.2624f, 0.2625f, 0.2626f, 0.2627f,
    0.2628f, 0.2629f, 0.2631f, 0.2632f, 0.2633f, 0.2634f, 0.2635f, 0.2636f, 0.2637f, 0.2638f,
    0.2639f, 0.2640f, 0.2641f, 0.2642f, 0.2643f, 0.2644f, 0.2645f, 0.2646f, 0.2647f, 0.2648f,
    0.2649f, 0.2650f, 0.2651f, 0.2652f, 0.2653f, 0.2654f, 0.2655f, 0.2656f, 0.2657f, 0.2658f
};

float u[500];
float y[500];
float y_zad[500];

// DMC variables
float u_min = -50.0f;
float u_max = 50.0f;

float Upp = 0.0f;
float Ypp = 41.18f;

int set_time = 10;
float set_value = Ypp;

int kend = 500;
float* s;

void invertMatrix(float A[Nu][Nu], float inverse[Nu][Nu]) 
{
	float augmented[Nu][2 * Nu];
	for (int i = 0; i < Nu; i++) {
		for (int j = 0; j < Nu; j++) {
			augmented[i][j] = A[i][j];        
			augmented[i][j + Nu] = (i == j) ? 1 : 0; 
		}
	}

	// Eliminacja Gaussa-Jordana
	for (int i = 0; i < Nu; i++) {
		float pivot = augmented[i][i];
		if (pivot == 0) {
			return 0; // Brak odwrotnej
		}

		for (int j = 0; j < 2 * Nu; j++) {
			augmented[i][j] /= pivot;
		}

		for (int k = 0; k < Nu; k++) {
			if (k != i) {
				float factor = augmented[k][i];
				for (int j = 0; j < 2 * Nu; j++) {
					augmented[k][j] -= factor * augmented[i][j];
				}
			}
		}
	}

	for (int i = 0; i < Nu; i++) {
		for (int j = 0; j < Nu; j++) {
			inverse[i][j] = augmented[i][j + Nu];
		}
	}
}


void initDMC(void)
{
	// Macierz M
	for (int i = 0; i < Nu; ++i) {
		for (int j = i; j < N; ++j) {
			M[j][i] = S[j - i];
		}
	}

	// Macierz MP
	for (int i = 0; i < D - 1; ++i) {
		for (int j = 0; j < N; ++j) {
			if (j + i + 1 <= D - 1) {
				MP[j][i] = S[j + i + 1] - S[i];
			}
			else {
				MP[j][i] = S[D - 1] - S[i];
			}
		}
	}

	// Macierz Lambda
	for (int i = 0; i < Nu; ++i) {
		Lambda[i][i] = lambda;
	}

	// Macierz K
	//
	// Obliczanie macierzy M^T * M + Lambda
	for (int i = 0; i < Nu; i++) {
		for (int j = 0; j < Nu; j++) {
			MTM_Lambda[i][j] = 0; 
			for (int k = 0; k < N; k++) {
				MTM_Lambda[i][j] += M[k][i] * M[k][j];
			}
			MTM_Lambda[i][j] += Lambda[i][j];
		}
	}

	// Odwracanie macierzy M^T * M + Lambda
	invertMatrix(MTM_Lambda, MTM_Lambda_inv);

	// Mnożenie macierzy (M^T * M + Lambda)^(-1) * M'
	for (int i = 0; i < Nu; i++) {
		for (int j = 0; j < N; j++) {
			K[i][j] = 0; 
			for (int k = 0; k < Nu; k++) {
				K[i][j] += MTM_Lambda_inv[i][k] * M[j][k];
			}
		}
	}

    updateYZad();
}

void updateYZad(void)
{
	for (int i = 0; i < set_time; i++) {
        y_zad[i] = Ypp;
    }
    for (int i=set_time; i < kend; i++) {
        y_zad[i] = set_value;
    }
}

float DMC(int k, float y_process)
{
	y[k] = y_process;
	float Y0[N][1];

	// Mnożenie macierzy MP * dUP
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < 1; j++) {
			Y0[i][j] = 0;
			for (int k = 0; k < (D - 1); k++) {
				Y0[i][j] += MP[i][k] * dUP[k][j];
			}
			Y0[i][j] += y[k];
		}
	}

	for (int i = 0; i < N; i++) {
		Y0[i][0] = y_zad[k] - Y0[i][0];
	}

	// Prawo regulacji
	for (int i = 0; i < Nu; i++) {
		for (int j = 0; j < 1; j++) {
			dU[i][j] = 0;
			for (int k = 0; k < N; k++) {
				dU[i][j] += K[i][k] * Y0[k][j];
			}
		}
	}

	// Obliczanie sterowania
	u[k] = u[k - 1] + dU[0][0];
	
	// Aktualizacja zmian sterowania
	for (int i = (D - 2); i > 0; i--) {
		dUP[i][0] = dUP[i - 1][0];
	}
	dUP[0][0] = dU[0][0];

	return u[k];
}
