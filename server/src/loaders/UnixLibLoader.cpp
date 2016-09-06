#include	<iostream>
#include	<dlfcn.h>
#include	<string>
#include	"UnixLibLoader.hh"
#include	"ClassException.hh"

UnixLibLoader::UnixLibLoader()
{
  m_openedLib = NULL;
}

UnixLibLoader::~UnixLibLoader()
{
  close();
}

void	UnixLibLoader::open(std::string const &libPath) throw(ClassException)
{
  std::cout << "[UnixLibLoader] Open dynamic libarry : " << libPath << std::endl;
  m_openedLib = dlopen(libPath.c_str(), RTLD_LAZY);
  if (!m_openedLib)
    throw ClassException(std::string("dlopen failed: ") + dlerror());
}

void	*UnixLibLoader::findSymbol(std::string const &symbol) throw(ClassException)
{
  char	*strError;
  void	*link = dlsym(m_openedLib, symbol.c_str());

  if ((strError = dlerror()) != NULL)
    throw ClassException(std::string("dlsym failed: ") + strError);
  return link;
}

void	UnixLibLoader::close()
{
  if (m_openedLib)
    dlclose(m_openedLib);
  m_openedLib = NULL;
}
