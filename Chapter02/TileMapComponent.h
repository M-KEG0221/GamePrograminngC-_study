#pragma once
#include "SpriteComponent.h"
#include "SDL/SDL.h"
#include <vector>
#include "Math.h"
#include <string>

class TileMapComponent :
	public SpriteComponent
{
public:
	TileMapComponent(class Actor* owner, int drawOrder = 20);
	void Update(float deltaTime) override;
	void Draw(SDL_Renderer* renderer) override;
	void SetScreenSize(const Vector2& size) { mScreenSize = size; }
	void SetTileSet(
		SDL_Texture* texture,
		const int rows,
		const int columns
	);
	void LoadCSV(const std::string& fileName);
private:
	//�^�C���Z�b�g�̏��
	struct TileSet
	{
		SDL_Texture* mTexture;
		int width, height;
		int rows = 1, columns = 1;//0�ł̏��Z�����
	};
	TileSet mTileSet;

	Vector2 mScreenSize;
	std::vector<std::vector<int>> mTileMap;
};

