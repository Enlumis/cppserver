#include	<iostream>
#include	<string>
#include	"Win32LibLoader.hh"

Win32LibLoader::Win32LibLoader()
{
}

Win32LibLoader::~Win32LibLoader()
{
  close();
}

void	Win32LibLoader::open(std::string const &libPath) throw(ClassException) 
{
  m_openedLib = LoadLibrary(libPath.c_str());
  if (!m_openedLib)
	  throw ClassException("LoadLibrary failed: " + GetLastError());
}

void	*Win32LibLoader::findSymbol(std::string const &symbol) throw(ClassException) 
{
  FARPROC link = GetProcAddress(m_openedLib, symbol.c_str());
  if (link == NULL)
	  throw ClassException("GetProcAddress failed: " + GetLastError());
  return link;
}

void	Win32LibLoader::close()
{
  if (m_openedLib)
    FreeLibrary(m_openedLib);
  m_openedLib = NULL;
}
