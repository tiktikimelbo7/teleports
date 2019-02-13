add_definitions(-DSRCDIR="${CMAKE_CURRENT_SOURCE_DIR}")
add_executable(qmltests "main.cpp" "${CMAKE_SOURCE_DIR}/app/qml/qml.qrc")
target_include_directories(qmltests PUBLIC
    ${CMAKE_SOURCE_DIR}/libs
    ${CMAKE_SOURCE_DIR}/libs/qtdlib
    ${CMAKE_SOURCE_DIR}/tests
)
target_link_libraries(qmltests Qt5::Core Qt5::Quick Qt5::QuickControls2 Qt5::Test Qt5::QuickTest quickflux QTdlib testrunner)

add_test(TeleportsQmlTests qmltests)

add_subdirectory(middleware)

