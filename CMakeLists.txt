cmake_minimum_required(VERSION 3.23)
include(GenerateExportHeader)
include(GNUInstallDirs)

project(universal-pddl-parser
    LANGUAGES CXX)

add_subdirectory(parser)

add_subdirectory(examples)

enable_testing()
add_subdirectory(tests)