#include <stdio.h>
#include <math.h>

const double EPS = 10e-3, INTEGRAL_P = 1.0 / 15.0;
const int RIGHT_TO_LEFT = 1, LEFT_TO_RIGHT = 0;

typedef double(*function)(double);

double abs_val(double t) {
	return t < 0 ? -t : t;
}

double min(double t1, double t2) {
	return t1 < t2 ? t1 : t2;
}

double max(double t1, double t2) {
	return t1 > t2 ? t1 : t2;
}

double f1(double x) {
	return exp(x) + 2.0;
}

double f2(double x) {
	return -1.0 / x;
}

double f3(double x) {
	return -2.0 * (x + 1.0) / 3.0;
}

double der_f1(double x) {
	return exp(x);
}

double der_f2(double x) {
	return 1 / (x * x);
}

double der_f3(double x) {
	return - 2.0 / 3.0;
}


//**************************************************************************
//***********TESTING*************************TESTING************************
//**************************************************************************
double zero(double x) {
	return 0;
}

double root_test_f1(double x) {
	return -x * x * x * x + 13;
}

double root_test_f2(double x) {
	return exp(x) - 10;
}

const double PI = acos(-1.0);

double root_test_f3(double x) {
	return sin(x) - 0.5;
}

double der_zero(double x) {
	return 0;
}

double der_root_test_f1(double x) {
	return -4 * x * x * x;
}

double der_root_test_f2(double x) {
	return exp(x);
}

double der_root_test_f3(double x) {
	return cos(x);
}

//Integral test functions

double integral_test_f1(double x) {
	return -x * x * x * x + 13.0;
}

double integral_test_f2(double x) {
	return cos(x);
}

double integral_test_f3(double x) {
	return 1.0 / x;
}
//**************************************************************************
//***********TESTING*************************TESTING************************
//**************************************************************************


int get_direction(function f, function g,
	function der_f, function der_g,
	double left_bound, double right_bound) {

	double med = (left_bound + right_bound) / 2.0, med_der = der_f(med) - der_g(med);
	double substr = (f(med) - g(med)) - (f(right_bound) - g(right_bound) + f(left_bound) - g(left_bound)) / 2.0;
	if (substr > 0 && med_der > 0 || substr < 0 && med_der < 0)
		return LEFT_TO_RIGHT;
	else
		return RIGHT_TO_LEFT;
}
                                                 	
double get_root(function f, function g,
	function der_f, function der_g,
	double left_bound, double right_bound, double EPS) {

	double cur_point = get_direction(f, g, der_f, der_g, left_bound, right_bound) == RIGHT_TO_LEFT ? right_bound : left_bound;
	while (1) {
		double dx = (f(cur_point) - g(cur_point)) / (der_f(cur_point) - der_g(cur_point));
		if (abs_val(dx) < EPS)
			break;
		cur_point -= dx;
	}
	return cur_point;
}

double get_integral(function f, double left_bound, double right_bound, double EPS) {
	double last, sum, bound_sum = f(left_bound) + f(right_bound);
	int n, i, j;
	sum = f((left_bound + right_bound) / 2.0);
	for (n = 3, i = 0; ; n = n * 2 - 1, i++) {
		double new_sum = 0.0, dx = (right_bound - left_bound) / (2 * n - 1);
		for (j = 0; j < n - 1; j++)
			new_sum += f(left_bound + (2 * j + 1) * dx);
		double cur_res = dx * (bound_sum + 2 * sum + 4 * new_sum) / 3.0;
		if (i && INTEGRAL_P * abs_val(cur_res - last) < EPS) {
			last = cur_res;
			break;
		}
		last = cur_res;
		sum += new_sum;
	}
	return last;
}

int main(void) {


//**************************************************************************
//***********TESTING*************************TESTING************************
//**************************************************************************
	printf("Root tests:\n");
	printf("Root test #1:    %.9Lf\n", get_root(root_test_f1, zero, der_root_test_f1, der_zero, -3.0, -0.1, EPS / 10e5));
	printf("Root test #2:    %.9Lf\n", get_root(root_test_f2, zero, der_root_test_f2, der_zero, 0.0, 3.0, EPS / 10e5));
	printf("Root test #3:    %.9Lf\n", get_root(root_test_f3, zero, der_root_test_f3, der_zero, 0.0, PI / 2 - 10e-4, EPS / 10e5));

	printf("\nIntegral tests:\n");
	printf("Integral test #1:    %.9Lf\n", get_integral(integral_test_f1, -2.0, 1.0, EPS / 10e4));
	printf("Integral test #2:    %.9Lf\n", get_integral(integral_test_f2, -0.5, 0.5, EPS / 10e4));
	printf("Integral test #3:    %.9Lf\n", get_integral(integral_test_f3, 0.5, 0.6, EPS / 10e4));
//**************************************************************************
//***********TESTING*************************TESTING************************
//**************************************************************************


	double int_1_2 = get_root(f1, f2, der_f1, der_f2, -1.0, -0.1, EPS / 10e5);
	double int_1_3 = get_root(f1, f3, der_f1, der_f3, -6.0, -3.0, EPS / 10e5);
	double int_2_3 = get_root(f2, f3, der_f2, der_f3, -5.0, -1.0, EPS / 10e5);
	printf("\nIntersections:\n");
	printf("f1 and f2:   %.9Lf\n", int_1_2);
	printf("f1 and f3:   %.9Lf\n", int_1_3);
	printf("f2 and f3:   %.9Lf\n", int_2_3);
	double res = get_integral(f1, min(int_1_2, int_1_3), max(int_1_2, int_1_3), EPS / 10e4) -
		get_integral(f2, min(int_1_2, int_2_3), max(int_1_2, int_2_3), EPS / 10e4) -
		get_integral(f3, min(int_1_3, int_2_3), max(int_1_3, int_2_3), EPS / 10e4);
	printf("\nResult square:   %.9Lf\n", res);
	return 0;
}