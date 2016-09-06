#-------------------------------------------------
#
# Project created by QtCreator 2014-09-30T15:57:14
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Babel
TEMPLATE = app

SOURCES +=  main.cpp                \
            babel.cpp               \
            myqdockwidget.cpp       \
            connectdiag.cpp         \
            convers.cpp             \
            contactlist.cpp         \
            qmycontact.cpp          \
            tcpsocket.cpp           \
            contactname.cpp         \
            config.cpp              \
            profile.cpp             \
            clicklabel.cpp          \
            rgbdial.cpp             \
            configstylesheet.cpp    \
            guistrstyle.cpp         \
            PacketHandler.cpp       \
            AnswerHandler.cpp       \
            PacketCreator.cpp       \
            addcontact.cpp          \
            lineedit.cpp            \
            udpqsocket.cpp          \
            channelmanager.cpp      \
            callhost.cpp            \
            CallClient.cpp          \
            audio/AudioException.cpp\
            audio/AudioManager.cpp  \
            audio/OpusCodec.cpp     \
            audio/OpusException.cpp \
            audio/PortAudio.cpp     \
            audio/PortAudioException.cpp


SOURCES += ../server/src/packets/APacket.cpp            \
    ../server/src/packets/PacketAuthentification.cpp    \
    ../server/src/packets/PacketAddContact.cpp          \
    ../server/src/packets/PacketAddUserToChannel.cpp    \
    ../server/src/packets/PacketRefuseContact.cpp       \
    ../server/src/packets/PacketRegister.cpp            \
    ../server/src/packets/PacketAnswer.cpp              \
    ../server/src/utils/md5.cpp                         \
    ../server/src/packets/PacketHello.cpp               \
    ../server/src/packets/PacketLoginSuccess.cpp        \
    ../server/src/packets/PacketContactRequest.cpp      \
    ../server/src/packets/PacketContactList.cpp         \
    ../server/src/packets/PacketContactAdded.cpp        \
    ../server/src/packets/PacketContactRefused.cpp      \
    ../server/src/packets/PacketCreateChannel.cpp       \
    ../server/src/packets/PacketUserEvent.cpp           \
    ../server/src/packets/PacketRecvMessage.cpp         \
    ../server/src/packets/PacketSearchUser.cpp          \
    ../server/src/packets/PacketSearchResult.cpp        \
    ../server/src/packets/PacketSendMessage.cpp         \
    ../server/src/packets/PacketOpenChannel.cpp         \
    ../server/src/packets/PacketChannelEvent.cpp        \
    ../server/src/packets/PacketException.cpp           \
    ../server/src/packets/PacketUpdateUsername.cpp      \
    ../server/src/packets/PacketUpdateStatus.cpp        \
    ../server/src/ConfManager.cpp                       \
    ../server/src/ConfException.cpp                     \
    ../server/src/packets/AUdpPacket.cpp                \
    ../server/src/packets/PacketVoice.cpp               \
    ../server/src/packets/PacketOnVoice.cpp

HEADERS  += babel.h                             \
            myqdockwidget.h                     \
            connectdiag.h                       \
            convers.h                           \
            contactlist.h                       \
            qmycontact.h                        \
            tcpsocket.h                         \
            contactname.h                       \
            config.h                            \
            profile.h                           \
            clicklabel.h                        \
            rgbdial.h                           \
            configstylesheet.h                  \
            guistrstyle.h                       \
            addcontact.h                        \
            ../server/src/network/ISocket.hh    \
            PacketHandler.hh                    \
            AnswerHandler.hh                    \
            lineedit.h                          \
            udpqsocket.h                        \
            channelmanager.h                    \
            callhost.h                          \
            ICall.hh                            \
            CallClient.hh                       \
            audio/AudioData.hh                  \
            audio/AudioException.hh             \
            audio/AudioManager.hh               \
            audio/IAudio.hh                     \
            audio/IAudioCodec.hh                \
            audio/OpusCodec.hh                  \
            audio/OpusException.hh              \
            audio/PortAudio.hh                  \
            audio/PortAudioException.hh

HEADERS += ../server/src/packets/APacket.hh             \
    ../server/src/packets/PacketAuthentification.hh     \
    ../server/src/packets/PacketAddContact.hh           \
    ../server/src/packets/PacketRegister.hh             \
    ../server/src/utils/md5.hh                          \
    ../server/src/packets/PacketHello.hh                \
    ../server/src/packets/PacketAnswer.hh               \
    ../server/src/packets/PacketLoginSuccess.hh         \
    ../server/src/packets/PacketContactRequest.hh       \
    ../server/src/packets/PacketContactList.hh          \
    ../server/src/packets/PacketCreateChannel.hh        \
    ../server/src/packets/PacketContactAdded.hh         \
    ../server/src/packets/PacketContactRefused.hh       \
    ../server/src/packets/PacketUserEvent.hh            \
    ../server/src/packets/PacketRecvMessage.hh          \
    ../server/src/packets/PacketSearchUser.hh           \
    ../server/src/packets/PacketSearchResult.hh         \
    ../server/src/packets/PacketOpenChannel.hh          \
    ../server/src/packets/PacketChannelEvent.hh         \
    ../server/src/packets/PacketException.hh            \
    ../server/src/packets/PacketUpdateUsername.hh       \
    ../server/src/packets/PacketUpdateStatus.hh         \
    ../server/src/ConfManager.hh                        \
    ../server/src/ConfException.hh                      \
    ../server/src/packets/Type.hh                       \
    ../server/src/packets/AUdpPacket.hh                 \
    ../server/src/packets/PacketVoice.hh                \
    ../server/src/packets/PacketOnVoice.hh

FORMS    += babel.ui    \
    connectdiag.ui      \
    qmycontact.ui       \
    config.ui           \
    profile.ui          \
    rgbdial.ui          \
    configstylesheet.ui \
    addcontact.ui



INCLUDEPATH = ../Babel ../server/src/network ../server/src/packets

RESOURCES += BabelImgPack.qrc

win32: CONFIG += C++11
linux: QMAKE_CXXFLAGS += -std=c++0x
linux: QMAKE_CXXFLAGS += -Werror -Wall -Wextra -W
