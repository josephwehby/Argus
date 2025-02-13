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

  window = glfwCreateWindow(1280, 720, "Crypto Trading", nullptr, nullptr);
  
  if (window == nullptr) return;

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  io = &ImGui::GetIO(); 

  io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

  io->Fonts->AddFontFromFileTTF("../fonts/JetBrainsMono-Regular.ttf", 18);
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
}

App::~App() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
}

void App::update() {

  drawMenuBar();
  for (auto& widget : widgets) {
    widget->draw();
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

// potentially make these a dropdown menu with cryptos they can choose from instead of having input box. this will reduce error checking as everything will be preprogrammed in
void App::drawMenuBar() {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::Button("Price")) {
      widgets.push_back(std::make_unique<Ticker>());
    }
    ImGui::SameLine();
    if (ImGui::Button("OrderBook")) {
      widgets.push_back(std::make_unique<OrderBook>());
    }
    ImGui::SameLine();
    if (ImGui::Button("Chart")) {

    }
    ImGui::SameLine();
    if (ImGui::Button("Trades")) {
      widgets.push_back(std::make_unique<Trades>());
    }

    ImGui::EndMainMenuBar();
  }
}
