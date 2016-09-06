
# include               "AudioManager.hh"
# include               "PortAudio.hh"
# include               "OpusCodec.hh"


// The functions on this ctor might throw an AudioException.
AudioManager::          AudioManager()
    : QObject(NULL)
{
    this->_sys_audio = new PortAudio(48000);
    this->_sys_audio->init();
    this->_run = false;

    this->_codec = new OpusCodec();
    this->_codec->init(48000);
}

AudioManager::          ~AudioManager()
{
    this->_listen_lock.lock();
    while (this->_stack_listen.empty() == false)
    {
        AudioData       *toDel = this->_stack_listen.front();
        this->_stack_listen.pop();
        delete toDel;
    }
    this->_listen_lock.unlock();
    this->_playback_lock.lock();
    while (this->_stack_playback.empty() == false)
    {
        AudioData       *toDel = this->_stack_playback.front();
        this->_stack_playback.pop();
        delete toDel;
    }
    this->_playback_lock.unlock();

    if (this->_sys_audio)
        delete this->_sys_audio;
    if (this->_codec)
        delete this->_codec;
}

// Threaded method
void                    AudioManager::  operator()()
{
    this->_run = true;

    // Start the lib audio.
    this->_sys_audio->start();

    while (this->checkRun())
    {
        AudioData   *listen = NULL;
        AudioData   *playback = NULL;

        // Read the stream.
        try {
            this->_sys_audio->listen(listen);
            // Store data.
            this->_listen_lock.lock();
            this->_stack_listen.push(listen);
            this->_listen_lock.unlock();
        } catch (AudioException &e) {
            this->speak(e.what());
        }
        emit  somethingToListen();
        // Get playback data
        this->_playback_lock.lock();
        if (this->_stack_playback.empty() == false)
        {
            playback = this->_stack_playback.front();
            this->_stack_playback.pop();
        }
        this->_playback_lock.unlock();

        // Write on the stream.
        if (playback != NULL)
        {
            try {
                this->_sys_audio->playback(playback);
                // delete playback->in; // TODO
                // delete playback->out; // TODO
                delete playback;
            } catch (AudioException &e) {
                this->speak(e.what());
            }
        }
    }

    /* Clear the stacks
     * Here _run == false, no need to lock the mutex.
     */
    while (this->_stack_listen.empty() == false)
    {
        AudioData   *toDel = this->_stack_listen.front();
        this->_stack_listen.pop();
        delete toDel;
    }
    while (this->_stack_playback.empty() == false)
    {
        AudioData   *toDel = this->_stack_playback.front();
        this->_stack_playback.pop();
        delete toDel;
    }
    // Stop the lib audio
    this->_sys_audio->stop();
}

void                    AudioManager:: stop()
{
    this->_run_lock.lock();
    this->_run = false;
    this->_run_lock.unlock();
}

bool                    AudioManager:: checkRun()
{
    bool                status;

    this->_run_lock.lock();
    status = this->_run;
    this->_run_lock.unlock();

    return (status);
}

bool                    AudioManager:: toListen(AudioData *&data)
{
    if (this->checkRun() == false)
        return (false);
    this->_listen_lock.lock();
    if (this->_stack_listen.empty())
    {
        this->_listen_lock.unlock();
        return (false);
    }
    data = this->_stack_listen.front();
    this->_stack_listen.pop();
    this->_listen_lock.unlock();

    try {
        this->_codec->encode(data);
    } catch (AudioException &e) {
        this->speak(e.what());
        return (false);
    }
    return (true);
}

bool                    AudioManager:: toPlayback(AudioData *&data)
{
    if (this->checkRun() == false)
        return (false);
    try {
        this->_codec->decode(data);
    } catch (AudioException &e) {
        this->speak(e.what());
        return (false);
    }
    this->_playback_lock.lock();
    this->_stack_playback.push(data);
    this->_playback_lock.unlock();
    return (true);
}

void                    AudioManager:: speak(std::string const &msg)
{
    std::cout << "[AudioManager] ~> " << msg << "." << std::endl;
}

AudioManager::          AudioManager(AudioManager const &oth)
{
    this->_sys_audio = oth._sys_audio;
}

AudioManager&           AudioManager:: operator=(AudioManager const &oth)
{
    if (this != &oth)
        this->_sys_audio = oth._sys_audio;
    return (*this);
}
