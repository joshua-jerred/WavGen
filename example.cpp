#include "wavgen.h"

int main() {
    WavGen wav("test.wav");
    wav.addSineWave(200, 1, 0.5);
    wav.addSineWave(600, 1, 0.5);
    wav.done();
    return 0;
}