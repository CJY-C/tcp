cmake_minimum_required(VERSION 3.16) 

project(tcp-udp)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(./Vender)
add_subdirectory(./Source)

add_custom_target(
    copy-compile-commands ALL
    ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_BINARY_DIR}/../
    )


# set(GLAD_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Vender/glad/include")
# set(GLFW_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Vender/glfw/include")
# set(IMGUI_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Vender/imgui")
# set(IMPLOT_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Vender/implot")

# # set(IMPLOT_INC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Vender/implot/*h")
# # set(IMPLOT_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Vender/implot/*.cpp")
    

# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# add_custom_target(
#     copy-compile-commands ALL
#     ${CMAKE_COMMAND} -E copy_if_different
#         ${CMAKE_BINARY_DIR}/compile_commands.json
#         ${CMAKE_BINARY_DIR}/../
#     )

# set(PROJECT_SRC_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/Source")

# include_directories(
#     ${PROJECT_SRC_ROOT} 
#     ${GLAD_INC_DIR}
#     ${GLFW_INC_DIR}
#     ${IMGUI_INC_DIR}
#     ${IMPLOT_INC_DIR}
# )

# file(GLOB PROJECT_FILES_HEADER 
#     "${PROJECT_SRC_ROOT}/*.h"
#     "${PROJECT_SRC_ROOT}/*/*.h"
#     # "${IMPLOT_INC_FILES}"
# )
# file(GLOB PROJECT_FILES_SOURCE 
#     "${PROJECT_SRC_ROOT}/*.cpp"
#     "${PROJECT_SRC_ROOT}/*/*.cpp"
#     # "${IMPLOT_SRC_FILES}"
# )

# add_executable(${PROJECT_NAME}
#     ${PROJECT_FILES_HEADER} 
#     ${PROJECT_FILES_SOURCE}
# )

# target_link_libraries(
#     ${PROJECT_NAME} 
#     glad
#     glfw
#     imgui
#     implot
# )
