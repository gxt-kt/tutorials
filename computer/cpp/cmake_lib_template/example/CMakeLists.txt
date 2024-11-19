add_executable(example example_main.cc)
target_include_directories(example PRIVATE
                            ${${PROJECT_NAME}_SOURCE_DIR}/include/mytest
)
target_link_libraries(example PRIVATE ${MY_LIB_NAME})
