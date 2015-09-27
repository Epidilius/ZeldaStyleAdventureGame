//
//  ExplodingBomb.cpp
//  GameDev2D
//
//  Student:    Joel Cright & Rachel Joannis
//  Creation Date: Fall 2014
//  Course Number: GAM1540
//  Professor: Bradley Flood
//  Purpose:    To drop exploding bombs
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "ExplodingBomb.h"
#include "../World.h"
#include "../Player/Enemy.h"
#include "../Player/Hero.h"
#include "../SubSection.h"
#include "../../Source/UI/UI.h"
#include "../../Source/Audio/Audio.h"
#include "../../Source/Services/ServiceLocator.h"

namespace GameDev2D
{
    ExplodingBomb::ExplodingBomb(World* aWorld) : Pickup("ExplodingBomb"),
        m_BombExplodeAudio(nullptr)
    {
        Sprite* sprite = new Sprite("MainAtlas", "Bomb");
        sprite->AddFrame("MainAtlas", "Fire-1");
        sprite->SetDoesLoop(false);
        sprite->SetFrameSpeed(1.0f);
        sprite->SetAnchorPoint(0.5f, 0.5f);
        AddChild(sprite, true);

        m_World = aWorld;

        m_BombDropAudio = new Audio("BombDrop", "wav", false, false);
        m_BombExplodeAudio = new Audio("BombExplode", "wav", false, false);
        m_BombDropAudio->Play();

        DelayGameObjectMethod(&ExplodingBomb::ExplodeBomb, BOMB_EXPLOSION_DELAY);
    }

    ExplodingBomb::~ExplodingBomb()
    {
        SafeDelete(m_BombDropAudio);
        SafeDelete(m_BombExplodeAudio);
    }

    PickupType ExplodingBomb::GetPickupType()
    {
        return PickupTypeExplodingBomb;
    }

    void ExplodingBomb::ExplodeBomb()
    {
        //Camera Shake
        m_World->GetCamera()->Shake(5.0f, 0.5);

        DestroyEverything();

         //Play Audio
        m_BombExplodeAudio->Play();

        // Remove bomb after delay
        DelayGameObjectMethod(&ExplodingBomb::RemoveBomb, BOMB_EXPLOSION_DELAY / 2);
      
    }

    void ExplodingBomb::RemoveBomb()
    {
        SetIsEnabled(false);
    }
    
    void ExplodingBomb::DestroyEverything()
    {
        Tile* tile = GetTile();

        //If green boulder, change to ground and add random pickup
        if (tile->GetTileType() == TileTypeBoulder && tile->GetVariant() == 1)
        {
            tile = tile->GetSubSection()->SetTileTypeAtIndex(tile->GetSubSection()->GetTileIndexForTile(tile), TileTypeGround, 0);
            tile->AddRandomPickup();
        }

        // If the hero is on the tile, damage the hero
        if (m_World->GetHero()->GetTile() == tile)
            m_World->GetHero()->ApplyDamage(2);

        // Go through all enemies and apply damage if needed
        vector<Enemy*> enemies;
        tile->GetSubSection()->GetActiveEnemies(enemies);

        for (int i = 0; i < enemies.size(); i++)
        {
            if (enemies[i]->GetTile() == tile)
                enemies[i]->ApplyDamage(2);
        }
    }

}
