find_package(Qt5 COMPONENTS Core Quick REQUIRED)

#"qtdclient.cpp" "qtdthread.cpp" "qml.qrc"
add_executable(${PROJECT_NAME} "main.cpp" "messagedelegatemap.cpp" "messagecontentdelegatemap.cpp" "qml/qml.qrc" "qml/icons/icons.qrc")
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Quick Qt5::QuickControls2 quickflux QTdlib)
target_compile_definitions(quickflux PRIVATE QUICK_FLUX_DISABLE_AUTO_QML_REGISTER=1)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/libs
    ${CMAKE_SOURCE_DIR}/libs/qtdlib
)
list(APPEND QML_DIRS "plugins")
set(QML_IMPORT_PATH "${QML_DIRS}")

