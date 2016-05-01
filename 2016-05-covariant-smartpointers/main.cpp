#include <iostream>
#include <memory>

// abstract widgets
struct AbstractButton {
  virtual void click() = 0;
  void setText(char const*) {};
  virtual ~AbstractButton() = default;
};

struct AbstractWindow { 
  virtual void add(std::unique_ptr<AbstractButton>) = 0;
  virtual void addText(std::string const&) = 0;
  virtual ~AbstractWindow() = default;
};

struct AbstractWidgetFactory {
  virtual std::unique_ptr<AbstractButton> createButton() const = 0;
  virtual std::unique_ptr<AbstractWindow> createWindow() const = 0;
  virtual std::unique_ptr<AbstractWindow> createMessageWindow(std::string const& text) = 0;
  virtual ~AbstractWidgetFactory() = default;
};


// fancy widgets
struct FancyButton : AbstractButton {
  void click() final override { 
    std::cout << "You clicked me, I'm so fancy!\n"; 
  }
  void doFancyStuff() {};
};

struct FancyWindow : AbstractWindow {
  virtual void add(std::unique_ptr<AbstractButton>) final override { /*...*/}
  virtual void addText(std::string const&) final override {/*...*/}
};

struct FancyWidgetFactory : AbstractWidgetFactory {
  std::unique_ptr<AbstractButton> createButton() const final override {
    return std::make_unique<FancyButton>();  
  }
  
  std::unique_ptr<AbstractWindow> createWindow() const final override { 
    return std::make_unique<FancyWindow>(); 
  }
  
  std::unique_ptr<AbstractWindow> createMessageWindow(std::string const& text) final override {
    auto theWindow = createWindow();
    theWindow->addText(text);
 
    auto theButton = createButton(); //unique_ptr<AbstractButton>
    static_cast<FancyButton*>(theButton.get())->doFancyStuff(); //EWW!
    theButton->setText("OK");
    theWindow->add(std::move(theButton));
    return theWindow;  
  }  
};



int main() {
  std::shared_ptr<AbstractWidgetFactory> theWidgetFactory = std::make_shared<FancyWidgetFactory>();
   
  auto theButton = theWidgetFactory->createButton();
  theButton->click();
}
