project(ui)

set(RESOURCE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../Resources/")

set(GLAD_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../Vender/glad/include")
set(GLFW_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../Vender/glfw/include")
set(IMGUI_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../Vender/imgui")
set(IMPLOT_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../Vender/implot")

set(PROJECT_SRC_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/")

include_directories(
    ${PROJECT_SRC_ROOT} 
    ${GLAD_INC_DIR}
    ${GLFW_INC_DIR}
    ${IMGUI_INC_DIR}
    ${IMPLOT_INC_DIR}
)

# set(TEST_FILE "${PROJECT_SRC_ROOT}/main.cpp")

file(GLOB PROJECT_FILES_HEADER 
    "${PROJECT_SRC_ROOT}/*.h"
    "${PROJECT_SRC_ROOT}/*/*.h"
)
file(GLOB PROJECT_FILES_SOURCE 
    "${PROJECT_SRC_ROOT}/*.cpp"
    "${PROJECT_SRC_ROOT}/*/*.cpp"
)

add_library(${PROJECT_NAME}
    STATIC
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

target_compile_definitions(${PROJECT_NAME} PUBLIC
    RESOURCE_ROOT="${RESOURCE_ROOT}" 
)

# add_executable("${PROJECT_NAME}_test"
#     ${TEST_FILE}
# )

# target_link_libraries("${PROJECT_NAME}_test"
#     ui
# )
