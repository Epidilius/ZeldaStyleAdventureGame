//
//  BridgeTile.cpp
//  GameDev2D
//
//  Student:    Joel Cright & Rachel Joannis
//  Cretion Date: Fall 2014
//  Course Number: GAM1540
//  Professor: Bradley Flood
//  Purpose:    To create bridges
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "BridgeTile.h"
#include "../../Source/UI/UI.h"


namespace GameDev2D
{
    BridgeTile::BridgeTile(SubSection* aSubSection, uvec2 aCoordinates, unsigned int aVariant) : Tile("BridgeTile", aSubSection, aCoordinates)
    {
        //Set the variant
        SetVariant(aVariant);
        
        //Set that the Bridge tile IS walkable
        SetIsWalkable(true);
    }
    
    BridgeTile::~BridgeTile()
    {
    
    }
    
    TileType BridgeTile::GetTileType()
    {
        return TileTypeBridge;
    }
    
    unsigned int BridgeTile::GetVariantCount()
    {
        return BRIDGE_VARIANT_COUNT;
    }
    
    void BridgeTile::GetAtlasKeyForVariant(unsigned int aVariant, string& aAtlasKey)
    {
        switch (aVariant)
        {
            case 0:
                aAtlasKey = "Bridge-1";
                break;

            case 1:
                aAtlasKey = "Bridge-2";
                break;

            default:
                break;
        }
    }
}