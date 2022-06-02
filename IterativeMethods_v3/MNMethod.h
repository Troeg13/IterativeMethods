#pragma once
#include<vector>
#include<math.h>
#include<iostream>

#include "SparseMatrix.h"
#include "MethodTemplate.h"

using namespace std;

class MMN {
public:
	MethodTemp p;
	double size_slay; // размер сетки внутренних узлов
	double size; // размер всей сетки

	MMN() {};

	void max(double &m1, double &m2) {
		double tmp;
		double tmp_res_m1 = 0;
		double tmp_res_m2 = 0;
		double n1 = 1.0 / 2000;
		for (int i = 0; i < 2001; i++) {
			for (int j = 0; j < 2001; j++) {
				tmp = pow(PI, 4) * exp(pow(sin(PI * i*n1 * j*n1), 2));
				tmp *= (-4.0 * pow(sin(2 * PI * i * n1 * j * n1), 2) + 6.0 * sin(4 * PI * i * n1 * j * n1) * sin(2 * PI * i * n1 * j * n1) + pow(sin(2 * PI * i * n1 * j * n1), 4) - 8.0 * cos(2 * PI * i * n1 * j * n1) + 12.0 * cos(4 * PI * i * n1 * j * n1));
				if (tmp_res_m1 < tmp * pow(i * n1, 4))
					tmp_res_m1 = tmp * pow(i * n1, 4);
				if (tmp_res_m2 < tmp * pow(j * n1, 4))
					tmp_res_m2 = tmp * pow(j * n1, 4);
			}
		}
		m1 = tmp_res_m1;
		m2 = tmp_res_m2;
	}

	void  InitialCalcul() {
		p.Satka();
		size_slay = (p.n - 1) * (p.m - 1);
		size = (p.n + 1) * (p.m + 1);
		for (int i = 0; i < p.n + 1; i++) {
			p.V[i].resize(p.m + 1, 0);
			for (int j = 0; j < p.m + 1; j++) {
				if (i != p.n) {
					if (!p.test && !i)
						p.V[i][j] = Borders(p.a + i * p.h, p.c + j * p.k);
					else
						p.V[i][j] = Borders(p.a + i * p.h, p.d);
				}
				else
					p.V[i][j] = Borders(p.d, p.c + j * p.k);
			}
			p.V[i][0] = Borders(p.a + i * p.h, p.c + 0 * p.k);


			if (p.test) {
				p.U[i].resize(p.m + 1);
				for (int j = 0; j < p.m + 1; j++)
					p.U[i][j] = ExactSolutionTest(p.a + i * p.h, p.c + j * p.k);
			}
		}
	}

	double Borders(double x, double y) {
		if (p.test)
			return exp(pow(sin(PI * x * y), 2));
		return x - x * x + sin(PI * y);
	}

	double ExactSolutionTest(double x, double y) { //точное решение тестовой задачи
		return exp(pow(sin(PI * x * y), 2));
	}

	double F(double x, double y) { //значение f в узле (x,y) 
		if (p.test)
			return -((2.0 * cos(2.0 * PI * x * y) + pow(sin(PI * 2 * x * y), 2.0)) * exp(pow(sin(PI * x * y), 2.0)) * (pow(x, 2.0) + pow(y, 2.0)) * pow(PI, 2.0));
		else
			return pow(sin(PI * x * y), 2);
	}

	vector<double> Get_V_s() {
		vector<double> res;
		for (int j = 1; j < p.m; j++) {
			for (int i = 1; i < p.n; i++) {
				res.push_back(p.V_0[i][j]);
			}
		}
		return res;
	}

	

	vector<double> Get_b(const vector<vector<double>>& f) {
		vector<double> res;
		double tmp;
		for (int i = 1; i < p.n; i++) {
			for (int j = 1; j < p.m; j++) {
				tmp = f[i][j];
				if (j == 1) {
					tmp += p.yellow_coeff * p.V_0[i][j - 1];
				}
				if (j == p.m - 1) {
					tmp += p.yellow_coeff * p.V_0[i][j + 1];
				}
				if (i == 1) {
					tmp += p.green_coeff * p.V_0[i - 1][j];
				}
				if (i == p.n - 1) {
					tmp += p.green_coeff * p.V_0[i + 1][j];
				}
				res.push_back(tmp);
			}
		}
		return res;
	}

	double Scalar_v(const std::vector<double>& a, const std::vector<double>& b) {
		double res = 0;
		for (int i = 0; i < size_slay; i++) {
			res += a[i] * b[i];
		}
		return res;
	}

	vector<double> Diff(const std::vector<double>& a, const std::vector<double>& b) {
		vector<double> res(size_slay, 0);
		for (int i = 0; i < size_slay; i++) {
			res[i] = a[i] - b[i];
		}
		return res;
	}

	vector<double> Mul_scal(double a, const std::vector<double>& b) {
		vector<double> res(size_slay, 0);
		for (int i = 0; i < size_slay; i++) {
			res[i] = a * b[i];
		}
		return res;
	}

	vector<double> Mul_mat(const std::vector<vector<double>>& A, const std::vector<double>& x) {
		vector<double> res(size_slay, 0);
		for (int j = 0; j < size_slay; j++) {
			for (int i = 0; i < size_slay; i++) {
				res[j] += A[i][j] * x[i];
				
			}
		}
		return res;
	}

	void Solution()
	{
		InitialCalcul();
		p.V_0 = p.V;
		vector<vector<double>> f(p.n + 1); // значение вектоpа F во всех узлах сетки

		for (int i = 0; i <= p.n; i++) {
			f[i].resize(p.m + 1, 0);
			for (int j = 0; j <= p.m ; j++) {
				f[i][j] = F(p.a + i * p.h, p.c + j * p.k);
			}
		}

		vector<double> V_s = Get_V_s(); // вектор, соотв. текущему приблежению, на начальном этапе - нулевое приблежение
		vector<double> b = Get_b(f);
		int j = 0;
		SparseMatrix M_A(size_slay, size_slay);
		for (int i = 0; i < size_slay; i++) {
			M_A.set(i,j, -p.A);
			if (i - p.n + 1 >= 0) {
				M_A.set(i - p.n + 1, j, -p.yellow_coeff);
			}
			if (i % int(p.n - 1) != 0) {
				M_A.set(i - 1,j,-p.green_coeff);
			}
			if (i % int(p.n - 1) != (p.n - 2)) {
				M_A.set(i + 1,j,  -p.green_coeff);
			}
			if (i + p.n - 1 <= size_slay - 1) {
				M_A.set(i + p.n - 1,j,  -p.yellow_coeff);
			}
			j++;
		}

		vector<double> r_s(size_slay); // вектор невязки на текущем шаге
		vector<double> Ar_s(size_slay); 
		double tau = 0;

		r_s = Diff(M_A.dot(V_s), b); // начальная невязка
		p.Norm_R_Null(r_s);

		for (p.s; p.s < p.N + 1; p.s++) { // for отвечает за индекс приближения (итерацию метода)
			p.res_eps = 0;
		// обновляем значение Vij во внутренних узлах сетки
			
			// r_s = AV_s - b
			r_s = Diff(M_A.dot(V_s), b);
			

			Ar_s = M_A.dot( r_s);
			tau = Scalar_v(Ar_s, r_s) / Scalar_v(Ar_s, Ar_s);

			V_s = Diff(V_s, Mul_scal(tau, r_s));
			
			int k = 0;
			for (int i = 1; i < p.n; i++) {
				for (int j = 1; j < p.m; j++) {
					if (abs(p.V[i][j] - V_s[k]) > p.res_eps)
						p.res_eps = abs(p.V[i][j] - V_s[k]);
					p.V[i][j] = V_s[k++];
				}
			}

			if (p.res_eps <= p.eps) {
				p.output_eps = true;
				break;
			}
		}

		if (!p.output_eps)
			p.s--;

		if (p.test) {
			p.TotalError();
			p.Norm_R(r_s);
			p.ErrorSLAY();
			p.ErrorSLAY_Null();
			double m1, m2;
			max(m1, m2);
			p.ErrorRS(m1, m2);
			p.CreateCSV();
			p.CreateReferenceCSVTEST();
		} 
		else {
			if (!p.main)
				p.V_1 = p.V;
			else
				p.Difference(); // разность решения с одинарныи и половинным шагом
			p.Norm_R(r_s);
			p.ErrorSLAY();
			p.CreateCSV();
			p.ErrorSLAY_Null();
			if (p.main)
				p.CreateReferenceCSVMAIN();
			else {
				p.norm_r_1 = p.norm_r;
				p.s_1 = p.s;
				p.res_eps_1 = p.res_eps;
				p.est_error_SLAY_1 = p.est_error_SLAY;
				p.norm_r_1 = p.norm_r;
				p.norm_r_null_1 = p.norm_r_null;
			}

		}
	}
};

