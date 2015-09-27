//
//  Game.h
//  GameDev2D
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#ifndef GAME_H
#define GAME_H

#include "../Source/Core/Scene.h"


namespace GameDev2D
{
    //Local Constants
    const double GAME_INTRO_DURATION = 1.5;
    
    //Forward declarations
    class World;
    class HeadsUpDisplay;

    //The main Game class
    class Game : public Scene
    {
    public:
        Game();
        ~Game();
        
        //Load game content here
        float LoadContent();
        
        //Methods to update and draw the Game World and Heads Up Display
        void Update(double delta);
        void Draw();

        //Handle events in this method
        void HandleEvent(Event* event);
        
    private:
        //Member variables
        World* m_World;
        HeadsUpDisplay* m_HeadsUpDisplay;
    };
}

#endif
