message("installing node deps")
execute_process(COMMAND powershell -c "npm install"
WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/js
)

message("Generating Binary Test file")
execute_process(COMMAND powershell -c "node index.js"
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/js
                )
if(EXISTS "${CMAKE_SOURCE_DIR}/js/data.3.rgb")
  configure_file(${CMAKE_SOURCE_DIR}/js/data.3.rgb ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
endif()
if(EXISTS "${CMAKE_SOURCE_DIR}/js/data.4.rgb")
  configure_file(${CMAKE_SOURCE_DIR}/js/data.4.rgb ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
endif()
# if(EXISTS "${CMAKE_SOURCE_DIR}/js/small.3.rgb")
#   configure_file(${CMAKE_SOURCE_DIR}/js/small.3.rgb ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
# endif()
add_executable(TestQC main.c)
include_directories(../src)
include_directories(../heap/src)

target_link_libraries(TestQC LibColorQuant)