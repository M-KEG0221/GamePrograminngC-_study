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

void TileMapComponent::SetTileSet(SDL_Texture* tileSetTexture, const int rows, const int columns)
{
	mTileSet.mTexture = tileSetTexture;
	mTileSet.columns = columns;
	mTileSet.rows = rows;
	SDL_QueryTexture(tileSetTexture, NULL, NULL, &mTileSet.width, &mTileSet.height);
}

void TileMapComponent::Update(float deltaTime)
{
	//�����X�N���[���ɑΉ����������Ȃ�
}

void TileMapComponent::Draw(SDL_Renderer* renderer)
{
	SDL_Rect srcRect;
	//�^�C���͑S�ē����傫���Ȃ̂ŗ\�ߐݒ�
	srcRect.w = static_cast<int>(mTileSet.width / mTileSet.columns);
	srcRect.h = static_cast<int>(mTileSet.height / mTileSet.rows);

	int rowIdx = 0;
	for (auto& tileMapRow : mTileMap)
	{
		int columnIdx = 0;
		for (auto& tile : tileMapRow)
		{
			if (tile != -1)
			{
				// �`�悵�����^�C���}�b�v��̃^�C���ʒu
				srcRect.x = static_cast<int>((tile % mTileSet.columns) * srcRect.w);
				srcRect.y = static_cast<int>((tile / mTileSet.columns) * srcRect.h);

				SDL_Rect dstRect;
				// �^�C���̕`��T�C�Y
				dstRect.w = static_cast<int>(mScreenSize.x / tileMapRow.size());
				dstRect.h = static_cast<int>(mScreenSize.y / mTileMap.size());
				// �^�C���̕`��ʒu
				dstRect.x = static_cast<int>(columnIdx * dstRect.w);
				dstRect.y = static_cast<int>(rowIdx * dstRect.h);

				SDL_RenderCopy(renderer,
					mTileSet.mTexture,
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

	//csv�t�@�C��1�s��tilesRowStr�ɓ����B������Ō�̍s�܂ŌJ��Ԃ�
	std::string tilesRowStr;
	while (std::getline(tileMap, tilesRowStr))
	{
		std::vector<int> addTilesRow;
		std::string tileStr;
		std::istringstream iss(tilesRowStr);//���͐�p�̕�����X�g���[��

		while (std::getline(iss, tileStr, ','))
		{
			addTilesRow.push_back(std::stoi(tileStr));//String TO Int );
		}

		mTileMap.push_back(addTilesRow);
	}
}