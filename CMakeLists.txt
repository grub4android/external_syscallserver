cmake_minimum_required(VERSION 2.8) 
project(syscall_server)
set(CMAKE_BUILD_TYPE Release)

# main executable
add_executable(syscall_server "syscall_server.c")

# cflags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wunused -Wshadow -Wunreachable-code -Werror -static-libgcc -Wl,-static")
set(CMAKE_C_FLAGS_RELEASE "-O2")
set(CMAKE_C_FLAGS_DEBUG  "-g")

