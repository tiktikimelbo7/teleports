add_custom_target(copy-middleware-tests ALL
    COMMAND cmake -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS qmltests)
