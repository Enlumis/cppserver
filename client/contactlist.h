#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include <QWidget>
#include <QVBoxLayout>
#include <QDebug>
#include <QLabel>
#include "../server/src/packets/Type.hh"

class QMyContact;

class ContactList : public QWidget
{
    Q_OBJECT
    std::list<QMyContact*>        _selectlist;
    QVBoxLayout                   *_layout;
    QLabel*                       _errLab;
    QVector<QMyContact*>          _contactList;

    QMyContact*                   PopLastItem();
    void                          addNextItem(QMyContact *topush);
public:
    explicit                      ContactList(QWidget *parent = 0);
    QVector<QMyContact*> const&   getContactList() const;
    void                          clearContactList();
    void                          setContactList(QVector<Type::UserField> const& vec);
    void                          addToList(QMyContact *);
    void                          clearList();
    void                          removeList();
    std::list<QMyContact *>       &getList();
    const std::list<QMyContact *> &getList()const;
    std::list<QMyContact*> const& getSelectedList() const;
    QString                       getUsernameToId(uint32_t id) const;

    void                          addContact(QString const& userName, QString const& status, quint32 id);
    void                          addContact(QMyContact* newContact);
    void                          addContact(std::string const& userName, std::string const& status, uint32_t id);
    void                          addPendingContact(quint32 id, std::string const& userName, std::string const& status);
    void                          deletePendingContact(uint32_t userId);
    void                          updateContactUsername(quint32 id, std::string data);
    void                          updateContactStatus(quint32 id, std::string data);
    bool                          isAlreadyInContactList(QMyContact*);
    void                          removeContact(QMyContact* contact);
    void                          notify(bool isError, std::string const& msg);

signals:

public slots:
    void                          rmContact();
    void                          downContact();
    void                          UpContact();
    void                          endErrorMessage();

};

#endif // CONTACTLIST_H
