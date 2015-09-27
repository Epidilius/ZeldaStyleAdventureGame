#pragma once
//
// BombPickup.h
//  GameDev2D
//
//  Student:    Joel Cright & Rachel Joannis
//  Creation Date: Fall 2014
//  Course Number: GAM1540
//  Professor: Bradley Flood
//  Purpose:    To pick up bombs
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//


#ifndef __GameDev2D__BombPickup__
#define __GameDev2D__BombPickup__

#include "Pickup.h"

namespace GameDev2D{

    class BombPickup : public Pickup
    {
    public:
        BombPickup();
        ~BombPickup();

        PickupType GetPickupType();
    };
}
#endif /* defined(__GameDev2D__BombPickup__) */