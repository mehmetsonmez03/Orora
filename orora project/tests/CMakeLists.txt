# ORORA - Tests CMakeLists.txt
cmake_minimum_required(VERSION 3.20)

# Test executable
enable_testing()

find_package(GTest QUIET)
if(NOT GTest_FOUND)
    message(STATUS "GoogleTest not found, using simple tests")
endif()

# Test sources
set(TEST_SOURCES
    test_voice_system.cpp
    test_metahuman.cpp
    test_room.cpp
    test_appearance.cpp
    test_integration.cpp
    test_field_theory.cpp
)

# Compile each test
foreach(test_source ${TEST_SOURCES})
    get_filename_component(test_name ${test_source} NAME_WE)
    
    add_executable(${test_name} ${test_source})
    
    target_link_libraries(${test_name} PRIVATE
        Qt6::Core
        Qt6::Test
    )
    
    if(GTest_FOUND)
        target_link_libraries(${test_name} PRIVATE GTest::GTest GTest::Main)
    endif()
    
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
