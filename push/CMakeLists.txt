find_package(Qt5Gui REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5DBus REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(SOURCES
    push.cpp
    pushclient.cpp
    pushhelper.cpp)

add_executable(push ${SOURCES} )

qt5_use_modules(push Widgets Core Network DBus )

install(FILES push-apparmor.json DESTINATION /push)
install(FILES push-helper.json DESTINATION /push)
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/push DESTINATION /push)
