#include "TileMapComponent.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

TileMapComponent::TileMapComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	, mTileSet()
{
}

void TileMapComponent::SetTileSet(SDL_Texture* texture, const int rows, const int columns
)
{
	SetTexture(texture);
	TileSet tsd;
	tsd.mTexture = texture;
	tsd.c = columns;
	tsd.r = rows;
	SDL_QueryTexture(texture, NULL, NULL, &tsd.w, &tsd.h);
	mTileSet = tsd;
}

void TileMapComponent::Update(float deltaTime)
{
	//�����X�N���[���ɑΉ����������Ȃ�
}

void TileMapComponent::Draw(SDL_Renderer* renderer)
{
	SDL_Rect srcRect;
	//�^�C���͑S�ē����傫���Ȃ̂ŗ\�ߐݒ�
	srcRect.w = static_cast<int>(mTileSet.w / mTileSet.r);
	srcRect.h = static_cast<int>(mTileSet.h / mTileSet.c);

	int rowIdx = 0;
	for (auto& tileMapRow : mTileMap)
	{
		int columnIdx = 0;
		for (auto& tile : tileMapRow)
		{
			if (tile != -1)
			{
				// �`�悵�����^�C���}�b�v��̃^�C���ʒu
				srcRect.x = static_cast<int>((tile % mTileSet.r) * srcRect.w);
				srcRect.y = static_cast<int>((tile / mTileSet.r) * srcRect.h);

				SDL_Rect dstRect;
				// �^�C���̕`��T�C�Y
				dstRect.w = static_cast<int>(mScreenSize.x / tileMapRow.size());
				dstRect.h = static_cast<int>(mScreenSize.y / mTileMap.size());
				// �^�C���̕`��ʒu
				dstRect.x = static_cast<int>(columnIdx * dstRect.w);
				dstRect.y = static_cast<int>(rowIdx * dstRect.h);

				SDL_RenderCopy(renderer,
					mTexture,
					&srcRect,
					&dstRect
				);
			}
			columnIdx++;
		}
		rowIdx++;
	}
}

void TileMapComponent::LoadCSV(const std::string& fileName)
{
	//csv�t�@�C���ǂݍ���
	std::ifstream tileMap(fileName);//�utileMap�v�͕ϐ���

	//�t�@�C����������Ȃ������ꍇ�̃G���[����
	if (!tileMap)
	{
		SDL_Log("TileMapComponent�Fcsv�t�@�C����������܂���ł��� %s", fileName.c_str());
		return;//�v�����B�Q�[���𗎂Ƃ��������x�X�g�H
	}

	//csv�t�@�C��1�s��rowStr�ɓ����B������Ō�̍s�܂ŌJ��Ԃ�
	std::string rowStr;
	while (std::getline(tileMap, rowStr))
	{
		std::vector<int> rowTiles;
		std::string tileStr;
		std::istringstream iss(rowStr);//���͐�p�̕�����X�g���[��

		while (std::getline(iss, tileStr, ','))
		{
			int tile = std::stoi(tileStr);//String TO Int 
			rowTiles.push_back(tile);
		}

		mTileMap.push_back(rowTiles);
	}
}