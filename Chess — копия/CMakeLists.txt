cmake_minimum_required(VERSION 3.30)
project(Chess)

set(CMAKE_CXX_STANDARD 20)

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
        Types/Game_types.cpp
        DataBase.cpp
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
        My_Blocking_Queue.h
        Types/DataBase_types.h
        Types/Game_types.h
        DataBase.h
)
add_executable(Chess ${SOURCES} ${HEADERS})

find_package(Boost REQUIRED COMPONENTS system filesystem)
target_include_directories(Chess PRIVATE ${Boost_INCLUDE_DIRS})

target_include_directories(Chess PRIVATE /opt/homebrew/Cellar/libpqxx/7.10.1/include)

target_link_libraries(Chess PRIVATE
        ${Boost_LIBRARIES}
        /opt/homebrew/lib/libboost_thread.dylib
        /opt/homebrew/lib/libboost_atomic.dylib
        /opt/homebrew/Cellar/libpqxx/7.10.1/lib/libpqxx.dylib
        /opt/homebrew/Cellar/libpq/17.4_1/lib/libpq.dylib
)