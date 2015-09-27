//
//  PathFinder.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-20.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "PathFinder.h"
#include "PathNode.h"
#include "../SubSection.h"
#include "../Tiles/Tile.h"
#include "../../Source/UI/UI.h"
#include "../../Source/Graphics/Primitives/Primitives.h"


namespace GameDev2D
{
    PathFinder::PathFinder(SubSection* aSubSection) : BaseObject("PathFinder"),
        m_SubSection(aSubSection),
        m_State(StateIdle),
        m_DestinationTileIndex(-1),
        m_DebugLabel(nullptr),
        m_DebugRect(nullptr)
    {
#if DEBUG
        m_DebugLabel = new Label("OpenSans-CondBold", "ttf", 18);
        m_DebugRect = new Rect();
        m_DebugRect->SetIsFilled(false);
        m_DebugRect->SetAnchorPoint(0.5f, 0.5f);
#endif
    }

    PathFinder::~PathFinder()
    {
        SafeDelete(m_DebugLabel);
        SafeDelete(m_DebugRect);
    
        ClearPathNodes();
    }
    
    void PathFinder::DebugDraw()
    {
#if DEBUG
        //Paint the open list path scoring
        m_DebugRect->SetColor(Color::BlueColor());
        for(unsigned int i = 0; i < m_PathNodeOpen.size(); i++)
        {
            m_PathNodeOpen.at(i)->DebugDraw(m_DebugLabel, m_DebugRect);
        }

        //Paint the closed list path scoring
        m_DebugRect->SetColor(Color::RedColor());
        for(unsigned int i = 0; i < m_PathNodeClosed.size(); i++)
        {
            m_PathNodeClosed.at(i)->DebugDraw(m_DebugLabel, m_DebugRect);
        }

        //Paint the final path scoring
        m_DebugRect->SetColor(Color::YellowColor());
        for(unsigned int i = 0; i < m_PathNodeFinal.size(); i++)
        {
            m_PathNodeFinal.at(i)->DebugDraw(m_DebugLabel, m_DebugRect);
        }
#endif
    }
    
    void PathFinder::Reset()
    {
        //Set the state to idle
        m_State = StateIdle;
        
        //Clear the path nodes
        ClearPathNodes();
    }
    
    bool PathFinder::FindPath(Tile* aStartingTile, Tile* aDestinationTile)
    {
        //Reset the Pathfinder before we do a search
        Reset();
        
        //Get the starting tile and the destination tile index
        int startingtileIndex = m_SubSection->GetTileIndexForTile(aStartingTile);
        m_DestinationTileIndex = m_SubSection->GetTileIndexForTile(aDestinationTile);
        
        //Safety check that the destination and starting tile indexes are not the same
        if(startingtileIndex == m_DestinationTileIndex)
        {
            Error(false, "Couldn't find path, the current tile and the destination tile are the same");
            return false;
        }
    
        //Calculate the H score
        int scoreH = GetManhattanDistanceCost(aStartingTile, aDestinationTile);
        
        //Allocate the starting tile's Path Node and add it to the Open List
        PathNode* pathNode = new PathNode(aStartingTile, scoreH);
        AddPathNodeToOpenList(pathNode);
        
        //Set the state to searching
        m_State = StateSearchingPath;
        
        //Loop through the Open List until a path is found OR a path can NOT be found
        while(IsSearchingForPath() == true && m_DestinationTileIndex != -1)
        {
            //Safety check that there are Path NOdes in the Open List
            if(m_PathNodeOpen.size() == 0)
            {
                //Set the state to error and return false
                Error(false, "Couldn't find path");
                m_State = StateError;
                return false;
            }
            
            //Get the node with the lowest F score from the Open List, since it's sorted it will be the first element in the Open List
            PathNode* currentNode = m_PathNodeOpen.front();
            
            //Add the node to the closed list and remove it from the open list
            m_PathNodeClosed.push_back(currentNode);
            m_PathNodeOpen.erase(m_PathNodeOpen.begin());
            
            //Check to see if the node is at the destination tile
            int currentTileIndex = m_SubSection->GetTileIndexForTile(currentNode->GetTile());
            if(currentTileIndex == m_DestinationTileIndex)
            {
                //Build the final node path
                BuildFinalNodePath(currentNode);
                
                //Set the state to path found
                m_State = StateFoundPath;
                
                return true;
            }
            
            //If we got here that means we gaven't reached the destination tile. We need to get the adjacent tiles for the current node's tile and add them to the Open List
            vector<Tile*> adjacentTiles;
            
            //Check the adjacent tiles for walkability
            /*for(int i = -1; i < 1; i++)
            {
                for(int j = -1; j < 1; j++)
                {
                    //TODO: Test this
                    //TODO: Figure this out
                    //AddAdjacentTile(adjacentTiles, currentNode->GetTile(), i, j);
                }
            }*/
            
            /*
             for(int i = -1; i < 2; i++)
             {
             if(i == 0)
             continue;
             
             AddAdjacentTile(adjacentTiles, currentNode->GetTile(), i, 0);
             }
             
             for(int j = -1; j < 2; j++)
             {
             if(j == 0)
             continue;
             
             AddAdjacentTile(adjacentTiles, currentNode->GetTile(), 0, j);
             }
             */
            AddAdjacentTile(adjacentTiles, currentNode->GetTile(), 0, 1);
            AddAdjacentTile(adjacentTiles, currentNode->GetTile(), 0, -1);
            AddAdjacentTile(adjacentTiles, currentNode->GetTile(), 1, 0);
            AddAdjacentTile(adjacentTiles, currentNode->GetTile(), -1, 0);
            
            //Cycle through the walkable tiles
            for(unsigned int i = 0; i < adjacentTiles.size(); i++)
            {
                Tile* adjacentTile = adjacentTiles.at(i);
                
                //Does the tile exist in the closed list?
                if(DoesTileExistInClosedList(adjacentTile) == true)
                    continue;
            
                //Does the tile exist in the open list?
                if(DoesTileExistInOpenList(adjacentTile) == false)
                {
                    //Calculate the H score
                    Tile* destinationTile = m_SubSection->GetTileForIndex(m_DestinationTileIndex);
                    int scoreH = GetManhattanDistanceCost(adjacentTile, destinationTile);
                
                    //Create a PathNode object for the Tile
                    PathNode* pathNode = new PathNode(adjacentTile, currentNode, scoreH);
                
                    //Add the PathNode to the open list, this method also takes care of sorting the open list
                    AddPathNodeToOpenList(pathNode);
                }
                else
                {
                    //If the tile does exist in the open list, compare the G score and keep the PathNode with the lower G score
                    PathNode* existingNode = GetOpenPathNodeForTile(adjacentTile);
                    
                    //If the tile has a lower G score, update the new G score and the parent
                    if(currentNode->GetScoreG() + 1 < existingNode->GetScoreG())
                    {
                        //Update the existing node's parent, this will also update the G score of the existing node
                        existingNode->SetParentNode(currentNode);
                        
                        //Re-Sort the Open List
                        SortOpenList();
                    }
                }
            }
        }
        
        //If we got here, then a path was NOT found
        return false;
    }
    
    bool PathFinder::IsSearchingForPath()
    {
        return m_State == StateSearchingPath;
    }
    
    int PathFinder::GetPathSize()
    {
        return m_PathNodeFinal.size();
    }
    
    PathNode* PathFinder::GetPathNodeAtIndex(unsigned int aIndex)
    {
        if(aIndex < GetPathSize())
        {
            return m_PathNodeFinal.at(aIndex);
        }
        return nullptr;
    }
        
    void PathFinder::AddAdjacentTile(std::vector<Tile*>& aAdjacentTiles, Tile* aCurrentTile, int aDeltaX, int aDeltaY)
    {
        //Get the adjacent tile's coordinates
        ivec2 adjacentCoordinate = m_SubSection->GetTileCoordinatesForPosition(aCurrentTile->GetCenter(false));
        adjacentCoordinate.x += aDeltaX;
        adjacentCoordinate.y += aDeltaY;

        //Validate the tile coordinates
        if(m_SubSection->ValidateCoordinates(adjacentCoordinate) == true)
        {
            //Verify that the adjacent tile is walkable
            Tile* adjacentTile = m_SubSection->GetTileForCoordinates(adjacentCoordinate);
            if(adjacentTile != nullptr)
            {
                if(adjacentTile->IsWalkable() == true)
                {
                    aAdjacentTiles.push_back(adjacentTile);
                }
            }
        }
    }
    
    bool PathFinder::DoesTileExistInClosedList(Tile* aTile)
    {
        //Get the tile index from the subsection for the tile pointer
        int tileIndex = m_SubSection->GetTileIndexForTile(aTile);
        
        //Cycle through the closed list and compare the tile indexes
        for(int i = 0; i < m_PathNodeClosed.size(); i++)
        {
            PathNode* pathNode = m_PathNodeClosed.at(i);
            if(m_SubSection->GetTileIndexForTile(pathNode->GetTile()) == tileIndex)
            {
                return true;
            }
        }
        
        //The tile doesn't exist in the closed list
        return false;
    }

    bool PathFinder::DoesTileExistInOpenList(Tile* aTile)
    {
        return GetOpenPathNodeForTile(aTile) != nullptr;
    }

    PathNode* PathFinder::GetOpenPathNodeForTile(Tile* aTile)
    {
        //Get the tile index from the subsection for the tile pointer
        int tileIndex = m_SubSection->GetTileIndexForTile(aTile);
        
        //Cycle through the open list and compare the tile indexes
        for(int i = 0; i < m_PathNodeOpen.size(); i++)
        {
            PathNode* pathNode = m_PathNodeOpen.at(i);
            if(m_SubSection->GetTileIndexForTile(pathNode->GetTile()) == tileIndex)
            {
                return pathNode;
            }
        }
        
        //The tile doesn't exist in the open list, return null
        return nullptr;
    }
    
    void PathFinder::SortOpenList()
    {
        sort(m_PathNodeOpen.begin(), m_PathNodeOpen.end(), PathNode::CompareNodes);
    }
    
    void PathFinder::AddPathNodeToOpenList(PathNode* aPathNode)
    {
        //Insert the Path node into the Open path node vector
        m_PathNodeOpen.push_back(aPathNode);
        
        //Then, sort the open list
        SortOpenList();
    }
    
    void PathFinder::BuildFinalNodePath(PathNode* aPathNode)
    {
        do
        {
            //Safety check the parentNode
            if(aPathNode->GetParent() != nullptr)
            {
                m_PathNodeFinal.insert(m_PathNodeFinal.begin(), aPathNode);
            }
            
            //Set the path node's pointer to it's parent
            aPathNode = aPathNode->GetParent();
        }
        while (aPathNode != nullptr);
    }
    
    void PathFinder::ClearPathNodes()
    {
        //Now cycle through the Open node path vector, and delete all the path node
        while(m_PathNodeOpen.size() > 0)
        {
            //Get the last element in the vector
            PathNode* node = m_PathNodeOpen.back();
            
            //Delete the path node
            delete node;
            
            //Remove the last element in the vector
            m_PathNodeOpen.pop_back();
        }
        
        //Lastly cycle through the Closed node path vector, and delete all the path node
        while(m_PathNodeClosed.size() > 0)
        {
            //Get the last element in the vector
            PathNode* node = m_PathNodeClosed.back();
            
            //Delete the path node
            delete node;
            
            //Remove the last element in the vector
            m_PathNodeClosed.pop_back();
        }
        
        //Clear the final path node list
        m_PathNodeFinal.clear();
        
        //Reset the destination tile index
        m_DestinationTileIndex = -1;
    }
    
    int PathFinder::GetManhattanDistanceCost(Tile* aStartTile, Tile* aDestinationTile)
    {
        //Here we use the Manhattan method, which calculates the total number of step moved horizontally and vertically to reach the
        //final desired step from the current step, ignoring any obstacles that may be in the way
        uvec2 startCoordinates = m_SubSection->GetTileCoordinatesForPosition(aStartTile->GetCenter(false));
        uvec2 destinationCoordinates = m_SubSection->GetTileCoordinatesForPosition(aDestinationTile->GetCenter(false));
        
        //Calculate the manhattan distance
        int distance = std::abs((int)(destinationCoordinates.x - startCoordinates.x)) + std::abs((int)(destinationCoordinates.y - startCoordinates.y));
        return distance;
    }
}