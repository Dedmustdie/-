#include <algorithm>
#include <chrono>
#include <intrin.h>
#include <iostream>
#include <vector>
#include <clocale>

void generateArrPos(int arr[], int i);

int offset = 262144; // 1Mb*1024*1024/4 Расстояние между блоками
int n = offset * 21; // максимально возможный размер массива
int size = 10 ^ 7; // размер обходимого блока в КВ

int main()
{
	setlocale(LC_ALL, "Russian");
	unsigned clock = 0;
	int* arr = new int[n];
	std::vector<int> res;
	for (int i = 1; i <= 20; ++i)
	{
		generateArrPos(arr, i);
		for (size_t i = 0, j = 0; i < n; i++) //одиночный обход массива для "разогрева" кэш-памяти
			j = arr[j];
		__asm//получение времени в тактах
		{
			rdtscp;
			mov[clock], eax;
		}
		for (size_t i = 0, j = 0; i < n; i++)
			//обход
			j = arr[j];
		__asm
		{
			rdtscp;
			sub eax, [clock]
				mov[clock], eax;
		}
		res.push_back(clock / n);
	}
	for (int i = 1; i <= 20; ++i)
	{
		std::cout << "Количество фрагментов:" << i << " Время: " << res[i - 1] << std::endl;
	}
	return EXIT_SUCCESS;
}

void generateArrPos(int arr[], int i)// генерация массива обхода, значение элемента массива равно индексу следующего элемента 
{
	int countElement = size / i; // количество элементов в одном блоке обхода
	int count = 0; //количество добавленных элементов в один блок
	int countBlocks = 1; //количество добавленных блоков
	for (int j = 0; j < n - 1; ++j)
	{
		arr[j] = j + 1;
	}
	arr[n - 1] = 0;
	for (int j = 0; j < n; ++j)
	{
		if (count >= countElement)
		{
			j = offset * countBlocks;
			countBlocks++;
			count = 0;
		}
		if (countBlocks > i)
		{
			arr[offset * (countBlocks - 2) - 1 + countElement] = 0;
			break;
		}
		if (countBlocks == i)
		{
			count++;
			arr[j] = count;
		}
		else
		{
			arr[j] = j + offset;
			count++;
		}
	}
	arr[n - 1] = 0;
}