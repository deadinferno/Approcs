#include "approcs.h"

double Lagranz(vector< double > X, vector< double > Y, double t) {
	double z, p1, p2;
	z = 0;
	int n = X.size();
	for (int j = 0; j<n; j++) {
		p1 = 1; p2 = 1;
		for (int i = 0; i<n; i++) {
			if (i == j) {
				p1 = p1 * 1; p2 = p2 * 1;
			}
			else {
				p1 = p1 * (t - X[i]);
				p2 = p2 * (X[j] - X[i]);
			}
		}
		z = z + Y[j] * p1 / p2;
	}
	return z;
}

double Newton(vector< double > X, vector< double > Y, double x) {
	double res = Y[0], F, den;
	int i, j, k;
	int n = X.size();
	for (i = 1; i<n; i++) {
		F = 0;
		for (j = 0; j <= i; j++) {//следующее слагаемое полинома
			den = 1;
			//считаем знаменатель разделенной разности
			for (k = 0; k <= i; k++) {
				if (k != j) den *= (X[j] - X[k]);
			}
			//считаем разделенную разность
			F += Y[j] / den;
		}
		//домножаем разделенную разность на скобки (x-x[0])...(x-x[i-1])
		for (k = 0; k<i; k++)F *= (x - X[k]);
		res += F;//полином
	}
	return res;
}