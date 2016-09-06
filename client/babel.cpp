#include "babel.h"
#include "ui_babel.h"
#include <QDebug>
#include <QInputDialog>
#include <QDir>
#include "connectdiag.h"
#include "convers.h"
#include <QTimer>
#include <QMouseEvent>
#include "qmycontact.h"
#include "contactlist.h"
#include "ui_qmycontact.h"
#include "config.h"
#include "profile.h"
#include "addcontact.h"
#include "PacketCreateChannel.hh"
#include "configstylesheet.h"
#include "ICall.hh"

#include <stdexcept>


Babel::Babel(QWidget *parent) :
  QMainWindow(parent)
  , _config(new Config(this)),
  _addContact(new AddContact(this)), ui(new Ui::Babel)
{
  _tsocket = NULL;
  _udpsocket = NULL;
  _uiSetting = new configStyleSheet(this);
  _call = NULL;
  bab = this;
  ui->setupUi(this);
  ui->_profile->setWidget(new Profile(ui->_profile));
  ui->scrollArea->setWidget(new ContactList(ui->contactList));
  centralWidget()->setHidden(true);
  try {
    this->_audioManager = new AudioManager;
  } catch (std::exception &e) {
    throw std::runtime_error("[AudioManager Failed] : " + std::string(e.what()));
  }
  _codiag = new ConnectDiag(this);
  if(_codiag->exec() == -1)
    throw std::runtime_error("[Connection exit]");
  if (_tsocket == NULL)
    throw std::runtime_error("[Connection exit]");
  setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
  setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
  connect(ui->actionAdd_contact, SIGNAL(triggered()), this, SLOT(addContact()));
  connect(ui->actionEdit_profile, SIGNAL(triggered()), this, SLOT(configure()));
  connect(ui->actionUI_setings, SIGNAL(triggered()), this, SLOT(uiSetting()));
  connect(_audioManager, SIGNAL(somethingToListen()), this, SLOT(onSomethingToListen()));
  this->show();
}

Profile*            Babel::getProfile()
{
  return static_cast<Profile*>(ui->_profile->widget());
}

ContactList*        Babel::getContactList()
{
  return static_cast<ContactList*>(ui->scrollArea->widget());
}


Babel::~Babel()
{
    delete this->_audioManager;
    delete ui;
}

void                Babel::on_contactList_dockLocationChanged(const Qt::DockWidgetArea &)
{
    ui->contactList->setFeatures(ui->contactList->features() ^ QDockWidget::DockWidgetClosable);
}

void                Babel::on_contactList_topLevelChanged(bool)
{
    ui->contactList->setFeatures(ui->contactList->features() | QDockWidget::DockWidgetClosable);
}

void                Babel::setSSheet()
{

}

void                Babel::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        QMenu menu(this);
        QAction newConv(this);
        newConv.setText("New Conversation");
        QAction callConv(this);
        callConv.setText("New call conversation");
        ContactList* contact = static_cast<ContactList*>(ui->scrollArea->widget());
        QAction addToCurrentConversation(this);
        addToCurrentConversation.setText("add this/these user(s) to the conversation");
        if(contact->getList().empty())
        {
            newConv.setEnabled(false);
            callConv.setEnabled(false);
        }
        menu.addAction(&newConv);
        menu.addAction(&callConv);
        menu.addAction(&addToCurrentConversation);
        connect(&newConv, SIGNAL(triggered()), &_channelManager, SLOT(newConversation()));
        connect(&addToCurrentConversation, SIGNAL(triggered()),
                &bab->getChannelManager(), SLOT(addContactInThisChannel()));
        menu.exec(QCursor::pos());
    }
    else
    {
        static_cast<ContactList*>(ui->scrollArea->widget())->clearList();
    }
}

void Babel::on_actionConnect_triggered()
{
}

void                    Babel::addContact()
{
  _addContact->show();
}

void                    Babel::configure()
{
  _config->show();
}

void                    Babel::setTsocket(TcpSocket *tsocket)
{
  _tsocket = tsocket;
}

bool                    Babel::hasSocket() const
{
  return (_tsocket != NULL);
}

ISocket*                Babel::getTSocket()
{
  if (!_tsocket)
    {
      QMessageBox::critical(this, "Error: no socket", "The socket is empty !");
      throw std::exception();
    }
  return _tsocket;
}

ChannelManager&         Babel::getChannelManager()
{
  return _channelManager;
}

ChannelManager const&   Babel::getChannelManager() const
{
  return _channelManager;
}

PacketCreator const&    Babel::getPacketCreator() const
{
  return _packetCreator;
}

void                    Babel::addConvers(MyQDockWidget* widget)
{
  tabifyDockWidget(ui->dockWidget_14, widget);
}

void                   Babel::setDockWidgetFocus(MyQDockWidget* dockwidget)
{
  if (dockwidget != ui->_profile && dockwidget != ui->contactList)
    _dockWidgetFocus = dockwidget;
}

MyQDockWidget*         Babel::getDockWidgetFocus()
{
  return _dockWidgetFocus;
}

void                  Babel::uiSetting()
{
  _uiSetting->show();
}

ICall*                Babel::getCall()
{
  return _call;
}

void                  Babel::setCall(ICall* call)
{
  _call = call;
}

ISocket*              Babel::getUdpSocket()
{
  return _udpsocket;
}

void                  Babel::setUdpSocket(ISocket* socket)
{
  _udpsocket = socket;
}

void                  Babel::endCall()
{
  if (_udpsocket != NULL)
  {
    _udpsocket->close();
    delete _udpsocket;
    _udpsocket = NULL;
  }
  if (_call != NULL)
  {
    delete _call;
    _call = NULL;
  }
  if (this->_audioManager->checkRun())
    this->_audioManager->stop();
  if (this->_thread.isRun())
    this->_thread.stop();
}

AudioManager const     *Babel:: getAudioManager() const
{
  return (this->_audioManager);
}

AudioManager           *Babel:: getAudioManager()
{
  return (this->_audioManager);
}

BThread const          &Babel:: getThread() const
{
  return (this->_thread);
}

BThread                &Babel:: getThread()
{
  return (this->_thread);
}

void                    Babel::onSomethingToListen()
{
  _call->update();
}
