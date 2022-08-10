#ifndef WATCHY_STEVIE_H
#define WATCHY_STEVIE_H

#include <Watchy.h>
#include "tetris.h"
#include "moon_phases.h"
#include "DSEG7_Classic_Regular_15.h"

class WatchyStevie : public Watchy
{
public:
    using Watchy::Watchy;
    void drawWatchFace();
    void drawDate();
    void drawHand(uint8_t _radius_, float _angle_, bool _light_);
    void drawHand2(uint8_t _radius_, float _angle_, bool _light_);

    void button1();
    void button2();

    void drawWatchFace_moonPhases();
    void drawWatchFace_default();
};

#endif