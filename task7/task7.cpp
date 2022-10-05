#include <chrono>
#include <iostream>
#include <vector>

using namespace std;

void mul(float* m1, float* m2, float* res, int K, int L, int M);
void mulNoAsm(float* m1, float* m2, float* res, int K, int L, int M);
void transpose(float* m, float* res, int K, int L);
void printMatrix(float* m, int K, int L);
bool equals(float* m1, float* m2, int N);
void fillArrayRandom(float* arr, int N, float low, float high);

int main()
{
	int K, L, M;
	chrono::time_point<chrono::system_clock> start, end;
	cout << "Input count of lines of the first matrix: ";
	cin >> K;
	cout << "Input count of cols of the first matrix: ";
	cin >> L;
	cout << "Input count of cols of the second matrix: ";
	cin >> M;
	float* m1 = new float[K * L];
	/*cout << "Input the first matrix:" << endl;
	for (int i = 0; i < K; i++) {
		for (int j = 0; j < L; j++) {
			cin >> m1[i * L + j];
		}
	}*/
	fillArrayRandom(m1, K * L, 1.0, 100.0);
	float* m2 = new float[L * M];
	/*cout << "Input the second matrix:" << endl;
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < M; j++) {
			cin >> m2[i * M + j];
		}
	}
	cout << endl;*/
	fillArrayRandom(m2, L * M, 1.0, 100.0);
	float* result1 = new float[K * M];
	for (int i = 0; i < K * M; i++)
		result1[i] = 0;
	start = chrono::system_clock::now();
	mul(m1, m2, result1, K, L, M);
	end = chrono::system_clock::now();
	cout << "With assembler: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;
	printMatrix(result1, K, M);
	float* result2 = new float[K * M];
	for (int i = 0; i < K * M; i++)
		result2[i] = 0;
	start = chrono::system_clock::now();
	mulNoAsm(m1, m2, result2, K, L, M);
	end = chrono::system_clock::now();
	cout << "С++: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;
	printMatrix(result2, K, M);
	cout << "Results are " << (equals(result1, result2, K * M) ? "equal" : "not equal") << endl;
}

//m1: K * L, m2: L * M, res: K * M
void mul(float* m1, float* m2, float* res, int K, int L, int M) {
	float* transpose_m2 = new float[M * L];
	transpose(m2, transpose_m2, L, M);
	float* tmp_res = new float[8];

	for (int i = 0; i < K; ++i) {
		float* line_m1 = m1 + i * L;

		for (int j = 0; j < M; ++j) {
			float* line_transpose_m2 = transpose_m2 + j * L;

			for (int k = 0; k < L / 8 * 8; k += 8) {
				float* line_m1_segment = line_m1 + k;
				float* line_transpose_m2_segment = line_transpose_m2 + k;

				__asm {
					mov eax, [line_m1_segment]
					vmovups ymm0, [eax]
					mov edx, [line_transpose_m2_segment]
					vmovups ymm1, [edx]
					vmulps ymm0, ymm0, ymm1
					mov edx, [tmp_res]
					vmovups[edx], ymm0
				}

				for (int t = 0; t < 8; ++t) {
					res[i * M + j] += tmp_res[t];
				}
			}

			for (int k = L / 8 * 8; k < L; k++) {
				res[i * M + j] += line_m1[k] * line_transpose_m2[k];
			}
		}
	}
	delete[] tmp_res;
	delete[] transpose_m2;
}

//m1: K * L, m2: L * M, res: K * M
void mulNoAsm(float* m1, float* m2, float* res, int K, int L, int M) {
	float* transpose_m2 = new float[M * L];
	transpose(m2, transpose_m2, L, M);

	for (int i = 0; i < K; ++i) {
		float* line_m1 = m1 + i * L;

		for (int j = 0; j < M; ++j) {
			float* line_transpose_m2 = transpose_m2 + j * L;
			for (int k = 0; k < L; k++)
				res[i * M + j] += line_m1[k] * line_transpose_m2[k];
		}
	}
	delete[] transpose_m2;
}

//m: K * L, res: L * K
void transpose(float* m, float* res, int K, int L) {
	for (int i = 0; i < L; ++i) {
		for (int j = 0; j < K; ++j) {
			res[i * K + j] = m[j * L + i];
		}
	}
}

void printMatrix(float* m, int K, int L) {
	for (int i = 0; i < K; i++) {
		for (int j = 0; j < L; j++) {
			cout << m[i * L + j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

bool equals(float* m1, float* m2, int N) {
	//int count = 0;
	//cout << "No equal in: ";
	for (int i = 0; i < N; i++)
		if (m1[i] != m2[i]) {
			//cout << i << " ";
			//count++;
			return false;
		}
	//cout << endl;
	return true;
	//return count == 0;
}

void fillArrayRandom(float* arr, int N, float low, float high) {
	for (int i = 0; i < N; i++)
		arr[i] = low + (float)rand() / (float)(RAND_MAX / (high - low));
}