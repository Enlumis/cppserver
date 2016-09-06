# ifndef                    IAUDIO_HH_
# define                    IAUDIO_HH_

# include                   "AudioData.hh"

class                       IAudio
{
public:
    virtual void            init() = 0;
    virtual void            listen(AudioData *&) = 0;
    virtual void            playback(AudioData *&) = 0;
    virtual void            start() = 0;
    virtual void            stop() = 0;

public:
    virtual                 ~IAudio() { };
};

# endif                     /* !IAUDIO_HH_ */