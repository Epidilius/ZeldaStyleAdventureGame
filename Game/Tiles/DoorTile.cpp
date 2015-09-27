//
//  DoorTile.h
//  GameDev2D
//
//  Student:    Joel Cright & Rachel Joannis
//  Cretion Date: Fall 2014
//  Course Number: GAM1540
//  Professor: Bradley Flood
//  Purpose:    To create doors
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "DoorTile.h"
#include "../../Source/UI/UI.h"


namespace GameDev2D
{
    DoorTile::DoorTile(SubSection* aSubSection, uvec2 aCoordinates, unsigned int aVariant) : Tile("DoorTile", aSubSection, aCoordinates)
    {
        //Set the variant
        SetVariant(aVariant);
        
        //Set that the Door tile IS walkable
        SetIsWalkable(true);
    }
    
    DoorTile::~DoorTile()
    {
    
    }
    
    TileType DoorTile::GetTileType()
    {
        return TileTypeDoor;
    }
    
    unsigned int DoorTile::GetVariantCount()
    {
        return DOOR_VARIANT_COUNT;
    }
    
    void DoorTile::GetAtlasKeyForVariant(unsigned int aVariant, string& aAtlasKey)
    {
        switch (aVariant)
        {
            case 0:
                aAtlasKey = "Door";
                break;

            default:
                break;
        }
    }
}