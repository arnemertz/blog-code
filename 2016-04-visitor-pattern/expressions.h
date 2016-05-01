#pragma once
#include <cassert>
#include <memory>

#include "expressionvisitor.h"

class Expression {
public:
  virtual ~Expression() = default;
  virtual void accept(ExpressionVisitor&) = 0;
};
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
  void accept(ExpressionVisitor& visitor) override {
    visitor.visitAdd(*this);  
  }
};
    
class MultiplyExpression : public BinaryExpression {
public:
  using BinaryExpression::BinaryExpression;
  void accept(ExpressionVisitor& visitor) override {
    visitor.visitMultiply(*this);  
  }
};

class NumberExpression : public Expression {
  double number;
public:
  NumberExpression(double d) : number(d) {}
  double getNumber() const { return number; }
  void accept(ExpressionVisitor& visitor) override {
    visitor.visitNumber(*this);  
  }
};