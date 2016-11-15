cmake_minimum_required(VERSION 3.0)

PROJECT(test_)

SET(CMAKE_C_FLAGS "-std=c99 -g")

AUX_SOURCE_DIRECTORY(. DIR_SRC)

ADD_EXECUTABLE(test_ ${DIR_SRC})
