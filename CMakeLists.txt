cmake_minimum_required(VERSION 3.28)
project(ex_3_b C)

set(CMAKE_C_STANDARD 99)

add_executable(ex_3_b
		linked_list.c
		markov_chain.c
		snakes_and_ladders.c)
