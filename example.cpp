#include "wavgen.h"

int main() {
    WavGen wav("test.wav");
    wav.addSineWave(400, 0.5, 0.5);
    wav.addSineWave(600, 0.1, 0.5);
    wav.done();
    return 0;
}