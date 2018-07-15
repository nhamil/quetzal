cmake_minimum_required(VERSION 2.8) 
project(Qz C) 

set(APP_NAME quetzal) 

set(SOURCE_FOLDER Source) 

set(SOURCES 
    ${SOURCE_FOLDER}/main.c 

    ${SOURCE_FOLDER}/Qz/QzBlock.c 
    ${SOURCE_FOLDER}/Qz/QzBuffer.c 
    ${SOURCE_FOLDER}/Qz/QzEnv.c 
    ${SOURCE_FOLDER}/Qz/QzError.c 
    ${SOURCE_FOLDER}/Qz/QzMemory.c 
    ${SOURCE_FOLDER}/Qz/QzObject.c 
    ${SOURCE_FOLDER}/Qz/QzOp.c 
    ${SOURCE_FOLDER}/Qz/QzScanner.c 
    ${SOURCE_FOLDER}/Qz/QzTable.c 
    ${SOURCE_FOLDER}/Qz/QzVM.c 
)

include_directories(Include) 
include_directories(Source) 

set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG} -Wall -Wextra -g -std=c99 -lm") 

set(CMAKE_C_FLAGS_RELEASE     "${CMAKE_C_FLAGS_RELEASE} -Wall -std=c99 -lm")

add_executable(${APP_NAME} ${SOURCES})
