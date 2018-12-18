
aux_source_directory(./route_guide ROUTE_GUIDE_SRCS)

foreach(_target route_guide_client route_guide_server)
  add_executable(${_target} "route_guide/${_target}.cpp" "./route_guide/helper.cpp" ${GRPC_SRCS})
  target_link_libraries(${_target}
    ${PROTOBOF_LIBRARIES}
    ${GRPC_LIBRARIES} 
    ${GRPC_UNSECURE_LIBRARIES} 
    ${GRPCPP_UNSECURE_LIBRARIES} 
    ${GRPCPP_LIBRARIES})
endforeach()