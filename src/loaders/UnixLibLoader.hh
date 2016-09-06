#ifndef UNIXLIBLOADER_H_
# define UNIXLIBLOADER_H_

# include	<iostream>
# include	"ILibLoader.hh"
/**
 *
 * EXAMPLE HOW TO USE :

  UnixLibLoader *unixLibLoader = new UnixLibLoader;
  std::string lib_name = std::string("./test.so");

  Test*(*createObj)() = unixLibLoader->getCreator<Test* (*)()>(lib_name, "createTest");
  Test *test = createObj();
  test->sayHello();

  unixLibLoader->close();


  *
  */

class UnixLibLoader : public ILibLoader
 {
 private:
   void		*m_openedLib;

public:
  UnixLibLoader();
  ~UnixLibLoader();

private:
  UnixLibLoader(const UnixLibLoader &);
  UnixLibLoader &operator=(const UnixLibLoader &);

protected:
  virtual void	open(std::string const &libPath) throw(ClassException);
  virtual void	*findSymbol(std::string const &symbol) throw(ClassException);

public:
  virtual void	close();
};

#endif /* !UNIXLIBLOADER_H_ */
