find_package(Qt5 COMPONENTS Core Quick REQUIRED)

find_package(QuickFlux REQUIRED)

#"qtdclient.cpp" "qtdthread.cpp" "qml.qrc"
add_definitions(-DBUILD_VERSION="${BUILD_VERSION}" -DGIT_HASH="${GIT_HASH}")
add_executable(${PROJECT_NAME} "main.cpp" "messagedelegatemap.cpp" "messagecontentdelegatemap.cpp" "qml/qml.qrc" "qml/icons/icons.qrc")
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Quick Qt5::QuickControls2 QuickFlux::quickflux QTdlib)

list(APPEND QML_DIRS "plugins")
set(QML_IMPORT_PATH "${QML_DIRS}")
