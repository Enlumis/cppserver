#include	"CallService.hh"

#ifdef WIN32
extern "C" __declspec(dllexport)	CallService	*creatorService()
{
  return new CallService;
}
#else
extern "C"	CallService	*creatorService()
{
  return new CallService;
}
#endif
