#pragma once

#include <SFML/Audio.hpp>
#include <string>
#include <list>

struct SoundHandler
{
    size_t max_living_sounds;
    sf::SoundBuffer sound_buffer;
    std::list<sf::Sound> living_sounds;

    void update();
};

class SoundManager
{
public:
    static void      playInstanceOf(size_t soundID, float volume = 100.0f, sf::Vector2f position = {});
    static size_t    registerSound(const std::string& filename, size_t maxSounds = 10);
    static sf::Sound getInstanceOf(size_t soundID);
    static void      update();

private:
    static std::vector<SoundHandler> _buffers;
};
