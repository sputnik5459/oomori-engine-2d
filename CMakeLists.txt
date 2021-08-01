set(PROJECT_NAME oomori-engine-2d)
set(PROJECT_NAME_TEST test-oomori-engine-2d)

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} src/main.cpp)

add_executable(${PROJECT_NAME_TEST} src/test.cpp)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
add_subdirectory(external/glad)

target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)

target_link_libraries(${PROJECT_NAME_TEST} glfw)
target_link_libraries(${PROJECT_NAME_TEST} PRIVATE glad)
