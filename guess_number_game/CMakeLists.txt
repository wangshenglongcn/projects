cmake_minimum_required(VERSION 4.0.3)

# 项目名称
project(GuessNumberGame)

# 设置使用c++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# 添加可执行文件并指定所有源码文件
add_executable(guess_number_game
    main.cpp
)
