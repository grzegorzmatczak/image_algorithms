cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(image_algorithms VERSION 0.0 DESCRIPTION "image_algorithms Windows/Linux" LANGUAGES C CXX)

set(TARGET_NAME "image_algorithms")
include(GNUInstallDirs)

if (MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "/MTd /")
    set(CMAKE_CXX_FLAGS_RELEASE "/MT ")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS_COMMON}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILE_OPTIONS}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

list(APPEND CMAKE_PREFIX_PATH ${Qt5_DIR})
find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

list(APPEND CMAKE_PREFIX_PATH ${utils2_DIR_CUSTOM})
find_package(utils2 REQUIRED)

list(APPEND CMAKE_PREFIX_PATH ${processing2_DIR_CUSTOM})
find_package(processing2 REQUIRED)

list(APPEND CMAKE_PREFIX_PATH ${gtest_DIR_CUSTOM})
find_package(GTest REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

list(APPEND CMAKE_PREFIX_PATH ${OpenCV_DIR})
find_package(OpenCV QUIET COMPONENTS core highgui imgproc imgcodecs videoio video tracking)
#find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories( "src" )
include_directories( "include" )

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

add_library(${PROJECT_NAME} STATIC
    ${SRC} ${HEADERS})


target_link_libraries(${PROJECT_NAME}
    Qt::Core
    Threads::Threads
    ${OpenCV_LIBS}
)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Config
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include)
install(EXPORT ${PROJECT_NAME}Config DESTINATION share/${PROJECT_NAME}/cmake)
export(TARGETS ${PROJECT_NAME} FILE ${PROJECT_NAME}Config.cmake)

add_executable(${PROJECT_NAME}_test ${SRC_HEADERS_TEST})
target_include_directories(${PROJECT_NAME}_test PUBLIC ${gtest_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME} GTest::gtest)

if (WIN32)
    get_target_property(QMAKE_EXE Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR "${QMAKE_EXE}" DIRECTORY)
    message(STATUS "QT_BIN_DIR:" ${QT_BIN_DIR})
    find_program(WINDEPLOYQT_ENV_SETUP qtenv2.bat HINTS "${QT_BIN_DIR}")
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${QT_BIN_DIR}")
endif(WIN32)

if (WIN32)
    set(PREFIX_BUILD_TYPE "")
    if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
		set(PREFIX_BUILD_TYPE "d")
    endif()
    
    set(CUSTOM_BIN_DIR "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
    add_custom_command ( TARGET ${PROJECT_NAME}_test
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${utils2_DIR_CUSTOM}lib/utils2.dll                   ${CUSTOM_BIN_DIR}  

        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT_BIN_DIR}/Qt5Test${PREFIX_BUILD_TYPE}.dll        ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT_BIN_DIR}/Qt5Gui${PREFIX_BUILD_TYPE}.dll         ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT_BIN_DIR}/Qt5Multimedia${PREFIX_BUILD_TYPE}.dll  ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT_BIN_DIR}/Qt5Network${PREFIX_BUILD_TYPE}.dll     ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT_BIN_DIR}/Qt5Core${PREFIX_BUILD_TYPE}.dll        ${CUSTOM_BIN_DIR}

        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_stitching${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll  ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_bgsegm${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll     ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_tracking${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll   ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_videoio${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll    ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_video${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll      ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_imgcodecs${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll  ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_imgproc${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll    ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_core${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll       ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_calib3d${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll    ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_features2d${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_flann${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll      ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_dnn${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll        ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_highgui${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll    ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_stitching${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll  ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_gapi${OPENCV_VERSION_WINDOWS}${PREFIX_BUILD_TYPE}.dll       ${CUSTOM_BIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_videoio_ffmpeg${OPENCV_VERSION_WINDOWS}_64.dll              ${CUSTOM_BIN_DIR}
    
    )
endif(WIN32)