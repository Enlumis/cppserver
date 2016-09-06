#ifndef BABEL_H
#define BABEL_H

# include <QMainWindow>
# include <QDockWidget>
# include <QMessageBox>
# include "tcpsocket.h"
# include "channelmanager.h"
# include "PacketCreator.hh"
# include "AudioManager.hh"
# include "BThread.hpp"

class ICall;
class ConnectDiag;
class Config;
class AddContact;
class Profile;
class ContactList;
class MyQDockWidget;
class configStyleSheet;

namespace Ui {
class Babel;
}

class Babel : public QMainWindow
{
  Q_OBJECT

  ISocket*                  _tsocket;
  ISocket*                  _udpsocket;
  ConnectDiag*              _codiag;
  Config*                   _config;
  AddContact*               _addContact;
  ChannelManager            _channelManager;
  PacketCreator		          _packetCreator;
  MyQDockWidget*            _dockWidgetFocus;
  Convers*                  _currentConvers;
  configStyleSheet          *_uiSetting;
  ICall                     *_call;
  AudioManager              *_audioManager;
  BThread                   _thread;


public:
    explicit                Babel(QWidget *parent = 0);
    virtual                 ~Babel();
    void                    addConvers(MyQDockWidget* widget);
    BThread const           &getThread() const;
    BThread                 &getThread();
    void                    endCall();

public slots:
    void                    addContact();
    void                    configure();
    void                    uiSetting();
    void                    onSomethingToListen();

private slots:

    void                    mousePressEvent(QMouseEvent *event);
    void                    on_contactList_dockLocationChanged(const Qt::DockWidgetArea &area);
    void                    on_contactList_topLevelChanged(bool topLevel);
    void                    on_actionConnect_triggered();
    void                    setSSheet();

private:
    Ui::Babel               *ui;

public:
    Ui::Babel*              getUi() {return ui;}
    ConnectDiag*            getCoDiag() {return _codiag;}
    Config*                 getConfig() {return _config;}
    bool                    hasSocket() const;
    ISocket*                getTSocket();
    Profile*                getProfile();
    ContactList*            getContactList();
    ChannelManager&         getChannelManager();
    ChannelManager const&   getChannelManager() const;
    AudioManager const      *getAudioManager() const;
    AudioManager            *getAudioManager();
    PacketCreator const     &getPacketCreator() const;
    AddContact*             getAddContact() {return _addContact;}
    void                    setTsocket(TcpSocket *tsocket);
    void                    setDockWidgetFocus(MyQDockWidget*);
    MyQDockWidget*          getDockWidgetFocus();
    ICall*                  getCall();
    void                    setCall(ICall* call);
    ISocket*                getUdpSocket();
    void                    setUdpSocket(ISocket* socket);
};

extern Babel                *bab;

#endif // BABEL_H
