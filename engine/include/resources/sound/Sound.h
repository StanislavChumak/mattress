#ifndef SOUND_H
#define SOUND_H

typedef struct ma_engine ma_engine;
typedef struct ma_sound ma_sound;

typedef unsigned int ma_uint32;

class Sound
{
    ma_sound *_thisSound;
public:
    enum Flag : ma_uint32 {
        NONE               = 0b0,
        STREAM             = 0b1,
        LOOP               = 0b10000,
        NO_SPATIALIZATION  = 0b100000000000000
    };

    Sound(ma_engine *engine, const char *path, ma_uint32 flag);
    ~Sound();

    void play();
    void stop();
    void set_volume(float volume);
    void set_position(float x, float y, float z = 0.0f);

    bool is_playing() const;
};



#endif
