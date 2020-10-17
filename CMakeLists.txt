cmake_minimum_required(VERSION 3.0)

project(
    vmu931
)

add_library(vmu931 vmu931.c)
install(TARGETS vmu931 DESTINATION lib)
install(FILES vmu931.h DESTINATION include)

add_executable(vmu931-calibrate examples/vmu931-calibrate.c)
target_link_libraries(vmu931-calibrate vmu931)

add_executable(vmu931-estimate-bias examples/vmu931-estimate-bias.c)
target_link_libraries(vmu931-estimate-bias vmu931)

add_executable(vmu931-euler examples/vmu931-euler.c)
target_link_libraries(vmu931-euler vmu931)

add_executable(vmu931-read-all examples/vmu931-read-all.c)
target_link_libraries(vmu931-read-all vmu931)


