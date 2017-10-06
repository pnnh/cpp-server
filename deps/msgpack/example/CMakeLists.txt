
find_package(Boost COMPONENTS system thread REQUIRED)

LIST (APPEND exec_PROGRAMS
        demo.cpp
        streaming.cpp
        user_defined.cpp
        )

FOREACH (source_file ${exec_PROGRAMS})
    GET_FILENAME_COMPONENT (source_file_we ${source_file} NAME_WE)
    ADD_EXECUTABLE (
            ${source_file_we}
            ${source_file}
    )
        target_link_libraries(${source_file_we} ${Boost_LIBRARIES})
    TARGET_INCLUDE_DIRECTORIES (${source_file_we}
            PRIVATE
            $<TARGET_PROPERTY:msgpackc,INTERFACE_INCLUDE_DIRECTORIES>
            )
    target_include_directories(${source_file_we}
            PRIVATE
            ${Boost_INCLUDE_DIRS})

    IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        SET_PROPERTY (TARGET ${source_file_we} APPEND_STRING PROPERTY COMPILE_FLAGS "-Wall -Wextra -Werror -g -O3")
    ENDIF ()
ENDFOREACH ()
