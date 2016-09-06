#pragma once

# include	<string>
# include	<iostream>
#include "ClassException.hh"

/*
 * To use it, there is an example in UneLibLoader.hh
 *
 * TODO : gestion de plusieurs libs
 */
class ILibLoader
{
public:
  virtual void	open(std::string const &libPath) throw(ClassException) = 0;

  virtual void	*findSymbol(std::string const &symbol) throw(ClassException) = 0;

  virtual void	close() = 0;

};
