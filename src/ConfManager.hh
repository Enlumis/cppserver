#ifndef CONFMANAGER_H_
# define CONFMANAGER_H_

# include	<string>
# include	<map>
# include	"ConfException.hh"

class ConfManager
{
public:
  ConfManager();
  virtual ~ConfManager();

  bool			parseFile(const std::string &filename);

private:
  ConfManager(const ConfManager &);
  ConfManager &operator=(const ConfManager &);

public:
  template<typename T>
  T	get(const std::string &key) const
  {
    std::map<const std::string, const std::string>::const_iterator it = m_confValues.find(key);
    if (it != m_confValues.end())
    {
      std::istringstream  buffer(it->second);
      T     value;

      buffer >> value;
      return (value);
    }
    throw ConfException("[ConfManager] key '" + key + "' not found");
  }

private:
  std::map<const std::string, const std::string>	m_confValues;
  static ConfManager					*m_instance;

public:
  static ConfManager	*getInstance();
};

#endif /* !CONFMANAGER_H_ */
