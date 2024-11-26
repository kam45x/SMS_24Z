#include <stdio.h>

#define D 200
#define N 200
#define Nu 200
#define lambda 1

float M[N][Nu];
float MP[N][D - 1];
float K[Nu][N];
float dU[1][Nu];
float dUP[D - 1][1];
float Lambda[Nu][Nu];
float MTM_Lambda[Nu][Nu];
float MTM_Lambda_inv[Nu][Nu];

float S[D];

float u[300];
float y[300];
float y_zad = 2;


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
}


void DMC(int k, float y_process)
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
		Y0[i][0] = y_zad - Y0[i][0];
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
}

int main()
{
	for (int i = 0; i < D; ++i)
	{
		S[i] = -3.0 / ((float)i/10 + 1.0) + 3.0;
	}

	initDMC();

	y[0] = 0.0;
	u[0] = 0.0;
	for (int k = 1; k < 100; k++)
	{
		y[k] = y[k - 1] + u[k - 1] * 0.3;
		DMC(k, y[k]);
		printf("%.4f\t%.4f", u[k], y[k]);
		printf("\n");
	}
}