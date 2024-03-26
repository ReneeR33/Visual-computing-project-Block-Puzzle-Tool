cmake_minimum_required(VERSION 3.14)

project(puzzle-tool)

set(CMAKE_CXX_STANDARD 20)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-Wall -Werror -Wextra")
endif ()

# libraries------------------------------------------------------------

set(LIB_DIR ${PROJECT_SOURCE_DIR}/libs)

include_directories(
    ${LIB_DIR}/glfw/include
    ${LIB_DIR}/glad/include
    ${LIB_DIR}/glm/include
    ${LIB_DIR}/stb/include
    ${LIB_DIR}/imgui/include
    ${LIB_DIR}/entt/include
    ${LIB_DIR}/nlohmann_json/include/
    ${LIB_DIR}/tinyfiledialogs/include
)

file(GLOB_RECURSE LIB_sources CONFIGURE_DEPENDS
    "${LIB_DIR}/glad/src/*.cpp"
    "${LIB_DIR}/glad/src/*.c"
    "${LIB_DIR}/imgui/src/*.cpp"
    "${LIB_DIR}/imgui/src/*.c"
)

if (WIN32)
    set(LIBS 
        ${LIB_DIR}/GLFW/lib/glfw3.lib
    )
endif (WIN32)

if (UNIX)
    set(LIBS 
        ${LIB_DIR}/glfw/lib_linux/glfw3.a
    )
endif (UNIX)

#----------------------------------------------------------------------

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*.c"
)

add_executable(${PROJECT_NAME} ${sources} ${LIB_sources})

target_link_libraries(${PROJECT_NAME} ${LIBS})

# shaders--------------------------------------------------------------

file(GLOB_RECURSE GLSL_SOURCE_FILES
    "shaders/*.frag"
    "shaders/*.vert"
)

add_custom_target(
    ${PROJECT_NAME}-shaders 
    DEPENDS ${GLSL_SOURCE_FILES}
)

add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}-shaders)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/shaders"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders"
)

# resources--------------------------------------------------------------

file(GLOB_RECURSE RESOURCE_FILES ${PROJECT_SOURCE_DIR} "*")

add_custom_target(
        ${PROJECT_NAME}-resources
        DEPENDS ${RESOURCE_FILES}
)

add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}-resources)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources/"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources"
)

#----------------------------------------------------------------------