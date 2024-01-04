cmake_minimum_required(VERSION 3.5)

project(QtMecheyeCamera)

# Find Qt5 package
find_package(Qt5 COMPONENTS Core Gui Multimedia MultimediaWidgets Widgets REQUIRED)

# Find OpenCV
find_package(OpenCV REQUIRED)
if(NOT OpenCV_FOUND)
  message(
    FATAL_ERROR
      "OpenCV not found. Please point OpenCV_DIR to the directory of your OpenCV installation (containing the file OpenCVConfig.cmake)."
  )
endif()

# Find MechEyeApi
if(CMAKE_HOST_WIN32)
  find_package(MechEyeApi REQUIRED CONFIG PATHS "$ENV{MECHEYE_DIR}/API")
  if(NOT MechEyeApi_FOUND)
    message(
      FATAL_ERROR "MechEyeApi not found. Please install MechEyeApi first.")
  endif()
elseif(CMAKE_HOST_UNIX)
  find_package(PkgConfig)
  if(NOT PkgConfig_FOUND)
    message(FATAL_ERROR "PkgConfig not found.")
  else()
    pkg_check_modules(MECHEYEAPI REQUIRED MechEyeApi)
    if(NOT MECHEYEAPI_FOUND)
      message(
        FATAL_ERROR "MechEyeApi not found. Please install MechEyeApi first.")
    endif()
  endif()
endif()

include_directories(${MECHEYEAPI_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})
link_directories(${MECHEYEAPI_LIBRARY_DIRS} ${OpenCV_LIBRARY_DIRS})

# Set target properties
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set target sources
set(SOURCES
    main.cpp
    QtMecheyeCamera.cpp
)

set(HEADERS
    QtMecheyeCamera.h
)

# Add executable target
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link Qt5 libraries to the target
target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Gui
    Qt5::Multimedia
    Qt5::MultimediaWidgets
    Qt5::Widgets
    ${MECHEYEAPI_LIBRARIES} 
    ${OpenCV_LIBS}
)


# Set additional properties for Windows
if(WIN32)
    target_sources(${PROJECT_NAME} PRIVATE)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

# Set installation directory
if(QNX)
    set(target_path "/tmp/${PROJECT_NAME}/bin")
else()
    if(NOT ANDROID)
        set(target_path "/opt/${PROJECT_NAME}/bin")
    endif()
endif()

if(NOT target_path STREQUAL "")
    install(TARGETS ${PROJECT_NAME} DESTINATION ${target_path})
endif()

