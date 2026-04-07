cmake_minimum_required(VERSION 3.10)
project(DISTRIBUIDOS C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

# Biblioteca libclaves.so (implementación local)
add_library(claves SHARED claves.c claves.h
        common.c
        common.h)

# Biblioteca libproxyclaves.so (implementación con sockets)
add_library(proxyclaves SHARED proxy-sock.c claves.h)

# Ejecutable del servidor
add_executable(servidor servidor-sock.c)
target_link_libraries(servidor claves)

# Ejecutable del cliente (usa proxy)
add_executable(cliente app-cliente.c)
target_link_libraries(cliente proxyclaves)