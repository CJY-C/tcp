project(demo)

set(GLAD_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../Vender/glad/include")
set(GLFW_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../Vender/glfw/include")
set(IMGUI_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../Vender/imgui")
set(IMPLOT_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../Vender/implot")

# set(IMPLOT_INC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Vender/implot/*h")
# set(IMPLOT_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Vender/implot/*.cpp")
    
set(PROJECT_SRC_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/")

include_directories(
    ${PROJECT_SRC_ROOT} 
    ${GLAD_INC_DIR}
    ${GLFW_INC_DIR}
    ${IMGUI_INC_DIR}
    ${IMPLOT_INC_DIR}
)

file(GLOB PROJECT_FILES_HEADER 
    "${PROJECT_SRC_ROOT}/*.h"
    "${PROJECT_SRC_ROOT}/*/*.h"
    # "${IMPLOT_INC_FILES}"
)
file(GLOB PROJECT_FILES_SOURCE 
    "${PROJECT_SRC_ROOT}/*.cpp"
    "${PROJECT_SRC_ROOT}/*/*.cpp"
    # "${IMPLOT_SRC_FILES}"
)

add_executable(${PROJECT_NAME}
    ${PROJECT_FILES_HEADER} 
    ${PROJECT_FILES_SOURCE}
)

target_link_libraries(
    ${PROJECT_NAME} 
    glad
    glfw
    imgui
    implot
)

