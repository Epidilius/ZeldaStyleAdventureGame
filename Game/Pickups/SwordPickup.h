//
//  SwordPickup.h
//  GameDev2D
//
//  Student:    Joel Cright & Rachel Joannis
//  Creation Date: Fall 2014
//  Course Number: GAM1540
//  Professor: Bradley Flood
//  Purpose:    To pick up the sword
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__SwordPickup__
#define __GameDev2D__SwordPickup__

#include "Pickup.h"

namespace GameDev2D
{
    class SwordPickup : public Pickup
    {
    public:
        SwordPickup();
        ~SwordPickup();

        PickupType GetPickupType();
    };
}

#endif /* defined(__GameDev2D__SwordPickup__) */