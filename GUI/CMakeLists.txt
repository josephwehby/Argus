cmake_minimum_required(VERSION 3.26.3)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(app)

find_package(OpenGL REQUIRED)

set(LIBS_DIR "./libs")
set(GLFW_DIR "${LIBS_DIR}/GLFW")
set(IMGUI_DIR "${LIBS_DIR}/imgui")
set(IMPLOT_DIR "${LIBS_DIR}/implot")
set(GLFW_LIB_PATH "${LIBS_DIR}/GLFW/glfw3")

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

# Ignore conflicts with default libraries (MSVCRT)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:MSVCRT")

#target_link_libraries(App PRIVATE ${GLFW_LIB_PATH} OpenGL::GL user32.lib gdi32.lib)
target_link_libraries(App PRIVATE "D:/Code/C++/Argus/GUI/libs/GLFW/glfw3.lib" OpenGL::GL user32.lib gdi32.lib)
