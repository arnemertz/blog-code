#include <memory>
#include "expressions.h"

int main() {
  auto expression = std::make_unique<AddExpression>(
    std::make_unique<NumberExpression>(3),
    std::make_unique<MultiplyExpression>(
      std::make_unique<NumberExpression>(4),    
      std::make_unique<NumberExpression>(6)
    )
  );
}