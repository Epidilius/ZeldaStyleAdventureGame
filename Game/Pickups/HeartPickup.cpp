//
//  HeartPickup.cpp
//  GameDev2D
//
//  Student:    Joel Cright & Rachel Joannis
//  Creation Date: Fall 2014
//  Course Number: GAM1540
//  Professor: Bradley Flood
//  Purpose:    To pick up Hearts
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "HeartPickup.h"
#include "../../Source/UI/UI.h"

namespace GameDev2D
{
    HeartPickup::HeartPickup() : Pickup("HeartPickup")
    {
        Sprite* sprite = new Sprite("MainAtlas", "Heart-Full");
        sprite->AddFrame("MainAtlas", "Heart-2");
        sprite->SetDoesLoop(true);
        sprite->SetFrameSpeed(4.0f);
        sprite->SetAnchorPoint(0.5f, 0.5f);
        AddChild(sprite, true);
    }

    HeartPickup::~HeartPickup()
    {

    }

    PickupType HeartPickup::GetPickupType()
    {
        return PickupTypeHeart;
    }
}