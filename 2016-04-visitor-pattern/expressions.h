#include <cassert>
#include <memory>

class Expression {
public:
  virtual ~Expression() = 0;
};
inline Expression::~Expression() = default;
using ExpressionPtr = std::unique_ptr<Expression>;

class BinaryExpression : public Expression {
  ExpressionPtr lhs;
  ExpressionPtr rhs;
public:
  BinaryExpression(ExpressionPtr left, ExpressionPtr right) 
    : lhs(move(left)), rhs(move(right))
  { assert(lhs && rhs); }
  
  Expression& left() { return *lhs; }
  Expression& right() { return *rhs; }
};
  
class AddExpression : public BinaryExpression {
public:
  using BinaryExpression::BinaryExpression;
};
    
class MultiplyExpression : public BinaryExpression {
public:
  using BinaryExpression::BinaryExpression;
};

class NumberExpression : public Expression {
  double number;
public:
  NumberExpression(double d) : number(d) {}
  double getNumber() const { return number; }
};