#include	"UserService.hh"

#ifdef WIN32
extern "C" __declspec(dllexport)	UserService	*creatorService()
{
  return new UserService;
}
#else
extern "C"	UserService	*creatorService()
{
  return new UserService;
}
#endif