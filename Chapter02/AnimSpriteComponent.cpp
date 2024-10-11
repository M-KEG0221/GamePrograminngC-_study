// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "AnimSpriteComponent.h"
#include "Math.h"
#include "Actor.h"
#include <iterator>

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	, mCurrFrame(0.0f)
	, mAnimFPS(24.0f)
	, mOldAnimState(0)
{
}

void AnimSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	if (mAnimTextures.size() > 0)
	{
		// Update the current frame based on frame rate
		// and delta time
		mCurrFrame += mAnimFPS * deltaTime;

		//�ۑ�2 
		// int nextAnimStateHeadFrameIndex = mAnimations[mOwner->GetCurrAnimState()].last + 1;
		int currAnimState = mOwner->GetCurrAnimState();
		Animation currentAnim = mAnimations[currAnimState];

		//anim���؂�ւ�����ꍇ�A�`�悷��texture�����Z�b�g
		if (mOldAnimState != currAnimState)
		{
			mCurrFrame = static_cast<float>(currentAnim.first);

			//printf("change: %d\n", static_cast<int>(mCurrFrame));
		}

		//���[�v���Ȃ��A�j���[�V�����̏ꍇ�A���b�v�Œ�������邳���O�ɍŌ��frame��`�悷��悤�ɂ���
		if (!currentAnim.isLoop && mCurrFrame > currentAnim.last)
		{
			mCurrFrame = static_cast<float>(currentAnim.last);
		}

		// Wrap current frame if needed
		//�ۑ�2
		// �w > currentAnim.last�x���ƁA�L���X�g���ɏ����_�؂�̂ĂƂȂ�A1��O�̃A�j���[�V�������`���`���`�悳���
		while (mCurrFrame >= currentAnim.last + 1)
		{
			//�ۑ�2 �`��Ώۂ̃A�j���[�V�����̖��������Z����悤�ɂ���
			mCurrFrame -= currentAnim.CalcTotalFrames();
		}

		// Set the current texture
		SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);

		mOldAnimState = currAnimState;
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures)
{
	mAnimTextures = textures;
	if (mAnimTextures.size() > 0)
	{
		// Set the active texture to first frame
		mCurrFrame = 0.0f;
		SetTexture(mAnimTextures[0]);
	}
}

//�ۑ�2
//�f�t�H���g������h�t�@�C���ɂ̂ݏ���
//�������ɂ������ƁF�R���p�C���G���[�i https://qiita.com/yut-nagase/items/29d0fc0984e6dbace85e �j
//�������ɂ̂ݏ����ƁF�\���G���[
void AnimSpriteComponent::RegisterAnimation(const int animStateKey, const std::vector<SDL_Texture*>& textures, const bool isLoop)
{
	mAnimations[animStateKey] = Animation(
		mAnimTextures.size(),
		mAnimTextures.size() + textures.size() - 1,
		isLoop
	);

	std::vector<SDL_Texture*> result = mAnimTextures;
	result.reserve(mAnimTextures.size() + textures.size()); // ���O�ɒǉ����̃��������m�ۂ��邱�ƂŌ�����
	std::copy(textures.begin(), textures.end(), std::back_inserter(result));
	SetAnimTextures(result);
}