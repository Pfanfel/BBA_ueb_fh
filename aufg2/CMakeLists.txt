cmake_minimum_required(VERSION 3.1) 
project(bba)
find_package(OpenCV REQUIRED core highgui imgproc imgcodecs)
set(CMAKE_CXX_STANDARD 14) # C++17 braucht OpenCV neuer als 3.2.0
file(GLOB SOURCE_FILES src/*) # dies ist eine schlechte Idee – CMake kann nicht feststellen, ob Dateien fehlen
add_executable(bba ${SOURCE_FILES})
target_include_directories(bba PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(bba ${OpenCV_LIBS})
if(MSVC)
  if ((OpenCV_VERSION VERSION_LESS "3.3.0") AND (NOT (CMAKE_BUILD_TYPE STREQUAL "Release")))
    message(FATAL_ERROR "On Windows, this version of OpenCV must be used without debug information. Set CMAKE_BUILD_TYPE to Release.")
  endif()
  target_compile_options(bba PRIVATE /WX /Wall) # alle Warnungen aktivieren
  target_compile_options(bba PRIVATE /wd4514) # unreferenced inline function has been removed
  target_compile_options(bba PRIVATE /wd4711) # function selected for automatic inline expansion
  target_compile_options(bba PRIVATE /wd4710) # function not inlined
  target_compile_options(bba PRIVATE /wd4820) # padding added after data member
elseif(UNIX)
  target_compile_options(bba PRIVATE -Werror -Wall -Wextra) # alle Warnungen aktivieren
  target_compile_options(bba PRIVATE -Wvla) # C-style variable length arrays verbieten
  target_compile_options(bba PRIVATE -Wsign-compare)
  #target_compile_options(bba PRIVATE -Wconversion -Wsign-conversion) # zu nervig
else()
  message(WARNING "Ich habe keine Ahnung, welcher Compiler dies ist. Extra Warnungen sind wahrscheinlich nicht aktiviert.")
endif()
