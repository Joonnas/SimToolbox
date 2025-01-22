target_sources(SimToolbox PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Gui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Camera.cpp
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/gui_elements)