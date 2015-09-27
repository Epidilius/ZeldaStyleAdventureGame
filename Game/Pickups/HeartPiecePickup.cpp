//
//  HeartPiecePickup.cpp
//  GameDev2D
//
//  Student:    Joel Cright & Rachel Joannis
//  Creation Date: Fall 2014
//  Course Number: GAM1540
//  Professor: Bradley Flood
//  Purpose:    To pick up Hearts Pieces
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "HeartPiecePickup.h"
#include "../../Source/UI/UI.h"

namespace GameDev2D
{
    HeartPiecePickup::HeartPiecePickup() : Pickup("HeartPiecePickup")
    {
        Sprite* sprite = new Sprite("MainAtlas", "HeartPiece");
        sprite->SetAnchorPoint(0.5f, 0.5f);
        AddChild(sprite, true);
    }

    HeartPiecePickup::~HeartPiecePickup()
    {

    }

    PickupType HeartPiecePickup::GetPickupType()
    {
        return PickupTypeHeartPiece;
    }
}