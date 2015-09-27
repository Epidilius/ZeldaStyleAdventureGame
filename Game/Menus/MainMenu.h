//
//  MainMenu.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-16.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__MainMenu__
#define __GameDev2D__MainMenu__

#include "Menu.h"


namespace GameDev2D
{
    enum MainMenuOptions
    {
        MainMenuStart = 0,
        MainMenuLevelEditor,
        MainMenuExit
    };

    class MainMenu : public Menu
    {
    public:
        MainMenu();
        ~MainMenu();
        
        float LoadContent();
        
        void HandleButtonAction(unsigned int buttonIndex, const string& buttonText);
    };
}

#endif /* defined(__GameDev2D__MainMenu__) */
