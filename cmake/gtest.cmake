include(ExternalProject)

ExternalProject_Add(
        googletest
        GIT_REPOSITORY      https://github.com/google/googletest.git
        GIT_TAG             master
        # TIMEOUT 10
        CMAKE_ARGS -DBUILD_SHARED_LIBS=TRUE
        # Disable install step
        INSTALL_COMMAND "")

# Specify include dir
ExternalProject_Get_Property(googletest source_dir)
set(GTEST_INCLUDE_DIR ${source_dir}/googletest/include )


# Library
ExternalProject_Get_Property(googletest binary_dir)

set(GTEST_LIBRARY_PATH ${binary_dir}/googlemock/gtest/${CMAKE_FIND_LIBRARY_PREFIXES}gtest.so )
set(GTEST_LIBRARY gtest )
add_library(${GTEST_LIBRARY} UNKNOWN IMPORTED)
set_property(TARGET ${GTEST_LIBRARY} PROPERTY IMPORTED_LOCATION
        ${GTEST_LIBRARY_PATH} )
add_dependencies(${GTEST_LIBRARY} googletest)

#
#set(GTEST_MAIN_LIBRARY_PATH ${binary_dir}/googlemock/gtest/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main.so PARENT_SCOPE)
#set(GTEST_MAIN_LIBRARY gtest_main PARENT_SCOPE)
#add_library(${GTEST_MAIN_LIBRARY} UNKNOWN IMPORTED)
#set_property(TARGET ${GTEST_MAIN_LIBRARY} PROPERTY IMPORTED_LOCATION
#        ${GTEST_MAIN_LIBRARY_PATH} )
#add_dependencies(${GTEST_MAIN_LIBRARY} googletest)