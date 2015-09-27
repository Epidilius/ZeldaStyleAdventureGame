//
//  StairsTile.cpp
//  Algonquin College
//
//  Students: Joel Cright & Rachel Joannis
//  Creation Date: Fall 2014
//  Course Number: GAM1540
//  Professor:  Bradley Flood
//  Purpose: To create stairs tiles.
//

#include "StairsTile.h"
#include "../../Source/UI/UI.h"


namespace GameDev2D
{
    StairsTile::StairsTile(SubSection* aSubSection, uvec2 aCoordinates, unsigned int aVariant) : Tile("StairsTile", aSubSection, aCoordinates)
    {
        //Set the variant
        SetVariant(aVariant);
        
        //Set that the Stairs tile IS walkable
        SetIsWalkable(true);
    }
    
    StairsTile::~StairsTile()
    {
    
    }
    
    TileType StairsTile::GetTileType()
    {
        return TileTypeStairs;
    }
    
    unsigned int StairsTile::GetVariantCount()
    {
        return STAIRS_VARIANT_COUNT;
    }
    
    void StairsTile::GetAtlasKeyForVariant(unsigned int aVariant, string& aAtlasKey)
    {
        switch (aVariant)
        {
            case 0:
                aAtlasKey = "Stairs-1";
                break;

            case 1:
                aAtlasKey = "Stairs-2";
                break;

            default:
                break;
        }
    }
}