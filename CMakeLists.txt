cmake_minimum_required(VERSION 3.10)

project(imgui CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# build imgui and imguizmo
add_library(${PROJECT_NAME} STATIC imgui/imgui.cpp
                                   imgui/imgui_demo.cpp
                                   imgui/imgui_draw.cpp
                                   imgui/imgui_widgets.cpp
                                   ImGuizmo/ImGuizmo.cpp
)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_VISIBILITY_PRESET hidden)
set_property(TARGET ${PROJECT_NAME} PROPERTY VISIBILITY_INLINES_HIDDEN ON)
target_include_directories(${PROJECT_NAME} PRIVATE imgui ImGuizmo
                                           PUBLIC $<INSTALL_INTERFACE:include>)

install(FILES imgui/imgui.h
              imgui/imgui_internal.h
              imgui/imconfig.h
              imgui/imstb_rectpack.h
              imgui/imstb_textedit.h
              imgui/imstb_truetype.h
              ImGuizmo/ImGuizmo.h
              DESTINATION ${CMAKE_INSTALL_PREFIX}/include
)

install(TARGETS ${PROJECT_NAME}
    EXPORT IMGUIExport
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(EXPORT IMGUIExport 
    FILE ${PROJECT_NAME}Config.cmake 
    NAMESPACE ${PROJECT_NAME}:: 
    DESTINATION share/cmake/${PROJECT_NAME})
