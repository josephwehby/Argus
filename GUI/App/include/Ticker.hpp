#include "Widget.hpp"

class Ticker : public Widget {
  public:
    Ticker();
    void draw() override; 
  private:
    const std::string channel = "ticker";
    double price = 98.79;
};
