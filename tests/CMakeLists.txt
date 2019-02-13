find_package(Qt5 COMPONENTS Test QuickTest REQUIRED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(testrunner SHARED "testrunner.cpp")
target_link_libraries(testrunner Qt5::Core Qt5::Quick Qt5::Test Qt5::QuickTest )

add_subdirectory(qml)
