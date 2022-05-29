project(tcpserver)

set(GLAD_INC_DIR    "${CMAKE_CURRENT_SOURCE_DIR}/../../Vender/glad/include")
set(IMGUI_INC_DIR   "${CMAKE_CURRENT_SOURCE_DIR}/../../Vender/imgui")
set(IMPLOT_INC_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/../../Vender/implot")
set(UI_INC_DIR      "${CMAKE_CURRENT_SOURCE_DIR}/../uilibrary")
set(TCPNODE_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../tcpnode")

set(PROJECT_SRC_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/")

include_directories(
    ${PROJECT_SRC_ROOT} 
    ${GLAD_INC_DIR}
    ${IMGUI_INC_DIR}
    ${IMPLOT_INC_DIR}
    ${UI_INC_DIR}
    ${TCPNODE_INC_DIR}
)

file(GLOB PROJECT_FILES_HEADER 
    "${PROJECT_SRC_ROOT}/*.h"
    "${PROJECT_SRC_ROOT}/*/*.h"
)
file(GLOB PROJECT_FILES_SOURCE 
    "${PROJECT_SRC_ROOT}/*.cpp"
    "${PROJECT_SRC_ROOT}/*/*.cpp"
)

add_executable(${PROJECT_NAME}
    ${PROJECT_FILES_HEADER}
    ${PROJECT_FILES_SOURCE}
)

target_link_libraries(${PROJECT_NAME}
    ui
    tcpnode
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/Debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/Release
)
