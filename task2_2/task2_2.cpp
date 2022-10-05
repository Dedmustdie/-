#include <iostream>
#include <windows.h>
#include <string>
#include <clocale>
#include <conio.h>
#include <vector>

using namespace std;

int main()
{
	SwapMouseButton(GetSystemMetrics(SM_SWAPBUTTON) == 0);
}