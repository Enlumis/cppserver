#include	<iostream>
#include	<sstream>
#include	"ConfManager.hh"
#include	"ConfException.hh"

ConfManager*	ConfManager::m_instance = NULL;

ConfManager	*ConfManager::getInstance()
{
  if (!m_instance)
    m_instance = new ConfManager();
  return m_instance;
}


ConfManager::ConfManager()
{
  m_confValues.insert(std::make_pair("saltSize", "32"));

  m_confValues.insert(std::make_pair("accountNameMinSize", "5"));
  m_confValues.insert(std::make_pair("accountNameMaxSize", "32"));

  m_confValues.insert(std::make_pair("passwordMinSize", "32"));
  m_confValues.insert(std::make_pair("passwordMaxSize", "32"));

  m_confValues.insert(std::make_pair("usernameMinSize", "5"));
  m_confValues.insert(std::make_pair("usernameMaxSize", "32"));

  m_confValues.insert(std::make_pair("statusMinSize", "0"));
  m_confValues.insert(std::make_pair("statusMaxSize", "128"));

  m_confValues.insert(std::make_pair("patternMinSize", "5"));
  m_confValues.insert(std::make_pair("patternMaxSize", "32"));
}

ConfManager::~ConfManager()
{

}

bool	ConfManager::parseFile(const std::string &filename)
{
  (void) filename;
  // TODO parser un fichier json
  return true;
}

template<>
const std::string	&ConfManager::get(const std::string &key) const
{
  std::map<const std::string, const std::string>::const_iterator it = m_confValues.find(key);
  if (it != m_confValues.end())
    return it->second;
  throw ConfException("[ConfManager] key '" + key + "' not found");
}
