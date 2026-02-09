# Tests link to the application library (ambidb_app) defined in the root CMakeLists.txt

add_executable(app_tests test_app.cpp)
target_link_libraries(app_tests PRIVATE ambidb_app GTest::gtest_main)

enable_testing()
add_test(NAME AppTests COMMAND app_tests)
