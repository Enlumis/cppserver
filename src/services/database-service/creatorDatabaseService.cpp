#include	"FileDatabase.hh"

#ifdef WIN32
extern "C" __declspec(dllexport)	FileDatabase	*creatorService()
{
  return new FileDatabase;
}
#else
extern "C"	FileDatabase	*creatorService()
{
  return new FileDatabase;
}
#endif
