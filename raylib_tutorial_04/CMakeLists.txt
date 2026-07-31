cmake_minimum_required(VERSION 3.16)
project(raylib_tutorial_04 VERSION 1.0.0 LANGUAGES CXX)

# Modernes C++ aktivieren
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# FetchContent laden (ermöglicht das automatische Herunterladen von Bibliotheken)
include(FetchContent)

# 1. RAYLIB herunterladen und einbinden
message(STATUS "Lade Raylib von GitHub herunter...")
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG        6.0 # Du kannst auch "master" für die allerneueste Version nutzen
)

# Optionen für Raylib-Build setzen (wir brauchen keine Beispiele)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Raylib herunterladen und dem Build hinzufügen
FetchContent_MakeAvailable(raylib)

# Unsere ausführbare Datei definieren
add_executable(${PROJECT_NAME} src/main.cpp)

# Raylib mit unserem Spiel verknüpfen (automatisch plattformunabhängig)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
