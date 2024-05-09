#ifndef sounds_h
#define sounds_h

typedef struct sounds Sounds;

Sounds *createSounds();
void playGameplayMusic(Sounds *pSounds);
void playMenuMusic(Sounds *pSounds);
void playLoseMusic(Sounds *pSounds);
void musicCleanup(Sounds *pSounds);
void playButtonEffect(Sounds *pSounds);
void playThrowEffect(Sounds *pSounds);
void playHitEffect(Sounds *pSounds);
void playWinSound(Sounds *pSounds);

#endif