#include <vector>
#include <cmath>
#include <cassert>
#include <iostream>
#include <algorithm>


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


// Solve y=m*x for x
Vector gaussJordanElimination(Matrix m, Vector y) {
  int n = m.rows();
  assert(n==m.cols());
  vector<int> ref(n);

  for (int i=0;i<n;++i) {
    ref[i] = i;
  }

  for (int row=0; row<n; ++row) {
    // Find a row that has a non-zero value in the current column
    {
      int i = row;
      for (;;++i) {
        assert(i<n);
        if (m[i][row]!=0) {
          break;
        }
      }
      std::swap(m[i], m[row]);
      std::swap(y[i], y[row]);
      std::swap(ref[i], ref[row]);
    }
    {
      // Normalize row to have diagonal element be 1.0
      float v = m[row][row];
      for (int j=row;j<n;++j) {
        m[row][j] /= v;
      }
      y[row] /= v;
    }
    // Make all lower rows have zero in this column
    for (int j=0;j<n;++j) {
      if (j!=row) {
        float v = m[j][row];
        for (int k=row;k<n;++k) {
          m[j][k] -= m[row][k]*v;
        }
        y[j] -= y[row]*v;
      }
    }
  }
  for (int i=0;i<n;++i) {
    std::swap(y[i], y[ref[i]]);
  }
  return y;
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
