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
	int N; // ����������� ��������� ����� �������� ������
	int s; //������� ��������
	double eps; // �������� �������� �������������� ������ ������
	double res_eps; //�������� � ������� ����
	double n, m; // ����������� �����

	double h; // ��� �� �
	double k; // ��� �� �
	// �������� ��������� ��������� ������� �
	double green_coeff;
	double yellow_coeff;
	double A;

	int size_V = (n + 1) * (m + 1); // ����������� �������� �������, ���������� ��������, �������� �������� Vij �� ���������� ����� c����
	double a, b, c, d; //������� ������� G

	// ������, ������ ������� ��������, ������������� ���� �����
	// ��������� ������ ���������� ��������� � ��������� (������� ������������) 
	std::vector<std::vector<double>> V;
	std::vector<std::vector<double>> U; //������ ������� ��
	std::vector<std::vector<double>> V_0; // ��������� �����������
	std::vector<std::vector<double>> V_; // �������� ������� � ��������� � ���������� ����� 
	double E2;

	bool main = false;
	std::vector<std::vector<double>> V_1; //���-� �� ����� � ������� �����
	double norm_r_1, s_1, res_eps_1, est_error_SLAY_1, w_1;

	std::vector<std::vector<double>> V_20; // ��������� �����������

	std::vector<std::vector<double>> Z; // ������ ����� �����������

	bool test; //���� ������ �������� - true, ����� false
	bool output_eps; //���� ����� �������� �� �������� - true, ����� - false

	double total_error; // ����� �����������
	int x_total_error, y_total_error; //����, ��� ���������� �����������
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
		double M = max_lamd / min_lamd; // ������ ����� ��������������� ������ �
		est_error_SLAY_null = M * pow(((M - 1) / (M + 1)), s);
		est_error_SLAY_null *= (pow(double(n - 1) * (m - 1), 0.5) * norm_r_null) / min_lamd; // ��������� �� ||z_0||
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
		
		wData << "���������� �������� �������� ������";
		wData << endl;

		wData << "������ ������ � ������������ E = " << E2;
		wData << endl;
		wData << "������������ ���������� ������� � ���������� ������� � ���� � = " << x_total_error
			<< "  y = " << y_total_error;
		wData << endl;
		wData << endl;

		wData << "��� ������� �������� ������ ������������ �����: ";
		wData << endl;
		wData << "����� ���������� �� x   n = " << n/2 << "    ����� ��������� �� y    m = " << m/2;
		wData << endl;
		wData << "�������� ��������� ��������� ������: ";
		wData << endl;
		wData << "�� �������� E_��� = " << eps << "    �� ����� ��������    N_max = " << N;
		wData << endl;
		wData << endl;



		wData << "�� ������� ���� ��������� N = " << s_1;
		wData << endl;
		wData << "���������� �������� ������ �� E_N = " << res_eps_1;
		wData << endl;
		wData << "���� ������ � �������� ||R_N|| = " << norm_r_1;
		wData << endl;
		wData << "��� ������� ������������ ����� �������� ||R||inf = max|R_ij|, ��� i = 1,2,...n    j = 1,2,...m ";
		wData << endl;
		wData << "����������� ������� ���� ||Z_N||inf <= ||Z_N||2 <= " << est_error_SLAY_1;
		wData << endl;
		wData << "��������� ����������� ������������� ������ - �������� ������������ ��������� ������� �� ����������� x";
		wData << endl;
		wData << endl;

		wData << "������� �� ��������� ����������� ||R_0|| = " << norm_r_null_1;
		wData << endl;
		wData << "����������� ������� ���� ||Z_N||inf <= ||Z_N||2 <= " << est_error_SLAY_null_1;
		wData << endl;
		wData << endl;

		wData << "��� �������� �������� ������������ �����:";
		wData << endl;
		wData << "����� ��������� �� x_2n = " << n << "    ����� ��������� �� �_2n = " << m;
		wData << endl;

		wData << "�������� ��������� ��������� ������ ����������� �������: ";
		wData << endl;
		wData << "�� �������� E_���_2 = " << eps << "    �� ����� ��������    N_max_2 = " << N;
		wData << endl;
		wData << endl;

		wData << "�� ������� ���� ��������� N2 = " << s;
		wData << endl;
		wData << "���������� �������� ������ �� E_N2 = " << res_eps;
		wData << endl;
		wData << "���� ������ � �������� ||R_N2|| = " << norm_r;
		wData << endl;
		wData << "��� ������� ������������ ����� �������� ||R||inf = max|R_ij|, ��� i = 1,2,...n    j = 1,2,...m ";
		wData << endl;

		wData << "����������� ������� ���� ||Z_N2||inf <= ||Z_N2||2 <= " << est_error_SLAY;
		wData << endl;
		wData << "��������� ����������� ������������� ������ - �������� ������������ ���������� ��������� �� ����������� x";
		wData << endl;
		wData << endl;

		wData << "������� �� ��������� ����������� ||R_0|| = " << norm_r_null;
		wData << endl;
		wData << "����������� ������� ���� ||Z_N||inf <= ||Z_N||2 <= " << est_error_SLAY_null;
		wData << endl;
		wData << endl;
		wData.close();
	}


	void CreateReferenceCSVTEST() {
		string ref = "reference_test.txt";
		ofstream wData;
		wData.open(ref);
		if (test) {
			wData << "���������� �������� �������� ������";
			wData << endl;
			wData << "������ ������ � ������������ Z_��� = " << total_error;
			wData << endl;
			wData << "������������ ���������� ���������� ������� �� ������� ������� � ���� � = " << x_total_error
				<< "  y = " << y_total_error;
			wData << endl;
			wData << endl;

			wData << "��� ������� �������� ������ ������������ �����: ";
			wData << endl;
			wData << "����� ���������� �� x   n = " << n << "    ����� ��������� �� y    m = " << m;
			wData << endl;
			wData << "�������� ��������� ��������� ������ ����������� �������: ";
			wData << endl;
			wData << "�� �������� E_��� = " << eps << "    �� ����� ��������    N_max = " << N;
			wData << endl;
			wData << endl;


			wData << "�� ������� ���� ��������� N = " << s;
			wData << endl;
			wData << "���������� �������� ������ �� E_N = " << res_eps;
			wData << endl;
			wData << "���� ������ � �������� ||R_N|| = " << norm_r;
			wData << endl;
			wData << "��� ������� ������������ ����� �������� ||R||inf = max|R_ij|, ��� i = 1,2,...n    j = 1,2,...m";
			wData << endl;
			wData << "����������� ������� ���� ||Z_N||inf <= ||Z_N||2 <= " << est_error_SLAY;
			wData << endl;
			wData << "��������� ����������� ������������� ������ - �������� ������������ ��������� ������� �� ����������� x";
			wData << endl;
			wData << endl;
			wData << "������� �� ��������� ����������� ||R_0|| = " << norm_r_null;
			wData << endl;
			wData << "����������� ������� ���� ||Z_N||inf <= ||Z_N||2 <= " << est_error_SLAY_null;
			wData << endl;
			wData << endl;
			wData << "�� ������� � ���������� ����� ����������� ����� ||z_�����||inf <= " << est_error_RS;
			wData << endl;
			wData << "��� ������ ����������� ����� ������������ ����� �������� ||z_�����||inf = max|z_ij|, ��� i = 1,2,...n    j = 1,2,...m";
			wData << endl;
			wData << endl;
			wData << "����� ����������� ������� �������� ������ � ������ � ��������� ||z_���||inf <= ||Z_N||inf +  ||z_�����||inf <= " << est_error_RS + est_error_SLAY;
			wData << endl;
			wData << "��� ������ ����� ����������� ������������ ����� �������� ||z_���||inf = max|z_ij|, ��� i = 1,2,...n    j = 1,2,...m ";
			wData << endl;
		}
		wData.close();
	}

	void Record(string name, const vector<vector<double>>& x) {
		string table = name;
		ofstream wData;
		wData.open(table);
		// ������ ����� �� x
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
			// ������ ���������� �������
			Record("table.csv", V);

			// ������ ���������� ����������� 
			Record("table_v0.csv", V_0);

			// ������ ������� �������
			Record("table_u.csv", U);

			// ������ �������� ������� � ����������
			Record("table_z.csv", Z);
		}
		else {
			if (!main) {
				// ������ ���������� �������
				Record("table.csv", V);

				// ������ ���������� ����������� 
				Record("table_v0.csv", V_0);
			} 
			else {
				// ������ ������� �������
				Record("table_v2.csv", V);

				// ������ �������� ������� � ����������
				Record("table_v20.csv", V_0);

				// ������ �������� ������� �� �������� ����� � ����� � ���������� �����
				string table = "table_ra.csv";
				ofstream wData;
				wData.open(table);
				// ������ ����� �� x
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

		//������ ���������� ������������
		
	}
};