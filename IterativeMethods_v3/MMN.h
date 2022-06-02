#pragma once
#include<vector>
#include<math.h>
#include<iostream>
#include "MethodTemplate.h"

using namespace std;


class MMN {
public:

	MethodTemp p;

	MMN() {
		p.Satka();
		for (int i = 0; i < p.n + 1; i++) {
			p.V[i].resize(p.m + 1, 0);
			if (i == 0 || i == p.n) {
				for (int j = 0; j < p.m + 1; j++)
					p.V[i][j] = ExactSolutionTest(p.a + i * p.h, p.c + j * p.k);
			}
			else {
				p.V[i][0] = ExactSolutionTest(p.a + i * p.h, p.c + 0 * p.k);
				p.V[i][p.m] = ExactSolutionTest(p.a + i * p.h, p.c + p.m * p.k);
			}

			if (p.test) {
				p.U[i].resize(p.m + 1);
				for (int j = 0; j < p.m + 1; j++)
					p.U[i][j] = ExactSolutionTest(p.a + i * p.h, p.c + j * p.k);
			}

		}
	}

	double ExactSolutionTest(double x, double y) { //точное решение тестовой задачи
		return exp(pow(sin(PI * x * y), 2));
	}

	double F(double x, double y) { //значение f в узле (x,y) 
		if (p.test)
			return -((2.0 * cos(2.0 * PI * x * y) + pow(sin(PI * x * y), 2)) * exp(pow(sin(PI * x * y), 2)) * (pow(x, 2) + pow(y, 2)) * pow(PI, 2));
		else
			return pow(sin(PI * x * y), 2);
	}

	void Solution()
	{

		vector<vector<double>> V_s = p.V;//вектор, соотв. текущему приблежению, на начальном этапе - нулевое приблежение

		double eps_s = 0; //точность на шаге s
		for (p.s; p.s < p.N + 1; p.s++) {



			for (int i = 1; i < p.n; i++) {
				for (int j = 1; j < p.m; j++) {
					if (abs(p.V[i][j] - V_s[i][j]) > eps_s)
						eps_s = abs(p.V[i][j] - V_s[i][j]);
					V_s[i][j] = p.V[i][j];
				}
			}
			if (eps_s <= p.eps)
				break;
		}
		
	}

};