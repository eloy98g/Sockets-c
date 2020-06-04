CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(redes_1)
add_executable(servidor Servidor.cpp usuario.cpp registro.cpp Tablero.cpp Partida.cpp)
add_executable(cliente Cliente.cpp)
