cmake_minimum_required(VERSION 3.30)
project(Chess)

set(CMAKE_CXX_STANDARD 20)

# Указываем пути к заголовочным файлам
include_directories(${CMAKE_SOURCE_DIR})
include_directories(/opt/homebrew/include)

# Подключаем Boost
find_package(Boost REQUIRED COMPONENTS system filesystem)
include_directories(${Boost_INCLUDE_DIRS})

# Подключаем cpp-httplib
include_directories(/opt/homebrew/include/cpp-httplib)

# Добавляем файлы проекта
set(SOURCES
        Chess/main.cpp
        Bishop_Cell.cpp
        Cell.cpp
        Empty_Cell.cpp
        Game.cpp
        King_Cell.cpp
        Knight_Cell.cpp
        Manager.cpp
        Pawn_Cell.cpp
        Queen_Cell.cpp
        Rook_Cell.cpp
        Run.cpp
        Table.cpp
        Server_Interface.cpp
)

set(HEADERS
        Bishop_Cell.h
        Cell.h
        Empty_Cell.h
        Game.h
        King_Cell.h
        Knight_Cell.h
        Manager.h
        Pawn_Cell.h
        Queen_Cell.h
        Rook_Cell.h
        Run.h
        Table.h
        Server_Interface.h
)

# Создаём исполняемый файл
add_executable(Chess ${SOURCES} ${HEADERS})

# Линкуем Boost и cpp-httplib
target_link_libraries(Chess ${Boost_LIBRARIES})