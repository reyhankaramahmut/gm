cmake_minimum_required(VERSION 3.16)
project(Assignment1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Assignment1
    Aufgabe01.cpp
    AffineGeometry.cpp  AffineGeometry.h
    AffineMap.cpp       AffineMap.h
    quader.cpp          quader.h
    quaternion.cpp      quaternion.h
    viewSystem.cpp      viewSystem.h
    color.h
)

target_link_libraries(Assignment1 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/freeglut.lib
    opengl32
)

include(GNUInstallDirs)
install(TARGETS Assignment1
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)