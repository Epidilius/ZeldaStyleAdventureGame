//
//  SandTile.h
//  GameDev2D
//
//  Student:    Joel Cright & Rachel Joannis
//  Cretion Date: Fall 2014
//  Course Number: GAM1540
//  Professor: Bradley Flood
//  Purpose:    To create sand. And DESERTS! Not desserts.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "SandTile.h"
#include "../../Source/UI/UI.h"


namespace GameDev2D
{
    SandTile::SandTile(SubSection* aSubSection, uvec2 aCoordinates, unsigned int aVariant) : Tile("SandTile", aSubSection, aCoordinates)
    {
        //Set the variant
        SetVariant(aVariant);
        
        //Set that the Sand tile IS walkable
        SetIsWalkable(true);
    }
    
    SandTile::~SandTile()
    {
    
    }
    
    TileType SandTile::GetTileType()
    {
        return TileTypeSand;
    }
    
    unsigned int SandTile::GetVariantCount()
    {
        return SAND_VARIANT_COUNT;
    }
    
    void SandTile::GetAtlasKeyForVariant(unsigned int aVariant, string& aAtlasKey)
    {
        switch (aVariant)
        {
            case 0:
                aAtlasKey = "Sand";
                break;

            default:
                break;
        }
    }
}