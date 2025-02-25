#include "App.hpp"
#include <iostream>

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

App::App() {
  glfwSetErrorCallback(glfw_error_callback);
  
  if (!glfwInit()) return;

  glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  window = glfwCreateWindow(1280, 720, "Argus", nullptr, nullptr);
  
  if (window == nullptr) return;

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImPlot::CreateContext();
  io = &ImGui::GetIO(); 

  io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  io->ConfigWindowsMoveFromTitleBarOnly;

  io->Fonts->AddFontFromFileTTF("../fonts/JetBrainsMono-Regular.ttf", 18);
  
  styleApp();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
}

App::~App() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImPlot::DestroyContext();
  ImGui::DestroyContext();
  
  glfwDestroyWindow(window);
  glfwTerminate();
}

void App::update() {

  drawMenuBar();
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

  style.WindowRounding = 5.3f;
  style.FrameRounding = 5.3f;
  style.Colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.098f, 0.11f, 1.0f);
  style.Colors[ImGuiCol_TitleBgActive] = ImVec4(.14f, 0.26f, 0.45f, 1.0f);
  style.Colors[ImGuiCol_Tab] = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
}

// potentially make these a dropdown menu with cryptos they can choose from instead of having input box. this will reduce error checking as everything will be preprogrammed in
void App::drawMenuBar() {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("Price")) {
      if (ImGui::MenuItem("BTC")) {
        widgets.push_back(std::make_unique<Ticker>("BTC"));
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("OrderBook")) {
      if (ImGui::MenuItem("BTC")) {
        widgets.push_back(std::make_unique<OrderBook>("BTC"));
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Chart")) {
      if (ImGui::MenuItem("BTC")) {
        widgets.push_back(std::make_unique<Chart>("BTC"));
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Trades")) {
      if (ImGui::MenuItem("BTC")) {
        widgets.push_back(std::make_unique<Trades>("BTC"));
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
}
