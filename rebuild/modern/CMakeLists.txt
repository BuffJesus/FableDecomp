cmake_minimum_required(VERSION 3.25)

project(FableTLCModern LANGUAGES C CXX)

include(CTest)

add_subdirectory(multiplayer)
add_subdirectory(codec)
add_subdirectory(compat)
add_subdirectory(bank)
add_subdirectory(save)
add_subdirectory(texture)
