#include <vector>
#include <cmath>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <numeric>


using std::vector;
using std::cout;

class Matrix {
  typedef vector<float> Row;
  vector<Row> values;
public:
  Matrix(std::initializer_list<vector<float>> matrixValues)
    : values{matrixValues}
  {}

  int rows() const {
    return values.size();
  }
  int cols() const {
    return values[0].size();
  }
  Row& operator[](std::size_t index) {
    return values[index];
  }
  Row const& operator[](std::size_t index) const {
    return values[index];
  }
};


typedef vector<float> Vector;

class GaussJordanMatrix {
  Matrix m;
  Vector y;
  vector<int> rowIndices;

public:
  GaussJordanMatrix(Matrix matrix, Vector vector)
    : m{std::move(matrix)}, y{std::move(vector)}, rowIndices{}
  { 
    assert(rowCount()==m.cols());

    rowIndices.resize(rowCount());
    std::iota(std::begin(rowIndices), std::end(rowIndices), 0);
  }

  int rowCount() const {
    return m.rows();
  }

  int indexOfRowWithNonzeroColumn(int columnIndex) {
    for (int rowIndex = columnIndex; rowIndex < rowCount(); ++rowIndex) {
      if (m[rowIndex][columnIndex]!=0) {
        return rowIndex;
      }
    }
    assert(false);
    return -1;
  }

  void swapRows(int i, int j) {
    std::swap(m[i], m[j]);
    std::swap(y[i], y[j]);
    std::swap(rowIndices[i], rowIndices[j]);
  }

  void normalizeRow(int rowIndex) {
    auto& row = m[rowIndex];
    auto diagonalElement = row[rowIndex];
    for (auto& rowEntry : row) {
      rowEntry /= diagonalElement;
    }
    y[rowIndex] /= diagonalElement;
  }

  void subtractRow(int rowIndex, float factor, int fromRowIndex) {
    auto const& row = m[rowIndex];
    auto& fromRow = m[fromRowIndex];
    for (int k=0;k<rowCount();++k) {
      fromRow[k] -= row[k]*factor;
    }
    y[fromRowIndex] -= y[rowIndex]*factor;
  }

  void subtractToZeroInColumn(int masterRowIndex) {
    for (int rowIndex=0;rowIndex<rowCount();++rowIndex) {
      if (rowIndex!=masterRowIndex) {
        float factor = m[rowIndex][masterRowIndex];
	subtractRow(masterRowIndex, factor, rowIndex);
      }
    }
  }

  Vector getVectorInOriginalOrder() {
    Vector v = y;
    for (int i=0;i<rowCount();++i) {
      std::swap(v[i], v[rowIndices[i]]);
    }
    return v;
  }
};

// Solve y=m*x for x
Vector gaussJordanElimination(Matrix m, Vector y) {
  GaussJordanMatrix gaussJordan{std::move(m), std::move(y)};
  int rowCount = gaussJordan.rowCount();

  for (int row=0; row<rowCount; ++row) {
    int i = gaussJordan.indexOfRowWithNonzeroColumn(row);
    gaussJordan.swapRows(row,i);
    gaussJordan.normalizeRow(row);
    gaussJordan.subtractToZeroInColumn(row);
  }
  return gaussJordan.getVectorInOriginalOrder();
}


static void print_vector(const char *name,const Vector &b) {
  cout << name << "=" << "\n";
  for (int i=0, n=b.size(); i!=n; ++i) {
    cout << "  " << b[i] << "\n";
  }
  cout << "\n";
}


static void print_matrix(const char *name,const Matrix &temp) {
  cout << name << "=\n";
  for (int i=0, m=temp.rows(); i!=m; ++i) {
    for (int j=0, n=temp[i].size(); j!=n; ++j) {
      cout << "  " << temp[i][j];
    }
    cout << "\n";
  }
  cout << "\n";
}


static bool is_near(float actual,float expected,float tolerance) {
  float delta = fabsf(actual-expected);
  return delta<=tolerance;
}


static Vector product(const Matrix &m,const Vector &x) {
  Vector a(x.size());

  for (int i=0; i!=3; ++i) {
    float sum = 0;
    for (int j=0; j!=3; ++j) {
      sum += m[i][j]*x[j];
    }
    a[i] = sum;
  }

  return a;
}


int main() {
  Matrix m = {
    {1.1, 2.4, 3.7},
    {1.2, 2.5, 4.8},
    {2.3, 3.6, 5.9},
  };

  Vector y = {0.5,1.2,2.3};

  Vector x = gaussJordanElimination(m, y);

  Vector mx = product(m,x);

  print_matrix("m",m);
  print_vector("y",y);
  print_vector("x",x);
  print_vector("m*x",mx);

  float tolerance = 1e-5;

  for (int i=0, n=y.size(); i!=n; ++i) {
    assert(is_near(mx[i],y[i],tolerance));
  }
}
