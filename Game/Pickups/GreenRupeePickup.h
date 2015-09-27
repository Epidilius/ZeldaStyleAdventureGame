//
//  GreenRupeePickup.h
//  GameDev2D
//
//  Student:    Joel Cright & Rachel Joannis
//  Creation Date: Fall 2014
//  Course Number: GAM1540
//  Professor: Bradley Flood
//  Purpose:    To pick up Green Rupees
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__GreenRupeePickup__
#define __GameDev2D__GreenRupeePickup__

#include "Pickup.h"

namespace GameDev2D
{
    class GreenRupeePickup : public Pickup
    {
    public:
        GreenRupeePickup();
        ~GreenRupeePickup();

        PickupType GetPickupType();
    };
}

#endif /* defined(__GameDev2D__GreenRupeePickup__) */