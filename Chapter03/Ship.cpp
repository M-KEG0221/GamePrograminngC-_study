// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Ship.h"
#include "SpriteComponent.h"
#include "InputComponent.h"
#include "CircleComponent.h"
#include "Game.h"
#include "Laser.h"
#include "Asteroid.h"
#include <iostream>

Ship::Ship(Game* game)
	:Actor(game)
	, mLaserCooldown(0.0f)
	, mRespawnTime(2.0f)
	, mRespawnTimeRemaining(mRespawnTime)
	, mState(ShipState::Alive)
{
	// Create a sprite component
	mSc = new SpriteComponent(this, 150);
	mSc->SetTexture(game->GetTexture("Assets/Ship.png"));

	// Create an input component and set keys/speed
	mMove = new InputComponent(this);
	mMove->SetForwardKey(SDL_SCANCODE_W);
	mMove->SetBackKey(SDL_SCANCODE_S);
	mMove->SetClockwiseKey(SDL_SCANCODE_A);
	mMove->SetCounterClockwiseKey(SDL_SCANCODE_D);
	mMove->SetMaxForwardSpeed(300.0f);
	mMove->SetMaxAngularSpeed(Math::TwoPi);
	mMove->SetFriction(60.0f);

	mCircle = new CircleComponent(this);
	mCircle->SetRadius(40.0f);
}

void Ship::UpdateActor(float deltaTime)
{
	mLaserCooldown -= deltaTime;

	Game* game = GetGame();
	std::vector<Asteroid*> asteroids = game->GetAsteroids();

	switch (mState)
	{
	case ShipState::Alive:
		for (auto asteroid : asteroids)
		{
			if (Intersect(*asteroid->GetCircle(), *mCircle))
			{
				mState = ShipState::Dead;
				mSc->SetTexture(nullptr);
			}
		}
		break;
	case ShipState::Dead:
		mRespawnTimeRemaining -= deltaTime;

		if (mRespawnTimeRemaining <= 0.0f) {
			mRespawnTimeRemaining = mRespawnTime;

			SetPosition(Vector2(512.0f, 384.0f));
			SetRotation(Math::PiOver2);

			mState = ShipState::Alive;
			mSc->SetTexture(game->GetTexture("Assets/Ship.png"));
		}
		break;
	default:
		break;
	}
}

void Ship::ActorInput(const uint8_t* keyState)
{
	if (keyState[SDL_SCANCODE_SPACE] && mLaserCooldown <= 0.0f && mState != ShipState::Dead)
	{
		// Create a laser and set its position/rotation to mine
		Laser* laser = new Laser(GetGame());
		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());
		laser->AddForce();


		// Reset laser cooldown (half second)
		mLaserCooldown = 0.5f;
	}
}
