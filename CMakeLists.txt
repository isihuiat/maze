# CMakeのバージョンを設定
cmake_minimum_required(VERSION 3.29)

if (UNIX)
    set(CMAKE_C_COMPILER "gcc")
elseif (WIN32)
    set(CMAKE_C_COMPILER "gcc")
endif ()
set(CMAKE_C_STANDARD 11)
# -g付きがデバッグ用
set(CMAKE_C_FLAGS "-Wall --pedantic-errors -MMD -MP -g")
# set(CMAKE_C_FLAGS "-Wall --pedantic-errors -MMD -MP")

# プロジェクト名と使用する言語を設定
project(Maze C)

# ビルドタイプを設定（必要に応じて）
set(CMAKE_BUILD_TYPE Debug)

# 出力ディレクトリを設定
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

# インクルードディレクトリを追加
include_directories(${PROJECT_SOURCE_DIR}/app/inc)

# ソースファイルを再帰的に収集
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/app/src/*.c)

# 実行ファイルの出力
add_executable(maze ${SOURCES})

# モジュールごとに個別のCMakeLists.txtを用意する場合（オプション）
# add_subdirectory(src/module1)
# add_subdirectory(src/module2)

# 必要に応じてライブラリをリンク
# target_link_libraries(my_project <library_name>)
