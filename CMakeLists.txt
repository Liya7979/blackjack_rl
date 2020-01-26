cmake_minimum_required(VERSION 3.15)
project(blackjack)

set(CMAKE_CXX_STANDARD 14)

add_executable(blackjack main.cpp Blackjack.cpp Blackjack.h strategy.cpp strategy.h training.cpp training.h)