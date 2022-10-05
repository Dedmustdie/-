#include <iostream>
#include <chrono>
#include <clocale>

double function_asm(double x);

double x;

int main()
{
	setlocale(LC_ALL, "Russian");
	std::cout << "Введите x: " << std::endl;
	std::cin >> x;
	double res, res2;
	double p = 1 / x;
	res = cos(abs(x)) / (sin(x) + pow(2.7182818284, p));
	std::cout << "Значение функции на C++: " << res << std::endl;
	res2 = function_asm(x);
	std::cout << "Значение функции на Ассемблере: " << res2 << std::endl;

}


double function_asm(double x)
{
	double res, exp_const = 2.7182818284;
	__asm {

		//exp(1/2) -> 2^(exp*log2(1/x))
		finit
		fld1
		fld x
		fdivp st(1), st
		fld exp_const
		fyl2x
		fld1
		fld st(1)
		fprem
		f2xm1
		fadd
		fscale
		//sin(x)
		fld x
		fsin
		//sin(x) *exp(1/x)
		fxch 
		faddp st(1), st
		//cos(abs(x))
		fld x
		fabs
		fcos
		//cos(abs(x))/(sin(x) *exp(1/x))
		fxch
		fdivp st(1), st
		fst res
	}
	return res;
}