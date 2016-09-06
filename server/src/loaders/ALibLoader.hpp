#ifndef ALIBLOADER_H_
# define ALIBLOADER_H_

# include	<string>
# include	<iostream>

/*
 * To use it, there is an example in UneLibLoader.hh
 *
 * TODO : gestion de plusieurs libs
 */
class ALibLoader
{
protected:
  void		*m_openedLib;

public:
  ALibLoader()
  {
    m_openedLib = NULL;
  }

  virtual ~ALibLoader() {}

private:
  ALibLoader(const ALibLoader &);
  ALibLoader &operator=(const ALibLoader &);

protected:
  virtual void	open(std::string const &libPath) = 0;

  /*
   * for windows LoadLibrary return a HMODULE
   * who HMODULE is a typedef of void *
   */
  virtual void	*findSymbol(std::string const &symbol) = 0;

public:
  virtual void	close() = 0;

  template <typename T>
  T	getCreator(std::string const &libPath, std::string const &symbol)
  {
    if (!m_openedLib)
      {
	open(libPath);
	if (!m_openedLib)
	  return NULL;
      }
    T object;
    *((void **) &object) = findSymbol(symbol);
    return static_cast<T>(*object);
  }

};

#endif /* !ALIBLOADER_H_ */
