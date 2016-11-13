#include <stdio.h>
#include <math.h>

const double EPS1 = 10e-6, EPS2 = 10e-6;
const int RIGHT_TO_LEFT = 1, LEFT_TO_RIGHT = 0;

struct point {
	double x, y;
};

typedef struct point point;
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

double f1_sub_f2_der(double x) {
	return exp(x) - 1.0 / (x * x);
}

double f1_sub_f3_der(double x) {
	return exp(x) + 2.0 / 3.0;
}

double f2_sub_f3_der(double x) {
	return 1.0 / (x * x) + 2.0 / 3.0;
}

function get_derivative(function f, function g) {
	if (f == f1 && g == f2)
		return f1_sub_f2_der;
	if (f == f1 && g == f3)
		return f1_sub_f3_der;
	if (f == f2 && g == f3)
		return f2_sub_f3_der;
	return NULL;
}
                                                 	
double get_root(function f, function g, double left_bound, double right_bound, int dir) {
	function derivative = get_derivative(f, g);
	double cur_point = dir == RIGHT_TO_LEFT ? right_bound : left_bound;
	while (1) {
		double dx = (f(cur_point) - g(cur_point)) / derivative(cur_point);
		if (abs_val(dx) < EPS1)
			break;
		cur_point -= dx;
	}
	return cur_point;
}

void get_parabola(point p1, point p2, point p3, double *a, double *b, double *c) {
	double det = pow(p1.x, 2.0) * p2.x * 1.0 +
		pow(p2.x, 2.0) * p3.x * 1.0 +
		pow(p3.x, 2.0) * p1.x * 1.0 -
		pow(p3.x, 2.0) * p2.x * 1.0 -
		pow(p2.x, 2.0) * p1.x * 1.0 -
		pow(p1.x, 2.0) * p3.x * 1.0;
	double det_a = p1.y * p2.x * 1.0 +
		p2.y * p3.x * 1.0 +
		p3.y * p1.x * 1.0 -
		p3.y * p2.x * 1.0 -
		p2.y * p1.x * 1.0 -
		p1.y * p3.x * 1.0;
	double det_b = pow(p1.x, 2.0) * p2.y * 1.0 +
		pow(p2.x, 2.0) * p3.y * 1.0 +
		pow(p3.x, 2.0) * p1.y * 1.0 -
		pow(p3.x, 2.0) * p2.y * 1.0 -
		pow(p2.x, 2.0) * p1.y * 1.0 -
		pow(p1.x, 2.0) * p3.y * 1.0;
	double det_c = pow(p1.x, 2.0) * p2.x * p3.y +
		pow(p2.x, 2.0) * p3.x * p1.y +
		pow(p3.x, 2.0) * p1.x * p2.y -
		pow(p3.x, 2.0) * p2.x * p1.y -
		pow(p2.x, 2.0) * p1.x * p3.y -
		pow(p1.x, 2.0) * p3.x * p2.y;
	*a = det_a / det;
	*b = det_b / det;
	*c = det_c / det;
}

double get_integral(function f, double left_bound, double right_bound) {
	double last;
	int n, i, j;
	for (n = 5, i = 0; 1; n *= 2, i++) {
		double cur_res = 0.0, dx = (right_bound - left_bound) / n;
		for (j = 0; j < n; j++) {
			cur_res += dx * (f(left_bound + j * dx) + 4 * f(left_bound + (j + 0.5) * dx) + f(left_bound + (j + 1.0) * dx)) / 6.0;
/*			point p1 = { left_bound + j * dx, f(left_bound + j * dx) };
			point p2 = { left_bound + (j + 0.5) * dx, f(left_bound + (j + 0.5) * dx) };
			point p3 = { left_bound + (j + 1.0) * dx, f(left_bound + (j + 1.0) * dx) };
			double a, b, c;
			get_parabola(p1, p2, p3, &a, &b, &c);
			cur_res += a * pow(left_bound + (j + 1.0) * dx, 3.0) / 3.0 +
				b * pow(left_bound + (j + 1.0) * dx, 2.0) / 2.0 +
				c * (left_bound + (j + 1.0) * dx) -
				a * pow(left_bound + j * dx, 3.0) / 3.0 -
				b * pow(left_bound + j * dx, 2.0) / 2.0 -
				c * (left_bound + j * dx);*/
		}
		if (i && abs_val(cur_res - last) < EPS2)
			return last;
		last = cur_res;
	}
	return last;
}

int main(void) {
	double int_1_2 = get_root(f1, f2, -1.0, -0.1, RIGHT_TO_LEFT);
	double int_1_3 = get_root(f1, f3, -7.0, 0.0, RIGHT_TO_LEFT);
	double int_2_3 = get_root(f2, f3, -5.0, -1.0, RIGHT_TO_LEFT);
	printf("Intersections:\n");
	printf("f1 and f2:   %.9Lf\n", int_1_2);
	printf("f1 and f3:   %.9Lf\n", int_1_3);
	printf("f2 and f3:   %.9Lf\n", int_2_3);
	double res = get_integral(f1, min(int_1_2, int_1_3), max(int_1_2, int_1_3)) -
		get_integral(f2, min(int_1_2, int_2_3), max(int_1_2, int_2_3)) -
		get_integral(f3, min(int_1_3, int_2_3), max(int_1_3, int_2_3));
	printf("\nResult square:   %.9Lf\n", res);
	return 0;
}