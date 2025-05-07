add_executable(parser_example parser.cpp)
target_link_libraries(parser_example
    PUBLIC
        parser
)

target_compile_features(parser_example PUBLIC cxx_std_20)

install(
  TARGETS 
    parser_example
  #EXPORT
  #  gpu_voxels-targets
  LIBRARY
    DESTINATION lib
  FILE_SET HEADERS
    DESTINATION ${INCLUDE_DIR}
)