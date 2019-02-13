if (WIN32)
    message("installing node deps")
    execute_process(COMMAND powershell -c "npm install"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/script
    )
    message("Generating Binary Test file")
    execute_process(COMMAND powershell -c "node index.js"
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/script
                    )
else()
    message("installing node deps")
    execute_process(COMMAND bash -c "npm install"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/script
    )
    message("Generating Binary Test file")
    execute_process(COMMAND bash -c "node index.js"
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/script
                    )
endif ()
add_executable(TestQC main.c)
include_directories(../src)
include_directories(../heap/src)

target_link_libraries(TestQC LibColorQuant)