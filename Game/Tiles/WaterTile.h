//
//  WaterTile.h
//  Algonquin College
//
//  Students: Joel Cright & Rachel Joannis
//  Creation Date: Fall 2014
//  Course Number: GAM1540
//  Professor:  Bradley Flood
//  Purpose: To create water tiles.
//

#ifndef __GameDev2D__WaterTile__
#define __GameDev2D__WaterTile__

#include "Tile.h"


namespace GameDev2D
{
    //Forward Declarations
    class SubSection;

    // Constants
    const unsigned short WATER_VARIANT_COUNT = 9;

    //Water Tile class
    class WaterTile : public Tile
    {
    public:
        WaterTile(SubSection* subSection, uvec2 coordinates, unsigned int variant);
        ~WaterTile();
        
        //Returns the type of tile
        TileType GetTileType();
        
        //Returns the number of tile variants
        unsigned int GetVariantCount();
        
        //Gets the atlas key for the tile variant
        void GetAtlasKeyForVariant(unsigned int variant, string& atlasKey);
    };
}

#endif /* defined(__GameDev2D__WaterTile__) */
