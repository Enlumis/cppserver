
# include       "PortAudio.hh"

int const PortAudio::FRAME_PER_BUFFER = 2880;
int const PortAudio::NB_CHANNEL = 1;

PortAudio::     PortAudio(int const &rate)
    : _init(false)
    , _run(false)
    , _stream(NULL)
    , _rate(rate)
{

}

PortAudio::     ~PortAudio()
{
    if (this->_init == true)
    {
        PaError     status;
        if (this->_run == true)
            if ((status = Pa_StopStream(this->_stream)) != paNoError)
                this->speak("~ : " + std::string(Pa_GetErrorText(status)), true);
        if ((status = Pa_Terminate()) != paNoError)
            this->speak("~ : " + std::string(Pa_GetErrorText(status)), true);
    }
    this->speak("Terminated");
}

void            PortAudio:: init()
{
    PaError     status;

    if ((status = Pa_Initialize()) != paNoError)
        throw PortAudioException("init : " + std::string(Pa_GetErrorText(status)));

    // Listen Params.
    if ((this->_listenParams.device = Pa_GetDefaultInputDevice()) == paNoDevice)
      {
        throw PortAudioException("init : No default input device detected");
      }
    this->_listenParams.channelCount = PortAudio::NB_CHANNEL;
    this->_listenParams.sampleFormat = paFloat32;
    this->_listenParams.suggestedLatency
        = Pa_GetDeviceInfo(this->_listenParams.device)->defaultHighInputLatency;
    this->_listenParams.hostApiSpecificStreamInfo = NULL;

    // Playback Params.
    if ((this->_playbackParams.device = Pa_GetDefaultOutputDevice()) == paNoDevice)
        throw PortAudioException("init : No default output device detected");
    this->_playbackParams.channelCount = PortAudio::NB_CHANNEL;
    this->_playbackParams.sampleFormat = paFloat32;
    this->_playbackParams.suggestedLatency
        = Pa_GetDeviceInfo(this->_playbackParams.device)->defaultHighOutputLatency;
    this->_playbackParams.hostApiSpecificStreamInfo = NULL;
    // Open the stream.
    status = Pa_OpenStream(&this->_stream
                         , &this->_listenParams
                         , &this->_playbackParams
                         , this->_rate
                         , PortAudio::FRAME_PER_BUFFER
                         , paClipOff
                         , NULL
                         , NULL);

    if (status != paNoError)
        throw PortAudioException("init : " + std::string(Pa_GetErrorText(status)));

    this->_init = true;
    this->speak("Initialized");
}

void            PortAudio:: listen(AudioData *&data)
{
    if (this->_run == false)
        throw PortAudioException("listen : The stream should be started");

    PaError     status;

    data = new AudioData;
    data->frequency = this->_rate;
    data->in = this->getSampleBlock();
    data->in_size = PortAudio::FRAME_PER_BUFFER;

    if ((status = Pa_ReadStream(this->_stream, data->in, data->in_size)) != paNoError && 0)
        throw PortAudioException("listen : " + std::string(Pa_GetErrorText(status)));
    data->out = this->getSampleBlock();
    data->out_size = PortAudio::FRAME_PER_BUFFER;

    // this->speak("Listen");
}

void            PortAudio:: playback(AudioData *&data)
{
    if (this->_run == false)
        throw PortAudioException("playback : The stream should be started");

    PaError     status;

    if ((status = Pa_WriteStream(this->_stream, data->out, data->out_size)) != paNoError && 0)
        throw PortAudioException("playback : " + std::string(Pa_GetErrorText(status)));

    // this->speak("Playback");
}

void            PortAudio:: start()
{
    if (this->_init == false)
        throw PortAudioException("start : PortAudio should be initialized");

    PaError     status;

    if ((status = Pa_StartStream(this->_stream)) != paNoError)
        throw PortAudioException("start : _stream : " + std::string(Pa_GetErrorText(status)));
    this->_run = true;
    this->speak("Started");
}

void            PortAudio:: stop()
{
    if (this->_run == true)
    {
        PaError status;

        this->_run = false;
        if ((status = Pa_StopStream(this->_stream)) != paNoError)
            throw PortAudioException("stop : " + std::string(Pa_GetErrorText(status)));
    }
    this->speak("Stopped");
}

void            PortAudio:: speak(std::string const &msg, bool const &error)
{
    if (error == false)
        std::cout << "[PortAudio] ~> " << msg << "." << std::endl;
    else
        std::cerr << "[PortAudio] {Error} ~> " << msg << "." << std::endl;
}

unsigned char   *PortAudio:: getSampleBlock() const
{
    return new unsigned char[PortAudio::FRAME_PER_BUFFER
                            * PortAudio::NB_CHANNEL
                            * Pa_GetSampleSize(paFloat32)];
}
