//
//  SandTile.h
//  GameDev2D
//
//  Student:    Joel Cright & Rachel Joannis
//  Cretion Date: Fall 2014
//  Course Number: GAM1540
//  Professor: Bradley Flood
//  Purpose:    To create sand
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__SandTile__
#define __GameDev2D__SandTile__

#include "Tile.h"


namespace GameDev2D
{
    //Constants
    const unsigned short SAND_VARIANT_COUNT = 1;
    
    //Forward Declarations
    class SubSection;

    //Sand Tile class
    class SandTile : public Tile
    {
    public:
        SandTile(SubSection* subSection, uvec2 coordinates, unsigned int variant);
        ~SandTile();
        
        //Returns the type of tile
        TileType GetTileType();
        
        //Returns the number of tile variants
        unsigned int GetVariantCount();
        
        //Gets the atlas key for the tile variant
        void GetAtlasKeyForVariant(unsigned int variant, string& atlasKey);
    };
}

#endif /* defined(__GameDev2D__SandTile__) */
