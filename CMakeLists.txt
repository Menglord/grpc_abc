cmake_minimum_required(VERSION 3.5)
project(grpc_test)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -march=native -O3 -pthread" )
set(BINARY_DIR ${PROJECT_SOURCE_DIR}/devel)

include_directories(./src)
add_subdirectory(./src)

find_package(PkgConfig REQUIRED)

pkg_check_modules(PROTOBOF REQUIRED protobuf)
pkg_check_modules(GRPC REQUIRED grpc)
pkg_check_modules(GRPC_UNSECURE REQUIRED grpc_unsecure)
pkg_check_modules(GRPCPP_UNSECURE REQUIRED grpc++_unsecure)
pkg_check_modules(GRPCPP REQUIRED grpc++)

include_directories(${PROTOBOF_INCLUDE_DIRS})
include_directories(${GRPC_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

link_directories(${GRPC_LIBRARY_DIRS})

FILE(GLOB_RECURSE PROTO_FILES ${PROJECT_SOURCE_DIR}/protos *.proto)

set(GRPC_SRCS "")
set(GRPC_HRDS "")
set(_PROTOBUF_PROTOC "/usr/local/bin/protoc")
set(_GRPC_CPP_PLUGIN_EXECUTABLE "/usr/local/bin/grpc_cpp_plugin")

foreach(FIL ${PROTO_FILES})
    get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
    get_filename_component(FIL_WE ${FIL} NAME_WE)
    get_filename_component(FIL_DIR ${ABS_FIL} DIRECTORY)
    get_filename_component(FIL_PATH ${FIL} PATH)

    set(proto_h "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.h")
    set(proto_cc "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.cc")
    set(grpc_h "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.grpc.pb.h")
    set(grpc_cc "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.grpc.pb.cc")

    LIST(APPEND GRPC_HRDS ${proto_h})
    LIST(APPEND GRPC_HRDS ${grpc_h})
    LIST(APPEND GRPC_SRCS ${proto_cc})
    LIST(APPEND GRPC_SRCS ${grpc_cc})
    
    add_custom_command(
        OUTPUT "${proto_h}" "${grpc_h}" "${proto_cc}" "${grpc_cc}"
        COMMAND echo "AAAAAA"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
             --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
             -I "${FIL_PATH}"
             --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}" "${ABS_FIL}"
        DEPENDS "${ABS_FIL}")

endforeach()
 
set_source_files_properties(${${GRPC_SRCS}} ${${GRPC_HRDS}} PROPERTIES GENERATED TRUE)

foreach(_target greeter_client greeter_server)
  add_executable(${_target} "src/${_target}.cpp" ${GRPC_SRCS})
  target_link_libraries(${_target}
    ${PROTOBOF_LIBRARIES}
    ${GRPC_LIBRARIES} 
    ${GRPC_UNSECURE_LIBRARIES} 
    ${GRPCPP_UNSECURE_LIBRARIES} 
    ${GRPCPP_LIBRARIES})
endforeach()