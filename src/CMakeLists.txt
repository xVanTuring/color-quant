aux_source_directory(. DIR_SRCS)

include_directories(../heap/src)

add_library(LibColorQuant ${DIR_SRCS})

target_link_libraries(LibColorQuant LibHeap)
