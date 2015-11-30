/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#ifndef GAMELEVEL_H_
#define GAMELEVEL_H_

#include <GL\glew.h>
#include <GL\glut.h>
#include <vector>

#include "Vertex.h"
#include "CollisionAABB.h"
#include "Texture.h"

#define TILE_NOTHING					'-'
#define TILE_FLOOR						' '
#define TILE_WALL						'W'

/** LevelLayouts must be represented as a 2D array of characters. 
  * Assumes a levelLayout with a closed border of walls for best results.
  * Still works if levelLayout does not have a closed border.
  */

class GameLevel
{
	public:
		GameLevel(const char* levelLayout, int width, int height);
		~GameLevel();

		bool Init();

		void Draw();

		bool IsWithinLevel(glm::dvec2 position, glm::dvec2 size);
		bool IsWithinLevel(glm::dvec2 position);
		bool IsWithinLevel(int row, int col);

		int GetWidth();
		int GetHeight();
		float GetWidthInPixels();
		float GetHeightInPixels();
		std::vector<CollisionAABB>* GetWallCollisionObjects();

	private:
		int m_Width;
		int m_Height;

		const char* m_LevelLayout;

		struct WallColliderData
		{
			double m_PosX;
			double m_PosY;
			double m_Width;
			double m_Height;

			WallColliderData(double xPos, double yPos, double width, double height)
			{
				m_PosX = xPos;
				m_PosY = yPos;
				m_Width = width;
				m_Height = height;
			}
		};
		std::vector<WallColliderData> m_WallColliderData;
		std::vector<CollisionAABB> m_WallColliders;
		bool** m_WallNeedsCollider;

		const int m_NumVerticesPerTile = 6;
		int m_TotalNumVertices;
		std::vector<Vertex>* m_pTileVertices;

		GLuint m_VBO;

		Texture* m_pTileSet;

		bool IsValidLevelLayout();
		bool IsValidCharacter(char character);

		void GenerateWallColliders();
		void InitializeColliderFlags();
		void UpdateColliderFlags(int tileX, int tileY, int widthInTiles, int heightInTiles);
		void GenerateRowColliders(int row);
		void GenerateColumnColliders(int col);
		void GenerateBlockColliders();
		void GenerateLargestPossibleCollider(int tileX, int tileY);
		bool IsTopLeftCornerOfWall(int tileX, int tileY);

		void StoreWallColliderData(int tileX, int tileY, int widthInTiles, int heightInTiles);
		void FinalizeWallColliders();

		void GenerateBufferObjects();
		void GenerateTileVertices();
		void PackBufferObjects();

		glm::vec2 CalculateTopLeftTexCoord(int textureIndex);
};

#endif /* GAMELEVEL_H_ */