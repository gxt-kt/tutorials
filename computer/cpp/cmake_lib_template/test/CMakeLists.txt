find_package(GTest REQUIRED)

add_executable(test_main test_main.cc test_add.cc)
target_include_directories(test_main PRIVATE
                            ${${PROJECT_NAME}_SOURCE_DIR}/include/mytest
)
target_link_libraries(test_main PRIVATE ${MY_LIB_NAME})
target_link_libraries(test_main PUBLIC gtest)
