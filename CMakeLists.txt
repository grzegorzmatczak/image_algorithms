cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(image_algorithms  VERSION 0.0  DESCRIPTION "image_algorithms Windows/Linux"  LANGUAGES C CXX)

set(TARGET_NAME "image_algorithms")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS_COMMON}")
if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -lX11 -Wall" )
endif (UNIX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")


if (WIN32)
    set(QT5_PATH  "D:\\Qt5\\5.15.2\\msvc2019_64")
    list(APPEND CMAKE_PREFIX_PATH ${QT5_PATH})
    set(OpenCV_DIR "D:\\repo\\opencv-4.10.0-build")
endif (WIN32)

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(OpenCV REQUIRED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
MESSAGE(STATUS "Include dirs ${OpenCV_INCLUDE_DIRS}")
MESSAGE(STATUS "LINK LIBRARIES ${OpenCV_LIBS}")
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories( "src" )
include_directories( "include" )

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


set(SRC
src/medianfilter.cpp
)

set(HEADERS
include/medianfilter.hpp
)

set(SRC_HEADERS_TEST
	gtest/gtest.cpp
    gtest/gtest_medianfilter.cpp
    gtest/gtest_medianfilter.hpp
)


include(GNUInstallDirs)

add_library(${PROJECT_NAME}_lib STATIC
    ${SRC} ${HEADERS})


target_link_libraries(${PROJECT_NAME}_lib Qt::Core Threads::Threads ${OpenCV_LIBS})

install(TARGETS ${PROJECT_NAME}_lib
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    
install(TARGETS ${PROJECT_NAME}_lib EXPORT ${PROJECT_NAME}Config
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

MESSAGE(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
MESSAGE(STATUS "CMAKE_INSTALL_LIBDIR: ${CMAKE_INSTALL_LIBDIR}")
MESSAGE(STATUS "CMAKE_INSTALL_BINDIR: ${CMAKE_INSTALL_BINDIR}")

add_executable(${PROJECT_NAME}_test ${SRC_HEADERS_TEST})
target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME}_lib gtest gmock)