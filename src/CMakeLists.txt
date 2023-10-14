cmake_minimum_required(VERSION 3.12)

set(PROFLOG_SRCS proflog.c)
set(PROJECT_NAME "proflog")

project(${PROJECT_NAME} C CXX)
include_directories(./)
add_library(${PROJECT_NAME} ${PROFLOG_SRCS})