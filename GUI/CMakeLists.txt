cmake_minimum_required(VERSION 3.26.3)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(app)

set(LIBS_DIR "${CMAKE_SOURCE_DIR}/libs")
set(GLFW_DIR "${LIBS_DIR}/glfw/include")
set(IMGUI_DIR "${LIBS_DIR}/imgui")
set(IMPLOT_DIR "${LIBS_DIR}/implot")
set(GLFW_LIB "${LIBS_DIR}/GLFWBUILD/src/Debug/glfw3.lib")

find_package(OpenGL REQUIRED)

if (MSVC)
  add_compile_options(/std:c++20)
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
endif()

file(GLOB APP_SOURCES "src/*.cpp")

# Add ImGui source files
file(GLOB IMGUI_SOURCES
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
)

file(GLOB IMGUI_BACKEND_SOURCES
    "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
)

file(GLOB IMPLOT_SOURCES
  "${IMPLOT_DIR}/implot.cpp"
  "${IMPLOT_DIR}/implot_items.cpp"
)

add_executable(App ${APP_SOURCES} ${IMGUI_SOURCES} ${IMGUI_BACKEND_SOURCES} ${IMPLOT_SOURCES})

target_include_directories(App PRIVATE
  ${GLFW_DIR}
  ${IMGUI_DIR}
  ${IMGUI_DIR}/backends
  ${IMPLOT_DIR}
  "./include"
)

#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:MSVCRT")
target_link_libraries(App PRIVATE "${GLFW_LIB}" OpenGL::GL user32.lib gdi32.lib)

add_custom_command(
  TARGET App
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "../fonts/JetBrainsMono-Regular.ttf" "${CMAKE_BINARY_DIR}/Debug/"
  COMMENT "Copying jet brains font to output directory"
)
