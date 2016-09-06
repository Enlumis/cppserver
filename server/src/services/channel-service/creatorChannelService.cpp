#include	"ChannelService.hh"

#ifdef WIN32
extern "C" __declspec(dllexport)	ChannelService	*creatorService()
{
  return new ChannelService;
}
#else
extern "C"	ChannelService	*creatorService()
{
  return new ChannelService;
}
#endif
