//
//  Enemy.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-20.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Enemy__
#define __GameDev2D__Enemy__

#include "Player.h"


namespace GameDev2D
{
    //Enum to help manage the enemy's internal state
    enum EnemyState
    {
        EnemyIdle = 0,
        EnemySearching,
        EnemyAttacking,
        EnemyWalking,
    
        //Add additonal enemy states here

        EnemyStateCount,
        EnemyDead,
        EnemyUnknown
    };
    
    //Local constants
    const string ENEMY_WALKING_ATLAS_KEY_1[] = {"EnemyUp-1", "EnemyDown-1", "EnemyLeft-1", "EnemyRight-1"};
    const string ENEMY_WALKING_ATLAS_KEY_2[] = {"EnemyUp-2", "EnemyDown-2", "EnemyLeft-2", "EnemyRight-2"};
    const string ENEMY_BLUE_WALKING_ATLAS_KEY_1[] = {"EnemyBlueUp-1", "EnemyBlueDown-1", "EnemyBlueLeft-1", "EnemyBlueRight-1"};
    const string ENEMY_BLUE_WALKING_ATLAS_KEY_2[] = {"EnemyBlueUp-2", "EnemyBlueDown-2", "EnemyBlueLeft-2", "EnemyBlueRight-2"};
    const float ENEMY_WALKING_SPEED = 128.0f;
    const float ENEMY_WALKING_ANIMATION_FRAME_SPEED = 5.0f;
    const double ENEMY_IDLE_TIME_INCREMENTS = 0.5;
    const unsigned int ENEMY_MIN_IDLE_TIME = 1;
    const unsigned int ENEMY_MAX_IDLE_TIME = 4;
    const double ENEMY_PROJECTILE_DELAY = 1.5;
    const float ENEMY_PROJECTILE_SPEED = 300.0f;
    const unsigned int ENEMY_PROJECTILE_ATTACK_DAMAGE = 2;
    const unsigned int ENEMY_HEALTH_CAPACITY = 2;
    const unsigned int ENEMY_BLUE_HEALTH_CAPACITY = 2;
    const double ENEMY_INVINCIBILITY_DURATION = 0.65;
    
    //Forward declarations
    class PathFinder;
    class SubSection;
    class Random;
    class Audio;
    class Tile;

    //Create an enemy object, enemies can be killed by the Hero's sword and can apply damage to the hero when they share
    //the same time. The enemy will cycle its behaviour between Idle, randomly walking, searching for the hero and
    //firing a projectile at the hero.
    class Enemy : public Player
    {
    public:
        Enemy(World* world, Tile* spawnTile, string enemyType = "Enemy");
        ~Enemy();
        
        //Update method, used to perform pathfinding operations
        void Update(double delta);
        
        //Debug Draw method, use to draw pathfinding debug data
        void DebugDraw();
        
        //Used to reset the enemy
        void Reset();

        //Pure virtual function used to manage the enemy's internal state
        void SetState(unsigned int state);

        //Returns wether the hero is invincible or not
        bool IsInvincible();

        //Applies damage to the hero
        void ApplyDamage(unsigned int attackDamage);
        
    private:
        //Called when the Enemy has died
        void HasDied();
        
        //Called when the pathfinding rist returns a found path
        void StartWalking();

        //Walk to the next tile in the pathfinder path - called delayed recursively
        void Walk();
        
        //Method used to fire a projectile
        void FireProjectile();
        
        //Changes the enemy's current state to something different
        void ChangeState();
        
        //Refreshes the active sprite and updates it based on the current direction
        void RefreshActiveSprite();
    
        //Member variables
        PathFinder* m_PathFinder;
        EnemyState m_EnemyState;
        Random* m_Random;
        unsigned int m_PathIndex;
        Timer* m_InvincibilityTimer;
        Audio * m_EnemyDeathAudio;
        Audio * m_EnemyHitAudio;
        Tile* m_DestinationTile;
    };
}

#endif /* defined(__GameDev2D__Enemy__) */
