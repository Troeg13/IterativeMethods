#pragma once

#include <vector>

// �� ���������� "using namespace std;", ��� ��� ��� �������� ��������������� ��� ����������� ��������� pair (���������� � ������������ ���� std).

struct pair1
{
  int index; // ������ ����������.
  double value; // �������� � ������ � �������� index.
};

/*
���:
1. ������������.
2. ������������.
3. �����������.
*/

// ����� - ������������ ������������� ��� ������.

class SparseMatrix
{
private: // ������ ������� � ������� ��� ������� ������ ������ ������.
  int rows, cols;
  std::vector<std::vector<pair1>> data; // ��������� ��� ������ vector<pair> (������������ ������ �������� � ���������� - ������).
public: // ������ �������
  SparseMatrix(int _rows, int _cols)
  {
    rows = _rows;
    cols = _cols;
    data.resize(rows);
    for (int i = 0; i < rows; i++)
      data[i].resize(0);
  }

  SparseMatrix(SparseMatrix& a)
  {
    rows = a.rows;
    cols = a.cols;
    data.resize(rows);
    for (int i = 0; i < rows; i++)
      data[i].resize(0);

    for (int i = 0; i < rows; i++)
      for (int k = 0; k < a.data[i].size(); k++)
        this->set(i, a.data[i][k].index, a.data[i][k].value);
  }


  ~SparseMatrix()
  {
    data.clear();
    rows = 0;
    cols = 0;
  }

  void set(int i, int j, double value) // ������ ������� �������
  {
    if (value != 0)
        data[i].push_back(pair1{ j, value });
  }

  double get(int i, int j) // ������� ������� �������
  {
    for (int k = 0; k < data[i].size(); k++)
    {
      if (data[i][k].index == j)
        return data[i][k].value;
    }
    return 0.;
  }

  SparseMatrix operator+(SparseMatrix& a) // �������� ������
  {
    if (rows == a.rows && cols == a.cols)
    {
      SparseMatrix b(*this);
      for (int i = 0; i < rows; i++)
        for (int k = 0; k < data[i].size(); k++)
          b.data[i][k].value += a.data[i][k].value;
      return b;
    }
  }

  SparseMatrix operator-(SparseMatrix& a) // �������� ������
  {
    if (rows == a.rows && cols == a.cols)
    {
      SparseMatrix b(*this);
      for (int i = 0; i < rows; i++)
        for (int k = 0; k < data[i].size(); k++)
          b.data[i][k].value -= a.data[i][k].value;
      return b;
    }
  }

  SparseMatrix operator*(SparseMatrix& a) // ������� * ������� (�����������)
  {
    if (rows == a.rows && cols == a.cols)
    {
      SparseMatrix b(*this);
      for (int i = 0; i < rows; i++)
        for (int k = 0; k < data[i].size(); k++)
          b.data[i][k].value *= a.data[i][k].value;
      return b;
    }
  }

  std::vector<double> dot(std::vector<double>& v) // ������� * ������
  {
    if (cols == v.size())
    {
      std::vector<double> b(v.size());
      for (int i = 0; i < rows; i++)
        for (int k = 0; k < data[i].size(); k++)
          b[i] += data[i][k].value * v[data[i][k].index];
      return b;
    }
    else
      throw(-1);
  }

  int getRows() // ���������� �����
  {
    return this->rows;
  }

  int getCols() // ���������� ��������
  {
    return this->cols;
  }

  int getSize() // ������ �������
  {
    return this->cols * this->rows;
  }

  void ChangeSign() // �������� ���� ������� �� ���������������
  {
    for (int i = 0; i < this->getRows(); i++)
      for (int k = 0; k < this->data[i].size(); k++)
        data[i][k].value = (-1.) * data[i][k].value;
  }

};

