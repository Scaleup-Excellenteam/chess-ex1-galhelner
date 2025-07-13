cmake_minimum_required (VERSION 3.15)

project (Chess)

# New CMake versions map C++20 to /std:c++20 instead of c++latest, so <format>
# and other unstable ABI features aren't supported. Use C++23 for such versions
if (MSVC AND CMAKE_VERSION VERSION_GREATER_EQUAL 3.21)
    set(CMAKE_CXX_STANDARD 23)
else ()
    set(CMAKE_CXX_STANDARD 20)
endif ()

set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options (/W4 /permissive- /Zc:externConstexpr /Zc:inline)
endif ()

add_executable (Chess ""
        include/ChessPiece.h
        src/ChessPiece.cpp
        include/Rook.h
        src/Rook.cpp
        include/King.h
        src/King.cpp
        include/Queen.h
        src/Queen.cpp
        include/Bishop.h
        src/Bishop.cpp
        include/ChessBoard.h
        src/ChessBoard.cpp
        include/Colors.h
        include/Pawn.h
        src/Pawn.cpp
        include/Knight.h
        src/Knight.cpp
        include/PriorityQueue.h
        include/EmptyQueueException.h
        include/Move.h
        include/InvalidPieceException.h
        include/ThreadPool.h
        src/ThreadPool.cpp
)


add_subdirectory (include)
add_subdirectory (src)



