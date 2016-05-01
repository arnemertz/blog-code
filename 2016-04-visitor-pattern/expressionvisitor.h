#pragma once
#include "expressions.h"

class ExpressionVisitor {
public:
  void visit(Expression& expr) {
    switch (expr.expressionType) {
      case Expression::ADD:
        visitAdd(static_cast<BinaryExpression&>(expr));
        break;
      case Expression::MULTIPLY:
        visitMultiply(static_cast<BinaryExpression&>(expr));
        break;
      case Expression::NUMBER:
        visitNumber(static_cast<NumberExpression&>(expr));
        break;
    }
  }
  virtual void visitAdd(BinaryExpression&) = 0;
  virtual void visitMultiply(BinaryExpression&) = 0;
  virtual void visitNumber(NumberExpression&) = 0;
  //...
};