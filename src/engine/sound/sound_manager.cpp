#include "sound_manager.hpp"

std::vector<SoundHandler> SoundManager::_buffers;

void SoundHandler::update()
{
    living_sounds.remove_if( [](sf::Sound& s){return s.getStatus() == sf::Sound::Stopped;} );
}

size_t SoundManager::registerSound(const std::string& filename, size_t max_sounds)
{
    _buffers.push_back(SoundHandler());
    SoundHandler& handler = _buffers.back();
    handler.sound_buffer.loadFromFile(filename);
    handler.max_living_sounds = max_sounds;

    return _buffers.size()-1;
}

void SoundManager::playInstanceOf(size_t sound_ID, float volume, sf::Vector2f position)
{
    SoundHandler& handler = _buffers[sound_ID];
    std::list<sf::Sound>& sound_list(handler.living_sounds);
    if (sound_list.size() < handler.max_living_sounds)
    {
        sound_list.emplace_back();
        sf::Sound& new_sound = sound_list.back();
        new_sound.setBuffer(_buffers[sound_ID].sound_buffer);
        new_sound.setVolume(volume);
        new_sound.setPosition({ position.x, position.y, 3.0f });
        new_sound.play();
    }
}

sf::Sound SoundManager::getInstanceOf(size_t soundID)
{
    sf::Sound sound(_buffers[soundID].sound_buffer);
    return sound;
}

void SoundManager::update()
{
    for (SoundHandler& sh : _buffers) {
        sh.update();
    }
}
