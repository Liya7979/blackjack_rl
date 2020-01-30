cmake_minimum_required(VERSION 3.15)
project(blackjack)

set(CMAKE_CXX_STANDARD 17)

add_executable(blackjack main.cpp DefaultDict.h Blackjack.cpp Blackjack.h State.h State.cpp Strategy.cpp Strategy.h training.cpp training.h)