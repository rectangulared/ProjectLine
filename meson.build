project('projectLine', 'cpp', default_options: 'default_library=static')

glfw3_dep = dependency('glfw3')

include_directories = ['./include']
src = ['src/main.cpp', 'src/LineEngine.cpp']
cpp_args = ['-std=c++17']
link_args = ['-lvulkan']
dependencies = [glfw3_dep]

executable('projectLine', src, win_subsystem: 'windows', dependencies: dependencies, cpp_args: cpp_args, link_args: link_args, include_directories: include_directories)