#pragma once
class AddExpression;
class MultiplyExpression;
class NumberExpression;

class ExpressionVisitor {
public:
  virtual void visitAdd(AddExpression&) = 0;
  virtual void visitMultiply(MultiplyExpression&) = 0;
  virtual void visitNumber(NumberExpression&) = 0;
  //...
};