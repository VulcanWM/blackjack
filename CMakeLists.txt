cmake_minimum_required(VERSION 3.20)
project(blackjack)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/W4 /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(blackjack
        main.cpp
        deck.cpp
        old_code/helpers.cpp
        game.cpp
)