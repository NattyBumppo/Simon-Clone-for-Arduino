/*
   melody.h - a class to play various melodies (inspired by the original Super Mario Bros).
 */

#ifndef MELODYPLAYER_H
#define MELODYPLAYER_H

class MelodyPlayer
{
    public:
        MelodyPlayer();
       
        void playTone(int, int);
        void playWinningMelody(int);
        void playLosingMelody();
        void playEasterEggMelody();
        void playFireworks(int);
    private:
        void playMelody(const int[], const int[], const int, const int);

        static const int losingMelodyTempo;
        static const int losingMelodyLength;
        static const int losingMelodyTones[14];
        static const int losingMelodyBeats[14];

        static const int winningMelodyTempo;
        static const int winningMelodyLength;
        static const int winningMelodyTones[27];
        static const int winningMelodyBeats[27];

        static const int easterEggMelodyTempo;
        static const int easterEggMelodyLength;
        static const int easterEggMelodyTones[13];
        static const int easterEggMelodyBeats[13];
};

#endif