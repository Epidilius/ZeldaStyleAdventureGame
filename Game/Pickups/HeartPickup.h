//
//  HeartPickup.h
//  GameDev2D
//
//  Student:    Joel Cright & Rachel Joannis
//  Creation Date: Fall 2014
//  Course Number: GAM1540
//  Professor: Bradley Flood
//  Purpose:    To pick up Hearts
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__HeartPickup__
#define __GameDev2D__HeartPickup__

#include "Pickup.h"

namespace GameDev2D
{
    class HeartPickup : public Pickup
    {
    public:
        HeartPickup();
        ~HeartPickup();

        PickupType GetPickupType();
    };
}

#endif /* defined(__GameDev2D__HeartPickup__) */