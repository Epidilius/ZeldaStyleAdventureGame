//
//  Projectile.h
//  GameDev2D
//
//  Created by Joel Cright on 2014-11-13.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Projectile__
#define __GameDev2D__Projectile__

#include "../Source/Core/GameObject.h"

namespace GameDev2D
{
    //Forward Declarations
    class Tile;
    class SubSection;
    
    //
    class Projectile : public GameObject
    {
    public:
        Projectile(SubSection* aSubSection);
        ~Projectile();
        
        //Updates the projectile, disable it when iot goes out of the subsection
        void Update(double delta);
        
        //Conveniacne method used to setup the projectile
        void Setup(vec2 aStartingPosition, vec2 aDirection, float aSpeed, unsigned int aAttackDamage);
        
        //Returns the subsection the projectile is on
        SubSection* GetSubSection();
        
        //Returns the tile the projectile is currentl over
        Tile* GetTile();
        
        //Returns the current direction of the projectile
        vec2 GetDirection();
        
        //Returns the current speed of the projectile
        float GetSpeed();
        
        //Returns the current attack damage of the projectile
        unsigned int GetAttackDamage();
        
        //TODO:
        /*
         SetDirection
         SetSpeed
         SetAttackDamage
         */
        
        
    private:
        //Member variables
        SubSection* m_SubSection;
        vec2 m_Direction;
        float m_Speed;
        unsigned int m_AttackDamage;
    };
}

#endif /* defined(__GameDev2D__Projectile__) */
