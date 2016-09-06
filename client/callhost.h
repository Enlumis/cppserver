# ifndef                CALLHOST_H
# define                CALLHOST_H

# include               <QObject>
# include               <map>
# include               <cstdint>
# include               <utility>
# include               "ICall.hh"

class                   CallHost : public QObject, public ICall
{
  Q_OBJECT
  std::map<uint32_t, std::pair<std::string, uint16_t> >    _usersIp;

public:
    explicit            CallHost(QObject *parent = 0);
    virtual             ~CallHost();

public:
    uint32_t            getUserIdFromIp(std::string const &ip) const;
    const std::map<uint32_t, std::pair<std::string, uint16_t> > &getCalledUsers() const;

public:
    void                addUser(uint32_t userId, std::string const& ip, uint16_t port);
    void                removeUser(uint32_t userId);
    void                update();
};

# endif                 // CALLHOST_H
