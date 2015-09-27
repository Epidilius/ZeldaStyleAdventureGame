//
//  WaterTile.cpp
//  Algonquin College
//
//  Students: Joel Cright & Rachel Joannis
//  Creation Date: Fall 2014
//  Course Number: GAM1540
//  Professor:  Bradley Flood
//  Purpose: To create water tiles.
//

#include "WaterTile.h"
#include "../../Source/UI/UI.h"


namespace GameDev2D
{
    WaterTile::WaterTile(SubSection* aSubSection, uvec2 aCoordinates, unsigned int aVariant) : Tile("WaterTile", aSubSection, aCoordinates)
    {
        //Set the variant
        SetVariant(aVariant);
        
        //Set that the Water tile is NOT walkable
        SetIsWalkable(false);
    }
    
    WaterTile::~WaterTile()
    {
    
    }
    
    TileType WaterTile::GetTileType()
    {
        return TileTypeWater;
    }
    
    unsigned int WaterTile::GetVariantCount()
    {
        return WATER_VARIANT_COUNT;
    }
    
    void WaterTile::GetAtlasKeyForVariant(unsigned int aVariant, string& aAtlasKey)
    {
        switch (aVariant)
        {
            case 0:
                aAtlasKey = "Water";
                break;

            case 1:
                aAtlasKey = "WaterEdge-1";
                break;

            case 2:
                aAtlasKey = "WaterEdge-2";
                break;

            case 3:
                aAtlasKey = "WaterEdge-3";
                break;

            case 4:
                aAtlasKey = "WaterEdge-4";
                break;

            case 5:
                aAtlasKey = "WaterEdge-5";
                break;

            case 6:
                aAtlasKey = "WaterEdge-6";
                break;

            case 7:
                aAtlasKey = "WaterEdge-7";
                break;

            case 8:
                aAtlasKey = "WaterEdge-8";
                break;

            default:
                break;
        }
    }
}