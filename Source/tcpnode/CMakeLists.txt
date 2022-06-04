project(tcpnode)
    
set(GLAD_INC_DIR    "${CMAKE_CURRENT_SOURCE_DIR}/../../Vender/glad/include")
set(IMGUI_INC_DIR   "${CMAKE_CURRENT_SOURCE_DIR}/../../Vender/imgui")
set(IMPLOT_INC_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/../../Vender/implot")
set(UI_INC_DIR      "${CMAKE_CURRENT_SOURCE_DIR}/../uilibrary")

set(PROJECT_SRC_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/")

include_directories(
    ${PROJECT_SRC_ROOT} 
    ${GLAD_INC_DIR}
    ${IMGUI_INC_DIR}
    ${IMPLOT_INC_DIR}
    ${UI_INC_DIR}
)

# set(SERVER_TEST "${CMAKE_CURRENT_SOURCE_DIR}/server_test.cpp")
# set(CLIENT_TEST "${CMAKE_CURRENT_SOURCE_DIR}/client_test.cpp")

file(GLOB PROJECT_FILES_HEADER 
    "${PROJECT_SRC_ROOT}/*.h"
    "${PROJECT_SRC_ROOT}/*/*.h"
)
file(GLOB PROJECT_FILES_SOURCE 
    "${PROJECT_SRC_ROOT}/*.cpp"
    "${PROJECT_SRC_ROOT}/*/*.cpp"
)

add_library(${PROJECT_NAME} STATIC
    ${PROJECT_FILES_HEADER} 
    ${PROJECT_FILES_SOURCE}
)

target_link_libraries(${PROJECT_NAME}
    ui
)

# add_executable("${PROJECT_NAME}_server"
#     ${SERVER_TEST}
# )
# target_link_libraries("${PROJECT_NAME}_server"
#     ui
#     tcpnode
# )
# set_target_properties("${PROJECT_NAME}_server"
#     PROPERTIES 
#     RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/Debug
#     RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/Release
# )

# add_executable("${PROJECT_NAME}_client"
#     ${CLIENT_TEST}
# )
# target_link_libraries("${PROJECT_NAME}_client"
#     ui
#     tcpnode
# )
# set_target_properties("${PROJECT_NAME}_client"
#     PROPERTIES 
#     RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/Debug
#     RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/Release
# )
