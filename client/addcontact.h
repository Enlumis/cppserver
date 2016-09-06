#ifndef ADDCONTACT_H
#define ADDCONTACT_H

#include <QDialog>
#include "PacketSearchResult.hh"

class LineEdit;

namespace Ui {
  class AddContact;
}

class AddContact : public QDialog
{
  Q_OBJECT

public:
  explicit        AddContact(QWidget *parent = 0);
                  ~AddContact();
  void            setSearchResultList(std::vector<PacketSearchResult::user_t>const & foundContactList);
  void            notify(bool isError, std::string const& msg);

private slots:

  void            on__searchButton_clicked();
  void            on__addUser_clicked();

private:
  Ui::AddContact                          *ui;
  std::vector<PacketSearchResult::user_t> _foundContactList;
  LineEdit                                *_contactFocus;

public slots:
  void            onLineEditFocus(LineEdit *contactFocus);
  void            closeEvent(QCloseEvent *ev);

};

#endif // ADDCONTACT_H
