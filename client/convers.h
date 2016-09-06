#ifndef CONVERS_H
#define CONVERS_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "Type.hh"

class Convers : public QWidget
{
    Q_OBJECT

    QVBoxLayout                   *_layout;
    QPushButton                   *_callButton;
    QPushButton                   *_callAccept;
    QPushButton                   *_callRefuse;
    QPushButton                   *_callEnd;
    QLabel                        *_callLab;
    QTextEdit                     *_text;
    QLineEdit                     *_line;
    quint32                       _id;
    std::vector<Type::UserField>  _userList;

public:
    explicit Convers(quint32 id, std::vector<Type::UserField> const& userList, QWidget *parent = 0);

    quint32   getId() const;
    void      setId(const quint32 &id);
    std::vector<Type::UserField> const& getUserList() const;
    void      addUserToList(Type::UserField);
    void      removeUserToList(Type::UserField);
    void      userWriteMessage(uint32_t userId, std::string const& msg);
    void      onCallRecveive(uint32_t uid);

private slots:
    void      onCallButtonClicked();
    void      onCallAcceptClicked();
    void      onCallRefuseClicked();

signals:
    void      ReturnPressed();
public slots:
    void      WriteInText();
    void      mousePressEvent(QMouseEvent *event);
};

#endif // CONVERS_H
