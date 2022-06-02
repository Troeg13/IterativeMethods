#pragma once
#include <vector>
#include <iostream>
#include <cmath>
#include <locale>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

const double PI = 3.14159265358979323846;
struct  MethodTemp
{
	int N; // максимально возможное число итераций метода
	int s; //счётчик итераций
	double eps; // параметр контроля точностичилось решить задачу
	double res_eps; //точность с которой полу
	double n, m; // размерность сетки

	double h; // шаг по х
	double k; // шаг по у
	// значения ненулевых элементов матрицы А
	double green_coeff;
	double yellow_coeff;
	double A;

	int size_V = (n + 1) * (m + 1); // резмерность искомого вектора, элементами которого, являются значения Vij во внутренних узлах cетки
	double a, b, c, d; //границы области G

	// вектор, каждый элемент которого, соответствует узлу сетки
	// заполняем вектор граничными условиями и начальным (нулевым приближением) 
	std::vector<std::vector<double>> V;
	std::vector<std::vector<double>> U; //точное решение ду
	std::vector<std::vector<double>> V_0; // начальное приближение
	std::vector<std::vector<double>> V_; // разность решений с одинарным и половинным шагом 
	double E2;

	bool main = false;
	std::vector<std::vector<double>> V_1; //реш-е на сетке с обычным шагом
	double norm_r_1, s_1, res_eps_1, est_error_SLAY_1, w_1;

	std::vector<std::vector<double>> V_20; // начальное приближение

	std::vector<std::vector<double>> Z; // вектор общей погрешности

	bool test; //если задача тестовая - true, иначе false
	bool output_eps; //если выход совершён по точности - true, инече - false

	double total_error; // общая погрешность
	int x_total_error, y_total_error; //узел, где отклонение максимально
	double norm_r;
	double norm_r_null, norm_r_null_1;
	double est_error_SLAY_null, est_error_SLAY_null_1;
	double est_error_SLAY;
	double est_error_RS;

	void Satka() {
		est_error_SLAY_null = 0;
		est_error_SLAY_null_1 = 0;
		norm_r_null = 0;
		norm_r_null_1 = 0;
		s = 1;
		total_error = 0;
		x_total_error = 0;
		y_total_error = 0;
		res_eps = 0;
		est_error_SLAY = 0;
		norm_r = 0;
		E2 = 0;

		output_eps = false;
		h = (b - a) / n;
		k = (d - c) / m;
		green_coeff = double(1) / (h * h);
		yellow_coeff = double(1) / (k * k);
		A = double(-2) * (green_coeff + yellow_coeff);
		V.resize(n + 1);
		if (test) {
			U.resize(n + 1);

		}	
	}


	void TotalError() {
		Z = V_0;
		for (int i = 0; i < n + 1; i++) {
			for (int j = 0; j < m + 1; j++) {
				Z[i][j] = U[i][j] - V[i][j];
				if (abs(Z[i][j]) > total_error) {
					total_error = abs(Z[i][j]);
					x_total_error = i;
					y_total_error = j;
				}
					
			}
		}
	}

	void Norm_R(const std::vector<double>& r)
	{
		for (int i = 0; i < (n - 1) * (m - 1); i++) {
			if (abs(r[i]) > norm_r) {
				norm_r = abs(r[i]);
			}
		}
	}

	void Norm_R_Null(const std::vector<double>& r)
	{
		for (int i = 0; i < (n - 1) * (m - 1); i++) {
			if (abs(r[i]) > norm_r_null) {
				norm_r_null = abs(r[i]);
			}
		}
	}

	void ErrorSLAY_Null() {
		double min_lamd = (double(4) / (h * h) * (sin(PI / (2.0 * n))) * (sin(PI / (2.0 * n))) + double(4) / (k * k) * (sin(PI / (2.0 * m))) * (sin(PI / (2.0 * m))));
		double max_lamd = (double(4) / (h * h) * (sin((PI * (n - 1)) / (2.0 * n))) * (sin((PI * (n - 1)) / (2.0 * n))) + double(4) / (k * k) * (sin((PI * (m - 1)) / (2.0 * m))) * (sin((PI * (m - 1)) / (2.0 * m))));
		double M = max_lamd / min_lamd; // оценка числа обусловленности матриы А
		est_error_SLAY_null = M * pow(((M - 1) / (M + 1)), s);
		est_error_SLAY_null *= (pow(double(n - 1) * (m - 1), 0.5) * norm_r_null) / min_lamd; // домножаем на ||z_0||
	}

	void ErrorSLAY() {
		double min_lamd = (double(4) / (h * h) * (sin(PI / (2.0 * n))) * (sin(PI / (2.0 * n))) + double(4) / (k * k) * (sin(PI / (2.0 * m))) * (sin(PI / (2.0 * m))));
		//cout << pow(double((n - 1) * (m - 1)), 0.5);
		est_error_SLAY = (pow(double(n - 1)*(m - 1), 0.5) * norm_r) / min_lamd;
	}

	double M1_or_M2(int id, double x, double y) {
		double tmp = pow(PI, 4)* exp(pow(sin(PI * x * y), 2));
		tmp *= abs(-4.0 * pow(sin(2 * PI * x * y), 2) + 6.0 * sin(4 * PI * x * y) * sin(2 * PI * x * y) + pow(sin(2 * PI * x * y), 4) - 8.0 * cos(2 * PI * x * y)+ 12.0 * cos(4 * PI * x * y));
		if (id == 1)
			return tmp * abs(pow(x, 4));
		return tmp * abs(pow(y, 4));
	}

	void ErrorRS(double M1_max, double M2_max) {
		est_error_RS = double(1) / double(16) * (M1_max * h * h + M2_max * k * k) * (pow(b - a, 2) + pow(d - c, 2));
	}

	void CreateReferenceCSVMAIN() {
		
		string ref = "reference_main.txt";
		ofstream wData;
		wData.open(ref);
		
		wData << "Результаты расчётов основной задачи";
		wData << endl;

		wData << "Задача решена с погрешностью E = " << E2;
		wData << endl;
		wData << "Максимальное отклонение точного и численного решений в узле х = " << x_total_error
			<< "  y = " << y_total_error;
		wData << endl;
		wData << endl;

		wData << "Для решения основной задачи использована сетка: ";
		wData << endl;
		wData << "Число оразбиений по x   n = " << n/2 << "    число разбиений по y    m = " << m/2;
		wData << endl;
		wData << "Значения критериев остановки метода: ";
		wData << endl;
		wData << "по точности E_мет = " << eps << "    по числу итераций    N_max = " << N;
		wData << endl;
		wData << endl;



		wData << "На решение СЛАУ затрачено N = " << s_1;
		wData << endl;
		wData << "Достигнута точность метода на E_N = " << res_eps_1;
		wData << endl;
		wData << "СЛАУ решена с невязкой ||R_N|| = " << norm_r_1;
		wData << endl;
		wData << "Для невязки использована норма Чебычева ||R||inf = max|R_ij|, где i = 1,2,...n    j = 1,2,...m ";
		wData << endl;
		wData << "Погрешность решения СЛАУ ||Z_N||inf <= ||Z_N||2 <= " << est_error_SLAY_1;
		wData << endl;
		wData << "Начальное приближение итерационного метода - линейная интерполяция граничных условий по направлению x";
		wData << endl;
		wData << endl;

		wData << "Невязка на начальном приближении ||R_0|| = " << norm_r_null_1;
		wData << endl;
		wData << "Погрешность решения СЛАУ ||Z_N||inf <= ||Z_N||2 <= " << est_error_SLAY_null_1;
		wData << endl;
		wData << endl;

		wData << "Для контроля точности использована сетка:";
		wData << endl;
		wData << "Число разбиений по x_2n = " << n << "    число разбиений по у_2n = " << m;
		wData << endl;

		wData << "Значения критериев остановки метода минимальных невязок: ";
		wData << endl;
		wData << "по точности E_мет_2 = " << eps << "    по числу итераций    N_max_2 = " << N;
		wData << endl;
		wData << endl;

		wData << "На решение СЛАУ затрачено N2 = " << s;
		wData << endl;
		wData << "Достигнута точность метода на E_N2 = " << res_eps;
		wData << endl;
		wData << "СЛАУ решена с невязкой ||R_N2|| = " << norm_r;
		wData << endl;
		wData << "Для невязки использована норма Чебычева ||R||inf = max|R_ij|, где i = 1,2,...n    j = 1,2,...m ";
		wData << endl;

		wData << "Погрешность решения СЛАУ ||Z_N2||inf <= ||Z_N2||2 <= " << est_error_SLAY;
		wData << endl;
		wData << "Начальное приближение итерационного метода - линейная интерполяция граничными условиями по направлению x";
		wData << endl;
		wData << endl;

		wData << "Невязка на начальном приближении ||R_0|| = " << norm_r_null;
		wData << endl;
		wData << "Погрешность решения СЛАУ ||Z_N||inf <= ||Z_N||2 <= " << est_error_SLAY_null;
		wData << endl;
		wData << endl;
		wData.close();
	}


	void CreateReferenceCSVTEST() {
		string ref = "reference_test.txt";
		ofstream wData;
		wData.open(ref);
		if (test) {
			wData << "Результаты расчётов тестовой задачи";
			wData << endl;
			wData << "Задача решена с погрешностью Z_общ = " << total_error;
			wData << endl;
			wData << "Максимальное отклонение численного решения от точного решения в узле х = " << x_total_error
				<< "  y = " << y_total_error;
			wData << endl;
			wData << endl;

			wData << "Для решения тестовой задачи использована сетка: ";
			wData << endl;
			wData << "Число оразбиений по x   n = " << n << "    число разбиений по y    m = " << m;
			wData << endl;
			wData << "Значения критериев остановки метода минимальных невязок: ";
			wData << endl;
			wData << "по точности E_мет = " << eps << "    по числу итераций    N_max = " << N;
			wData << endl;
			wData << endl;


			wData << "На решение СЛАУ затрачено N = " << s;
			wData << endl;
			wData << "Достигнута точность метода на E_N = " << res_eps;
			wData << endl;
			wData << "СЛАУ решена с невязкой ||R_N|| = " << norm_r;
			wData << endl;
			wData << "Для невязки использована норма Чебычева ||R||inf = max|R_ij|, где i = 1,2,...n    j = 1,2,...m";
			wData << endl;
			wData << "Погрешность решения СЛАУ ||Z_N||inf <= ||Z_N||2 <= " << est_error_SLAY;
			wData << endl;
			wData << "Начальное приближение итерационного метода - линейная интерполяция граничных условий по направлению x";
			wData << endl;
			wData << endl;
			wData << "Невязка на начальном приближении ||R_0|| = " << norm_r_null;
			wData << endl;
			wData << "Погрешность решения СЛАУ ||Z_N||inf <= ||Z_N||2 <= " << est_error_SLAY_null;
			wData << endl;
			wData << endl;
			wData << "По теореме о сходимости схемы погрешность схемы ||z_схемы||inf <= " << est_error_RS;
			wData << endl;
			wData << "Для оценки погрешности схемы использована норма Чебышева ||z_схемы||inf = max|z_ij|, где i = 1,2,...n    j = 1,2,...m";
			wData << endl;
			wData << endl;
			wData << "Общая погрешность решения тестовой задачи с учётом её компонент ||z_общ||inf <= ||Z_N||inf +  ||z_схемы||inf <= " << est_error_RS + est_error_SLAY;
			wData << endl;
			wData << "Для оценки общей погрешности использована норма Чебышева ||z_общ||inf = max|z_ij|, где i = 1,2,...n    j = 1,2,...m ";
			wData << endl;
		}
		wData.close();
	}

	void Record(string name, const vector<vector<double>>& x) {
		string table = name;
		ofstream wData;
		wData.open(table);
		// запись узлов по x
		double xi = a;
		wData << "y_j / x_i:" << ",";
		for (int i = 0; i < n + 1; i++)
		{
			wData << xi << ",";
			xi += h;
		}
		wData << endl;
		double yj = c;
		for (int j = 0; j < m + 1; j++)
		{
			wData << yj << ",";
			for (int i = 0; i < n + 1; i++)
			{
				if (i != n)
					wData << x[i][j] << ",";
				else
					wData << x[i][j];
			}
			yj += k;
			wData << endl;
		}
		system("python csv_to_xlsx.py");
		wData.close();
	}

	void Difference() {
		V_ = V_1;

		for (int i = 0; i < n / 2 + 1; i++) {
			for (int j = 0; j < m / 2 + 1; j++) {
				V_[i][j] = V_[i][j] - V[i * 2][j * 2];
				if (E2 < abs(V_[i][j])) {
					E2 = V_[i][j];
					x_total_error = i;
					y_total_error = j;
				}
			}
		}
	}

	void CreateCSV() {
		
		
		if (test) {
			// запись численного решения
			Record("table.csv", V);

			// запись начального приближения 
			Record("table_v0.csv", V_0);

			// запись точного решения
			Record("table_u.csv", U);

			// запись разности точного и численоого
			Record("table_z.csv", Z);
		}
		else {
			if (!main) {
				// запись численного решения
				Record("table.csv", V);

				// запись начального приближения 
				Record("table_v0.csv", V_0);
			} 
			else {
				// запись точного решения
				Record("table_v2.csv", V);

				// запись разности точного и численоого
				Record("table_v20.csv", V_0);

				// запись разность решения на основной сетке и сетке с половинным шагом
				string table = "table_ra.csv";
				ofstream wData;
				wData.open(table);
				// запись узлов по x
				double xi = a;
				wData << "y_j / x_i:" << ",";
				for (int i = 0; i < n / 2 + 1; i++)
				{
					wData << xi << ",";
					xi += h * 2;
				}
				wData << endl;
				double yj = c;
				for (int j = 0; j < m / 2 + 1; j++)
				{
					wData << yj << ",";
					for (int i = 0; i < n / 2 + 1; i++)
					{
						if (i != n)
							wData << V_[i][j] << ",";
						else
							wData << V_[i][j];
					}
					yj += k * 2;
					wData << endl;
				}
				system("python csv_to_xlsx.py");
				wData.close();
			}

		}

		//запись начального оприближения
		
	}
};