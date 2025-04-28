#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"

#include <stdio.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>

#include "Ticker.hpp"
#include "OrderBook.hpp"
#include "Trades.hpp"
#include "Chart.hpp"
#include "DataStore.hpp"
#include "WebSocket.hpp"

class App {
  public:
    App();
    ~App();
    void update();
    void run();
  private:
    void render();
    void drawMenuBar();
    void styleApp();
    bool initWebSocket();

    const char* glsl_version;
    ImGuiIO* io;
    GLFWwindow* window;
    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
    
    std::vector<std::unique_ptr<Widget>> widgets;
    bool connected = false;

    std::shared_ptr<DataStore> datastore;
    
    std::shared_ptr<WebSocket> ws;
    net::io_context ioc;
    boost::system::error_code ec;
    ssl::context ctx {ssl::context::tlsv12_client};
    std::thread io_thread;
};
