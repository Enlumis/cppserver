
# ifndef            AUDIO_DATA_HH_
# define            AUDIO_DATA_HH_

typedef struct      AudioData {
    unsigned char   *in;
    unsigned char   *out;
    unsigned short  in_size;
    unsigned short  out_size;
    unsigned short  frequency;
}                   AudioData;

# endif         /* !AUDIO_DATA_HH_ */