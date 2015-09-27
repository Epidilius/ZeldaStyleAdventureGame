//
//  Game.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#include "Game.h"
#include "World.h"
#include "SubSection.h"
#include "HeadsUpDisplay.h"
#include "../Source/Services/ServiceLocator.h"
#include "../Source/Platforms/PlatformLayer.h"
#include "../Source/Audio/Audio.h"
#include "../Source/UI/UI.h"
#include "../Source/Graphics/Graphics.h"
#include "../Source/Events/Input/InputEvents.h"

namespace GameDev2D
{
    Game::Game() : Scene("Game"),
        m_World(nullptr),
        m_HeadsUpDisplay(nullptr)
    {

    }
    
    Game::~Game()
    {
        SafeDelete(m_World);
        SafeDelete(m_HeadsUpDisplay);
    }
    
    float Game::LoadContent()
    {
        //Create the World object
        if(m_World == nullptr)
        {
            m_World = new World();
        }
        
        //Create the heads up display object
        if(m_HeadsUpDisplay == nullptr)
        {
            m_HeadsUpDisplay = new HeadsUpDisplay(m_World);
            ServiceLocator::GetSceneManager()->PreloadScene(m_HeadsUpDisplay);
        }
        
        //Return the percentage loaded
        return m_World->LoadContent();
    }
    
    void Game::Update(double aDelta)
    {
        //Update the world
        if(m_World != nullptr)
        {
            m_World->Update(aDelta);
        }
        
        //Update the heads up display
        if(m_HeadsUpDisplay != nullptr)
        {
            m_HeadsUpDisplay->Update(aDelta);
        }
        
        Scene::Update(aDelta);
    }
    
    void Game::Draw()
    {
        //Draw the world
        if(m_World != nullptr)
        {
            ServiceLocator::GetSceneManager()->DrawScene(m_World);
        }
        
        //Draw the Heads up display
        if(m_HeadsUpDisplay != nullptr)
        {
            ServiceLocator::GetSceneManager()->DrawScene(m_HeadsUpDisplay);
        }
        
        Scene::Draw();
    }
    
    void Game::HandleEvent(Event* aEvent)
    {
        switch (aEvent->GetEventCode())
        {
            case MOUSE_CLICK_EVENT:
            {

            }
            break;
            
            case KEYBOARD_EVENT:
            {
            #if DEBUG
                //Handle keyboard to enable debug drawing
                KeyEvent* keyEvent = (KeyEvent*)aEvent;
                if(keyEvent->GetKeyEventType() == KeyUp)
                {
                    if(m_World != nullptr && m_World->GetActiveSubSection() != nullptr)
                    {
                        if(keyEvent->GetKeyCode() == KEY_CODE_0)
                        {
                            m_World->GetActiveSubSection()->DisableDebugDrawing();
                        }
                        else if(keyEvent->GetKeyCode() == KEY_CODE_1)
                        {
                            m_World->GetActiveSubSection()->EnableDebugDrawing(m_World->GetActiveSubSection()->GetDebugDrawFlags() ^ DebugDrawTileIndex);
                        }
                        else if(keyEvent->GetKeyCode() == KEY_CODE_2)
                        {
                            m_World->GetActiveSubSection()->EnableDebugDrawing(m_World->GetActiveSubSection()->GetDebugDrawFlags() ^ DebugDrawTileCoordinates);
                        }
                        else if(keyEvent->GetKeyCode() == KEY_CODE_3)
                        {
                            m_World->GetActiveSubSection()->EnableDebugDrawing(m_World->GetActiveSubSection()->GetDebugDrawFlags() ^ DebugDrawTilePositionLocal);
                        }
                        else if(keyEvent->GetKeyCode() == KEY_CODE_4)
                        {
                            m_World->GetActiveSubSection()->EnableDebugDrawing(m_World->GetActiveSubSection()->GetDebugDrawFlags() ^ DebugDrawTilePositionWorld);
                        }
                        else if(keyEvent->GetKeyCode() == KEY_CODE_5)
                        {
                            m_World->GetActiveSubSection()->EnableDebugDrawing(m_World->GetActiveSubSection()->GetDebugDrawFlags() ^ DebugDrawPathFindingScores);
                        }
                        else if(keyEvent->GetKeyCode() == KEY_CODE_6)
                        {
                            m_World->GetActiveSubSection()->EnableDebugDrawing(m_World->GetActiveSubSection()->GetDebugDrawFlags() ^ DebugDrawSubSectionIndex);
                        }
                        else if(keyEvent->GetKeyCode() == KEY_CODE_7)
                        {
                            m_World->GetActiveSubSection()->EnableDebugDrawing(m_World->GetActiveSubSection()->GetDebugDrawFlags() ^ DebugDrawSubSectionCoordinates);
                        }
                        else if(keyEvent->GetKeyCode() == KEY_CODE_8)
                        {
                            m_World->GetActiveSubSection()->EnableDebugDrawing(m_World->GetActiveSubSection()->GetDebugDrawFlags() ^ DebugDrawSubSectionPosition);
                        }
                    }
                }
            #endif
            }
            break;
        
            default:
                break;
        }
    }
}