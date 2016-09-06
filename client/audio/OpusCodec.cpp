# include	<string>
# include	"OpusCodec.hh"
# include	"OpusException.hh"

# include <iostream>

// A changer ! Maybe put it on the AudioManager, or send it on the ctor.
int const		OpusCodec::NB_CHANNELS = 1;

OpusCodec::	OpusCodec()
  : _encoder_bitrate(64000)
  , _encoder_rate(0)
  , _decoder_rate(0)
  , _encoder(NULL)
  , _decoder(NULL)
{

}

OpusCodec::	~OpusCodec()
{
  if (this->_encoder != NULL)
    opus_encoder_destroy(this->_encoder);
  if (this->_decoder != NULL)
    opus_decoder_destroy(this->_decoder);
}

void		OpusCodec::init(int const &rate)
{
  this->_encoder_rate = rate;
  this->_decoder_rate = rate;
  this->_encoder = opus_encoder_create(this->_encoder_rate, NB_CHANNELS, OPUS_APPLICATION_VOIP, &this->error);
  if (this->error)
    throw OpusException("init error : " + std::string(opus_strerror(this->error)));
  this->_decoder = opus_decoder_create(this->_decoder_rate, NB_CHANNELS, &this->error);
  if (this->error)
    throw OpusException("init error : " + std::string(opus_strerror(this->error)));
  if ((this->error = opus_encoder_ctl(this->_encoder, OPUS_SET_BITRATE(this->_encoder_bitrate))))
    throw OpusException("init encoder ctl error : " + std::string(opus_strerror(this->error)));
}

void      OpusCodec::checkDecoderCtl(AudioData *&data)
{
  if (this->_decoder_rate != data->frequency)
  {
    this->_decoder_rate = data->frequency;
    opus_decoder_destroy(this->_decoder);
    this->_decoder = opus_decoder_create(this->_decoder_rate, NB_CHANNELS, &this->error);
    if (this->error)
      throw OpusException("ctl error : " + std::string(opus_strerror(this->error)));
  }
}

void		  OpusCodec::encode(AudioData *&data)
{
  int  i = 0;

  if ((i = opus_encode_float(this->_encoder
      , reinterpret_cast<const float *>(data->in)
      , data->in_size
      , data->out
      , data->in_size)) < 0)
    throw OpusException("encode error : " + std::string(opus_strerror(i)));
  data->out_size = i;
  data->in_size = data->in_size * 4;
  // std::cout << "Encode Out size = " << i << std::endl;
}

void		OpusCodec::decode(AudioData *&data)
{
  int   i = 0;

  this->checkDecoderCtl(data);
  if ((i = opus_decode_float(this->_decoder
      , data->in
      , data->in_size
  		,	reinterpret_cast<float *>(data->out)
      , data->out_size, 0)) < 0)
    throw OpusException("decode error : " + std::string(opus_strerror(i)));
  data->out_size = i;
  // std::cout << "Decode Out size = " << i << std::endl;
}
