//
//  MainMenu.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-16.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "MainMenu.h"
#include "../Game.h"
#include "../LevelEditor/LevelEditor.h"
#include "../../Source/Services/ServiceLocator.h"
#include "../../Source/Platforms/PlatformLayer.h"


namespace GameDev2D
{
    MainMenu::MainMenu() : Menu("MainMenu")
    {
    
    }
    
    MainMenu::~MainMenu()
    {
    
    }
    
    float MainMenu::LoadContent()
    {
        SetTitle("Main Menu");
        AddButton("Start Game");
        AddButton("Level Editor");
        AddButton("Exit");
        return 1.0f;
    }
    
    void MainMenu::HandleButtonAction(unsigned int aButtonIndex, const string& aButtonText)
    {
        switch (aButtonIndex)
        {
            case MainMenuStart:
                ServiceLocator::GetSceneManager()->PushScene(new Game(), true);
                break;

            case MainMenuLevelEditor:
                ServiceLocator::GetSceneManager()->PushScene(new LevelEditor(), true);
                break;
                
            case MainMenuExit:
                ServiceLocator::GetPlatformLayer()->Shutdown();
                break;

            default:
                break;
        }
    }
}