cmake_minimum_required(VERSION 3.30)
project(Chess)

set(CMAKE_CXX_STANDARD 20)

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
        Server_Manager.cpp
        My_Blocking_Queue.cpp
        My_Blocking_Queue.h
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
        Server_Manager.h
        My_Blocking_Queue.cpp
        My_Blocking_Queue.h
)

# Создаём исполняемый файл
add_executable(Chess ${SOURCES} ${HEADERS})

# Указываем пути к заголовочным файлам
target_include_directories(Chess PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(Chess PRIVATE /opt/homebrew/include)


# Подключаем Boost
find_package(Boost REQUIRED COMPONENTS system filesystem)
target_include_directories(Chess PRIVATE ${Boost_INCLUDE_DIRS})

# Подключаем cpp-httplib
target_include_directories(Chess PRIVATE /opt/homebrew/include/cpp-httplib)


# Линкуем Boost и cpp-httplib
target_link_libraries(Chess ${Boost_LIBRARIES})