set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/external/nanopb/extra)
find_package(Nanopb REQUIRED)

nanopb_generate_cpp(TARGET proto example.proto)


target_link_libraries(proto)