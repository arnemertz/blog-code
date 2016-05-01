#pragma once
#include <cassert>
#include <memory>

#include "expressionvisitor.h"

struct Expression {
  enum ExpressionType {
    ADD,
    MULTIPLY,
    NUMBER
  };
  ExpressionType expressionType;
  
  Expression(ExpressionType eType) : expressionType{eType} {}
  virtual ~Expression() = default;
  virtual void accept(ExpressionVisitor&) = 0;
};
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
  void accept(ExpressionVisitor& visitor) override {
    visitor.visitAdd(*this);  
  }
};
    
struct MultiplyExpression : BinaryExpression {
  using BinaryExpression::BinaryExpression;
  void accept(ExpressionVisitor& visitor) override {
    visitor.visitMultiply(*this);  
  }
};

struct NumberExpression : Expression {
  double number;
  NumberExpression(double d) : Expression{NUMBER}, number{d} {}
  void accept(ExpressionVisitor& visitor) override {
    visitor.visitNumber(*this);  
  }
};