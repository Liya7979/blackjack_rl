cmake_minimum_required(VERSION 3.15)
project(blackjack)

set(CMAKE_CXX_STANDARD 17)

add_executable(blackjack src/main.cpp include/DefaultDict.h src/Blackjack.cpp include/Blackjack.h include/State.h src/State.cpp src/Strategy.cpp include/Strategy.h src/training.cpp include/training.h)