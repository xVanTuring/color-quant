aux_source_directory(. DIR_SRCS)

add_library(LibColorQuant ${DIR_SRCS})
set_target_properties(LibColorQuant
PROPERTIES CXX_STANDARD 17
)

