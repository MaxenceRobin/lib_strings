cmake_minimum_required(VERSION 3.16.3)
project(libstrings)

# Variables --------------------------------------------------------------------

set(TARGET_NAME strings)

set(SOURCES
        src/libstrings.c
)

set(PUBLIC_HEADERS
        include/public/libstrings.h
)

set(INSTALL_DIR
        bin
)

# Configuration ----------------------------------------------------------------

add_library(${TARGET_NAME} SHARED ${SOURCES})
target_include_directories(${TARGET_NAME} PUBLIC include/public)

set_target_properties(${TARGET_NAME}
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${INSTALL_DIR}
        ARCHIVE_OUTPUT_DIRECTORY ${INSTALL_DIR}
        C_STANDARD 11
)
