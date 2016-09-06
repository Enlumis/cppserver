# ifndef        PORT_AUDIO_HH_
# define        PORT_AUDIO_HH_

# include       <iostream>
# include       <string>

# include       "IAudio.hh"
# include       "PortAudioException.hh"

// Lib Portaudio Include.
extern "C" {
    # include       "portaudio.h"
}

class           PortAudio : public IAudio
{
public:
  static int const FRAME_PER_BUFFER;
  static int const SAMPLE_RATE;
  static int const NB_CHANNEL;

private:
    bool        _init;
    bool        _run;

private:
    PaStream    *_stream;
    PaStreamParameters _listenParams;
    PaStreamParameters _playbackParams;
    unsigned int _rate;

public:
    PortAudio(int const &rate);
    ~PortAudio();

private:
    PortAudio(PortAudio const &);
    PortAudio&  operator=(PortAudio const &);

public:
    void            init();
    void            listen(AudioData *&);
    void            playback(AudioData *&);
    void            start();
    void            stop();

private:
    unsigned char   *getSampleBlock() const;

private:
    void        speak(std::string const &msg, bool const &error = false);
};

# endif     /* !PORT_AUDIO_HH_ */
