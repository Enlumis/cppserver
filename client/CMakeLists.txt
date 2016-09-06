cmake_minimum_required(VERSION 2.8.11)
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(CMAKE_PREFIX_PATH "C:/Qt/Qt5.3.2/5.3/msvc2013_opengl")
elif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(CMAKE_PREFIX_PATH "/opt/Qt/5.3/gcc")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

project(client)

if(CMAKE_COMPILER_IS_GNUCXX)
	list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -O2")
endif()

# Find includes in corresponding build directories
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
	set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
# REQUIRED
    find_package(Qt5Core)
    find_package(Qt5Network)
    find_package(Qt5Widgets)

add_subdirectory("lib/opus-1.1")

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_subdirectory("lib/portaudio")
endif()

set(QT_USE_QTNETWORK)

set(PORT)

set(FORM_FILES
        babel.ui
        connectdiag.ui
        qmycontact.ui
        config.ui
        profile.ui
        rgbdial.ui
        configstylesheet.ui
        addcontact.ui
)

qt5_wrap_ui(UI_HEADERS ${FORM_FILES})

set(SOURCE_FILES
        main.cpp
        babel.cpp
        myqdockwidget.cpp
        connectdiag.cpp
        convers.cpp
        contactlist.cpp
        qmycontact.cpp
        tcpsocket.cpp
        contactname.cpp
        config.cpp
        profile.cpp
        clicklabel.cpp
        rgbdial.cpp
        configstylesheet.cpp
        guistrstyle.cpp
        PacketHandler.cpp
        AnswerHandler.cpp
        PacketCreator.cpp
        addcontact.cpp
        lineedit.cpp
        udpqsocket.cpp
        channelmanager.cpp
        callhost.cpp
        CallClient.cpp
        audio/AudioException.cpp
        audio/AudioManager.cpp
        audio/OpusCodec.cpp
        audio/OpusException.cpp
        audio/PortAudio.cpp
        audio/PortAudioException.cpp
        ../server/src/packets/APacket.cpp
    	../server/src/packets/PacketAuthentification.cpp
    	../server/src/packets/PacketAddContact.cpp
    	../server/src/packets/PacketAddUserToChannel.cpp
    	../server/src/packets/PacketRefuseContact.cpp
    	../server/src/packets/PacketRegister.cpp
    	../server/src/packets/PacketAnswer.cpp
    	../server/src/utils/md5.cpp
    	../server/src/packets/PacketHello.cpp
    	../server/src/packets/PacketLoginSuccess.cpp
    	../server/src/packets/PacketContactRequest.cpp
    	../server/src/packets/PacketContactList.cpp
    	../server/src/packets/PacketContactAdded.cpp
    	../server/src/packets/PacketContactRefused.cpp
    	../server/src/packets/PacketCreateChannel.cpp
    	../server/src/packets/PacketUserEvent.cpp
    	../server/src/packets/PacketRecvMessage.cpp
    	../server/src/packets/PacketSearchUser.cpp
    	../server/src/packets/PacketSearchResult.cpp
    	../server/src/packets/PacketSendMessage.cpp
    	../server/src/packets/PacketOpenChannel.cpp
    	../server/src/packets/PacketChannelEvent.cpp
    	../server/src/packets/PacketException.cpp
    	../server/src/packets/PacketUpdateUsername.cpp
    	../server/src/packets/PacketUpdateStatus.cpp
    	../server/src/ConfManager.cpp
    	../server/src/ConfException.cpp
    	../server/src/packets/AUdpPacket.cpp
    	../server/src/packets/PacketVoice.cpp
    	../server/src/packets/PacketOnVoice.cpp
        ../server/src/packets/PacketCall.cpp
        ../server/src/packets/PacketCallStop.cpp
        ../server/src/packets/PacketOnCall.cpp
        ../server/src/packets/PacketCallJoin.cpp
        ../server/src/packets/PacketCallOnJoin.cpp
        ../server/src/packets/PacketCallQuit.cpp
        ../server/src/packets/PacketCallOnQuit.cpp

        )

set(HEADER_FILES
            babel.h
            myqdockwidget.h
            connectdiag.h  
            convers.h      
            contactlist.h  
            qmycontact.h   
            tcpsocket.h    
            contactname.h  
            config.h       
            profile.h      
            clicklabel.h   
            rgbdial.h      
            configstylesheet.h
            guistrstyle.h  
            addcontact.h   
            ISocket.hh
            PacketHandler.hh  
            AnswerHandler.hh  
            lineedit.h     
            udpqsocket.h   
            channelmanager.h  
            callhost.h     
            ICall.hh       
            CallClient.hh  
            AudioData.hh
            AudioException.hh
            AudioManager.hh
            IAudio.hh
            IAudioCodec.hh
            OpusCodec.hh
            OpusException.hh
            PortAudio.hh
            PortAudioException.hh
            ISocket.hh
            APacket.hh
            PacketAuthentification.hh
            PacketAddContact.hh
            PacketRegister.hh
            md5.hh     
            PacketHello.hh
            PacketAnswer.hh
            PacketLoginSuccess.hh
            PacketContactRequest.hh
            PacketContactList.hh
            PacketCreateChannel.hh
            PacketContactAdded.hh
            PacketContactRefused.hh
            PacketUserEvent.hh
            PacketRecvMessage.hh
            PacketSearchUser.hh
            PacketSearchResult.hh
            PacketOpenChannel.hh
            PacketChannelEvent.hh
            PacketException.hh
            PacketUpdateUsername.hh
            PacketUpdateStatus.hh
            ConfManager.hh   
            ConfException.hh 
            Type.hh  
            AUdpPacket.hh
            PacketVoice.hh
            PacketOnVoice.hh
            PacketCall.hh
            PacketCallStop.hh
            PacketOnCall.hh
            PacketCallJoin.hh
            PacketCallOnJoin.hh
            PacketCallQuit.hh
            PacketCallOnQuit.hh
            portaudio.h
	)


set(OPUS_INCLUDE_DIR "lib/opus1-1/include")


QT5_ADD_RESOURCES(RESSOURCE_FILES BabelImgPack.qrc)
include_directories("../server/src/packets/" "../server/src/network/" "../server/src/" "audio/" "lib/opus-1.1/include" "lib/portaudio/include" ${OPUS_INCLUDE_DIR})

add_executable(client ${SOURCE_FILES} ${UI_HEADERS} ${RESSOURCE_FILES})
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(client Opus portaudio Qt5::Core Qt5::Network Qt5::Widgets)
else()
    ADD_CUSTOM_COMMAND(
        WORKING_DIRECTORY ./lib/portaudio/
        OUTPUT  Makefile
        COMMAND ./configure
        COMMAND make
    )
#    link_directories(${PROJECT_BINARY_DIR}/lib/portaudio/lib/.libs/)
    target_link_libraries(client Opus portaudio rt asound pthread Qt5::Core Qt5::Network Qt5::Widgets)
endif()