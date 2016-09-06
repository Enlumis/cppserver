# ifndef	    OPUSCODEC_HH_
# define      OPUSCODEC_HH_

# include     <opus.h>
# include     "IAudioCodec.hh"

class		       OpusCodec : public IAudioCodec
{
private:
  unsigned int _encoder_bitrate;
  unsigned int _encoder_rate;
  unsigned int _decoder_rate;

public:
  OpusCodec();
  ~OpusCodec();

public:
  void		      init(int const &rate);
  void		      encode(AudioData *&);
  void		      decode(AudioData *&);

private:
  void          checkDecoderCtl(AudioData *&);

private:
  OpusEncoder	  *_encoder;
  OpusDecoder	  *_decoder;
  int		         error;

public:
  static int const		NB_CHANNELS;
};

#endif		      /* !OPUSCODEC_HH_ */
