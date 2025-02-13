cmake_minimum_required(VERSION 3.21.1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(app)

set(CMAKE_TOOLCHAIN_FILE "/opt/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set(CMAKE_PREFIX_PATH "/opt/vcpkg/installed/x64-linux" CACHE STRING "")
set(IMGUI_DIR "../imgui")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

file(GLOB APP_SOURCES "src/*.cpp")

# Add ImGui source files
file(GLOB IMGUI_SOURCES
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/imgui_demo.cpp"  # Optional
)

# Add the ImGui backends for GLFW + OpenGL
file(GLOB IMGUI_BACKEND_SOURCES
    "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
)

add_executable(App ${APP_SOURCES} ${IMGUI_SOURCES} ${IMGUI_BACKEND_SOURCES})

target_link_libraries(App PRIVATE glfw OpenGL::GL)

target_include_directories(App PRIVATE ${IMGUI_DIR} ${IMGUI_DIR}/backends "./include")
