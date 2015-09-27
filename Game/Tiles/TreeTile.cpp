//
//  TreeTile.cpp
//  Algonquin College
//
//  Students: Joel Cright & Rachel Joannis
//  Creation Date: Fall 2014
//  Course Number: GAM1540
//  Professor:  Bradley Flood
//  Purpose: To create tree tiles.
//

#include "TreeTile.h"
#include "../../Source/UI/UI.h"


namespace GameDev2D
{
    TreeTile::TreeTile(SubSection* aSubSection, uvec2 aCoordinates, unsigned int aVariant) : Tile("TreeTile", aSubSection, aCoordinates)
    {
        //Set the variant
        SetVariant(aVariant);
        
        //Set that the Tree tile is NOT walkable
        SetIsWalkable(false);
    }
    
    TreeTile::~TreeTile()
    {
    
    }
    
    TileType TreeTile::GetTileType()
    {
        return TileTypeTree;
    }
    
    unsigned int TreeTile::GetVariantCount()
    {
        return TREE_VARIANT_COUNT;
    }
    
    void TreeTile::GetAtlasKeyForVariant(unsigned int aVariant, string& aAtlasKey)
    {
        switch (aVariant)
        {
            case 0:
                aAtlasKey = "Tree-Light";
                break;

            case 1:
                aAtlasKey = "Tree-Dark";
                break;

            case 2:
                aAtlasKey = "EvilTree-1";
                break;

            case 3:
                aAtlasKey = "EvilTree-2";
                break;

            case 4:
                aAtlasKey = "EvilTree-3";
                break;

            case 5:
                aAtlasKey = "EvilTree-4";
                break;

            case 6:
                aAtlasKey = "EvilTree-5";
                break;

            default:
                break;
        }
    }
}