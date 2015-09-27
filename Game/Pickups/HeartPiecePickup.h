//
//  HeartPiecePickup.h
//  GameDev2D
//
//  Student:    Joel Cright & Rachel Joannis
//  Creation Date: Fall 2014
//  Course Number: GAM1540
//  Professor: Bradley Flood
//  Purpose:    To pick up Hearts Pieces
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__HeartPiecePickup__
#define __GameDev2D__HeartPiecePickup__

#include "Pickup.h"

namespace GameDev2D
{
    class HeartPiecePickup : public Pickup
    {
    public:
        HeartPiecePickup();
        ~HeartPiecePickup();

        PickupType GetPickupType();
    };
}

#endif /* defined(__GameDev2D__HeartPiecePickup__) */