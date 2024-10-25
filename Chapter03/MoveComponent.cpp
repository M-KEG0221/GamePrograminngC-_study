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

		std::cout << rot << std::endl;
	}

	float registSpeed = -mForwardSpeed;

	//if (!Math::NearZero(mForwardSpeed)) // mForward�ˑ����ƁANo Input��Ԃő��x���ύX����Ȃ����ߕύX����
		//if (!Math::NearZero(mVerocity.Length()))
		//if (
		//	!(Math::NearZero(mVerocity.x)
		//		&& Math::NearZero(mVerocity.y)
		//		&& Math::NearZero(mForce.Length())
		//		&& Math::NearZero(mForwardSpeed)
		//		//&& !Math::NearZero(mForce.LengthSq())
		//		))
	//{
		//}
		//F=m.a����a=F/m�����A�����x�����߂�
	Vector2 acceleration = (mForce - mVerocity * mFriction) / mMass;
	/*std::cout << mForce.x << std::endl;
	std::cout << (mOwner->GetForward() * mForwardSpeed).x << std::endl;
	std::cout << "-----" << std::endl;*/

	//v=v0+at���猻�݂̑��x��a*deltatime�����Z
	mVerocity += acceleration * deltaTime;

	/*std::cout << "limit speed" << std::endl;
	std::cout << mVerocity.LengthSq() << std::endl;*/
	//mVerocity.Normalize();
	//mVerocity *= mForwardSpeed;

	/*if (Vector2::CalcAngle(mOwner->GetForward(), mVerocity) == Math::Pi)
	{
		mVerocity = Vector2();
	}*/

	//x=x0+v0t���猻�݂̈ʒu��v*deltatime�����Z
	Vector2 pos = mOwner->GetPosition();
	//mVerocity = mOwner->GetForward() * mForwardSpeed;

	//pos += mOwner->GetForward() * mForwardSpeed * deltaTime;
	pos += mVerocity * deltaTime;//deltaTime��2��|�����Ă邩��ɒ[�ɒx���Ȃ�

	// (Screen wrapping code only for asteroids)
	if (pos.x < 0.0f) { pos.x = 1022.0f; }
	else if (pos.x > 1024.0f) { pos.x = 2.0f; }

	if (pos.y < 0.0f) { pos.y = 766.0f; }
	else if (pos.y > 768.0f) { pos.y = 2.0f; }

	mOwner->SetPosition(pos);

	if (mForce.Length() > 0)
	{
		//std::cout << mVerocity.x << std::endl;
		//std::cout << acceleration.x << std::endl;
		//std::cout << "----" << std::endl;
	}
	mForce = Vector2();
	//}
}

void MoveComponent::AddForce(Vector2 force)
{
	float deltaTime = 1.0f;

	std::cout << "�ǉ�: " << force.x << "," << force.y << std::endl;

	mForce += force;
	std::cout << "����: " << mForce.x << "," << mForce.y << std::endl;

	Vector2 a(mForce / mMass);
	std::cout << "�����x: " << a.x << "," << a.y << std::endl;
	Vector2 vBeforeN(mOwner->GetForward() * mForwardSpeed * deltaTime);
	Vector2 vAfterN(a * deltaTime);
	std::cout << "�j���[�g���O���x: " << vBeforeN.x << "," << vBeforeN.y << std::endl;
	std::cout << "�j���[�g���㑬�x: " << vAfterN.x << "," << vAfterN.y << std::endl;
	std::cout << "��C��R: " << vAfterN.x * mFriction << "," << vAfterN.y * mFriction << std::endl;
}

void MoveComponent::AddForce(float force)
{
	AddForce(mOwner->GetForward() * force);
}

void MoveComponent::AddForce()
{
	AddForce(mForwardSpeed * 60);
}
