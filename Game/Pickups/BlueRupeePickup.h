//
//  BlueRupeePickup.h
//  GameDev2D
//
//  Student:    Joel Cright & Rachel Joannis
//  Creation Date: Fall 2014
//  Course Number: GAM1540
//  Professor: Bradley Flood
//  Purpose:    To pick up Blue Rupees
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__BlueRupeePickup__
#define __GameDev2D__BlueRupeePickup__

#include "Pickup.h"

namespace GameDev2D
{
    class BlueRupeePickup : public Pickup
    {
    public:
        BlueRupeePickup();
        ~BlueRupeePickup();

        PickupType GetPickupType();
    };
}

#endif /* defined(__GameDev2D__BlueRupeePickup__) */