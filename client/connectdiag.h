#ifndef CONNECTDIAG_H
#define CONNECTDIAG_H
#include "ui_connectdiag.h"

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QCheckBox>


namespace Ui {
  class ConnectDiag;
}

class ConnectDiag : public QDialog
{
    Q_OBJECT
    Ui::ConnectDiag *ui;
    QSize           _ssize;
    std::string     _salt;

public:
    explicit    ConnectDiag(QWidget *parent = 0);
    ~ConnectDiag(){}
    bool        isRegisterChecked() const;
    void        notify(bool isError, const std::string &msg);
signals:

public slots:
    void    on__adopt_stateChanged(int val);
    void    on__buttons_rejected();
    void    on__buttons_accepted();
    void    tryToConnect();
    void    closeEvent(QCloseEvent *ev);
    void    keyPressEvent(QKeyEvent *);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

public:
    std::string const&  getSalt() const;
    void                setSalt(const std::string &salt);
    void                setIpPortErrorLab(QString const& errmsg);
    void                setIpErrorStylesheet();
    void                setPortErrorStylesheet();
    void                setIpPortSuccesLab(const QString &msg);
    QString             getUsername() const;
    QString             getPassword() const;
    void                setUsername( QString& username );
    void                setPassword( QString& password );
    QLabel*             getErrorLab() {return ui->_errLab;}
};

#endif // CONNECTDIAG_H
