
# ifndef                    AUDIO_MANAGER_HH_
# define                    AUDIO_MANAGER_HH_

# include                   "IAudio.hh"
# include                   "IAudioCodec.hh"

# include                   "AudioData.hh"

# include                   <string>
# include                   <queue>
# include                   <thread>
# include                   <mutex>
# include                   <QObject>

class                       AudioManager : public QObject
{
  Q_OBJECT

private:
    std::mutex              _run_lock;
    std::mutex              _listen_lock;
    std::mutex              _playback_lock;

private:
    IAudio                  *_sys_audio;
    IAudioCodec             *_codec;

private:
    bool                    _run;
    std::queue<AudioData*>  _stack_listen;
    std::queue<AudioData*>  _stack_playback;

public:
    void                    operator()();
    void                    stop();

public:
    bool                    toListen(AudioData *&);
    bool                    toPlayback(AudioData *&);
    bool                    checkRun();

public:
    AudioManager();
    ~AudioManager();

public:
    AudioManager(AudioManager const &);
    AudioManager&           operator=(AudioManager const &);

private:
    void                    speak(std::string const &);

signals:
    void                    somethingToListen();

};

# endif                     /* !AUDIO_MANAGER_HH_ */
