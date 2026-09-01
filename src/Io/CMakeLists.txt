cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(IO_SOURCES
        KeyboardController.cpp
        InputEnumFunctions.cpp
        KeyboardActionMapping.cpp
        KeyboardInputHandler.cpp
        Mouse.cpp)

set(IO_HEADERS
        KeyboardController.h
        InputEnums.h
        InputEnumFunctions.h
        KeyboardActionMapping.h
        KeyboardInputHandler.h
        Mouse.h)

add_library(io STATIC ${IO_SOURCES} ${IO_HEADERS})
target_link_libraries(io PUBLIC library_serialization)
if(ANDROID)
    # KeyboardInputHandler uses SDL's text-input API on Android to open/close the IME.
    target_link_libraries(io PRIVATE SDL3::SDL3OE)
endif()
target_check_style(io)

if(OE_BUILD_TESTS)
    set(TEST_IO_SOURCES Tests/InputEnumFunctions_ut.cpp)

    add_library(test_io OBJECT ${TEST_IO_SOURCES})
    target_link_libraries(test_io PUBLIC testing_unit io)

    target_check_style(test_io)

    target_link_libraries(OpenEnroth_UnitTest PUBLIC test_io)
endif()
