//
//  GreenRupeePickup.cpp
//  GameDev2D
//
//  Student:    Joel Cright & Rachel Joannis
//  Creation Date: Fall 2014
//  Course Number: GAM1540
//  Professor: Bradley Flood
//  Purpose:    To pick up Green Rupees
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "GreenRupeePickup.h"
#include "../../Source/UI/UI.h"

namespace GameDev2D
{
    GreenRupeePickup::GreenRupeePickup() : Pickup("GreenRupeePickup")
    {
        Sprite* sprite = new Sprite("MainAtlas", "Rupee-Green");
        sprite->SetAnchorPoint(0.5f, 0.5f);
        AddChild(sprite, true);
        //TODO: Make all Rupees inherit from Rupee pickup instead of pickup
    }

    GreenRupeePickup::~GreenRupeePickup()
    {

    }

    PickupType GreenRupeePickup::GetPickupType()
    {
        return PickupTypeGreenRupee;
    }
}