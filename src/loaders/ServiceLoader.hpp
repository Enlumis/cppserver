#pragma once

#include <vector>
#include "services/IService.hh"
#include "loaders/ILibLoader.hh"
#ifdef WIN32
#include "loaders/Win32LibLoader.hh"
#else
#include "loaders/UnixLibLoader.hh"
#endif

class ServiceLoader
{
public:
  ServiceLoader(const std::string &libPath) : _libPath(libPath)
  {
  }
  ~ServiceLoader() {
    std::vector<std::pair<IService * , ILibLoader * > >::iterator it = this->_servicesLoaded.begin();
    while (it != this->_servicesLoaded.end())
      {
	std::pair<IService * , ILibLoader * > pair = *it;
	delete pair.first;
	delete pair.second;
	++it;
      }
  }

  template<typename T>
  T *loadService(const std::string &serviceName) throw(ClassException)
  {
    ILibLoader *loader;
    std::string sharedName(this->_libPath);

#ifdef WIN32
    loader = new Win32LibLoader();
    sharedName += "/" + serviceName;
    sharedName += ".dll";
#else
    loader = new UnixLibLoader();
    sharedName += "/lib" + serviceName;
    sharedName += ".so";
#endif

    std::cout << "[Service Loader] : load " << serviceName << std::endl;

    loader->open(sharedName);
    T*(*createObj)() = reinterpret_cast<T* (*)()>(loader->findSymbol("creatorService"));

    T *service = createObj();
    this->_servicesLoaded.push_back(std::pair<IService * , ILibLoader * >(reinterpret_cast<IService *>(service), loader));
    return service;
  }
private:
  std::vector<std::pair<IService * , ILibLoader * > > _servicesLoaded;
  std::string _libPath;
};
