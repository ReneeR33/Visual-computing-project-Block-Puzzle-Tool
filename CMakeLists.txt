cmake_minimum_required(VERSION 3.14)

project(puzzle-tool)

set(CMAKE_CXX_STANDARD 20)

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
    ${LIB_DIR}/assimp/include/
    ${LIB_DIR}/tinyfiledialogs/include/
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
        ${LIB_DIR}/assimp/lib/windows/assimp-vc143-mt.lib
    )
endif (WIN32)

if (UNIX)
    set(LIBS 
        ${LIB_DIR}/glfw/lib_linux/glfw3.a
        ${LIB_DIR}/assimp/lib/linux/libassimp.so.5
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

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/shaders"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders"
)

# resources--------------------------------------------------------------

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources/"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources"
)

#----------------------------------------------------------------------

if (WIN32)
    file(GLOB_RECURSE SHARED_LIBS ${LIB_DIR} *.dll)

    foreach(SHARED_LIB ${SHARED_LIBS})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SHARED_LIB}
            $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    endforeach()
endif (WIN32)

if (UNIX)
    file(GLOB_RECURSE SHARED_LIBS ${LIB_DIR} *.so.*)

    foreach(SHARED_LIB ${SHARED_LIBS})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SHARED_LIB}
            $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    endforeach()
endif (UNIX)