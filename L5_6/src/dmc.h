#include <stdio.h>

#define D 20
#define N 10
#define Nu 1
#define lambda 10
#define MAX_LINE_LENGTH 1024

float M[N][Nu];
float MP[N][D - 1];
float K[Nu][N];
float dU[1][Nu];
float dUP[D - 1][1];
float Lambda[Nu][Nu];
float MTM_Lambda[Nu][Nu];
float MTM_Lambda_inv[Nu][Nu];

float S[] = {0.019f, 0.057f, 0.153f, 0.264f, 0.367f, 0.475f, 0.564f, 0.636f, 0.706f, 0.755f,
             0.806f, 0.847f, 0.878f, 0.906f, 0.928f, 0.943f, 0.957f, 0.970f, 0.983f, 0.997f};

float u[100];
float y[100];
float y_zad[100];

float set_value = 500.0f;

// Funkcja do obliczania macierzy odwrotnej za pomocą eliminacji Gaussa-Jordana
void invertMatrix(float A[Nu][Nu], float inverse[Nu][Nu]) {
	// Tworzenie rozszerzonej macierzy [A | I]
	float augmented[Nu][2 * Nu];
	for (int i = 0; i < Nu; i++) {
		for (int j = 0; j < Nu; j++) {
			augmented[i][j] = A[i][j];         // Kopiowanie macierzy A
			augmented[i][j + Nu] = (i == j) ? 1 : 0;  // Dodawanie macierzy jednostkowej
		}
	}

	// Gauss-Jordan eliminacja
	for (int i = 0; i < Nu; i++) {
		// Szukanie elementu głównego (pivot)
		float pivot = augmented[i][i];
		if (pivot == 0) {
			printf("Macierz jest osobliwa i nieodwracalna.\n");
			return 0;  // Brak odwrotnej
		}

		// Dzielenie całego wiersza przez pivot
		for (int j = 0; j < 2 * Nu; j++) {
			augmented[i][j] /= pivot;
		}

		// Zerowanie pozostałych elementów w kolumnie
		for (int k = 0; k < Nu; k++) {
			if (k != i) {
				float factor = augmented[k][i];
				for (int j = 0; j < 2 * Nu; j++) {
					augmented[k][j] -= factor * augmented[i][j];
				}
			}
		}
	}

	// Wyciąganie macierzy odwrotnej z rozszerzonej macierzy
	for (int i = 0; i < Nu; i++) {
		for (int j = 0; j < Nu; j++) {
			inverse[i][j] = augmented[i][j + Nu];
		}
	}
}


void initDMC(void)
{
	// Macierz M
	for (int i = 0; i < Nu; ++i)
	{
		for (int j = i; j < N; ++j)
		{
			M[j][i] = S[j - i];
		}
	}

	// Macierz MP
	for (int i = 0; i < D - 1; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			if (j + i + 1 <= D - 1)
			{
				MP[j][i] = S[j + i + 1] - S[i];
			}
			else
			{
				MP[j][i] = S[D - 1] - S[i];
			}
		}
	}

	// Macierz Lambda
	for (int i = 0; i < Nu; ++i)
	{
		Lambda[i][i] = lambda;
	}

	// Macierz K
	//
	// Obliczanie macierzy M^T * M + Lambda
	for (int i = 0; i < Nu; i++)
	{
		for (int j = 0; j < Nu; j++)
		{
			MTM_Lambda[i][j] = 0;  // Inicjalizacja elementu wynikowego
			for (int k = 0; k < N; k++)
			{
				MTM_Lambda[i][j] += M[k][i] * M[k][j];
			}
			MTM_Lambda[i][j] += Lambda[i][j];
		}
	}
	//
	// Odwracanie macierzy M^T * M + Lambda
	invertMatrix(MTM_Lambda, MTM_Lambda_inv);
	//
	// Mnożenie macierzy (M^T * M + Lambda)^(-1) * M'
	for (int i = 0; i < Nu; i++)
	{
		for (int j = 0; j < N; j++)
		{
			K[i][j] = 0;  // Inicjalizacja elementu wynikowego
			for (int k = 0; k < Nu; k++)
			{
				K[i][j] += MTM_Lambda_inv[i][k] * M[j][k];
			}
		}
	}

	for (int i = 0; i < 30; i++) {
        y_zad[i] = 25.0f;
    }
    for (int i = 30; i < 100; i++) {
        y_zad[i] = set_value;
    }
}


float DMC(int k, float y_process)
{
	y[k] = y_process;

	float Y0[N][1];

	// Mnożenie macierzy MP * dUP
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < 1; j++)
		{
			Y0[i][j] = 0;  // Inicjalizacja elementu wynikowego
			for (int k = 0; k < (D - 1); k++)
			{
				Y0[i][j] += MP[i][k] * dUP[k][j];
			}
			Y0[i][j] += y[k];
		}
	}

	for (int i = 0; i < N; i++)
	{
		Y0[i][0] = y_zad[k] - Y0[i][0];
	}

	// Prawo regulacji
	for (int i = 0; i < Nu; i++)
	{
		for (int j = 0; j < 1; j++)
		{
			dU[i][j] = 0;  // Inicjalizacja elementu wynikowego
			for (int k = 0; k < N; k++)
			{
				dU[i][j] += K[i][k] * Y0[k][j];
			}
		}
	}

	// Obliczanie sterowania
	u[k] = u[k - 1] + dU[0][0];

	// Aktualizacja zmian sterowania
	for (int i = (D - 2); i > 0; i--)
	{
		dUP[i][0] = dUP[i - 1][0];
	}
	dUP[0][0] = dU[0][0];

	return u[k];
}
