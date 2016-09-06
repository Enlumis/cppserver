# help at http://florian-goujeon.developpez.com/cours/cmake/initiation/#LIII-D-1
cmake_minimum_required(VERSION 2.6)

#project name
project(babel-server)

#output for the executables
IF(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
  ADD_DEFINITIONS(-fPIC)
ENDIF(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")

if(WIN32)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ".")
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY ".")
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ".")
else()
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BUILD_TYPE}/.)
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BUILD_TYPE}/.)
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BUILD_TYPE}/.)
endif()

#main executable name
set(EXECUTABLE_NAME babel-server)
set(CMAKE_CXX_COMPILER g++)

list(APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")

#use static boost
set (Boost_USE_STATIC_LIBS TRUE)

#add the boost library
SET(CMAKE_DL_LIBS "-ldl" "-lpthread")

#
# BABEL SERVER CORE
#

find_package(
  Boost 
  1.36.0
  REQUIRED
  serialization
  system
  signals
  )

link_directories(${Boost_LIBRARY_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

include_directories(src/)
include_directories(src/managers/)
include_directories(src/network/)
include_directories(src/packets/)
include_directories(src/storage/)
include_directories(src/services/)

file(
  GLOB_RECURSE
  sources
  src/utils/*
  src/managers/*
  src/network/*
  src/storage/*
  src/packets/APacket.cpp
  src/packets/APacket.hh
  src/packets/PacketException.cpp
  src/packets/PacketException.hh
  src/loaders/ClassException.cpp
  src/loaders/ClassException.hh
  src/loaders/ServiceLoader.hpp
  src/loaders/ILibLoader.hh
  src/UserController.cpp
  src/UserController.hh
  src/ConfManager.cpp
  src/ConfManager.hh
  src/ConfException.cpp
  src/ConfException.hh
  src/services/ServiceManager.cpp
  src/services/ServiceManager.hh
  src/DataContainer.hpp
  )

if(WIN32)
  file(
    GLOB_RECURSE
    loadersources
    src/loaders/Win32LibLoader.cpp
    src/loaders/Win32LibLoader.hh
  )
else()
  file(
    GLOB_RECURSE
    loadersources
    src/loaders/UnixLibLoader.cpp 
    src/loaders/UnixLibLoader.hh
  )
endif()

SET(LIB_BABEL_SERVER_CORE_NAME babel-server-core)

add_library(${LIB_BABEL_SERVER_CORE_NAME} STATIC
  ${sources}
  ${loadersources}
  )

target_link_libraries(
  ${LIB_BABEL_SERVER_CORE_NAME}
  ${Boost_LIBRARIES}
  ${CMAKE_DL_LIBS}
  )

#
# DYNAMICS LIBRARY
#

#output for the libraries

# DATABASE SERVICE
file(
  GLOB_RECURSE
  database-service-sources
  src/packets/APacket.cpp
  src/packets/APacket.hh
  src/packets/PacketException.cpp
  src/packets/PacketException.hh
  src/services/database-service/*
  src/services/UserData.hh
  src/services/UserData.cpp
  src/services/IService.hh
  )

SET(LIB_DATABASE_SERVICE_NAME database-service)
add_library(${LIB_DATABASE_SERVICE_NAME} SHARED
  ${database-service-sources}
  )

target_link_libraries(
  ${LIB_DATABASE_SERVICE_NAME}
  ${LIB_BABEL_SERVER_CORE_NAME}
  )

# USER SERVICE
file(
  GLOB_RECURSE
  user-service-sources
  src/packets/APacket.cpp
  src/packets/APacket.hh
  src/packets/PacketAnswer.cpp
  src/packets/PacketAnswer.hh
  src/packets/PacketUpdateStatus.cpp
  src/packets/PacketUpdateStatus.hh
  src/packets/PacketUpdateUsername.cpp
  src/packets/PacketUpdateUsername.hh
  src/packets/PacketAddContact.cpp
  src/packets/PacketAddContact.hh
  src/packets/PacketLoginSuccess.cpp
  src/packets/PacketLoginSuccess.hh
  src/packets/PacketLoginRefused.cpp
  src/packets/PacketLoginRefused.hh
  src/packets/PacketSearchUser.cpp
  src/packets/PacketSearchUser.hh
  src/packets/PacketSearchResult.cpp
  src/packets/PacketSearchResult.hh
  src/packets/PacketException.cpp
  src/packets/PacketException.hh
  src/packets/PacketContactRequest.cpp
  src/packets/PacketContactRequest.hh
  src/packets/PacketContactList.cpp
  src/packets/PacketContactList.hh
  src/packets/PacketContactAdded.cpp
  src/packets/PacketContactAdded.hh
  src/packets/PacketContactRefused.cpp
  src/packets/PacketContactRefused.hh
  src/packets/PacketRefuseContact.cpp
  src/packets/PacketRefuseContact.hh
  src/packets/PacketUserEvent.cpp
  src/packets/PacketUserEvent.hh
  src/services/user-service/*
  src/services/IService.hh
  src/services/UserData.hh
  src/services/UserData.cpp
  src/services/IUserService.hh
  )

SET(LIB_USER_SERVICE_NAME user-service)
add_library(${LIB_USER_SERVICE_NAME} SHARED
  ${user-service-sources}
  )

target_link_libraries(
  ${LIB_USER_SERVICE_NAME}
  ${LIB_BABEL_SERVER_CORE_NAME}
  ${LIB_DATABASE_SERVICE_NAME}
  )

# LOGGIN SERVICE
file(
  GLOB_RECURSE
  loggin-service-sources
  src/packets/APacket.cpp
  src/packets/APacket.hh
  src/packets/PacketAnswer.cpp
  src/packets/PacketAnswer.hh
  src/packets/PacketAuthentification.cpp
  src/packets/PacketAuthentification.hh
  src/packets/PacketRegister.cpp
  src/packets/PacketRegister.hh
  src/packets/PacketHello.cpp
  src/packets/PacketHello.hh
  src/packets/PacketException.cpp
  src/packets/PacketException.hh
  src/services/loggin-service/*
  src/services/IService.hh
  src/services/UserData.hh
  src/services/UserData.cpp
  src/services/IUserService.hh
  )

SET(LIB_LOGGIN_SERVICE_NAME loggin-service)
add_library(${LIB_LOGGIN_SERVICE_NAME} SHARED
  ${loggin-service-sources}
  )

target_link_libraries(
  ${LIB_LOGGIN_SERVICE_NAME}
  ${LIB_USER_SERVICE_NAME}
  ${LIB_BABEL_SERVER_CORE_NAME}
  ${LIB_DATABASE_SERVICE_NAME}
  )

# CHANNEL SERVICE
file(
  GLOB_RECURSE
  channel-service-sources
  src/packets/APacket.cpp
  src/packets/APacket.hh
  src/packets/PacketAnswer.cpp
  src/packets/PacketAnswer.hh
  src/packets/PacketException.cpp
  src/packets/PacketException.hh
  src/packets/PacketCreateChannel.cpp
  src/packets/PacketCreateChannel.hh
  src/packets/PacketOpenChannel.cpp
  src/packets/PacketOpenChannel.hh
  src/packets/PacketAddUserToChannel.cpp
  src/packets/PacketAddUserToChannel.hh
  src/packets/PacketSendMessage.cpp
  src/packets/PacketSendMessage.hh
  src/packets/PacketRecvMessage.cpp
  src/packets/PacketRecvMessage.hh
  src/packets/PacketChannelEvent.cpp
  src/packets/PacketChannelEvent.hh
  src/services/channel-service/*
  src/services/UserData.hh
  src/services/UserData.cpp
  src/services/IService.hh
  )

SET(LIB_CHANNEL_SERVICE_NAME channel-service)
add_library(${LIB_CHANNEL_SERVICE_NAME} SHARED
  ${channel-service-sources}
  )

target_link_libraries(
  ${LIB_CHANNEL_SERVICE_NAME}
  ${LIB_BABEL_SERVER_CORE_NAME}
  )

# CALL SERVICE
file(
  GLOB_RECURSE
  call-service-sources
  src/packets/APacket.cpp
  src/packets/APacket.hh
  src/packets/PacketCallOnQuit.hh
  src/packets/PacketCallOnQuit.cpp
  src/packets/PacketCallQuit.cpp
  src/packets/PacketCallQuit.hh
  src/packets/PacketCallJoin.hh
  src/packets/PacketCallJoin.cpp
  src/packets/PacketCallOnJoin.hh
  src/packets/PacketCallOnJoin.cpp
  src/packets/PacketCallStop.hh
  src/packets/PacketCallStop.cpp
  src/packets/PacketOnCall.cpp
  src/packets/PacketOnCall.hh
  src/packets/PacketCall.cpp
  src/packets/PacketCall.hh
  src/packets/PacketAnswer.cpp
  src/packets/PacketAnswer.hh
  src/packets/PacketException.cpp
  src/packets/PacketException.hh
  src/services/call-service/*
  src/services/channel-service/Room.cpp
  src/services/channel-service/Room.hh
  src/services/UserData.hh
  src/services/UserData.cpp
  src/services/IService.hh
  )

SET(LIB_CALL_SERVICE_NAME call-service)
add_library(${LIB_CALL_SERVICE_NAME} SHARED
  ${call-service-sources}
  )

target_link_libraries(
  ${LIB_CALL_SERVICE_NAME}
  ${LIB_USER_SERVICE_NAME}
  ${LIB_CHANNEL_SERVICE_NAME}
  ${LIB_BABEL_SERVER_CORE_NAME}
  )

#
# Main executable : babel-server
#

add_executable(
  ${EXECUTABLE_NAME}
  src/main.cpp
  )

target_link_libraries(
  ${EXECUTABLE_NAME}
  ${LIB_BABEL_SERVER_CORE_NAME}
  ${Boost_LIBRARIES}
  ${CMAKE_DL_LIBS}
  )