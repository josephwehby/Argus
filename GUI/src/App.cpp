#include "App.hpp"

ImFont* App::default_font = nullptr;
ImFont* App::large_font = nullptr;

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

App::App() : cs(State::CONNECTING), sm(eb), dp(eb), hc(dp) {
  glfwSetErrorCallback(glfw_error_callback);
  
  if (!glfwInit()) return;

  glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  window = glfwCreateWindow(1700, 1100, "Argus - Binance", nullptr, nullptr);
  
  if (window == nullptr) return;

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImPlot::CreateContext();
  io = &ImGui::GetIO(); 

  io->ConfigFlags |= ImGuiConfigFlags_None;
  io->ConfigWindowsMoveFromTitleBarOnly;

  App::default_font = io->Fonts->AddFontFromFileTTF("JetBrainsMono-Regular.ttf", 20);
  App::large_font = io->Fonts->AddFontFromFileTTF("JetBrainsMono-Regular.ttf", 40);
  
  styleApp();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  initWebSocket();
  sm.setWebSocket(ws);
}

App::~App() {
  widgets.clear();
  hc.shutdown();
  dp.shutdown();
  
  // might rename this to shutdown for consistency 
  ws->close(); 
  ws->waitClose();

  ws.reset();
  ioc.stop();
  std::cout << "io stopped" << std::endl;
  
  if (io_thread.joinable()) {
    std::cout << "io joined" << std::endl;
    io_thread.join();
  }
  //std::this_thread::sleep_for(std::chrono::seconds(2));
  
  
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImPlot::DestroyContext();
  ImGui::DestroyContext();
  
  glfwDestroyWindow(window);
  glfwTerminate();
  std::cout << "app ran" << std::endl;
}

void App::initWebSocket() {
  ctx.set_default_verify_paths();
  
  if (ec) {
    std::cout << "Error " << ec << std::endl;
    return;
  }

  ws = std::make_shared<WebSocket>(ioc, ctx, dp, cs);

  io_thread = std::thread([&ioc = this->ioc](){
    ioc.run();     
  });

  ws->connect();
  std::cout << "connected" << std::endl;
  return;
}

void App::update() {
  
  drawMenuBar();
  ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
  for (auto it = widgets.begin(); it != widgets.end();) {

    if (!(*it)->isOpen()) {
      it = widgets.erase(it); 
    } else {
      (*it)->draw();
      it++;
    }
  }
}

void App::run() {

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
      ImGui_ImplGlfw_Sleep(10);
      continue;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    eb.dispatchAll();
    update();
    render();
  }
}

void App::render() {
  ImGui::Render();

  int display_w, display_h;
  glfwGetFramebufferSize(window, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  glfwSwapBuffers(window);
    
}

void App::styleApp() {
  ImGuiStyle& style = ImGui::GetStyle();

  style.WindowRounding = 5.f;
  style.FrameRounding = 5.f;
  style.Colors[ImGuiCol_WindowBg] = ImVec4(40/255.f, 42/255.f, 54/255.f, 1.0f);
  style.Colors[ImGuiCol_Border] = ImVec4(55/255.f, 57/255.f, 69/255.f, 1.0f);
  style.Colors[ImGuiCol_TitleBg] = ImVec4(45/255.f, 47/255.f, 59/255.f, 1.0f);
  style.Colors[ImGuiCol_TitleBgActive] = ImVec4(60/255.f, 62/255.f, 74/255.f, 1.0f);
  style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(45/255.f, 47/255.f, 59/255.f, 0.8f);
  style.Colors[ImGuiCol_FrameBg] = ImVec4(40/255.f, 42/255.f, 54/255.f, 1.0f);
  style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(40/255.f, 42/255.f, 54/255.f, 1.0f);
  style.Colors[ImGuiCol_FrameBgActive] = ImVec4(40/255.f, 42/255.f, 54/255.f, 1.0f);
  
  style.Colors[ImGuiCol_Button] = ImVec4(68/255.f, 71/255.f, 90/255.f, 1.0f);
  style.Colors[ImGuiCol_ButtonHovered] = ImVec4(98/255.f, 114/255.f, 164/255.f, 1.0f);
  style.Colors[ImGuiCol_ButtonActive]  = ImVec4(84/255.f, 94/255.f, 140/255.f, 1.0f);

}

void App::drawMenuBar() {
  State state = cs.getState();

  bool isDisabled = (state != State::CONNECTED) ? true : false;
  
  ImGui::BeginDisabled(isDisabled);

  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("Price")) {
      if (ImGui::MenuItem("BTC")) widgets.push_back(Utils::createWidget<Ticker>(sm, "BTCUSDT"));
      if (ImGui::MenuItem("ETH")) widgets.push_back(Utils::createWidget<Ticker>(sm, "ETHUSDT"));
      if (ImGui::MenuItem("SOL")) widgets.push_back(Utils::createWidget<Ticker>(sm, "SOLUSDT"));
      if (ImGui::MenuItem("DOGE")) widgets.push_back(Utils::createWidget<Ticker>(sm, "DOGEUSDT"));
      if (ImGui::MenuItem("ETC")) widgets.push_back(Utils::createWidget<Ticker>(sm, "ETCUSDT"));
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("OrderBook")) {
      if (ImGui::MenuItem("BTC")) widgets.push_back(Utils::createWidget<OrderBook>(sm, hc, "BTCUSDT"));
      if (ImGui::MenuItem("ETH")) widgets.push_back(Utils::createWidget<OrderBook>(sm, hc, "ETHUSDT"));
      if (ImGui::MenuItem("SOL")) widgets.push_back(Utils::createWidget<OrderBook>(sm, hc, "SOLUSDT"));
      if (ImGui::MenuItem("DOGE")) widgets.push_back(Utils::createWidget<OrderBook>(sm, hc, "DOGEUSDT"));
      if (ImGui::MenuItem("ETC")) widgets.push_back(Utils::createWidget<OrderBook>(sm, hc, "ETCUSDT"));
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Chart")) {
      if (ImGui::MenuItem("BTC")) widgets.push_back(Utils::createWidget<Chart>(sm, hc, "BTCUSDT"));
      if (ImGui::MenuItem("ETH")) widgets.push_back(Utils::createWidget<Chart>(sm, hc, "ETHUSDT"));
      if (ImGui::MenuItem("SOL")) widgets.push_back(Utils::createWidget<Chart>(sm, hc, "SOLUSDT"));
      if (ImGui::MenuItem("DOGE")) widgets.push_back(Utils::createWidget<Chart>(sm, hc, "DOGEUSDT"));
      if (ImGui::MenuItem("ETC")) widgets.push_back(Utils::createWidget<Chart>(sm, hc, "ETCUSDT"));
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Trades")) {
      if (ImGui::MenuItem("BTC")) widgets.push_back(Utils::createWidget<Trades>(sm, "BTCUSDT"));
      if (ImGui::MenuItem("ETH")) widgets.push_back(Utils::createWidget<Trades>(sm, "ETHUSDT"));
      if (ImGui::MenuItem("SOL")) widgets.push_back(Utils::createWidget<Trades>(sm, "SOLUSDT"));
      if (ImGui::MenuItem("DOGE")) widgets.push_back(Utils::createWidget<Trades>(sm, "DOGEUSDT"));
      if (ImGui::MenuItem("ETC")) widgets.push_back(Utils::createWidget<Trades>(sm, "ETCUSDT"));
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Other")) {
      if (ImGui::MenuItem("Alerts")) widgets.push_back(Utils::createWidget<Alerts>(sm));
      ImGui::EndMenu();
    }
    
    std::string status = "";
    ImVec4 status_color;

    switch(state) {
      case State::CONNECTING:
        status_color = ImVec4(1.00f, 0.84f, 0.00f, 1.00f);
        status = "Connecting";
        break;
      case State::CONNECTED:
        status_color = ImVec4(50/255.f, 205/255.f, 50/255.f, 1.f);
        status = "Online";
        break;
      case State::CLOSED:
        status_color = ImVec4(220/255.f, 20/255.f, 60/255.f, 1.f);
        status = "Offline";
        break;
      default:
        break;
    }
    
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 text_size = ImGui::CalcTextSize(status.c_str());
    ImVec2 screen_size = ImGui::GetContentRegionAvail();

    float new_x = pos.x + screen_size.x - text_size.x - 5; 
    ImGui::SetCursorPosX(new_x);
    ImGui::TextColored(status_color, status.c_str());

    ImGui::EndMainMenuBar();
  }

  ImGui::EndDisabled();
}
