#pragma once
//
//  ExplodingBomb.h
//  GameDev2D
//
//  Student:    Joel Cright & Rachel Joannis
//  Creation Date: Fall 2014
//  Course Number: GAM1540
//  Professor: Bradley Flood
//  Purpose:    To drop exploding bombs
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//


#ifndef __GameDev2D__ExplodingBomb__
#define __GameDev2D__ExplodingBomb__

#include "Pickup.h"

namespace GameDev2D{

    class World;
    class Tile;
    class Audio;

    const int BOMB_DAMAGE = 1;
    const double BOMB_EXPLOSION_DELAY = 1.0;

    class ExplodingBomb : public Pickup
    {
    public:
        ExplodingBomb(World* world);
        ~ExplodingBomb();

        PickupType GetPickupType();

    private:
        void ExplodeBomb();
        void RemoveBomb();

        void DestroyEverything();

        World* m_World;
        Audio* m_BombDropAudio;
        Audio* m_BombExplodeAudio;
    };
}
#endif /* defined(__GameDev2D__ExplodingBomb__) */