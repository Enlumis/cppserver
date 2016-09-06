
# include   "AudioManager.hh"
# include   "PortAudio.hh"

# include   "AudioData.hh"

int         main()
{
    try {
        AudioManager _amanager;
        std::thread t(std::ref(_amanager));

        while (0x42)
        {
            AudioData *data = NULL;

            if (_amanager.toListen(data) == true)
            {
                AudioData *play = new AudioData;
                play->in = data->out;
                play->in_size = data->out_size;
                play->out = data->in;
                play->out_size = data->in_size;
                play->frequency = data->frequency;
                _amanager.toPlayback(play);
                delete data;
            }
        }
        t.join();
    } catch (AudioException &e) {
        std::cout << e.what() << std::endl;
        return (1);
    }
    return (0);
}