# cmake_minimum_required(VERSION 3.16)
# project(test)

# 添加测试文件
add_executable(test_polynomial test_polynomial.cpp)
add_executable(test_ckksencoder test_ckksencoder.cpp)
add_executable(test_qpolynomial test_qpolynomial.cpp)
add_executable(test_polynomialsampler test_polynomialsampler.cpp)

# 添加链接库
# find_library(MY_LIB_FOUND my_lib)
# if(MY_LIB_FOUND)
#     message(STATUS "Found my_lib: ${MY_LIB_FOUND}")
# #    target_link_libraries(test_polynomial my_lib)
# else()
#     message(FATAL_ERROR "my_lib not found")
# endif()

# target_include_directories(test_polynomial PUBLIC "${MY_LIB_DIR}")
target_link_libraries(test_polynomial my_lib)
target_link_libraries(test_ckksencoder my_lib)
target_link_libraries(test_qpolynomial my_lib)
target_link_libraries(test_polynomialsampler my_lib)