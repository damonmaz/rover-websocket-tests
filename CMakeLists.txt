cmake_minimum_required(VERSION 3.10)
project(WebSocketExample)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Boost REQUIRED COMPONENTS system)
find_package(Threads REQUIRED)

# Include directories
include_directories(${Boost_INCLUDE_DIRS})

# Build Server executable
add_executable(Server
    Server.cpp
    UDPHandler.cpp
    UDPHandler.h
    RoverState/StateManagers/ArmMotorStateManager.h
    RoverState/StateManagers/ArmStateManager.h
    RoverState/StateManagers/DriveMotorStateManager.h
    RoverState/StateManagers/DriveStateManager.h
    RoverState/StateManagers/MotorStateManager.h
    RoverState/StateManagers/MotorStateManager.cpp
    RoverState/StateManagers/RoverStateManager.h
    RoverState/StateManagers/RoverStateManager.cpp
    RoverState/StateManagers/StateManager.h
    RoverState/ArmMotorState.h
    RoverState/ArmMotorState.h
    RoverState/ArmState.h
    RoverState/DriveMotorState.h
    RoverState/DriveState.h
    RoverState/MotorState.h
    RoverState/RoverState.h
    MessageQueue.h
    MessageQueue.cpp
    pub_general.h
)

target_link_libraries(Server
    ${Boost_LIBRARIES}
    Threads::Threads
)

# Build Client executable
add_executable(Client
    Client.cpp
    UDPHandler.cpp
    UDPHandler.h
    RoverState/StateManagers/ArmMotorStateManager.h
    RoverState/StateManagers/ArmStateManager.h
    RoverState/StateManagers/DriveMotorStateManager.h
    RoverState/StateManagers/DriveStateManager.h
    RoverState/StateManagers/MotorStateManager.h
    RoverState/StateManagers/MotorStateManager.cpp
    RoverState/StateManagers/RoverStateManager.h
    RoverState/StateManagers/RoverStateManager.cpp
    RoverState/StateManagers/StateManager.h
    RoverState/ArmMotorState.h
    RoverState/ArmMotorState.h
    RoverState/ArmState.h
    RoverState/DriveMotorState.h
    RoverState/DriveState.h
    RoverState/MotorState.h
    RoverState/RoverState.h
    MessageQueue.h
    MessageQueue.cpp
    pub_general.h
)

target_link_libraries(Client
    ${Boost_LIBRARIES}
    Threads::Threads
)

# Compiler-specific options
if(MSVC)
    target_compile_definitions(Server PRIVATE _WIN32_WINNT=0x0601)
    target_compile_definitions(Client PRIVATE _WIN32_WINNT=0x0601)
endif()

# Set output directory
set_target_properties(Server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
set_target_properties(Client PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)