cmake_minimum_required(VERSION 3.15)
project(Password_Generator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(pwg
    src/Random.cpp
    src/PasswordGenerator.cpp
    src/PasswordStrengthEvaluator.cpp
)

target_include_directories(pwg PUBLIC include)

add_executable(password_generator
    src/main.cpp
)

target_link_libraries(password_generator PRIVATE pwg)
