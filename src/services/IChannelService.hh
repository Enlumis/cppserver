#ifndef ICHANNELSERVICE_H_
# define ICHANNELSERVICE_H_

# include	<cstdint>
# include	"channel-service/Room.hh"
# include	"IService.hh"

class IChannelService : public IService
{
public:
  virtual ~IChannelService() {}

  virtual const Room	&findRoomById(uint32_t roomId) const = 0;
  virtual const bool	roomExistsById(uint32_t roomId) const = 0;
  virtual const std::map<uint32_t, Room>	&getRooms() const = 0;
};

#endif /* !ICHANNELSERVICE_H_ */
