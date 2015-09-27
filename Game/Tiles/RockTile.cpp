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

#include "RockTile.h"
#include "../../Source/UI/UI.h"


namespace GameDev2D
{
    RockTile::RockTile(SubSection* aSubSection, uvec2 aCoordinates, unsigned int aVariant) : Tile("RockTile", aSubSection, aCoordinates)
    {
        //Set the variant
        SetVariant(aVariant);
        
        //Set that the Rock tile is NOT walkable
        SetIsWalkable(false);
    }
    
    RockTile::~RockTile()
    {
    
    }
    
    TileType RockTile::GetTileType()
    {
        return TileTypeRock;
    }
    
    unsigned int RockTile::GetVariantCount()
    {
        return ROCK_VARIANT_COUNT;
    }
    
    void RockTile::GetAtlasKeyForVariant(unsigned int aVariant, string& aAtlasKey)
    {
        switch (aVariant)
        {
            case 0:
                aAtlasKey = "RedRock-1";
                break;

            case 1:
                aAtlasKey = "RedRock-2";
                break;

            case 2:
                aAtlasKey = "RedRock-3";
                break;

            case 3:
                aAtlasKey = "RedRock-4";
                break;

            case 4:
                aAtlasKey = "RedRock-5";
                break;

            case 5:
                aAtlasKey = "RedRock-6";
                break;

            case 6:
                aAtlasKey = "Rock-1";
                break;

            case 7:
                aAtlasKey = "Rock-2";
                break;

            case 8:
                aAtlasKey = "Rock-3";
                break;

            case 9:
                aAtlasKey = "Rock-4";
                break;

            case 10:
                aAtlasKey = "Rock-5";
                break;

            case 11:
                aAtlasKey = "Rock-6";
                break;



            default:
                break;
        }
    }
}