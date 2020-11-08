cmake_minimum_required(VERSION 2.8.9)

project(Henjin)

set(CMAKE_CXX_STANDARD 17)
CMAKE_POLICY(SET CMP0072 NEW)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)

find_package(Qt5 COMPONENTS OpenGL REQUIRED)
if (Qt5OpenGL_FOUND)
    message(STATUS "Found Qt ${Qt5OpenGL_VERSION}")
else (Qt5OpenGL_FOUND)
    message(ERROR "Unable to find QTOpenGL library")
endif(Qt5OpenGL_FOUND)
find_package(OpenGL REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON )



add_executable(${PROJECT_NAME} main.cpp
        src/mainwindow.cpp
        src/mainwindow.h
        src/mainwindow.ui
        src/glwidget.cpp
        src/glwidget.h
        src/scene.cpp
        src/scene.h
        src/shader.cpp
        src/shader.h
        src/camera.cpp
        src/camera.h
        src/Object/mesh.cpp
        src/Object/mesh.h
        src/Object/model.cpp
        src/Object/model.h
        src/Light/pointlight.cpp 
        src/Light/pointlight.h 
    src/Object/shape.cpp src/Object/shape.h src/Object/plane.cpp src/Object/plane.h src/Light/directionallight.cpp src/Light/directionallight.h src/Object/marchingcube.cpp src/Object/marchingcube.h src/Object/isosurface.cpp src/Object/isosurface.h src/Object/metaball.cpp src/Object/metaball.h src/Texture/texture.cpp src/Texture/texture.h)


set(EXT_LIBS Qt5::OpenGL ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${EXT_LIBS} )
