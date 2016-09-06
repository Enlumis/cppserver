#pragma once

# include	<iostream>
#include <windows.h>
# include	"ILibLoader.hh"

class Win32LibLoader : public ILibLoader
{
public:
  Win32LibLoader();
  ~Win32LibLoader();

private:
  Win32LibLoader(const Win32LibLoader &);
  Win32LibLoader &operator=(const Win32LibLoader &);
  
public:
  virtual void	open(std::string const &libPath) throw(ClassException);
  virtual void	*findSymbol(std::string const &symbol) throw(ClassException);
  virtual void	close();

private:
	HMODULE		m_openedLib;
};