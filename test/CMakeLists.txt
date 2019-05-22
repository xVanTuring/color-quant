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
add_executable(TestQC main.cpp)
set_target_properties(TestQC
PROPERTIES CXX_STANDARD 17
)

include_directories(../src)


target_link_libraries(TestQC LibColorQuant)