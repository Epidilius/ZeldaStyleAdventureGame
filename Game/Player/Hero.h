//
//  Hero.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-14.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Hero__
#define __GameDev2D__Hero__

#include "Player.h"
//#include "World.h"
#include "../World.h"
#include "../../Source/Input/KeyCodes.h"


namespace GameDev2D
{
    //Enum to help manage the hero's internal state
    enum HeroState
    {
        HeroIdle = 0,
        HeroWalking,
        HeroAttacking,
        HeroUnknown
    };
    
    //Local constants
    const string HERO_WALKING_ATLAS_KEY_1[] = {"LinkUp-1", "LinkDown-1", "LinkLeft-1", "LinkRight-1"};
    const string HERO_WALKING_ATLAS_KEY_2[] = {"LinkUp-2", "LinkDown-2", "LinkLeft-2", "LinkRight-2"};
    const string HERO_ATTACKING_ATLAS_KEY[] = {"LinkAttack-Up", "LinkAttack-Down", "LinkAttack-Left", "LinkAttack-Right"};
    const vec2 HERO_ATTACKING_ANCHOR_POINT[] = {vec2(0.5f, 0.3f), vec2(0.5f, 0.675f), vec2(0.7f, 0.5f), vec2(0.3f, 0.5f)};
    const double HERO_ATTACK_TIME = 0.2;
    const float HERO_WALKING_SPEED = 200.0f;
    const float HERO_WALKING_ANIMATION_FRAME_SPEED = 8.0f;
    const unsigned int HERO_INITIAL_HEALTH = 6;
    const unsigned int HERO_INITIAL_HEALTH_CAPACITY = 6;
    const unsigned char HERO_MAX_BOMBS = 6;
    const vec2 HERO_SWORD_RANGE = vec2(70.0f, 68.0f);
    const double HERO_INVINCIBILITY_DURATION = 0.65;
    const float HERO_COLLISION_ZONE = 20.0f;
    const bool HERO_HAS_SWORD_BY_DEFAULT = false;

    //Forward Declarations
    class Sprite;
    class Tile;
    class Audio;
    class Circle;
    class Enemy;
    //enum Doors;
    
    //The Hero class is controlled by keyboard input
    class Hero : public Player, EventHandler
    {
    public:
        Hero(World* world, Tile* spawnTile);
        ~Hero();

        //Update method, used to determine if an enemy has been attacked
        void Update(double delta);

        //Used to reset the hero
        void Reset();

        //Pure virtual function used to manage the hero's internal state
        void SetState(unsigned int state);
        
        //Returns wether the hero has his sword or not
        bool HasSword();
        
        //Call this method when the hero picks up a sword, allows the
        //hero to now be able to swing his sword and attack with it
        void PickedUpSword();
        
        //Adds rupees to the hero's rupee stock-pile
        void AddRupees(unsigned short rupees);
        void AddBombs(unsigned short bombs);
        
        //Returns the number of rupees the hero has
        unsigned short GetRupees();
        unsigned short GetBombs();
        
        //Returns wether the hero is invincible or not
        bool IsInvincible();
        
        //Applies damage to the hero
        void ApplyDamage(unsigned int attackDamage);

        // Add a certain amount of health to the hero
        void AddHealth(unsigned int health);

        
    private:
        //Called when the Hero has ceased to exist
        void HasDied();
        
        //Methods to handle hero input
        void HandleEvent(Event* event);
        void HandleKeyDown(KeyCode keyCode);
        void HandleKeyUp(KeyCode keyCode);
        
        //Method used to stop attacking, called from a delayed method
        void StopAttacking();

        void HasChangedTiles(Tile* newTile, Tile* oldTile);
        void GetYourPickups(Tile* aTileToGetThingsFrom);
        void TeleportToCave();
    
        //Member variables
        Sprite* m_AttackingSprite[4];
        HeroState m_HeroState;
        unsigned short m_Rupees;
        unsigned short m_Bombs;
        bool m_HasSword;
        World::Doors m_LastDoor;
        Timer* m_InvincibilityTimer;
        Audio* m_LowHealthAudio;
        Audio* m_HeroDeathAudio;
        Audio* m_AttackingAudio;
        Audio* m_HeartPickupAudio;
        Audio* m_RupeePickupAudio;
        Audio* m_ItemPickupAudio;
        Audio* m_SwordPickupAudio;
        Audio* m_FirstTimeToCaveAudio;
       
        vector<Enemy*> m_Enemies;
    };
}

#endif /* defined(__GameDev2D__Hero__) */

