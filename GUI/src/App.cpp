#include "App.hpp"

ImFont* App::default_font = nullptr;
ImFont* App::large_font = nullptr;

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

App::App() {
  glfwSetErrorCallback(glfw_error_callback);
  
  if (!glfwInit()) return;

  glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  window = glfwCreateWindow(1700, 1100, "Argus", nullptr, nullptr);
  
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

  cs = std::make_shared<ConnectionState>(State::CONNECTING);
  ds = std::make_shared<DataStore>();
  eb = std::make_shared<EventBus>();
  dp = std::make_shared<DataParser>(ds, cs, eb);
  hc = std::make_shared<HttpsClient>(dp);
  initWebSocket();
  //mobm = std::make_shared<MasterOrderBookManager>(ws, dp);
}

App::~App() {
  widgets.clear();
  
  hc->shutdown();

  ws->close(); 
  ws->waitClose();

  ws.reset();
  ioc.stop();

  if (io_thread.joinable()) {
    io_thread.join();
  }
  
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImPlot::DestroyContext();
  ImGui::DestroyContext();
  
  glfwDestroyWindow(window);
  glfwTerminate();
}

void App::initWebSocket() {
  ctx.set_default_verify_paths();
  
  if (ec) {
    std::cout << "Error " << ec << std::endl;
    return;
  }

  ws = std::make_shared<WebSocket>(ioc, ctx, dp, cs);

  io_thread = std::thread([this](){
    ioc.run();     
  });

  ws->connect();

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
    
    eb->dispatchAll();
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
}

void App::drawMenuBar() {
  State state = cs->getState();

  bool isDisabled = (state != State::CONNECTED) ? true : false;
  
  ImGui::BeginDisabled(isDisabled);

  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("Price")) {
      if (ImGui::MenuItem("BTC")) {
        widgets.push_back(std::make_unique<Ticker>(ws, eb, "BTCUSDT"));
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("OrderBook")) {
      if (ImGui::MenuItem("BTC")) {
        widgets.push_back(std::make_unique<OrderBook>(ws, eb, hc, "BTCUSDT"));
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Chart")) {
      if (ImGui::MenuItem("BTC")) {
        widgets.push_back(std::make_unique<Chart>(ws, eb, hc, "BTCUSDT"));
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Trades")) {
      if (ImGui::MenuItem("BTC")) {
        widgets.push_back(std::make_unique<Trades>(ws, eb, "BTCUSDT"));
      }
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
