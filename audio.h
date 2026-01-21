#pragma once
#include "raylib.h"
#include "globals.h"

class AudioManager {
public:
    Sound eatSound;
    Sound gameOverSound;
    Sound clickSound;
    Music backgroundMusic;
    
    bool soundsLoaded;
    bool musicLoaded;

    AudioManager() {
        soundsLoaded = false;
        musicLoaded = false;
    }

    void Init() {
        InitAudioDevice();
        LoadSounds();
    }

    void LoadSounds() {
        if (FileExists("Sounds/eat.wav")) {
            eatSound = LoadSound("Sounds/eat.wav");
            soundsLoaded = true;
        }
        if (FileExists("Sounds/gameover.wav")) {
            gameOverSound = LoadSound("Sounds/gameover.wav");
        }
        if (FileExists("Sounds/click.wav")) {
            clickSound = LoadSound("Sounds/click.wav");
        }
        if (FileExists("Sounds/music.wav")) {
            backgroundMusic = LoadMusicStream("Sounds/music.wav");
            musicLoaded = true;
            UpdateMusicVolume();
        }
    }

    float GetVolumeLevel() {
        return gameSettings.soundVolumeIndex * 0.25f;
    }

    void UpdateMusicVolume() {
        if (musicLoaded) {
            SetMusicVolume(backgroundMusic, GetVolumeLevel() * 0.3f);
        }
    }

    void PlayEatSound() {
        if (gameSettings.soundVolumeIndex > 0 && soundsLoaded) {
            SetSoundVolume(eatSound, GetVolumeLevel());
            PlaySound(eatSound);
        }
    }

    void PlayGameOverSound() {
        if (gameSettings.soundVolumeIndex > 0 && soundsLoaded) {
            SetSoundVolume(gameOverSound, GetVolumeLevel());
            PlaySound(gameOverSound);
        }
    }

    void PlayClickSound() {
        if (gameSettings.soundVolumeIndex > 0 && soundsLoaded) {
            SetSoundVolume(clickSound, GetVolumeLevel());
            PlaySound(clickSound);
        }
    }

    void PlayMusic() {
        if (gameSettings.soundVolumeIndex > 0 && musicLoaded) {
            if (!IsMusicStreamPlaying(backgroundMusic)) {
                PlayMusicStream(backgroundMusic);
            }
        }
    }

    void StopMusic() {
        if (musicLoaded && IsMusicStreamPlaying(backgroundMusic)) {
            StopMusicStream(backgroundMusic);
        }
    }

    void UpdateMusic() {
        if (musicLoaded) {
            UpdateMusicStream(backgroundMusic);
            UpdateMusicVolume();
            
            if (gameSettings.soundVolumeIndex > 0) {
                if (!IsMusicStreamPlaying(backgroundMusic)) {
                    PlayMusicStream(backgroundMusic);
                }
            } else {
                if (IsMusicStreamPlaying(backgroundMusic)) {
                    StopMusicStream(backgroundMusic);
                }
            }
        }
    }

    void Cleanup() {
        if (soundsLoaded) {
            UnloadSound(eatSound);
            UnloadSound(gameOverSound);
            UnloadSound(clickSound);
        }
        if (musicLoaded) {
            UnloadMusicStream(backgroundMusic);
        }
        CloseAudioDevice();
    }
};
