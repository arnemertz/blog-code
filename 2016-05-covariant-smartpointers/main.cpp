#include <iostream>
#include <memory>

// abstract widgets
struct AbstractButton {
  virtual void click() = 0;
  virtual ~AbstractButton() = default;
};

struct AbstractWidgetFactory {
  virtual std::unique_ptr<AbstractButton> createButton() const = 0;
  virtual ~AbstractWidgetFactory() = default;
};


// fancy widgets
struct FancyButton : AbstractButton {
  void click() final override { std::cout << "You clicked me, I'm so fancy!\n"; }
};

struct FancyWidgetFactory : AbstractWidgetFactory {
  std::unique_ptr<AbstractButton> createButton() const final override {
    return std::make_unique<FancyButton>();  
  }
};



int main() {
  std::shared_ptr<AbstractWidgetFactory> theWidgetFactory = std::make_shared<FancyWidgetFactory>();
   
  auto theButton = theWidgetFactory->createButton();
  theButton->click();
}
