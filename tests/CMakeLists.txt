find_package(Qt5 COMPONENTS Test QuickTest REQUIRED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(testrunner SHARED "testrunner.cpp" "testutils.cpp" "clientwrapper.cpp" "testing.qrc")
target_link_libraries(testrunner Qt5::Core Qt5::Quick Qt5::Test Qt5::QuickTest QTdlib)
target_include_directories(testrunner PUBLIC
    ${CMAKE_SOURCE_DIR}/libs
    ${CMAKE_SOURCE_DIR}/libs/qtdlib
)

add_subdirectory(qml)
