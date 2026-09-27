cmake_minimum_required(VERSION 4.2)
project(Practica_1)

set(CMAKE_CXX_STANDARD 20)

file(GLOB MY_FILES *.cpp)
file(GLOB MY_HEADERS *.h)
file(GLOB GLAD_FILES GLAD/src/*.c)
file(GLOB IMGUI_FILES imgui/src/*.cpp)

add_executable(${PROJECT_NAME} ${MY_FILES}
                               ${MY_HEADERS}
                               ${GLAD_FILES}
                               ${IMGUI_FILES}
)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIR}
                                      PRIVATE glfw
                                      PRIVATE ${OPENGL_LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC GLAD/include
                                                  imgui/include)