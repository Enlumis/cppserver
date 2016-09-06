# ifndef		IAUDIOCODEC_HH_
# define		IAUDIOCODEC_HH_

# include       "AudioData.hh"

class			IAudioCodec
{
public:
  virtual void		init(int const &rate) = 0;
  virtual void		encode(AudioData *&data) = 0;
  virtual void		decode(AudioData *&data) = 0;

public:
  virtual		~IAudioCodec()	{};
};

# endif			/* !IAUDIOCODEC_HH_ */
