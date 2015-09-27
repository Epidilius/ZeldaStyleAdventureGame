//
//  TreeTile.h
//  Algonquin College
//
//  Students: Joel Cright & Rachel Joannis
//  Creation Date: Fall 2014
//  Course Number: GAM1540
//  Professor:  Bradley Flood
//  Purpose: To create tree tiles.
//

#ifndef __GameDev2D__TreeTile__
#define __GameDev2D__TreeTile__

#include "Tile.h"


namespace GameDev2D
{
    //Forward Declarations
    class SubSection;

    // Constants
    const unsigned short TREE_VARIANT_COUNT = 7;

    //Tree Tile class
    class TreeTile : public Tile
    {
    public:
        TreeTile(SubSection* subSection, uvec2 coordinates, unsigned int variant);
        ~TreeTile();
        
        //Returns the type of tile
        TileType GetTileType();
        
        //Returns the number of tile variants
        unsigned int GetVariantCount();
        
        //Gets the atlas key for the tile variant
        void GetAtlasKeyForVariant(unsigned int variant, string& atlasKey);
    };
}

#endif /* defined(__GameDev2D__TreeTile__) */
