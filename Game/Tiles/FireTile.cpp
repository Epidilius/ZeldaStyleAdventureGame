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

#include "FireTile.h"
#include "../../Source/UI/UI.h"


namespace GameDev2D
{
    FireTile::FireTile(SubSection* aSubSection, uvec2 aCoordinates, unsigned int aVariant) : Tile("FireTile", aSubSection, aCoordinates)
    {
        //Set the variant
        SetVariant(aVariant);

        //Set that the Fire tile IS walkable (but will hurt you)
        SetIsWalkable(true);
    }

    FireTile::~FireTile()
    {

    }

    TileType FireTile::GetTileType()
    {
        return TileTypeFire;
    }

    unsigned int FireTile::GetVariantCount()
    {
        return FIRE_VARIANT_COUNT;
    }

    void FireTile::UpdateSprite(unsigned int aVariant)
    {
        //Safety check the variant index
        if (aVariant > GetVariantCount())
        {
            return;
        }

        //Cycle through and remove all the children from the tile
        for (unsigned int i = 0; i < GetNumberOfChildren(); i++)
        {
            RemoveChild(GetChildAtIndex(i));
        }

        //Get the atlas key for the sprite
        string atlasKey;
        GetAtlasKeyForVariant(aVariant, atlasKey);

        //Add the Sprite object as a child
        Sprite* sprite = (Sprite*)AddChild(new Sprite("MainAtlas", atlasKey), true);
        sprite->AddFrame("MainAtlas", "Fire-2");
        sprite->SetDoesLoop(true);
        sprite->SetFrameSpeed(4);
    }

    void FireTile::GetAtlasKeyForVariant(unsigned int aVariant, string& aAtlasKey)
    {
        switch (aVariant)
        {
        case 0:
            aAtlasKey = "Fire-1";
            break;

        default:
            break;
        }
    }
}