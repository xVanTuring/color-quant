add_executable(TestQC main.c)

include_directories(../src)
include_directories(../heap/src)

target_link_libraries(TestQC LibColorQuant)