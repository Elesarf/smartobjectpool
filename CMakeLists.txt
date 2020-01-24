cmake_minimum_required(VERSION 3.5)

project(SmartPool)
option (test "Build tests" OFF)

SET(HEADERS
    UniquePool.h
    SharedPool.h
    SomeHeavyObj.h
)

SET(SOURCES
    main.cpp
)
set(TESTS
    ./test/sharedPool_test.cpp
    ./test/uniquePool_test.cpp
)

include_directories(${INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})


add_compile_options(-std=c++11 -g -fsanitize=address)
set(LINKER_ADDITIONAL_LIBRARIES asan)
set(LINKER_ADDITIONAL_FLAGS -fsanitize=address -g)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_link_libraries(
    ${PROJECT_NAME}
    ${LINKER_ADDITIONAL_LIBRARIES}
    ${LINKER_ADDITIONAL_FLAGS}
)

if (test)

find_package(GTest REQUIRED)
enable_testing()

add_executable(${PROJECT_NAME}Test ${TESTS})

target_link_libraries(
    ${PROJECT_NAME}Test
    gtest
    gtest_main
    pthread
    ${LINKER_ADDITIONAL_LIBRARIES}
    ${LINKER_ADDITIONAL_FLAGS}
)

endif ()
