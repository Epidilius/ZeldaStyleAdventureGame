//
//  SwordPickup.cpp
//  GameDev2D
//
//  Student:    Joel Cright & Rachel Joannis
//  Creation Date: Fall 2014
//  Course Number: GAM1540
//  Professor: Bradley Flood
//  Purpose:    To pick up the sword
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "SwordPickup.h"
#include "../../Source/UI/UI.h"

namespace GameDev2D
{
    SwordPickup::SwordPickup() : Pickup("SwordPickup")
    {
        Sprite* sprite = new Sprite("MainAtlas", "Sword-Pickup");
        sprite->SetAnchorPoint(0.5f, 0.5f);
        AddChild(sprite, true);
    }

    SwordPickup::~SwordPickup()
    {

    }

    PickupType SwordPickup::GetPickupType()
    {
        return PickupTypeSword;
    }
}