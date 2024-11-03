cmake_minimum_required(VERSION 3.27)
project(Project)

set(CMAKE_CXX_STANDARD 20)

set(BUILD_SHARED_LIBS FALSE)

include(FetchContent)

# Fetch SFML
FetchContent_Declare(
        SFML
        GIT_REPOSITORY  https://github.com/SFML/SFML
        GIT_TAG         2.6.1
)
FetchContent_MakeAvailable(SFML)

# Fetch fmt
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG        10.2.1
)
FetchContent_MakeAvailable(fmt)

add_executable(Project main.cpp
        GameCharacter.h
        CharacterState.h
        CharDraw.h
        CharDraw.cpp
        GameCharacter.cpp
        Button.h
        Button.cpp
        MainMenu.h
        MainMenu.cpp
        InstructionWindow.h
        InstructionWindow.cpp
        LevelManager.h
        LevelManager.cpp
        Enemy.h
        Goblin.h
        Goblin.cpp
        EnemyState.h
        EnemyDirection.h
        FlyingEye.h
        FlyingEye.cpp
        Skeleton.h
        Skeleton.cpp
        Mushroom.h
        Mushroom.cpp
        Beast.h
        Beast.cpp
        Kitsune.h
        Kitsune.cpp
        Karasu.h
        Karasu.cpp
        Obstacle.h
        Obstacle.cpp
        BuffItem.cpp
        Item.h
)

# Link the libraries
target_link_libraries(Project PRIVATE sfml-graphics fmt::fmt)

