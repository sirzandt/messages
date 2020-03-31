cmake_minimum_required(VERSION 3.0.0)
project(Messages VERSION 0.1.0)

#include(CTest)
#enable_testing()

add_executable(Messages Messages.cpp)
add_library(MessageBase Src/MessageBase.cpp Src/Keys.cpp Src/Envelope.cpp)
target_include_directories(MessageBase PUBLIC ExternalLib/nlohmann/)
target_include_directories(MessageBase PUBLIC ExternalLib/openssl-1.1.1e/include/)

target_link_libraries(Messages PUBLIC MessageBase)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

project(UnitTesting)
#add_library(UnitTesting INTERFACE)
add_executable(UnitTesting Test/MessageBaseTest.cpp Test/KeyTest.cpp Test/EnvelopeTest.cpp)
target_include_directories(UnitTesting PUBLIC "ExternalLib/Catch2/")
target_include_directories(UnitTesting PUBLIC Src/)
target_include_directories(UnitTesting PUBLIC ExternalLib/nlohmann/)
target_include_directories(UnitTesting PUBLIC ExternalLib/openssl-1.1.1e/include/)
target_link_libraries(UnitTesting PUBLIC pthread dl MessageBase ${PROJECT_SOURCE_DIR}/ExternalLib/openssl-1.1.1e/libcrypto.a)
