cmake_minimum_required(VERSION 3.10)

# Set the project name
project(TP_Final_Tests)

# Enable testing
enable_testing()

# Add test executable
add_executable(run_tests test_main.cpp)

# Link libraries if needed
# target_link_libraries(run_tests your_library)

# Add tests
add_test(NAME ExampleTest COMMAND run_tests)