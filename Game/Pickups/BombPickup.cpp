//
//  BombPickup.cpp
//  GameDev2D
//
//  Student:    Joel Cright & Rachel Joannis
//  Creation Date: Fall 2014
//  Course Number: GAM1540
//  Professor: Bradley Flood
//  Purpose:    To pick up bombs
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "BombPickup.h"
#include "../../Source/UI/UI.h"

//TODO: Maybe have an items folder, with bomb being an item?
namespace GameDev2D
{
    BombPickup::BombPickup() : Pickup("BombPickup")
    {
        Sprite* sprite = new Sprite("MainAtlas", "Bomb");
        sprite->SetAnchorPoint(0.5f, 0.5f);
        AddChild(sprite, true);
    }


    BombPickup::~BombPickup()
    {
        
    }


    PickupType BombPickup::GetPickupType()
    {
        return PickupTypeBomb;
    }

}
