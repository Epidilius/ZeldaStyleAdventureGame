//
//  BoulderTile.cpp
//  GameDev2D
//
//  Student:    Joel Cright & Rachel Joannis
//  Cretion Date: Fall 2014
//  Course Number: GAM1540
//  Professor: Bradley Flood
//  Purpose:    To create boulders
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "BoulderTile.h"
#include "../../Source/UI/UI.h"


namespace GameDev2D
{
    BoulderTile::BoulderTile(SubSection* aSubSection, uvec2 aCoordinates, unsigned int aVariant) : Tile("BoulderTile", aSubSection, aCoordinates)
    {
        //Set the variant
        SetVariant(aVariant);
        
        //Set that the Boulder tile is NOT walkable
        SetIsWalkable(false);
    }
    
    BoulderTile::~BoulderTile()
    {
    
    }
    
    TileType BoulderTile::GetTileType()
    {
        return TileTypeBoulder;
    }
    
    unsigned int BoulderTile::GetVariantCount()
    {
        return BOULDER_VARIANT_COUNT;   //TODO: Make these constants
    }
    
    void BoulderTile::GetAtlasKeyForVariant(unsigned int aVariant, string& aAtlasKey)
    {
        switch (aVariant)
        {
            case 0:
                aAtlasKey = "Boulder-Dark";
                break;

            case 1:
                aAtlasKey = "Boulder-Light";
                break;

            default:
                break;
        }
    }
}