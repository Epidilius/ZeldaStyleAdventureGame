//
//  Projectile.cpp
//  GameDev2D
//
//  Created by Joel Cright on 2014-11-13.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "Projectile.h"
#include "SubSection.h"
#include "../Source/UI/UI.h"
namespace GameDev2D
{
    Projectile::Projectile(SubSection* aSubSection) : GameObject("Projectile"),
    m_SubSection(aSubSection),
    m_Direction(vec2(0.0f, 0.0f)),
    m_Speed(0.0f),
    m_AttackDamage(0)
    {
        //By default the projectile is disabled
        SetIsEnabled(false);
        
        //Load the sprite to represent the projectile
        Sprite* sprite = new Sprite("MainAtlas", "EnemyProjectile");
        sprite->SetAnchorPoint(0.5f, 0.5f);
        AddChild(sprite, true);
    }
    
    Projectile::~Projectile()
    {
        
    }
    
    void Projectile::Update(double aDelta)
    {
        //Update the projectile's position
        float x = GetLocalX() + GetDirection().x * GetSpeed() * aDelta;
        float y = GetLocalY() + GetDirection().y * GetSpeed() * aDelta;
        SetLocalPosition(x, y);
        
        //Check to see if the projectil has gone out of the subsection bounds
        vec2 localPosition = GetWorldPosition() - GetSubSection()->GetWorldPosition();
        float horizontalBounds = GetSubSection()->GetTileSize() * GetSubSection()->GetNumberOfHorizontalTiles();
        float verticalBounds = GetSubSection()->GetTileSize() * GetSubSection()->GetNumberOfVerticalTiles();
        
        if(localPosition.x < 0.0f || localPosition.x > horizontalBounds
           || localPosition.y < 0.0f || localPosition.y > verticalBounds)
        {
            //If the projectile has gone out of bounds, disable it
            SetIsEnabled(false);
        }
        
        //Update the base class
        GameObject::Update(aDelta);
    }
    
    void Projectile::Setup(vec2 aStartingPosition, vec2 aDirection, float aSpeed, unsigned int aAttackDamage)
    {
        SetLocalPosition(aStartingPosition);
        m_Direction = aDirection;
        m_Speed = aSpeed;
        m_AttackDamage = aAttackDamage;
        SetIsEnabled(true);
    }
    
    SubSection* Projectile::GetSubSection()
    {
        return m_SubSection;
    }
    
    Tile* Projectile::GetTile()
    {
        vec2 localPosition = GetWorldPosition() - GetSubSection()->GetWorldPosition();
        return GetSubSection()->GetTileForPosition(localPosition);
    }
    
    vec2 Projectile::GetDirection()
    {
        return m_Direction;
    }
    
    float Projectile::GetSpeed()
    {
        return m_Speed;
    }
    
    unsigned int Projectile::GetAttackDamage()
    {
        return m_AttackDamage;
    }

}