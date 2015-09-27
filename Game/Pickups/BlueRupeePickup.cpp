//
//  BlueRupeePickup.cpp
//  GameDev2D
//
//  Student:    Joel Cright & Rachel Joannis
//  Creation Date: Fall 2014
//  Course Number: GAM1540
//  Professor: Bradley Flood
//  Purpose:    To pick up Blue Rupees
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "BlueRupeePickup.h"
#include "../../Source/UI/UI.h"

namespace GameDev2D
{
    BlueRupeePickup::BlueRupeePickup() : Pickup("BlueRupeePickup")
    {
        Sprite* sprite = new Sprite("MainAtlas", "Rupee-Blue");
        sprite->SetAnchorPoint(0.5f, 0.5f);
        AddChild(sprite, true);
    }

    BlueRupeePickup::~BlueRupeePickup()
    {

    }

    PickupType BlueRupeePickup::GetPickupType()
    {
        return PickupTypeBlueRupee;
    }
}