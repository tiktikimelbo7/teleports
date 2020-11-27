find_package(Qt5 COMPONENTS Core Quick REQUIRED)

include(ExternalProject)

ExternalProject_Add(QuickFlux
	PREFIX "${PROJECT_BINARY_DIR}/QuickFlux-build"
	SOURCE_DIR "$ENV{SRC_DIR}/libs/quickflux"
	CMAKE_ARGS "-DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}"
                    "-DCMAKE_INSTALL_PREFIX=${PROJECT_BINARY_DIR}/QuickFlux"
                    "-DCMAKE_INSTALL_LIBDIR=${PROJECT_BINARY_DIR}/QuickFlux/lib"
                    "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
		    "-DCMAKE_CXX_FLAGS=-DQUICK_FLUX_DISABLE_AUTO_QML_REGISTER=1"
)

link_directories("${PROJECT_BINARY_DIR}/QuickFlux/lib")

#"qtdclient.cpp" "qtdthread.cpp" "qml.qrc"
add_definitions(-DBUILD_VERSION="${BUILD_VERSION}" -DGIT_HASH="${GIT_HASH}")
add_executable(${PROJECT_NAME} "main.cpp" "messagedelegatemap.cpp" "messagecontentdelegatemap.cpp" "qml/qml.qrc" "qml/icons/icons.qrc")
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Quick Qt5::QuickControls2 quickflux$<$<CONFIG:DEBUG>:d> QTdlib)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/libs
    ${CMAKE_SOURCE_DIR}/libs/qtdlib
)
list(APPEND QML_DIRS "plugins")
set(QML_IMPORT_PATH "${QML_DIRS}")

target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_BINARY_DIR}/QuickFlux/include/quickflux")
