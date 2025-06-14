#pragma once

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
#include <iostream>

#include "Ticker.hpp"
#include "OrderBook.hpp"
#include "Trades.hpp"
#include "Chart.hpp"
#include "DataStore.hpp"
#include "WebSocket.hpp"
#include "ConnectionState.hpp"
#include "DataParser.hpp"
#include "HttpsClient.hpp"
#include "EventBus.hpp"
//#include "MasterOrderBookManager.hpp"

class App {
  public:
    App();
    ~App();
    void update();
    void run();

    static ImFont* default_font;
    static ImFont* large_font;

  private:
    void render();
    void drawMenuBar();
    void styleApp();
    void initWebSocket();

    const char* glsl_version;
    ImGuiIO* io;
    GLFWwindow* window;
    ImVec4 clear_color = ImVec4(40/255.f, 42/255.f, 54/255.f, 1.0f);
    
    bool connected = false;
    
    std::vector<std::unique_ptr<Widget>> widgets;
    std::shared_ptr<DataStore> ds;
    std::shared_ptr<WebSocket> ws;
    std::shared_ptr<ConnectionState> cs;
    std::shared_ptr<DataParser> dp;
    std::shared_ptr<EventBus> eb;
   // std::shared_ptr<MasterOrderBookManager> mobm;
    std::shared_ptr<HttpsClient> hc;

    net::io_context ioc;
    boost::system::error_code ec;
    ssl::context ctx {ssl::context::tlsv12_client};
    std::thread io_thread;
};
