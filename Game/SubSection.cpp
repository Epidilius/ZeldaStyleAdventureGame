//
//  SubSection.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-15.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "SubSection.h"
#include "World.h"
#include "Projectile.h"
#include "Player/Player.h"
#include "Player/Hero.h"
#include "Player/Enemy.h"
#include "Tiles/GroundTile.h"
#include "Tiles/TreeTile.h"
#include "Tiles/WaterTile.h"
#include "Tiles/BoulderTile.h"
#include "Tiles/BridgeTile.h"
#include "Tiles/DoorTile.h"
#include "Tiles/FireTile.h"
#include "Tiles/RockTile.h"
#include "Tiles/SandTile.h"
#include "Tiles/StairsTile.h"
#include "LevelEditor/LevelEditor.h"
#include "../Source/UI/UI.h"
#include "../Source/Graphics/Core/RenderTarget.h"
#include "../Source/Services/ServiceLocator.h"
#include "../Source/Audio/Audio.h"


namespace GameDev2D
{
    SubSection::SubSection(World* aWorld, uvec2 aCoordinates) : GameObject("SubSection"),
        m_World(aWorld),
        m_LevelEditor(nullptr),
        m_Tiles(nullptr),
        m_HeroSpawnPoint(nullptr),
        m_EnemySpawnPoint(nullptr),
        m_DebugDrawingFlags(DebugDrawNothing),
        m_DebugRenderTarget(nullptr),
		m_IsEdge(false),
		m_WrapsTo(nullptr)
    {
        //Set the Sub-section's position
        SetLocalPosition((float)(aCoordinates.x * aWorld->GetSubSectionSize().x), (float)(aCoordinates.y * aWorld->GetSubSectionSize().y));
    
        //Allocate the tiles array, the inheriting class will populate this array with Tile objects
        m_Tiles = new Tile*[GetNumberOfTiles()];
    
        //Initialize all the tiles to NULL
        for(unsigned int i = 0; i < GetNumberOfTiles(); i++)
        {
            m_Tiles[i] = NULL;
        }
    }
    
    SubSection::SubSection(LevelEditor* aLevelEditor, const string& aFilename) : GameObject("SubSection"),
        m_World(nullptr),
        m_LevelEditor(aLevelEditor),
        m_Tiles(nullptr),
        m_HeroSpawnPoint(nullptr),
        m_EnemySpawnPoint(nullptr),
        m_DebugDrawingFlags(DebugDrawNothing),
        m_DebugRenderTarget(nullptr)
    {
        //Allocate the tiles array, the inheriting class will populate this array with Tile objects
        m_Tiles = new Tile*[GetNumberOfTiles()];
    
        //Initialize all the tiles to NULL
        for(unsigned int i = 0; i < GetNumberOfTiles(); i++)
        {
            m_Tiles[i] = NULL;
        }
        
        //Create the hero and enemy spawn point sprites
        m_HeroSpawnPoint = new Sprite("MainAtlas", "Link-Spawn");
        m_EnemySpawnPoint = new Sprite("MainAtlas", "Enemy-Spawn");
        
        //Load the subsection
        Load(aFilename);
    }
    
    SubSection::~SubSection()
    {
        //Safety check the tiles pointer
        if(m_Tiles != nullptr)
        {
            //Cycle through and delete the tile objects
            for(unsigned int i = 0; i < GetNumberOfTiles(); i++)
            {
                SafeDelete(m_Tiles[i]);
            }
            
            //Delete the tiles array
            SafeDeleteArray(m_Tiles);
        }
        
        for(int i = 0; i < m_Projectiles.size(); i++)
        {
            SafeDelete(m_Projectiles.at(i));
        }
        m_Projectiles.clear();
        
        SafeDelete(m_HeroSpawnPoint);
        SafeDelete(m_EnemySpawnPoint);
        SafeDelete(m_DebugRenderTarget);
    }
    
    void SubSection::Load(const string& aFilename)
    {
        //Flag used to determine if an empty level should be loaded
        bool loadEmptyLevel = false;
    
        //Ensure the filename actually has some data in it
        if(aFilename.length() > 0)
        {
            //loadEmptyLevel = false;
            
            ifstream inputStream;
            
            // open the file
            inputStream.open(aFilename, ifstream::in | ifstream::binary);
            
            //Unlike the output stream, if the input stream doesnt find the file, the input stream
            //wont create it, and is_open will return false
            if(inputStream.is_open() == true)
            {
                //determine how many bytes are in the file
                inputStream.seekg(0, inputStream.end);
                long long levelSize = inputStream.tellg();
                
                //seek back to the start of the file for reading
                inputStream.seekg(0, inputStream.beg);
                
                //create a buffer for our data
                const unsigned int bufferLength = (unsigned int) levelSize / sizeof(unsigned short);
                unsigned short* buffer = new unsigned short[bufferLength];
                
                //Read the data from input stream into our buffer
                inputStream.read((char*)buffer, (int)levelSize);
                
                //close the input stream
                inputStream.close();
                
                // let's validate our buffer data
                TileType tileType = TileTypeUnknown;
                bool isHeroSpawnPoint = false;
                bool isEnemySpawnPoint = false;
                
                for(int i = 0; i < bufferLength; i++)
                {
                    tileType = TileTypeUnknown;
                    isHeroSpawnPoint = false;
                    isEnemySpawnPoint = false;
                    //PickupType pickupType = PickupTypeUnknown;
                    
                    /*if((buffer[i] & PickupTypeRupee) > 0)
                     {
                     //It does
                     pickupType = PickupTypeRupee;
                     
                     //Clear the Pickup bit
                     buffer[i] &= ~PickupTypeRupee;
                     }*/

                    // Check the spawn points
                    isHeroSpawnPoint = (buffer[i] & TILE_HERO_SPAWN_POINT);
                    if (isHeroSpawnPoint > 0)
                        buffer[i] &= ~TILE_HERO_SPAWN_POINT;

                    isEnemySpawnPoint = (buffer[i] & TILE_ENEMY_SPAWN_POINT);
                    if (isEnemySpawnPoint > 0)
                        buffer[i] &= ~TILE_ENEMY_SPAWN_POINT;

                    for (int j = 0; j < TileTypeCount; j++)
                    {
                        //Check to see if the tile is set
                        unsigned int value = (buffer[i] & TILES_TYPES[j]);
                        if (value > 0)
                        {
                            tileType = TILES_TYPES[j];

                            //Turn off the tile type bit, leaving only the variant bit
                            buffer[i] &= ~TILES_TYPES[j];

                            

                            //Break out of the loop
                            break;
                        }
                    }

                    assert(tileType != TileTypeUnknown);                    

                    // Set the tile type
                    unsigned int tileVariant = buffer[i];
                    
                    Tile* tile = SetTileTypeAtIndex(i, tileType, tileVariant);

                    

                    //Set the spawn points
                    if (tile != nullptr)
                    {
                        tile->SetIsHeroSpawnPoint(isHeroSpawnPoint);
                        tile->SetIsEnemySpawnPoint(isEnemySpawnPoint);
                    }

                    //Spawn the enemy
                    if (isEnemySpawnPoint == true)
                    {
                        Random random;
                        random.RandomizeSeed();
                        
                        switch(random.RandomInt(10))
                        {
                            case 1:
                                SpawnEnemy(tile, true);
                                break;
                                
                            default:
                                SpawnEnemy(tile);
                                break;
                        }
                    }
                    
                    //Set the Pickup type
                    //setPickupTypeAtIndex(pickupType, i);
                }
                
                //Delete the buffer, it was allocated on the heap after all
                SafeDelete(buffer);
            }
            else
            {
                Error(false, "The input stream failed to open: %s", aFilename.c_str());
                loadEmptyLevel = true;
            }
        }
        else
        {
            //Filename was empty
            loadEmptyLevel = true;
        }
        //Is the load empty level flag set to true, if so, load an empty level
        if(loadEmptyLevel == true)
        {
            //Create an empty world section
            for(unsigned int i = 0; i < GetNumberOfTiles(); i++)
            {
                SetTileTypeAtIndex(i, TileTypeGround, 0);
            }
        }
    }
    
    void SubSection::Save(const string& aFilename)
    {
        if (aFilename.length() > 0)
        {
            
                //Save the tile's pickup if there is one    //TODO: Pickup is abstract, haven't implemented any yet
                //if(m_EditingSubSection->GetTileForIndex(i)->GetPickup() != NULL &&
                //m_EditingSubSection->GetTileForIndex(i)->GetPickup()->GetPickupType() != PickupTypeUnknown)
                {
                    //Buffer[i] |= m_Tiles[i]->getPickup()->getPickupType();
                }

            

            //File IO time
            ofstream outputStream;

            //Open the file
            outputStream.open(aFilename, ofstream::out | ofstream::binary);

            //Did the file open?
            if (outputStream.is_open() == true)
            {
                

                const unsigned short bufferLength = GetNumberOfTiles();   //TODO: Fix this
                unsigned short* buffer = new unsigned short[bufferLength];

                //Create the buffer
                for (int i = 0; i < bufferLength; i++)
                {
                    Tile* tile = GetTileForIndex(i);
                    buffer[i] = tile->GetTileType();

                    //Set the variant type
                    if (tile->GetVariant() > 0)
                        buffer[i] |= tile->GetVariant();

                    if (tile->IsHeroSpawnPoint() == true)
                    {
                        //Set the buffer as a hero spawn point
                        buffer[i] |= TILE_HERO_SPAWN_POINT;
                    }
                    else if (tile->IsEnemySpawnPoint() == true)
                    {
                        //Set the buffer as an enemy spawn point
                        buffer[i] |= TILE_ENEMY_SPAWN_POINT;
                    }
                }

                unsigned int bufferSize = bufferLength * sizeof(unsigned short);

                //Write the buffer
                outputStream.write((const char*)buffer, bufferSize);

                //Make sure to close the file stream
                outputStream.close();
                
                //Delete the buffer, it was allocated on the heap after all
                SafeDeleteArray(buffer);
            }
            else
            {
                //This may be a place where you could throw an exception in your game,
                //if you are opening a file for reading OR writing and it doesn't open.
                Error(false, "Failed to save %s", aFilename.c_str());
            }

        }
        else
        {
            Error(false, "Failed to save: %s", aFilename.c_str());
        }
    }
    
    void SubSection::Update(double aDelta)
    {
        //Is the sub-section active?
        if(IsEnabled() == true)
        {
            //Cycle through the tiles and update them
            unsigned int i = 0;
            for(i = 0; i < GetNumberOfTiles(); i++)
            {
                if(m_Tiles[i] != nullptr)
                {
                    m_Tiles[i]->Update(aDelta);
                }
            }
            
            //Cycle through and update all the enemies
            for(i = 0; i < m_Enemies.size(); i++)
            {
                m_Enemies.at(i)->Update(aDelta);
            }
            
            //Cycle through and update all the enemies
            for(i = 0; i < m_Projectiles.size(); i++)
            {
                if(m_Projectiles.at(i)->IsEnabled() == true)
                {
                    m_Projectiles.at(i)->Update(aDelta);
                }
            }
        }
    }
    
    void SubSection::Draw()
    {
        //Is the sub-section active?
        if(IsEnabled() == true)
        {
            //Cycle through the tiles and draw them
            unsigned int i = 0;
            for(i = 0; i < GetNumberOfTiles(); i++)
            {
                //
                if(m_Tiles[i] != nullptr)
                {
                    //Draw the tile
                    m_Tiles[i]->Draw();
                    
                    //Are we in the level editor (we know this because the pointer would be null if we weren't)
                    if(m_LevelEditor != nullptr)
                    {
                        //Draw the hero spawn point
                        if(m_Tiles[i]->IsHeroSpawnPoint() == true)
                        {
                            m_HeroSpawnPoint->SetLocalPosition(m_Tiles[i]->GetLocalPosition());
                            m_HeroSpawnPoint->Draw();
                        }
                        
                        //Draw the enemy spawn point
                        if(m_Tiles[i]->IsEnemySpawnPoint() == true)
                        {
                            m_EnemySpawnPoint->SetLocalPosition(m_Tiles[i]->GetLocalPosition());
                            m_EnemySpawnPoint->Draw();
                        }
                    }
                }
            }
            
            //If there is debug data to draw, draw it
            if(m_DebugDrawingFlags != DebugDrawNothing && m_DebugRenderTarget != nullptr)
            {
                mat4 matrix = translate(mat4(1.0f), vec3(GetWorldPosition().x, GetWorldPosition().y, 0.0f));
                m_DebugRenderTarget->GetTextureFrame()->Draw(matrix);
            }
            
            //Cycle through and draw all the projectiles
            for(i = 0; i < m_Projectiles.size(); i++)
            {
                if(m_Projectiles.at(i)->IsEnabled() == true)
                {
                    m_Projectiles.at(i)->Draw();
                }
            }
            
            //Cycle through and draw all the enemies
            for(i = 0; i < m_Enemies.size(); i++)
            {
                m_Enemies.at(i)->Draw();
            }
        }
    }
    
    uvec2 SubSection::GetSize()
    {
        return vec2(GetNumberOfHorizontalTiles() * GetTileSize(), GetNumberOfVerticalTiles() * GetTileSize());
    }
    
    unsigned int SubSection::GetNumberOfHorizontalTiles()
    {
        if(m_World != nullptr)
        {
            return m_World->GetNumberOfHorizontalTiles();
        }
        else if(m_LevelEditor != nullptr)
        {
            return m_LevelEditor->GetNumberOfHorizontalTiles();
        }
        return 0;
    }
    
    unsigned int SubSection::GetNumberOfVerticalTiles()
    {
        if(m_World != nullptr)
        {
            return m_World->GetNumberOfVerticalTiles();
        }
        else if(m_LevelEditor != nullptr)
        {
            return m_LevelEditor->GetNumberOfVerticalTiles();
        }
        return 0;
    }
    
    unsigned int SubSection::GetNumberOfTiles()
    {
        return GetNumberOfHorizontalTiles() * GetNumberOfVerticalTiles();
    }
        
    unsigned int SubSection::GetTileSize()
    {
        if(m_World != nullptr)
        {
            return m_World->GetTileSize();
        }
        else if(m_LevelEditor != nullptr)
        {
            return m_LevelEditor->GetTileSize();
        }
        return 0;
    }
    
    TileType SubSection::GetTileTypeForIndex(unsigned int aIndex)
    {
        if(aIndex < GetNumberOfTiles())
        {
            if(m_Tiles[aIndex] != nullptr)
            {
                return m_Tiles[aIndex]->GetTileType();
            }
        }
        return TileTypeUnknown;
    }
    
    Tile* SubSection::SetTileTypeAtIndex(unsigned int aIndex, TileType aTileType, unsigned int aVariant)
    {
        if(aIndex < GetNumberOfTiles())
        {


            //Don't replace the tile if its the same type of tile that already exists
            if(m_Tiles[aIndex] != nullptr && m_Tiles[aIndex]->GetTileType() == aTileType)
            {
                //Ensure the variant was updated before we return
                m_Tiles[aIndex]->SetVariant(aVariant);
                return m_Tiles[aIndex];
            }
            
            //Delete the tile at the index, if one exists
            RemoveChild(m_Tiles[aIndex]);
            SafeDelete(m_Tiles[aIndex]);
            
            //BeginProfile();
            //Set the tile at the index
            m_Tiles[aIndex] = CreateTile(aTileType, aVariant, GetTileCoordinatesForIndex(aIndex));
            
            //double duration = EndProfile();
            //Log("%f", duration);

            //Add the tile as a child
            if(m_Tiles[aIndex] != nullptr)
            {
                AddChild(m_Tiles[aIndex], false);
            }


            
            return m_Tiles[aIndex];
        }
        
        return nullptr;
    }
    
    bool SubSection::ValidateCoordinates(ivec2 aCoordinates)
    {
        if(aCoordinates.x < 0 || aCoordinates.y < 0 || aCoordinates.x >= GetNumberOfHorizontalTiles() || aCoordinates.y >= GetNumberOfVerticalTiles())
        {
            return false;
        }
        return true;
    }
    
    uvec2 SubSection::GetTileCoordinatesForIndex(unsigned int aIndex)
    {
        //If this assert is hit, the index you passed in is out of bounds
        assert(aIndex < GetNumberOfTiles());

        //Calculate the coordinates and set the tile position and size
        uvec2 coordinates = uvec2(0,0);
        coordinates.x = (aIndex % GetNumberOfHorizontalTiles());
        coordinates.y = ((aIndex - coordinates.x) / GetNumberOfHorizontalTiles());
        
        return coordinates;
    }
    
    uvec2 SubSection::GetTileCoordinatesForPosition(vec2 aPosition)//
    {
        uvec2 coordinates = uvec2(aPosition.x / (float)GetTileSize(), aPosition.y / (float)GetTileSize());
        return coordinates;
    }
    
    uvec2 SubSection::GetTileCoordinatesForTile(Tile* aTile)
    {
        return GetTileCoordinatesForIndex(GetTileIndexForTile(aTile));
    }
    
    unsigned int SubSection::GetTileIndexForPosition(vec2 aPosition)//
    {
        uvec2 coordinates = GetTileCoordinatesForPosition(aPosition);
        return GetTileIndexForCoordinates(coordinates);
    }
    
    unsigned int SubSection::GetTileIndexForCoordinates(uvec2 aCoordinates)
    {
        return aCoordinates.x + (aCoordinates.y * GetNumberOfHorizontalTiles());
    }
    
    unsigned int SubSection::GetTileIndexForTile(Tile* aTile)
    {
        return GetTileIndexForPosition(aTile->GetLocalPosition());
    }
    
    Tile* SubSection::GetTileForIndex(unsigned int aIndex)
    {
        //If this assert is hit, the index that was passed in is out of bounds
        assert(aIndex < GetNumberOfTiles());
    
        //If this assert is hit, then the tile object at the index is null
        assert(m_Tiles[aIndex] != nullptr);
    
        //Return the tile at the index
        return m_Tiles[aIndex];
    }
    
    Tile* SubSection::GetTileForPosition(vec2 aPosition)
    {
        return GetTileForIndex(GetTileIndexForPosition(aPosition));
    }
    
    Tile* SubSection::GetTileForCoordinates(uvec2 aCoordinates)
    {
        return GetTileForIndex(GetTileIndexForCoordinates(aCoordinates));
    }
    
    void SubSection::GetActiveEnemies(vector<Enemy*>& aEnemies)
    {
        for(unsigned int i = 0; i < m_Enemies.size(); i++)
        {
            if(m_Enemies.at(i)->IsEnabled() == true)
            {
                aEnemies.push_back(m_Enemies.at(i));
            }
        }
    }
    
    void SubSection::GetActiveProjectiles(vector<Projectile*>& aProjectiles)
    {
        for(unsigned int i = 0; i < m_Projectiles.size(); i++)
        {
            if(m_Projectiles.at(i)->IsEnabled() == true)
            {
                aProjectiles.push_back(m_Projectiles.at(i));
            }
        }
    }
    
    void SubSection::FireProjectile(vec2 aStartingPosition, vec2 aDirection, float aSpeed, unsigned int aAttackDamage)
    {
        for(unsigned int i = 0; i < m_Projectiles.size(); i++)
        {
            if(m_Projectiles.at(i)->IsEnabled() == false)
            {
                m_Projectiles.at(i)->Setup(aStartingPosition, aDirection, aSpeed, aAttackDamage);
                break;
            }
        }
    }
    
    void SubSection::CreateProjectilePool()
    {
        if(m_Projectiles.size() == 0)
        {
            //Lets base the projectile pool size off the number of enemies in the subsection
            for(unsigned int i = 0; i < m_Enemies.size() * SUBSECTION_PROJECTILES_PER_ENEMY; i++)
            {
                m_Projectiles.push_back(new Projectile(this));
            }
        }
    }
    
    Tile* SubSection::CreateTile(TileType aTileType, unsigned int aVariant, uvec2 aCoordinates)
    {
        //Create the new tile based on the TileType
        switch (aTileType)
        {
            case TileTypeGround:
                return new GroundTile(this, aCoordinates, aVariant);

            case TileTypeTree:
                return new TreeTile(this, aCoordinates, aVariant);

            case TileTypeWater:
                return new WaterTile(this, aCoordinates, aVariant);

            case TileTypeBoulder:
                return new BoulderTile(this, aCoordinates, aVariant);

            case TileTypeBridge:
                return new BridgeTile(this, aCoordinates, aVariant);

            case TileTypeFire:
                return new FireTile(this, aCoordinates, aVariant);

            case TileTypeRock:
                return new RockTile(this, aCoordinates, aVariant);

            case TileTypeSand:
                return new SandTile(this, aCoordinates, aVariant);

            case TileTypeStairs:
                return new StairsTile(this, aCoordinates, aVariant);

            case TileTypeDoor:
                return new DoorTile(this, aCoordinates, aVariant);

            case TileTypeUnknown:
            default:
                break;
        }
        
        return nullptr;
    }
    
    void SubSection::SetHeroSpawnPointForTile(Tile* aTile)
    {
        if(aTile->IsHeroSpawnPoint() == true)
        {
            aTile->SetIsHeroSpawnPoint(false);
        }
        else
        {
            //Cycle through and ensure there isn't already a spawn point set for this hero
            for(unsigned int i = 0; i < GetNumberOfTiles(); i++)
            {
                GetTileForIndex(i)->SetIsHeroSpawnPoint(false);
            }
            
            //Set the tile hero spawn point flag
            aTile->SetIsHeroSpawnPoint(true);
            
            //If there is an enemy spawn point set, turn it off
            if(aTile->IsEnemySpawnPoint() == true)
            {
                aTile->SetIsEnemySpawnPoint(false);
            }
        }
    }
    
    void SubSection::SpawnEnemy(Tile* aTile, bool aIsBlue)
    {
        if(m_World != nullptr)
        {
            if(aIsBlue == false)
                m_Enemies.push_back(new Enemy(m_World, aTile));
            else
                m_Enemies.push_back(new Enemy(m_World, aTile, "EnemyBlue"));
        }
    }
    
    void SubSection::EnableDebugDrawing(unsigned int aDebugDrawingFlags)
    {
#if DEBUG
        //Set the debug draw flags
        m_DebugDrawingFlags = aDebugDrawingFlags;
        
        //Is there anything to draw?
        if(m_DebugDrawingFlags != DebugDrawNothing)
        {
            //Create a render target, if one doesn't already exist
            if(m_DebugRenderTarget == nullptr)
            {
                unsigned int width = GetTileSize() * GetNumberOfHorizontalTiles();
                unsigned int height = GetTileSize() * GetNumberOfVerticalTiles();
                m_DebugRenderTarget = new RenderTarget(width, height);
                m_DebugRenderTarget->CreateFrameBuffer();
                m_DebugRenderTarget->GetTextureFrame()->SetShader(ServiceLocator::GetShaderManager()->GetPassthroughTextureShader());
            }

            //Refresh the debug draw render target
            RefreshDebugDraw();
        }
        else
        {
            //Delete the render target
            SafeDelete(m_DebugRenderTarget);
        }
#endif
    }
    
    void SubSection::DisableDebugDrawing()
    {
        EnableDebugDrawing(DebugDrawNothing);
    }
    
    unsigned int SubSection::GetDebugDrawFlags()
    {
        return m_DebugDrawingFlags;
    }
    
    void SubSection::RefreshDebugDraw()
    {
#if DEBUG
        //Cache the graphics active render target
        RenderTarget* oldRenderTarget = ServiceLocator::GetGraphics()->GetActiveRenderTarget();
        
        //Set the Label's render target as the active render target
        ServiceLocator::GetGraphics()->SetActiveRenderTarget(m_DebugRenderTarget);
        ServiceLocator::GetGraphics()->Clear();

        //Draw the debug tile data
        bool tileData = (m_DebugDrawingFlags & (DebugDrawTileIndex)) != 0 || (m_DebugDrawingFlags & (DebugDrawTileCoordinates)) != 0|| (m_DebugDrawingFlags & (DebugDrawTilePositionLocal)) != 0|| (m_DebugDrawingFlags & (DebugDrawTilePositionWorld)) != 0;
        if(tileData == true)
        {
            DebugDrawTileData();
        }
        
        //Draw the enemy pathfinding data
        bool pathFindingScores = (m_DebugDrawingFlags & (DebugDrawPathFindingScores)) != 0;
        if(pathFindingScores == true)
        {
            for(unsigned int i = 0; i < m_Enemies.size(); i++)
            {
                m_Enemies.at(i)->DebugDraw();
            }
        }
        
        //Draw the sub section data
        bool coordinateData = (m_DebugDrawingFlags & (DebugDrawSubSectionIndex)) != 0 || (m_DebugDrawingFlags & (DebugDrawSubSectionCoordinates)) != 0 || (m_DebugDrawingFlags & (DebugDrawSubSectionPosition)) != 0;
        if(coordinateData == true)
        {
            DebugDrawSubSectionData();
        }
        
        //Set the original graphic's render target back
        ServiceLocator::GetGraphics()->SetActiveRenderTarget(oldRenderTarget);
    #endif
    }

    void SubSection::DebugDrawTileData()
    {
        //Which debug tile data do we draw
        bool tileIndex = (m_DebugDrawingFlags & (DebugDrawTileIndex)) != 0;
        bool tileCoordinates = (m_DebugDrawingFlags & (DebugDrawTileCoordinates)) != 0;
        bool tilePositionLocal = (m_DebugDrawingFlags & (DebugDrawTilePositionLocal)) != 0;
        bool tilePositionWorld = (m_DebugDrawingFlags & (DebugDrawTilePositionWorld)) != 0;
        
        //Set the font size
        const unsigned int fontSizes[] = {32, 18, 13, 12};
        unsigned int fontSize = fontSizes[0];
        
        if(tilePositionWorld)
        {
            fontSize = fontSizes[3];
        }
        else
        {
            if(tilePositionLocal)
            {
                fontSize = fontSizes[2];
            }
            else
            {
                fontSize = fontSizes[1];
            }
        }

        
        //Create the debug draw label (on the stack), it will only be used in this method, which is seldomly called
        Label debugLabel("Ubuntu-B", "ttf", fontSize);
        debugLabel.SetAnchorPoint(0.5f, 0.5f);
        debugLabel.SetJustification(JustifyCenter);
        debugLabel.SetColor(Color::WhiteColor());
    
        //Create the rect outline
        Rect rect;
        rect.SetSize(GetTileSize(), GetTileSize());
        rect.SetAnchorPoint(0.5f, 0.5f);
        rect.SetColor(Color::WhiteColor());
        rect.SetIsFilled(false);
    
        //Initialize a string stream
        stringstream ss;
        
        //Cycle through all the tiles in the subsection
        for(unsigned int i = 0; i < GetNumberOfTiles(); i++)
        {
            //Set the position for the debug label
            vec2 position = GetTileForIndex(i)->GetCenter(false);
            debugLabel.SetLocalPosition(position);
            
            //Clear the string stream
            ss.str("");
            
            //Do we need to draw the tile index data
            if(tileIndex == true)
            {
                //Add the index to the string stream
                ss << i;
                
                //Do we also have to draw the tile coordinates data?
                if(tileCoordinates == true || tilePositionLocal == true || tilePositionWorld == true)
                {
                    //If we do - then do a new line
                    ss << "\n";
                }
                else
                {
                    //Otherwise set the text and draw the label
                    debugLabel.SetText(ss.str());
                    debugLabel.Draw();
                }
            }

            //Do we need to draw the tile coordinate data
            if(tileCoordinates == true)
            {
                //Add the tile coordinates to the string stream
                uvec2 coordinates = GetTileCoordinatesForIndex(i);
                ss << "(" << coordinates.x << "," << coordinates.y << ")";
                
                //Do we also have to draw the tile coordinates data?
                if(tilePositionLocal == true || tilePositionWorld == true)
                {
                    //If we do - then do a new line
                    ss << "\n";
                }
                else
                {
                    //Otherwise set the text and draw the label
                    debugLabel.SetText(ss.str());
                    debugLabel.Draw();
                }
            }
            
            //Do we need to draw the tile's local position
            if(tilePositionLocal == true)
            {
                //Add the tile coordinates to the string stream
                Tile* tile = GetTileForIndex(i);
                ss << "(" << tile->GetLocalX() << "," << tile->GetLocalY() << ")";
                
                //Do we also have to draw the tile coordinates data?
                if(tilePositionWorld == true)
                {
                    //If we do - then do a new line
                    ss << "\n";
                }
                else
                {
                    //Otherwise set the text and draw the label
                    debugLabel.SetText(ss.str());
                    debugLabel.Draw();
                }
            }
            
            //Do we need to draw the tile's world position
            if(tilePositionWorld == true)
            {
                //Add the tile coordinates to the string stream
                Tile* tile = GetTileForIndex(i);
                ss << "(" << tile->GetWorldX() << "," << tile->GetWorldY() << ")";
                

                //Set the text and draw the label
                debugLabel.SetText(ss.str());
                debugLabel.Draw();
            }
        
        
            //Draw a white Rect around the Tile
            rect.SetLocalPosition(position);
            rect.Draw();
        }
    }
    
    void SubSection::DebugDrawSubSectionData()
    {
        //Which debug tile data do we draw
        bool subSectionIndex = (m_DebugDrawingFlags & (DebugDrawSubSectionIndex)) != 0;
        bool subSectionCoordinates = (m_DebugDrawingFlags & (DebugDrawSubSectionCoordinates)) != 0;
        bool subSectionPosition = (m_DebugDrawingFlags & (DebugDrawSubSectionPosition)) != 0;
        unsigned int fontSize = subSectionIndex == true && subSectionCoordinates == false ? 60 : 42;
        
        //Create the debug draw label (on the stack), it will only be used in this method, which is seldomly called
        Label debugLabel("Ubuntu-B", "ttf", fontSize);
        debugLabel.SetAnchorPoint(0.5f, 0.5f);
        debugLabel.SetJustification(JustifyCenter);
        debugLabel.SetColor(Color::GhostWhiteColor());
        
        //Initialize a string stream
        stringstream ss;
    
        //Get the index for this subsection
        unsigned int index = m_World->GetSubSectionIndexForPosition(GetWorldPosition());
    
        //Set the position for the debug label
        vec2 position = vec2((float)(GetTileSize() * GetNumberOfHorizontalTiles()) / 2.0f, (float)(GetTileSize() * GetNumberOfVerticalTiles()) / 2.0f);
        debugLabel.SetLocalPosition(position);
        
        //Do we need to draw the subsection index data
        if(subSectionIndex == true)
        {
            //Add the index to the string stream
            ss << index;
            
            //Do we also have to draw the subsection coordinates data?
            if(subSectionCoordinates == true || subSectionPosition == true)
            {
                //If we do - then do a new line
                ss << "\n";
            }
            else
            {
                //Otherwise set the text and draw the label
                debugLabel.SetText(ss.str());
                debugLabel.Draw();
            }
        }

        //Do we need to draw the subsection coordinate data
        if(subSectionCoordinates == true)
        {
            //Add the subsection coordinates to the string stream
            uvec2 coordinates = m_World->GetSubSectionCoordinatesForIndex(index);
            ss << "(" << coordinates.x << "," << coordinates.y << ")";
            
            //Do we also have to draw the subsection coordinates data?
            if(subSectionPosition == true)
            {
                //If we do - then do a new line
                ss << "\n";
            }
            else
            {
                //Otherwise set the text and draw the label
                debugLabel.SetText(ss.str());
                debugLabel.Draw();
            }
        }
        
        //Do we need to draw the subsection position data
        if(subSectionPosition == true)
        {
            //Add the subsection coordinates to the string stream
            ss << "(" << GetWorldX() << "," << GetWorldY() << ")";
            

            //Set the text and draw the label
            debugLabel.SetText(ss.str());
            debugLabel.Draw();
        }
        
        //Draw a white Rect around the SubSection
        Rect rect(GetLocalPosition(), GetSize());
        rect.SetColor(Color::WhiteColor());
        rect.SetIsFilled(false);
        rect.Draw();
    }

	//For world wrapping
	void SubSection::SetEdge(bool aIsEdge)
	{
		m_IsEdge = aIsEdge;
	}
	bool SubSection::GetIsEdge()
	{
		return m_IsEdge;
	}

	void SubSection::SetWrapsTo(SubSection* aWrapsTo)
	{
		m_WrapsTo = aWrapsTo;
	}
	SubSection* SubSection::GetWrapsTo()
	{
		return m_WrapsTo;
	}
}