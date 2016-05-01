#pragma once
#include <cassert>
#include <memory>

struct Expression {
  enum ExpressionType {
    ADD,
    MULTIPLY,
    NUMBER
  };
  ExpressionType expressionType;
  
  Expression(ExpressionType eType) : expressionType{eType} {}
  virtual ~Expression() = 0;
};
inline Expression::~Expression() = default;
using ExpressionPtr = std::unique_ptr<Expression>;

struct BinaryExpression : Expression {
  ExpressionPtr lhs;
  ExpressionPtr rhs;

  BinaryExpression(ExpressionPtr left, ExpressionPtr right, ExpressionType eType) 
    : Expression{eType}, lhs{move(left)}, rhs{move(right)}
  { assert(lhs && rhs); }
};
  
struct AddExpression : BinaryExpression {
  using BinaryExpression::BinaryExpression;
};
    
struct MultiplyExpression : BinaryExpression {
  using BinaryExpression::BinaryExpression;
};

struct NumberExpression : Expression {
  double number;
  NumberExpression(double d) : Expression{NUMBER}, number{d} {}
};