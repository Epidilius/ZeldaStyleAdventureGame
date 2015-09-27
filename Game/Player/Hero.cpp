//
//  Hero.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-14.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "Hero.h"
#include "Enemy.h"
#include "../Projectile.h"
#include "../World.h"
#include "../SubSection.h"
#include "../Tiles/Tile.h"
#include "../Pickups/Pickup.h"
#include "../Pickups/HeartPickup.h"
#include "../Pickups/GreenRupeePickup.h"
#include "../Pickups/BlueRupeePickup.h"
#include "../Pickups/BombPickup.h"
#include "../Pickups/ExplodingBomb.h"
#include "../Pickups/SwordPickup.h"
#include "../Pickups/HeartPiecePickup.h"
#include "../../Source/UI/UI.h"
#include "../../Source/Events/Input/InputEvents.h"
#include "../../Source/Services/ServiceLocator.h"
#include "../../Source/Audio/Audio.h"


namespace GameDev2D
{
    Hero::Hero(World* aWorld, Tile* aSpawnTile) : Player("Hero", aWorld),
        m_HeroState(HeroUnknown),
        m_Rupees(0),
        m_Bombs(0),
        m_LastDoor(World::DoorTypeUnknown),
        m_HasSword(HERO_HAS_SWORD_BY_DEFAULT),
        m_InvincibilityTimer(nullptr),
        m_LowHealthAudio(nullptr),
        m_HeroDeathAudio(nullptr),
        m_AttackingAudio(nullptr),
        m_HeartPickupAudio(nullptr),
        m_RupeePickupAudio(nullptr),
        m_ItemPickupAudio(nullptr), 
        m_SwordPickupAudio(nullptr),
        m_FirstTimeToCaveAudio(nullptr)
    {
        //Set the local position of the hero, based on the center of the spawn tile
        SetLocalPosition(aSpawnTile->GetCenter(true));
    
        //Cycle through and create the walking and attacking sprites for all 4 directions
        for(unsigned int i = 0; i < PLAYER_DIRECTION_COUNT; i++)
        {
            //Create the walking sprite for the current direction
            m_WalkingSprite[i] = new Sprite("MainAtlas", HERO_WALKING_ATLAS_KEY_1[i]);
            m_WalkingSprite[i]->AddFrame("MainAtlas", HERO_WALKING_ATLAS_KEY_2[i]);
            m_WalkingSprite[i]->SetAnchorPoint(0.5f, 0.5f);
            m_WalkingSprite[i]->SetDoesLoop(true);
            m_WalkingSprite[i]->SetIsEnabled(false);
            AddChild(m_WalkingSprite[i], false);
            
            //Create the attacking sprite for the current direction
            m_AttackingSprite[i] = new Sprite("MainAtlas", HERO_ATTACKING_ATLAS_KEY[i]);
            m_AttackingSprite[i]->SetAnchorPoint(HERO_ATTACKING_ANCHOR_POINT[i]);
            m_AttackingSprite[i]->SetIsEnabled(false);
            AddChild(m_AttackingSprite[i], false);
        }
        
        //Add an event listener for keyboard input
        ServiceLocator::GetInputManager()->AddEventListener(this, KEYBOARD_EVENT);
        
        //Create the invincibility timer
        m_InvincibilityTimer = new Timer(HERO_INVINCIBILITY_DURATION);
        
        //Set the hero's initial health and health capacity
        SetHealthCapacity(HERO_INITIAL_HEALTH_CAPACITY);
        SetHealth(HERO_INITIAL_HEALTH);

        // Set Up audio
        m_LowHealthAudio = new Audio("LowHealth", "wav", false, true);
        m_HeroDeathAudio = new Audio("HeroDeath", "wav", false, false);
        m_AttackingAudio = new Audio("Sword", "wav", false, false);
        m_HeartPickupAudio = new Audio("HeartPickup", "wav", false, false);
        m_RupeePickupAudio = new Audio("RupeePickup", "wav", false, false);
        m_ItemPickupAudio = new Audio("ItemPickup", "wav", false, false);
        m_SwordPickupAudio = new Audio("Fanfare", "wav", false, false);
        m_FirstTimeToCaveAudio = new Audio("Secret", "wav", false, false);
        
        //Set the hero's state to idle
        SetState(HeroIdle);
    }
    
    Hero::~Hero()
    {
        //Remove the event listener
        ServiceLocator::GetInputManager()->RemoveEventListener(this, KEYBOARD_EVENT);
    
        //Cycle through and delete the hero Sprites
        for(unsigned int i = 0; i < PLAYER_DIRECTION_COUNT; i++)
        {
            SafeDelete(m_WalkingSprite[i]);
            SafeDelete(m_AttackingSprite[i]);
        }
        
        //Delete the timer object
        SafeDelete(m_InvincibilityTimer);

        SafeDelete(m_LowHealthAudio);
        SafeDelete(m_HeroDeathAudio);
        SafeDelete(m_AttackingAudio);
        SafeDelete(m_HeartPickupAudio);
        SafeDelete(m_RupeePickupAudio);
        SafeDelete(m_ItemPickupAudio);
        SafeDelete(m_SwordPickupAudio);
        SafeDelete(m_FirstTimeToCaveAudio);
    }
    
    void Hero::Update(double aDelta)
    {
        //Update the invicibility timer
        m_InvincibilityTimer->Update(aDelta);
    
        //Cache the previous tile and location of the player
        Tile* previousTile = GetTile();
        vec2 previousPosition = GetLocalPosition();
        
        //Calculate and set the new player's position
        SetLocalPosition(vec2(GetLocalX() + GetDirection().x * GetSpeed() * aDelta, GetLocalY() + GetDirection().y * GetSpeed() * aDelta));

        //Factor in the collision zone for the player's position, based on the direction the player is walking
        vec2 position = GetWorldPosition() + vec2(GetDirection() * vec2(HERO_COLLISION_ZONE, HERO_COLLISION_ZONE));
    
        //Determine the subsection the position is on
        SubSection* subSection = m_World->GetSubSectionForPosition(position);
        
        //Calculate the perpendicular angles for the player collision zone
        vec2 v = GetWorldPosition() - position;
        vec2 perpendicularA = vec2(-v.y, v.x) / sqrtf((v.x * v.x) + (v.y * v.y)) * HERO_COLLISION_ZONE;
        vec2 perpendicularB = vec2(-v.y, v.x) / sqrtf((v.x * v.x) + (v.y * v.y)) * -HERO_COLLISION_ZONE;

        //Factor in the sub-section the player is on
        position -= subSection->GetWorldPosition();

        //Create a vector for the 3 collision points to check against
        vector<vec2> collisionPoints = {position, vec2(position + perpendicularA), vec2(position + perpendicularB)};
        
        //Cycle through the collision points
        for(unsigned int i = 0; i < collisionPoints.size(); i++)
        {
            //Get the current tile for the collision point
            Tile* currentTile = subSection->GetTileForPosition(collisionPoints[i]);
            
            //If the current tile doesn't equal the previous tile, check to see if the current tile is walkable
            if(currentTile != previousTile)
            {
                //If its NOT walkable set the player's position back to the previous position
                if(currentTile->IsWalkable() == false)
                {
                    SetLocalPosition(previousPosition);
                    break;
                }
            }
        }
        
        //Check to see if the player has changed tiles since the start of the update method
        Tile* currentTile = GetTile();
        if(currentTile != previousTile)
        {
            HasChangedTiles(currentTile, previousTile);
        }
        
        //If the hero is attacking we need to see if the hero's sword has collided with any of the enemies
        if(m_HeroState == HeroAttacking)
        {
            //Get the current subsection the hero is on
            subSection = GetSubSection();
        
            //Factor in the collision zone for the player's position, based on the direction the player is walking
            vec2 swordTipPosition = GetWorldPosition() + vec2(GetDirection() * HERO_SWORD_RANGE);
        
            //Factor in the sub-section the player is on
            swordTipPosition -= subSection->GetWorldPosition();
            
            //Get the tile the sword is on
            Tile* swordTile = subSection->GetTileForPosition(swordTipPosition);
        
            //Get the active enemies in the subsection
            vector<Enemy*> m_Enemies;
            GetSubSection()->GetActiveEnemies(m_Enemies);
            
            //Cycle through the active enemies and see if the tip of the sword collided with any of them
            for(unsigned int i = 0; i < m_Enemies.size(); i++)
            {
                if(m_Enemies.at(i)->GetTile() == swordTile)
                {
                    m_Enemies.at(i)->ApplyDamage(GetAttackDamage());
                }
            }
        }
    
        //Get the active projectiles in the SubSection
        vector<Projectile*> projectiles;
        GetSubSection()->GetActiveProjectiles(projectiles);
        
        //Cycle through the active projectiles
        for(unsigned int i = 0; i < projectiles.size(); i++)
        {
            if(projectiles.at(i)->GetTile() == GetTile())
            {
                //Apply damage
                ApplyDamage(projectiles.at(i)->GetAttackDamage());
                //Disable the projectile
                projectiles.at(i)->SetIsEnabled(false);
            }
        }
        
        //Update the base class
        Player::Update(aDelta);
    }
    
    void Hero::Reset()
    {
        //Reset Health Capacity
        SetHealthCapacity(HERO_INITIAL_HEALTH_CAPACITY);
        AddHealth(HERO_INITIAL_HEALTH);

        // Reset Position
        SubSection* subSection = m_World->GetSubSectionForIndex(2);
        Tile* tile = subSection->GetTileForIndex(53);
        SetLocalPosition(tile->GetCenter(true));
        SetDirection(PLAYER_DIRECTION_DOWN);

        // Shift the camera
        //NOTE: This will only work properly if killed by fire or projectile (aka inside the hero class) and not by enemies or bomb (outside the class)
        m_World->SetShiftCamera(false);
        m_World->GetCamera()->SetPosition(-subSection->GetWorldPosition());

        //Set the hero's state to idle
        SetState(HeroIdle);
        
        //Reset the base class
        Player::Reset();
    }
    
    void Hero::SetState(unsigned int aState)
    {
        //Set the hero state
        m_HeroState = (HeroState)aState;
    
        //Disable the active Sprite
        if(m_ActiveSprite != nullptr)
        {
            m_ActiveSprite->SetIsEnabled(false);
        }

        //Handle the hero state switch
        switch (aState)
        {
            case HeroIdle:
            case HeroWalking:
            {
                //Set the active Sprite based on the direction
                m_ActiveSprite = m_WalkingSprite[m_DirectionIndex];
                
                //Set the player speed
                float playerSpeed = aState == HeroIdle ? 0.0f : HERO_WALKING_SPEED;
                SetSpeed(playerSpeed);
            
                //Set the frame speed
                float frameSpeed = aState == HeroIdle ? 0.0f : HERO_WALKING_ANIMATION_FRAME_SPEED;
                m_ActiveSprite->SetFrameSpeed(frameSpeed);
            }
            break;
                
            case HeroAttacking:
            {
                // Play Audio
                m_AttackingAudio->Play();

                //Set the active Sprite based on the direction
                m_ActiveSprite = m_AttackingSprite[m_DirectionIndex];
                
                //Set the player speed
                SetSpeed(0.0f);

                //Stop attacking after a delay
                DelayGameObjectMethod(&Hero::StopAttacking, HERO_ATTACK_TIME);
            }
            break;

            default:
                break;
        }
        
        //Reset the frame index to zero and enable it
        m_ActiveSprite->SetFrameIndex(0);
        m_ActiveSprite->SetIsEnabled(true);
    }
    
    bool Hero::HasSword()
    {
        return m_HasSword;
    }
    
    void Hero::PickedUpSword()
    {
        if(m_HasSword == false)
        {
            //Set the hasSword flag
            m_HasSword = true;
        }
    }
    
    void Hero::AddBombs(unsigned short aBombs)
    {
        //Safety check the bombs bounds
        if (m_Bombs + aBombs >= HERO_MAX_BOMBS)
        {
            m_Bombs = HERO_MAX_BOMBS;
        }
        else
        {
            m_Bombs += aBombs;
        }
    }

    void Hero::AddRupees(unsigned short aRupees)
    {
        //Safety check the rupees bounds
        if(m_Rupees + aRupees >= USHRT_MAX)
        {
            m_Rupees = USHRT_MAX;
        }
        else
        {
            m_Rupees += aRupees;
        }
    }
    
    unsigned short Hero::GetRupees()
    {
        return m_Rupees;
    }

    unsigned short Hero::GetBombs()
    {
        return m_Bombs;
    }
    
    bool Hero::IsInvincible()
    {
        return m_InvincibilityTimer->IsRunning();
    }
    
    void Hero::ApplyDamage(unsigned int aAttackDamage)
    {
        Player::ApplyDamage(aAttackDamage);
        
        //If there is still health left, reset the invincibility timer
        if(GetHealth() > 0)
        {
            m_InvincibilityTimer->Reset(true);
        }

        // Play audio if low on health
        if (GetHealth() <= 2)
        {
            m_LowHealthAudio->Play();
        }

    }

    void Hero::AddHealth(unsigned int aHealth)
    {
        // Add health
        SetHealth(GetHealth() + aHealth);

        //Stop audio if enough health
        if (GetHealth() > 2)
            m_LowHealthAudio->Stop();
    }
    
    void Hero::HasDied()
    {
        Player::HasDied();
        m_HeroDeathAudio->Play();
        Reset();
    }
    
    void Hero::HandleEvent(Event* aEvent)
    {
        if(IsEnabled() == true)
        {
            switch (aEvent->GetEventCode())
            {
                case KEYBOARD_EVENT:
                {
                    KeyEvent* keyEvent = (KeyEvent*)aEvent;
                    KeyEventType keyEventType = keyEvent->GetKeyEventType();
                    
                    //Handle key down and up events
                    if(keyEventType == KeyDown)
                    {
                        HandleKeyDown(keyEvent->GetKeyCode());
                    }
                    else if(keyEventType == KeyUp)
                    {
                        HandleKeyUp(keyEvent->GetKeyCode());
                    }
                }
                break;

                default:
                    break;
            }
        }
    }
    
    void Hero::HandleKeyDown(KeyCode aKeyCode)
    {
        switch (aKeyCode)
        {
            case KEY_CODE_UP:
                SetDirection(PLAYER_DIRECTION_UP);
                SetState(HeroWalking);
                break;
                
            case KEY_CODE_DOWN:
                SetDirection(PLAYER_DIRECTION_DOWN);
                SetState(HeroWalking);
                break;
                
            case KEY_CODE_LEFT:
                SetDirection(PLAYER_DIRECTION_LEFT);
                SetState(HeroWalking);
                break;
                
            case KEY_CODE_RIGHT:
                SetDirection(PLAYER_DIRECTION_RIGHT);
                SetState(HeroWalking);
                break;

            case KEY_CODE_SPACE:
                if(HasSword() == true)
                {
                    SetState(HeroAttacking);
                    Projectile* projectile = new Projectile(this->GetSubSection());
                    projectile->SetIsEnabled(true);
                    //TODO: Set the rest of the vars
                }
                break;

            case KEY_CODE_B:
                // Drop exploding bomb
                if (GetBombs() > 0)
                {
                    m_Bombs--;

                    //Find tile in front of player and drop a bomb
                    vec2 tileCoordinate = vec2(GetTile()->GetCoordinates().x + GetDirection().x, GetTile()->GetCoordinates().y + GetDirection().y);
                    GetTile()->GetSubSection()->GetTileForCoordinates(tileCoordinate)->AddPickup(new ExplodingBomb(m_World));
                }
                break;

            default:
                break;
        }
    }
    
    void Hero::HandleKeyUp(KeyCode aKeyCode)
    {
        //Handle scenarios where one (or more) of the arrow keys are still down
        bool isUpDown = ServiceLocator::GetInputManager()->IsKeyPressed(KEY_CODE_UP);
        bool isDownDown = ServiceLocator::GetInputManager()->IsKeyPressed(KEY_CODE_DOWN);
        bool isLeftDown = ServiceLocator::GetInputManager()->IsKeyPressed(KEY_CODE_LEFT);
        bool isRightDown = ServiceLocator::GetInputManager()->IsKeyPressed(KEY_CODE_RIGHT);
        
        //Handle the key up event
        switch (aKeyCode)
        {
            case KEY_CODE_UP:
                if(isDownDown == true)
                {
                    SetDirection(PLAYER_DIRECTION_DOWN);
                    SetState(HeroWalking);
                }
                else if(isLeftDown == true)
                {
                    SetDirection(PLAYER_DIRECTION_LEFT);
                    SetState(HeroWalking);
                }
                else if(isRightDown == true)
                {
                    SetDirection(PLAYER_DIRECTION_RIGHT);
                    SetState(HeroWalking);
                }
                else
                {
                    SetState(HeroIdle);
                }
                break;
                
            case KEY_CODE_DOWN:
                if(isUpDown == true)
                {
                    SetDirection(PLAYER_DIRECTION_UP);
                    SetState(HeroWalking);
                }
                else if(isLeftDown == true)
                {
                    SetDirection(PLAYER_DIRECTION_LEFT);
                    SetState(HeroWalking);
                }
                else if(isRightDown == true)
                {
                    SetDirection(PLAYER_DIRECTION_RIGHT);
                    SetState(HeroWalking);
                }
                else
                {
                    SetState(HeroIdle);
                }
                break;
                
            case KEY_CODE_LEFT:
                if(isRightDown == true)
                {
                    SetDirection(PLAYER_DIRECTION_RIGHT);
                    SetState(HeroWalking);
                }
                else if(isUpDown == true)
                {
                    SetDirection(PLAYER_DIRECTION_UP);
                    SetState(HeroWalking);
                }
                else if(isDownDown == true)
                {
                    SetDirection(PLAYER_DIRECTION_DOWN);
                    SetState(HeroWalking);
                }
                else
                {
                    SetState(HeroIdle);
                }
                break;
                
            case KEY_CODE_RIGHT:
                if(isLeftDown == true)
                {
                    SetDirection(PLAYER_DIRECTION_LEFT);
                    SetState(HeroWalking);
                }
                else if(isUpDown == true)
                {
                    SetDirection(PLAYER_DIRECTION_UP);
                    SetState(HeroWalking);
                }
                else if(isDownDown == true)
                {
                    SetDirection(PLAYER_DIRECTION_DOWN);
                    SetState(HeroWalking);
                }
                else
                {
                    SetState(HeroIdle);
                }
                break;

            default:
                break;
        }
    }
    
    void Hero::StopAttacking()
    {
        //Handle scenarios where one (or more) of the arrow keys are still down
        bool isUpDown = ServiceLocator::GetInputManager()->IsKeyPressed(KEY_CODE_UP);
        bool isDownDown = ServiceLocator::GetInputManager()->IsKeyPressed(KEY_CODE_DOWN);
        bool isLeftDown = ServiceLocator::GetInputManager()->IsKeyPressed(KEY_CODE_LEFT);
        bool isRightDown = ServiceLocator::GetInputManager()->IsKeyPressed(KEY_CODE_RIGHT);
    
        //Handle the case where a key might be still held down
        if(isUpDown == true)
        {
            SetDirection(PLAYER_DIRECTION_UP);
            SetState(HeroWalking);
        }
        else if(isDownDown == true)
        {
            SetDirection(PLAYER_DIRECTION_DOWN);
            SetState(HeroWalking);
        }
        else if(isLeftDown == true)
        {
            SetDirection(PLAYER_DIRECTION_LEFT);
            SetState(HeroWalking);
        }
        else if(isRightDown == true)
        {
            SetDirection(PLAYER_DIRECTION_RIGHT);
            SetState(HeroWalking);
        }
        else
        {
            SetState(HeroIdle);
        }
    }
    
    void Hero::HasChangedTiles(Tile* aNewTile, Tile* aOldTile)
    {
        // Deal with pickup if there is one
        if (aNewTile->GetPickup() != nullptr)
        {
            GetYourPickups(aNewTile);
        }

        // Teleport to somwhere if you enter a door
        if (aNewTile->GetTileType() == TileTypeDoor)
        {
            TeleportToCave();
        }

        // If you step on fire, receive damage
        if (aNewTile->GetTileType() == TileTypeFire)
        {
            ApplyDamage(1);
        }
    }

    void Hero::GetYourPickups(Tile* aTile)
    {
        switch (aTile->GetPickup()->GetPickupType())
        {
        case PickupTypeHeart:
            AddHealth(2);
            m_HeartPickupAudio->Play();
            break;

        case PickupTypeGreenRupee:
            AddRupees(1);
            m_RupeePickupAudio->Play();
            break;

        case PickupTypeBlueRupee:
            AddRupees(5);
            m_RupeePickupAudio->Play();
            break;

        case PickupTypeBomb:
            AddBombs(1);
            m_ItemPickupAudio->Play();
            break;

        case PickupTypeHeartPiece:
            SetHealthCapacity(GetHealthCapacity() + 2);
            AddHealth(2);
            m_ItemPickupAudio->Play();

        case PickupTypeSword:
            PickedUpSword();
            m_SwordPickupAudio->Play();
            break;

        default:
            break;
        }

        // Exploding bombs can't be picked up
        if (aTile->GetPickup()->GetPickupType() != PickupTypeExplodingBomb)
            aTile->RemovePickup();
    }

    void Hero::TeleportToCave()
    {       
            int subsectionIndex = -1;
            int tileIndex = -1;
            vector<DoorStructure*> doorList = m_World->GetDoors();

            do{
                Random random;
                random.RandomizeSeed();
                int randomNum = random.RandomRange(0, World::DoorCount);

                // If you don't have a sword, go directly to the cave to get a sword
                if (!HasSword())
                {
                    randomNum = World::DoorCave;
                    m_FirstTimeToCaveAudio->Play();
                }

                switch (randomNum)
                {
                    //TODO: Right here, add an if check for if you've been in the cave.
                    if (m_LastDoor == randomNum)
                        continue;

                case World::DoorIsland:
                        subsectionIndex = doorList.at(World::DoorIsland)->m_DoorIndex;
                        tileIndex = doorList.at(World::DoorIsland)->m_DoorTileIndex;
                        m_LastDoor = World::DoorIsland;
                    break;

                case World::DoorForestMaze:
                        m_LastDoor = World::DoorForestMaze;
                        subsectionIndex = doorList.at(World::DoorForestMaze)->m_DoorIndex;
                        tileIndex = doorList.at(World::DoorForestMaze)->m_DoorTileIndex;
                    break;

                case World::DoorEnemy:
                        m_LastDoor = World::DoorEnemy;
                        subsectionIndex = doorList.at(World::DoorEnemy)->m_DoorIndex;
                        tileIndex = doorList.at(World::DoorEnemy)->m_DoorTileIndex;
                    break;

                case World::DoorDesert:
                        m_LastDoor = World::DoorDesert;
                        subsectionIndex = doorList.at(World::DoorDesert)->m_DoorIndex;
                        tileIndex = doorList.at(World::DoorDesert)->m_DoorTileIndex;
                    break;

                case World::DoorMountain:
                        m_LastDoor = World::DoorMountain;
                        subsectionIndex = doorList.at(World::DoorMountain)->m_DoorIndex;
                        tileIndex = doorList.at(World::DoorMountain)->m_DoorTileIndex;
                    break;

                case World::DoorCave:
                        m_LastDoor = World::DoorCave;
                        subsectionIndex = doorList.at(World::DoorCave)->m_DoorIndex;
                        tileIndex = doorList.at(World::DoorCave)->m_DoorTileIndex;
                    break;

                default:
                    m_LastDoor = World::DoorTypeUnknown;
                    subsectionIndex = doorList.at(World::DoorCave)->m_DoorIndex;  //TODO: Default these somehow
                    tileIndex = doorList.at(World::DoorCave)->m_DoorTileIndex;
                    break;
                }
            } while (m_LastDoor == World::DoorTypeUnknown);

            
            SubSection* subSection = m_World->GetSubSectionForIndex(subsectionIndex);
            Tile* tile = subSection->GetTileForIndex(tileIndex);

            // Set up pick up in cave
            if (m_LastDoor == World::DoorCave)
            {
                Tile* pickupTile = subSection->GetTileForIndex(89);

                if (!HasSword())
                {
                    pickupTile->AddPickup(new SwordPickup());
                }
                else
                {
                    pickupTile->AddRandomPickup();
                }
            }

            // Change player position and Shift Camera
            SetLocalPosition(tile->GetCenter(true));
            SetDirection(PLAYER_DIRECTION_DOWN);

            m_World->SetShiftCamera(false);
            m_World->GetCamera()->SetPosition(-subSection->GetWorldPosition());
    }

    
}