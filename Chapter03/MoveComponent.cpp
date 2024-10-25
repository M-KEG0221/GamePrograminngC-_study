// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "MoveComponent.h"
#include "Actor.h"
#include <iostream>

MoveComponent::MoveComponent(class Actor* owner, int updateOrder)
	:Component(owner, updateOrder)
	, mAngularSpeed(0.0f)
	, mForwardSpeed(0.0f)
	, mForce(Vector2())
	, mFriction(0.0f)
{

}

void MoveComponent::Update(float deltaTime)
{
	if (!Math::NearZero(mAngularSpeed))
	{
		float rot = mOwner->GetRotation();
		rot += mAngularSpeed * deltaTime;
		mOwner->SetRotation(rot);
	}

	float registSpeed = -mForwardSpeed;

	//if (!Math::NearZero(mForwardSpeed)) // �����v�Z�͏�ɍs���邽�߁A�������O��

	// F=m.a����a=F/m�����A�����x�����߂�i��C��R���l���j
	Vector2 acceleration = (mForce - mVerocity * mFriction) / mMass;

	// v=v0+at���猻�݂̑��x��a*deltatime�����Z
	mVerocity += acceleration * deltaTime;

	//x=x0+v0t���猻�݂̈ʒu��v*deltatime�����Z
	Vector2 pos = mOwner->GetPosition();
	pos += mVerocity * deltaTime;//deltaTime��2��|�����Ă邩��ɒ[�ɒx���Ȃ�

	// (Screen wrapping code only for asteroids)
	if (pos.x < 0.0f) { pos.x = 1022.0f; }
	else if (pos.x > 1024.0f) { pos.x = 2.0f; }

	if (pos.y < 0.0f) { pos.y = 766.0f; }
	else if (pos.y > 768.0f) { pos.y = 2.0f; }

	mOwner->SetPosition(pos);

	mForce = Vector2();
}

void MoveComponent::AddForce(Vector2 force)
{
	mForce += force;
}

void MoveComponent::AddForce(float force)
{
	AddForce(mOwner->GetForward() * force);
}

void MoveComponent::AddForce()
{
	AddForce(mForwardSpeed * 60);
}
