//
//  FireTile.h
//  GameDev2D
//
//  Student:    Joel Cright & Rachel Joannis
//  Cretion Date: Fall 2014
//  Course Number: GAM1540
//  Professor: Bradley Flood
//  Purpose:    To create fires
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__FireTile__
#define __GameDev2D__FireTile__

#include "Tile.h"


namespace GameDev2D
{
    //Constants
    const unsigned short FIRE_VARIANT_COUNT = 1;
    
    //Forward Declarations
    class SubSection;

    //Fire Tile class
    class FireTile : public Tile
    {
    public:
        FireTile(SubSection* subSection, uvec2 coordinates, unsigned int variant);
        ~FireTile();
        
        //Returns the type of tile
        TileType GetTileType();

        void UpdateSprite(unsigned int variant);
        
        //Returns the number of tile variants
        unsigned int GetVariantCount();
        
        //Gets the atlas key for the tile variant
        void GetAtlasKeyForVariant(unsigned int variant, string& atlasKey);
    };
}

#endif /* defined(__GameDev2D__FireTile__) */
