add_executable(cpp-test
    test.cpp
    main.cpp
)

target_include_directories(cpp-test PUBLIC
  # list of include directories:
  ${CMAKE_SOURCE_DIR}/include
  )

target_link_libraries(cpp-test
    PUBLIC
    GTest::gtest_main
    gmock
    ${OpenCV_LIBS}
    perception_task
)

# Define the project root directory
target_compile_definitions(cpp-test PRIVATE
    PROJECT_ROOT="${PROJECT_SOURCE_DIR}"
)

# Copy YOLO files to build directory
add_custom_command(TARGET cpp-test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/yolo_classes
        ${CMAKE_BINARY_DIR}/yolo_classes
)

gtest_discover_tests(cpp-test)