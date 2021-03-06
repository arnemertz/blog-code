#include <iostream>
#include <memory>
#include "expressions.h"
#include "expressionvisitor.h"

class ExpressionPrinter : public ExpressionVisitor {
  std::ostream& os;
  
  void visitBinaryExpression(BinaryExpression& binExpr, std::string const& infix) {
    visit(*binExpr.lhs);
    os << infix;
    visit(*binExpr.rhs);
  }
  
public:
  ExpressionPrinter(std::ostream& ostream) : os(ostream) {}
  void print(Expression& expr) {
    visit(expr);
    os << '\n';
  }

  void visitAdd(BinaryExpression& addExpr) override {
    visitBinaryExpression(addExpr, " + ");  
  }
  void visitMultiply(BinaryExpression& mulExpr) override {
    visitBinaryExpression(mulExpr, " * ");  
  }
  void visitNumber(NumberExpression& numExpr) override {
    os << numExpr.number;
  }
};

int main() {
  auto expression = std::make_unique<BinaryExpression>(
    std::make_unique<NumberExpression>(3),
    std::make_unique<BinaryExpression>(
      std::make_unique<NumberExpression>(4),    
      std::make_unique<NumberExpression>(6),
	  Expression::MULTIPLY
    ),
	Expression::ADD
  );
  
  ExpressionPrinter printer(std::cout);
  printer.print(*expression);
}