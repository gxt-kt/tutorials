set(header_path ${${PROJECT_NAME}_SOURCE_DIR}/include/mytest)
set(detail_header
    ${header_path}/detail/add.h
)
set(header
    ${header_path}/mytest.h
)
set(src
    mytest.cc
    detail/add.cc
)


# configure config file
configure_file("config.hpp.in" "${CMAKE_CURRENT_BINARY_DIR}/config_impl.hpp")

add_library(${MY_LIB_NAME} SHARED ${detail_header} ${header} ${src})
target_include_directories(${MY_LIB_NAME}
                             PUBLIC
                                $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include/mytest>
                                $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> # for generated files in build mode
                                $<INSTALL_INTERFACE:${MY_LIBRARY_INC_INSTALL_DIR}/mytest> # for client in install mode
                             PRIVATE
                                ${header_path} # for source files
)
target_compile_definitions(${MY_LIB_NAME} PUBLIC
                            MY_LIBRARY_VERSION=${MY_LIBRARY_VERSION}
                            MY_LIBRARY_VERSION_MAJOR=${MY_LIBRARY_VERSION_MAJOR}
                            MY_LIBRARY_VERSION_MINOR=${MY_LIBRARY_VERSION_MINOR}
                            MY_LIBRARY_VERSION_PATCH=${MY_LIBRARY_VERSION_PATCH}
)
# set add_library lib name
set_target_properties(${MY_LIB_NAME} PROPERTIES
                                       OUTPUT_NAME "${MY_LIB_NAME}.${MY_LIBRARY_VERSION}"
                                       POSITION_INDEPENDENT_CODE ON)

# if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
#     if("${CMAKE_CXX_SIMULATE_ID}" STREQUAL "MSVC")
#         target_compile_options(foonathan_memory PRIVATE /WX /W3 /D _CRT_SECURE_NO_WARNINGS)
#     else()
#         target_compile_options(foonathan_memory PRIVATE -pedantic-errors -Werror -Wall -Wextra -Wconversion -Wsign-conversion)
#     endif()
# elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
#     target_compile_options(foonathan_memory PRIVATE -pedantic-errors -Werror -Wall -Wextra -Wconversion -Wsign-conversion)
# elseif(MSVC)
#     target_compile_options(foonathan_memory PRIVATE /WX /W3 /D _CRT_SECURE_NO_WARNINGS)
# endif()

# install lib
install(TARGETS ${MY_LIB_NAME} EXPORT ${MY_LIB_NAME}_targets
        RUNTIME       DESTINATION ${MY_LIBRARY_RUNTIME_INSTALL_DIR}
        LIBRARY       DESTINATION ${MY_LIBRARY_LIBRARY_INSTALL_DIR}
        ARCHIVE       DESTINATION ${MY_LIBRARY_ARCHIVE_INSTALL_DIR}
        FRAMEWORK     DESTINATION ${MY_LIBRARY_FRAMEWORK_INSTALL_DIR}
)

# Write/install version file
include(CMakePackageConfigHelpers) # import cmake version file generator
# set version_file name
set(version_file "${CMAKE_CURRENT_BINARY_DIR}/cmake/${MY_LIB_NAME}-config-version.cmake")
# generate version_file
write_basic_package_version_file(${version_file}
                                 VERSION ${MY_LIBRARY_VERSION}
                                 COMPATIBILITY AnyNewerVersion)

# install files
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/config_impl.hpp DESTINATION ${MY_LIBRARY_INC_INSTALL_DIR}/${MY_LIB_NAME}/)
install(FILES ${header}                                   DESTINATION ${MY_LIBRARY_INC_INSTALL_DIR}/${MY_LIB_NAME})
install(FILES ${detail_header}                            DESTINATION ${MY_LIBRARY_INC_INSTALL_DIR}/${MY_LIB_NAME}/detail)
install(FILES ${version_file}                             DESTINATION ${MY_LIBRARY_CMAKE_CONFIG_INSTALL_DIR})
