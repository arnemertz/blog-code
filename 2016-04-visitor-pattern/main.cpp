#include <iostream>
#include <memory>
#include "expressions.h"
#include "expressionvisitor.h"

class ExpressionPrinter : public ExpressionVisitor {
  std::ostream& os;
  
  void visitBinaryExpression(BinaryExpression& binExpr, std::string const& infix) {
    binExpr.left().accept(*this);
    os << infix;
    binExpr.right().accept(*this);
  }
  
public:
  ExpressionPrinter(std::ostream& ostream) : os(ostream) {}
  void print(Expression& expr) {
    expr.accept(*this);
    os << '\n';
  }

  void visitAdd(AddExpression& addExpr) override {
    visitBinaryExpression(addExpr, " + ");  
  }
  void visitMultiply(MultiplyExpression& mulExpr) override {
    visitBinaryExpression(mulExpr, " * ");  
  }
  void visitNumber(NumberExpression& numExpr) override {
    os << numExpr.getNumber();
  }
};

int main() {
  auto expression = std::make_unique<AddExpression>(
    std::make_unique<NumberExpression>(3),
    std::make_unique<MultiplyExpression>(
      std::make_unique<NumberExpression>(4),    
      std::make_unique<NumberExpression>(6)
    )
  );
  
  ExpressionPrinter printer(std::cout);
  printer.print(*expression);
}