
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(
    imgui STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui/imgui.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui/imgui_impl_glfw.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui/imgui_impl_opengl3.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui/imgui_impl_opengl3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui/imgui_impl_opengl3_loader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui/imconfig.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui/imgui_demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui/imgui_internal.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui/imstb_rectpack.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui/imstb_textedit.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui/imstb_truetype.h
)

add_library(
    aMath STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/aMath/a_math.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/aMath/a_math.cpp
)


target_link_libraries(SimToolbox ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3.lib)
target_link_libraries(SimToolbox ${CMAKE_CURRENT_SOURCE_DIR}/lib/glew32s.lib)
target_link_libraries(SimToolbox imgui)
target_link_libraries(SimToolbox aMath)

find_package(OpenGL REQUIRED)
target_link_libraries(SimToolbox OpenGL::GL)