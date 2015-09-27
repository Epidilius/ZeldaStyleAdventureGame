//
//  RockTile.h
//  GameDev2D
//
//  Student:    Joel Cright & Rachel Joannis
//  Cretion Date: Fall 2014
//  Course Number: GAM1540
//  Professor: Bradley Flood
//  Purpose:    To create rocks
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__RockTile__
#define __GameDev2D__RockTile__

#include "Tile.h"


namespace GameDev2D
{
    //Constants
    const unsigned short ROCK_VARIANT_COUNT = 12;
    
    //Forward Declarations
    class SubSection;

    //Rock Tile class
    class RockTile : public Tile
    {
    public:
        RockTile(SubSection* subSection, uvec2 coordinates, unsigned int variant);
        ~RockTile();
        
        //Returns the type of tile
        TileType GetTileType();
        
        //Returns the number of tile variants
        unsigned int GetVariantCount();
        
        //Gets the atlas key for the tile variant
        void GetAtlasKeyForVariant(unsigned int variant, string& atlasKey);
    };
}

#endif /* defined(__GameDev2D__RockTile__) */
