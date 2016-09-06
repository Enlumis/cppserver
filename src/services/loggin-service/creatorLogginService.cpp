#include	"LogginService.hh"

#ifdef WIN32
extern "C" __declspec(dllexport)	LogginService	*creatorService()
{
  return new LogginService;
}
#else
extern "C"	LogginService	*creatorService()
{
  return new LogginService;
}
#endif
