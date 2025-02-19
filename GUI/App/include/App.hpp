#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"

#include <stdio.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>

#include "Ticker.hpp"
#include "OrderBook.hpp"
#include "Trades.hpp"
#include "Chart.hpp"

class App {
  public:
    App();
    ~App();
    void update();
    void run();
  private:
    void render();
    void drawMenuBar();
    const char* glsl_version;
    ImGuiIO* io;
    GLFWwindow* window;
    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);

    std::vector<std::unique_ptr<Widget>> widgets;
};
