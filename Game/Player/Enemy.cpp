//
//  Enemy.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-20.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "Enemy.h"
#include "Hero.h"
#include "../World.h"
#include "../SubSection.h"
#include "../Tiles/Tile.h"
#include "../PathFinding/PathFinder.h"
#include "../Pickups/HeartPickup.h"
#include "../Pickups/GreenRupeePickup.h"
#include "../Pickups/BlueRupeePickup.h"
#include "../Pickups/BombPickup.h"
#include "../../Source/UI/UI.h"
#include "../../Source/Animation/Random.h"
#include "../../Source/Audio/Audio.h"

namespace GameDev2D
{
    Enemy::Enemy(World* aWorld, Tile* aSpawnTile, string aEnemyType) : Player(aEnemyType, aWorld),
        m_PathFinder(nullptr),
        m_EnemyState(EnemyUnknown),
        m_Random(nullptr),
        m_PathIndex(0),
        m_InvincibilityTimer(nullptr),
        m_EnemyHitAudio(nullptr),
        m_EnemyDeathAudio(nullptr)
    {
        //Set the local position of the hero, based on the center of the spawn tile
        SetLocalPosition(aSpawnTile->GetCenter(true));
    
        //Create the path finder object
        SubSection* subSection = m_World->GetSubSectionForPlayer(this);
        m_PathFinder = new PathFinder(subSection);
        
        //Cycle through and create the walking and attacking sprites for all 4 directions
        if(aEnemyType != "EnemyBlue")
        {
            for(unsigned int i = 0; i < PLAYER_DIRECTION_COUNT; i++)
            {
                //Create the walking sprite for the current direction
                m_WalkingSprite[i] = new Sprite("MainAtlas", ENEMY_WALKING_ATLAS_KEY_1[i]);
                m_WalkingSprite[i]->AddFrame("MainAtlas", ENEMY_WALKING_ATLAS_KEY_2[i]);
                m_WalkingSprite[i]->SetAnchorPoint(0.5f, 0.5f);
                m_WalkingSprite[i]->SetDoesLoop(true);
                m_WalkingSprite[i]->SetIsEnabled(false);
                AddChild(m_WalkingSprite[i], false);
                
                SetHealthCapacity(ENEMY_HEALTH_CAPACITY);
                SetHealth(ENEMY_HEALTH_CAPACITY);
            }
        }
        else
        {
            for(unsigned int i = 0; i < PLAYER_DIRECTION_COUNT; i++)
            {
                //Create the walking sprite for the current direction
                m_WalkingSprite[i] = new Sprite("MainAtlas", ENEMY_BLUE_WALKING_ATLAS_KEY_1[i]);
                m_WalkingSprite[i]->AddFrame("MainAtlas", ENEMY_BLUE_WALKING_ATLAS_KEY_2[i]);
                m_WalkingSprite[i]->SetAnchorPoint(0.5f, 0.5f);
                m_WalkingSprite[i]->SetDoesLoop(true);
                m_WalkingSprite[i]->SetIsEnabled(false);
                AddChild(m_WalkingSprite[i], false);
                
                SetHealthCapacity(ENEMY_BLUE_HEALTH_CAPACITY);
                SetHealth(ENEMY_BLUE_HEALTH_CAPACITY);
            }
        }
        
        //Create the random number generator object
        m_Random = new Random();
        m_Random->RandomizeSeed();

        //Set the enemy's state to idle
        SetState(EnemyIdle);

        //Create the invincibility timer
        m_InvincibilityTimer = new Timer(ENEMY_INVINCIBILITY_DURATION);

        // Set Audio
        m_EnemyDeathAudio = new Audio("EnemyDeath", "wav", false, false);
        m_EnemyHitAudio = new Audio("EnemyHit", "wav", false, false);
    }
    
    Enemy::~Enemy()
    {
        //Delete the path finder object
        SafeDelete(m_PathFinder);
        
        //Delete the random number generator object
        SafeDelete(m_Random);
        
        //Cycle through and delete the enemy Sprites
        for(unsigned int i = 0; i < PLAYER_DIRECTION_COUNT; i++)
        {
            SafeDelete(m_WalkingSprite[i]);
        }

        SafeDelete(m_InvincibilityTimer);

        SafeDelete(m_EnemyDeathAudio);
        SafeDelete(m_EnemyHitAudio);
    }
    
    void Enemy::Update(double aDelta)
    {
        if(IsEnabled() == true)
        {
            m_InvincibilityTimer->Update(aDelta);

            //Get the hero and the tile the hero is on
            Hero* hero = m_World->GetHero();
            Tile* heroTile = hero->GetTile();
            
            //If the hero tile is the same tile the enemy is on, apply some damage
            if(heroTile == GetTile())
            {
                hero->ApplyDamage(GetAttackDamage());
            }
            
            //Cache the tile before we update the player's position
            Tile* previousTile = GetTile();

            //Update the base class
            Player::Update(aDelta);
            
            //Check to see if the player has changed tiles since the start of the update method
            Tile* currentTile = GetTile();
            if(currentTile != previousTile)
            {
                HasChangedTiles(currentTile, previousTile);
            }
        }
    }
    
    void Enemy::DebugDraw()
    {
#if DEBUG
        m_PathFinder->DebugDraw();
#endif
    }
    
    void Enemy::Reset()
    {
        //Reset the enemy state
        SetState(EnemyIdle);

        //Reset Health Capacity
        SetHealth(ENEMY_HEALTH_CAPACITY);
        
        //Reset the base class
        Player::Reset();
    }

    void Enemy::SetState(unsigned int aState)
    {
        //Set the enemy state
        m_EnemyState = (EnemyState)aState;

        // Log the enemy State TODO:Delete
        Log("Enemy set state: %u", m_EnemyState);

        //Refresh the active sprite
        RefreshActiveSprite();

        //Handle the hero state switch
        switch (aState)
        {
            case EnemyIdle:
            {
                if(IsEnabled() == false)
                {
                    //Ensure  the active sprite is enabled
                    m_ActiveSprite->SetIsEnabled(true);
                
                    //Ensure the enemy object is enabled
                    SetIsEnabled(true);
                }
                
                //Delay call the change state method
                DelayGameObjectMethod(&Enemy::ChangeState, 1.0f);
            }
            break;
                
            case EnemySearching:
            {
                //Get the enemy's current subsection and the hero's
                SubSection* subSection = GetSubSection();
                SubSection* heroSubSection = m_World->GetHero()->GetSubSection();
                
                //Ensure the hero is on the subsection as our enemy
                if(subSection == heroSubSection)
                {
                    //Find the path
                    Tile* destination = m_World->GetHero()->GetTile();
                    bool pathFound = m_PathFinder->FindPath(GetTile(), destination);
                    
                    //Was a path found
                    if(pathFound == true)
                    {
                        StartWalking();
                    }
                    else
                    {
                        SetState(StateIdle);
                    }
                }
                else
                {
                    SetState(StateIdle);
                }
            }
                break;
                
            case EnemyAttacking:
            {
                DelayGameObjectMethod(&Enemy::FireProjectile, ENEMY_PROJECTILE_DELAY);
            }
                break;

            case EnemyWalking:
            {
                //Get the current subsection
                SubSection* subSection = GetSubSection();

                m_Random->RandomizeSeed();

                //Find the path
                Tile* destination = nullptr;
                do {
                    int randomTileIndex = m_Random->RandomInt(subSection->GetNumberOfTiles());
                    destination = subSection->GetTileForIndex(randomTileIndex);
                } while (!destination->IsWalkable());

                bool pathFound = m_PathFinder->FindPath(GetTile(), destination);

                //Was a path found
                if (pathFound == true)
                {
                    StartWalking();
                }
                else
                {
                    SetState(StateIdle);
                }

            }
                break;

            case EnemyDead:
            {
                //Disable the active sprite
                m_ActiveSprite->SetIsEnabled(false);
                
                //Disable the enemy object
                SetIsEnabled(false);
            }
            break;
                
            default:
                break;
        }
    }

    bool Enemy::IsInvincible()
    {
        return m_InvincibilityTimer->IsRunning();
    }

    void Enemy::ApplyDamage(unsigned int aAttackDamage)
    {
        Player::ApplyDamage(aAttackDamage);

        m_EnemyHitAudio->Play();

        //If there is still health left, reset the invincibility timer
        if (GetHealth() > 0)
        {
            m_InvincibilityTimer->Reset(true);
        }

    }
    
    void Enemy::HasDied()
    {
        //Set the enemy state to dead
        SetState(EnemyDead);

        // Play death sound
        m_EnemyDeathAudio->Play();

        //Drop an random item pickup
        m_Random->RandomizeSeed();
        unsigned int value = m_Random->RandomRange(0, 100);

        // if 1 heart (so 2) 50 drop heart, 50 drop nothing
        // if full health, 50 nothing, 20 green ruppe, 20 bomb, 10 blue rupee
        // else 15 heart, 15 green rupee, 15 bomb, 5 blue rupee
        Hero* hero = m_World->GetHero();
        if (hero->GetHealth() <= 2)
        {
            if (value < 50)
            {
                GetTile()->AddPickup(new HeartPickup());
            }
        }
        else if (hero->GetHealth() == hero->GetHealthCapacity())
        {
            if (value < 20)
            {
                GetTile()->AddPickup(new GreenRupeePickup());
            }
            else if (value < 40)
            {
                GetTile()->AddPickup(new BombPickup());
            }
            else if (value < 50)
            {
                GetTile()->AddPickup(new BlueRupeePickup());
            }
        }
        else
        {
            if (value < 15)
            {
                GetTile()->AddPickup(new HeartPickup());
            }
            else if (value < 30)
            {
                GetTile()->AddPickup(new GreenRupeePickup());
            }
            else if (value < 45)
            {
                GetTile()->AddPickup(new BombPickup());
            }
            else if (value < 50)
            {
                GetTile()->AddPickup(new BlueRupeePickup());
            }
        }

    }
    
    void Enemy::StartWalking()
    {
        //Refresh the A* debug drawing if the flags are set
        if((GetSubSection()->GetDebugDrawFlags() & DebugDrawPathFindingScores) > 0)
        {
            GetSubSection()->RefreshDebugDraw();
        }
        
        //Reset the path index
        m_PathIndex = 0;
        
        //Call the walk method, this will be called on a recursive delay until the end of the path is reached
        Walk();
    }
    
    void Enemy::Walk()
    {
        if(m_EnemyState == EnemySearching || m_EnemyState == EnemyWalking)
        {
            if(m_PathFinder->GetPathSize() > 0)
            {
                PathNode* pathNode = m_PathFinder->GetPathNodeAtIndex(m_PathIndex);
                Tile* tile = pathNode->GetTile();
                
                //Get the current position and the destination position and calculate the direction
                vec2 currentPosition = GetWorldPosition();
                vec2 destinationPosition = tile->GetCenter(true);
                vec2 direction = destinationPosition - currentPosition;
                
                //Normalize the direction vector
                direction = normalize(direction);
                
                //Set the enemy's new direction and refresh the active sprite
                SetDirection(direction);
                RefreshActiveSprite();
                
                //Calculate the distance and duration
                float distance = sqrtf((destinationPosition.x - currentPosition.x) *
                                       (destinationPosition.x - currentPosition.x) +
                                       (destinationPosition.y - currentPosition.y) *
                                       (destinationPosition.y - currentPosition.y));
                
                double duration = distance / ENEMY_WALKING_SPEED;
                
                //Animate the enemy to the center of the tile
                SetLocalPosition(tile->GetCenter(true), duration);
                
                //Increment the path index
                m_PathIndex++;
                
                //If the path index has reached the end point, set the state to idle
                if(m_PathIndex == m_PathFinder->GetPathSize())
                {
                    //Once the enemy has stopped walking, set the enemy state to idle
                    SetState(EnemyIdle);
                }
                else
                {
                    //Recursively call the walk function with a delay
                    DelayGameObjectMethod(&Enemy::Walk, duration);
                }
            }
            else
            {
                SetState(EnemyIdle);
            }
        }
    }
    
    void Enemy::FireProjectile()
    {
        //Ensure the enemy still has some health
        if(IsEnabled())
        {
            GetSubSection()->FireProjectile(GetWorldPosition(),
                                            GetDirection(),
                                            ENEMY_PROJECTILE_SPEED,
                                            ENEMY_PROJECTILE_ATTACK_DAMAGE);
            
            //Then set the enemy state back to the idle
            SetState(EnemyIdle);
        }
    }
    
    void Enemy::ChangeState()
    {
        unsigned int state = m_EnemyState;
        m_Random->RandomizeSeed();

        while (state == m_EnemyState)
        {
            state = m_Random->RandomRange(0, EnemyStateCount);
        }

        // Set the new state
        SetState(state);
    }
    
    void Enemy::RefreshActiveSprite()
    {
        //Disable the active Sprite
        if(m_ActiveSprite != nullptr)
        {
            m_ActiveSprite->SetIsEnabled(false);
        }
        
        //Set the active Sprite based on the direction
        m_ActiveSprite = m_WalkingSprite[m_DirectionIndex];
        
        //Set the frame speed
        m_ActiveSprite->SetFrameSpeed(ENEMY_WALKING_ANIMATION_FRAME_SPEED);
        
        //Reset the frame index to zero and enable it
        m_ActiveSprite->SetFrameIndex(0);
        m_ActiveSprite->SetIsEnabled(true);
    }
}