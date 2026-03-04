cmake_minimum_required(VERSION 4.1)
project(blackjack)

set(CMAKE_CXX_STANDARD 20)

add_executable(blackjack
        main.cpp
        deck.cpp
        old_code/helpers.cpp
        deck.h
        game.cpp
        game.h
)
