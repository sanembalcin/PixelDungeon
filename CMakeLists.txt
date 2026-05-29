cmake_minimum_required(VERSION 3.15)
project(PixelDungeonOOP VERSION 1.0 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


file(GLOB_RECURSE SOURCES 
    "main.cpp"
    "src/*.cpp"
)


include_directories(include)


set(SFML_STATIC_LIBRARIES FALSE)
find_package(SFML 2.6 COMPONENTS graphics window system audio REQUIRED)


add_executable(game ${SOURCES})


target_link_libraries(game PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)


if(WIN32)
    add_custom_command(TARGET game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:game>/assets"
    )
endif()