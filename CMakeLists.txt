cmake_minimum_required(VERSION 3.26.3)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_WINDOWS_SDK_VERSION 10.0.26100.0)
add_definitions(-D_WIN32_WINNT=0x0B00)

project(app)

set(CMAKE_TOOLCHAIN_FILE "D:/VCPKG/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set(CMAKE_PREFIX_PATH "D:/VCPKG/vcpkg/installed/x64-windows" CACHE STRING "")

set(LIBS_DIR "${CMAKE_SOURCE_DIR}/GUI/libs")
set(IMGUI_DIR "${LIBS_DIR}/imgui")
set(IMPLOT_DIR "${LIBS_DIR}/implot")

find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Boost COMPONENTS asio beast REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

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

file(GLOB COMMON_SOURCES
  "${CMAKE_SOURCE_DIR}/common/src/*.cpp"
)

file(GLOB GUI_SOURCES "${CMAKE_SOURCE_DIR}/GUI/src/*.cpp")

file(GLOB BACKEND_SOURCES "${CMAKE_SOURCE_DIR}/Backend/src/*.cpp")

add_executable(App ${GUI_SOURCES} ${BACKEND_SOURCES} ${IMGUI_SOURCES} ${IMGUI_BACKEND_SOURCES} ${IMPLOT_SOURCES} ${COMMON_SOURCES})

target_include_directories(App PRIVATE
  ${IMGUI_DIR}
  ${IMGUI_DIR}/backends
  ${IMPLOT_DIR}
  "${CMAKE_SOURCE_DIR}/GUI/include"
  "${CMAKE_SOURCE_DIR}/Backend/include"
  "${CMAKE_SOURCE_DIR}/common/include/"
  ${nlohmann_json_INCLUDE_DIRS}
)

target_link_libraries(App PRIVATE glfw OpenGL::GL Boost::asio Boost::beast OpenSSL::SSL OpenSSL::Crypto nlohmann_json::nlohmann_json user32.lib gdi32.lib)

if (MSVC) 
  target_compile_options(App PRIVATE "/bigobj")
endif()

add_custom_command(
  TARGET App
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/GUI/fonts/JetBrainsMono-Regular.ttf" "${CMAKE_BINARY_DIR}/Debug/"
  COMMENT "Copying jet brains font to output directory"
)
