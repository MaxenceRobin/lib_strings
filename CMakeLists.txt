cmake_minimum_required(VERSION 3.16.3)
project(lib_strings)

# Variables --------------------------------------------------------------------

set(TARGET_NAME strings)

set(SOURCES
        private/lib_strings.c
)

set(PUBLIC_HEADERS
        public
)

set(INSTALL_DIR
        bin
)

# Configuration ----------------------------------------------------------------

add_library(${TARGET_NAME} SHARED ${SOURCES})
target_include_directories(${TARGET_NAME} PUBLIC ${PUBLIC_HEADERS})

set_target_properties(${TARGET_NAME}
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${INSTALL_DIR}
        ARCHIVE_OUTPUT_DIRECTORY ${INSTALL_DIR}
        C_STANDARD 11
)
