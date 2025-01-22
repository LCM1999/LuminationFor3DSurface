cmake_minimum_required(VERSION 3.24.2)
project(xshade LANGUAGES CXX)

include("D:/vcpkg/scripts/buildsystems/vcpkg.cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(COMPUTE_CAPABILITY 75)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${EIGEN3_DIR})

find_package(VTK REQUIRED)
find_package(OpenMP REQUIRED)
if (NOT OpenMP_FOUND)
    message(FATAL_ERROR "OpenMP not found!")
endif ()

find_package(Eigen3 REQUIRED)
if (NOT EIGEN3_FOUND)
    message(FATAL_ERROR "Eigen3 not found!")
endif ()

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options("-O3")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_compile_options("-openmp:llvm")
endif ()

file(GLOB_RECURSE RT_SRC_FILES CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/ExaggeratedShadingInteractive.cpp")
file(GLOB_RECURSE RT_HEADER_FILES CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/*.h")

add_executable(xshade
        ${RT_SRC_FILES}
        ${RT_HEADER_FILES}
)

target_include_directories(xshade
        PRIVATE
        ${RT_HEADER_FILES}
        ${EIGEN3_INCLUDE_DIR}
)

#add_library(pugixml
#        STATIC
#        srcShonDy/3rdparty/pugixml/pugixml.hpp
#)

target_link_libraries(xshade
        PRIVATE
        ${VTK_LIBRARIES}
        $<$<BOOL:${WIN32}>:Shlwapi.lib>
        $<$<BOOL:${WIN32}>:Iphlpapi.lib>
        Eigen3::Eigen
        OpenMP::OpenMP_CXX
        glad::glad
        glm::glm
        imgui::imgui
        glfw
)

# if (WIN32)
#     target_link_libraries(${PROJECT_NAME}
#             PRIVATE
#             "${PROJECT_SOURCE_DIR}/srcShonDy/shonDyCommon/thirdParty/bitanswer/00003799_00000131_md_x64.lib"
#     )
# elseif (UNIX)
#     target_link_libraries(${PROJECT_NAME}
#             PRIVATE
#             "${PROJECT_SOURCE_DIR}/srcShonDy/shonDyCommon/thirdParty/bitanswer/lib00003799_00000131_x64.a"
#     )
# endif ()

