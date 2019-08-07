find_package(Qt5Sql REQUIRED)

set(SOURCES
    auxdb.cpp
    avatarmaptable.cpp
)

add_library(auxdb STATIC ${SOURCES} )
target_link_libraries(auxdb Qt5::Core Qt5::Sql)
link_directories(${CURRENT_TDLIB_BUILD_DIR}/)
