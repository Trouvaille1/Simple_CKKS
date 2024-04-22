cmake_minimum_required(VERSION 3.16)
project(Simple_CKKS)

set(CMAKE_CXX_STANDARD 17)

set(EIGEN_DIR "/home/hanggu/USTC/Simple_CKKS/eigen-3.4.0")
if(NOT EXISTS ${EIGEN_DIR})
    message(FATAL_ERROR "Please check that the set Eigen directory is valid!")
endif()

# 添加头文件路径
include_directories(${EIGEN_DIR})
include_directories(".")

# Add executable target
#add_executable(${PROJECT_NAME} ${SOURCE_FILES})

#生成静态库.helper.h是内联函数头文件，Polynomial.h是模板类，可以不用写进静态库中
add_library(my_lib CKKSEncoder.cpp QPolynomial.cpp PolynomialSampler.cpp Context.cpp Keygen.cpp CKKSEncryptor.cpp CKKSDecryptor.cpp)

add_executable(Simple_CKKS main.cpp)
target_link_libraries(Simple_CKKS my_lib)


add_subdirectory(test)
