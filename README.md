# HamPi
ACTIVE: Source Code for First Year ARM11 Group project Team HamPi

/*Cmake file for linking Clion*/
/*You generally want to install the development library for your system.*/
/*Put the resources in the cmake-build-debug folder*/
/*But make sure you put the contents of the lib folder directly in the cmake-build-debug folder
 where the game executable is*/ 

[SDL2](https://www.libsdl.org/download-2.0.php)
[SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/)
[SDL2_mixer](https://www.libsdl.org/projects/SDL_mixer/)

cmake_minimum_required(VERSION 3.7)
project(.....)                                    //Project Name here as normal                         

set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR}/.....)   //Put the directory path which corresponds to the include folder of SDL2

link_directories(${CMAKE_SOURCE_DIR}/.....)      //Put the directory path which corresponds to the library folder of SDL2

include_directories(${CMAKE_SOURCE_DIR}/.....)   //Put the directory path which corresponds to the include folder of SDL2_ttf

link_directories(${CMAKE_SOURCE_DIR}/.....)      //Put the directory path which corresponds to the library folder of SDL2_ttf

include_directories(${CMAKE_SOURCE_DIR}/.....)   //Put the directory path which corresponds to the include folder of SDL2_mixer

link_directories(${CMAKE_SOURCE_DIR}/.....)      //Put the directory path which corresponds to the library folder of SDL2_mixer


set(SOURCE_FILES .....)                          //As normal


add_executable(game ${SOURCE_FILES})
target_link_libraries(..... SDL2)               /*
target_link_libraries(..... SDL2_ttf)               Link the libraries used here
target_link_libraries(..... SDL2_mixer)         */
