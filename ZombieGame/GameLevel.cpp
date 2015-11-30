/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#include <random>

#include "GameLevel.h"

#define TILESET_TEXTURE_FILENAME		"./textures/tiles.png"

#define TILESET_TEXTURE_SIZE_WIDTH		640.0f
#define TILESET_TEXTURE_SIZE_HEIGHT		640.0f

#define TILE_TEXTURE_SIZE_WIDTH			64.0f
#define TILE_TEXTURE_SIZE_HEIGHT		64.0f

#define TILE_WALL_TEXTURE_INDEX			0
#define TILE_FLOOR0_TEXTURE_INDEX		2
#define TILE_FLOOR1_TEXTURE_INDEX		3
#define TILE_FLOOR2_TEXTURE_INDEX		4
#define TILE_FLOOR3_TEXTURE_INDEX		5
#define TILE_FLOOR4_TEXTURE_INDEX		6
#define TILE_FLOOR5_TEXTURE_INDEX		7
#define TILE_FLOOR6_TEXTURE_INDEX		8

#define FLOOR_TILE_VARIATION_COUNT		7

#define NO_WALL_FOUND					-1

#define TILE_SIZE_WIDTH					64.0f
#define TILE_SIZE_HEIGHT				64.0f

GameLevel::GameLevel(const char* levelLayout, int width, int height)
{
	m_LevelLayout = levelLayout;
	m_Width = width;
	m_Height = height;
}

GameLevel::~GameLevel()
{
	if (m_WallNeedsCollider != nullptr)
	{
		for (int i = 0; i < m_Height; ++i)
		{
			delete[] m_WallNeedsCollider[i];
			m_WallNeedsCollider[i] = nullptr;
		}
	}
	delete[] m_WallNeedsCollider;
	m_WallNeedsCollider = nullptr;

	delete m_pTileVertices;
	m_pTileVertices = nullptr;

	delete m_pTileSet;
	m_pTileSet = nullptr;
}

bool GameLevel::Init()
{
	if (!IsValidLevelLayout())
	{
		return false;
	}

	GenerateWallColliders();

	GenerateBufferObjects();

	m_pTileSet = new Texture(TILESET_TEXTURE_FILENAME);
	bool isTextureInitialized = m_pTileSet->Init();

	if (!isTextureInitialized)
	{
		return false;
	}

	return true;
}

void GameLevel::Draw()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(sizeof(float) * 2));

	m_pTileSet->Bind(GL_TEXTURE0);

	glDrawArrays(GL_TRIANGLES, 0, m_TotalNumVertices);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

bool GameLevel::IsWithinLevel(glm::dvec2 position, glm::dvec2 size)
{
	for (int xPos = (int)position.x; xPos <= position.x + size.x; xPos += (int)TILE_SIZE_WIDTH)
	{
		for (int yPos = (int)position.y; yPos <= position.y + size.y; yPos += (int)TILE_SIZE_HEIGHT)
		{
			if (!IsWithinLevel(glm::dvec2(xPos, yPos)))
			{
				return false;
			}
		}
	}

	return true;
}

bool GameLevel::IsWithinLevel(glm::dvec2 position)
{
	if (position.x < 0.0) return false;
	if (position.y < 0.0) return false;

	if (position.x > GetWidthInPixels()) return false;
	if (position.y > GetHeightInPixels()) return false;

	int col = (int)(position.x / (double)TILE_SIZE_WIDTH);
	int row = (int)(position.y / (double)TILE_SIZE_HEIGHT);

	return IsWithinLevel(row, col);
}

bool GameLevel::IsWithinLevel(int row, int col)
{
	return m_LevelLayout[row * m_Width + col] != TILE_NOTHING;
}

void GameLevel::GenerateWallColliders()
{
	InitializeColliderFlags();

	GenerateBlockColliders();

	FinalizeWallColliders();
}

void GameLevel::StoreWallColliderData(int tileX, int tileY, int widthInTiles, int heightInTiles)
{
	double xPos = tileX * (double)TILE_SIZE_WIDTH;
	double yPos = tileY * (double)TILE_SIZE_HEIGHT;
	double width = widthInTiles * (double)TILE_SIZE_WIDTH;
	double height = heightInTiles * (double)TILE_SIZE_HEIGHT;

	m_WallColliderData.push_back(WallColliderData(xPos, yPos, width, height));

	UpdateColliderFlags(tileX, tileY, widthInTiles, heightInTiles);
}

void GameLevel::FinalizeWallColliders()
{
	m_WallColliders.resize(m_WallColliderData.size());

	for (unsigned int i = 0; i < m_WallColliderData.size(); ++i)
	{
		WallColliderData data = m_WallColliderData[i];
		m_WallColliders[i].SetPosition(data.m_PosX, data.m_PosY);
		m_WallColliders[i].SetDimensions(data.m_Width, data.m_Height);
	}

	m_WallColliderData.clear();
}

void GameLevel::InitializeColliderFlags()
{
	m_WallNeedsCollider = new bool*[m_Height];
	for (int i = 0; i < m_Height; ++i)
	{
		m_WallNeedsCollider[i] = new bool[m_Width];
	}
	for (int row = 0; row < m_Height; ++row)
	{
		for (int col = 0; col < m_Width; ++col)
		{
			if (m_LevelLayout[row * m_Width + col] == TILE_WALL)
			{
				m_WallNeedsCollider[row][col] = true;
			}
			else
			{
				m_WallNeedsCollider[row][col] = false;
			}
		}
	}
}

void GameLevel::UpdateColliderFlags(int tileX, int tileY, int widthInTiles, int heightInTiles)
{
	for (int row = tileY; row < tileY + heightInTiles; ++row)
	{
		for (int col = tileX; col < tileX + widthInTiles; ++col)
		{
			m_WallNeedsCollider[row][col] = false;
		}
	}
}

void GameLevel::GenerateBlockColliders()
{
	for (int row = 0; row < m_Height; ++row)
	{
		for (int col = 0; col < m_Width; ++col)
		{
			if (!m_WallNeedsCollider[row][col]) continue;

			if (IsTopLeftCornerOfWall(col, row))
			{
				GenerateLargestPossibleCollider(col, row);
			}
		}
	}

}

bool GameLevel::IsTopLeftCornerOfWall(int tileX, int tileY)
{
	// if this tile is the top left corner of the entire level
	if (tileX == 0 && tileY == 0) return true;

	bool doesAboveTileNeedCollider;
	bool doesLeftTileNeedCollider;

	// if this tile is not along the top edge
	if (tileY != 0)
	{
		doesAboveTileNeedCollider = m_WallNeedsCollider[tileY - 1][tileX];
	}

	// if this tile is not along the left edge
	if (tileX != 0)
	{
		doesLeftTileNeedCollider = m_WallNeedsCollider[tileY][tileX - 1];
	}

	// if this tile is along the left edge of the level
	if (tileX == 0) return !doesAboveTileNeedCollider;
	
	// if the tile is along the top edge of the level
	if (tileY == 0) return !doesLeftTileNeedCollider;

	return !doesAboveTileNeedCollider && !doesLeftTileNeedCollider;
}

void GameLevel::GenerateLargestPossibleCollider(int tileX, int tileY)
{
	if (m_WallNeedsCollider[tileY][tileX])
	{
		// determine the bottom right corner of the largest possible AABB
		int bottomRightX = m_Width;
		int bottomRightY = m_Height;

		for (int row = tileY; row < m_Height; ++row)
		{
			// if reached the bottom-most part of the block
			if (!m_WallNeedsCollider[row][tileX]) break;

			// find the x-coordinate of the right-most possible tile in this row
			int rightMostTileOfRow = tileX;
			for (int col = tileX; col < m_Width; ++col)
			{
				bool colliderCanEncompassTile = m_WallNeedsCollider[row][col];

				if (colliderCanEncompassTile)
				{
					rightMostTileOfRow = col;
				}
				else
				{
					break;
				}
			}

			if (rightMostTileOfRow < bottomRightX)
			{
				bottomRightX = rightMostTileOfRow;
			}

			bottomRightY = row;
		}

		StoreWallColliderData(tileX, tileY, (bottomRightX - tileX) + 1, (bottomRightY - tileY) + 1);
	}
}

void GameLevel::GenerateRowColliders(int row)
{
	int wallStart = NO_WALL_FOUND;
	int wallEnd = NO_WALL_FOUND;

	for (int col = 0; col < m_Width; ++col)
	{
		bool doesWallNeedCollider = m_WallNeedsCollider[row][col];

		if (!doesWallNeedCollider) continue;

		if (wallStart == NO_WALL_FOUND)
		{
			if (doesWallNeedCollider)
			{
				wallStart = col;
				wallEnd = col;
			}
		}
		else
		{
			if (doesWallNeedCollider)
			{
				wallEnd = col;
			}
			// create a collider if the end of the wall has been found
			else if (!doesWallNeedCollider || (col == m_Width - 1))
			{
				int wallWidth = (wallEnd - wallStart) + 1;
				StoreWallColliderData(wallStart, row, wallWidth, 1);

				wallStart = NO_WALL_FOUND;
				wallEnd = NO_WALL_FOUND;
			}
		}
	}

	// create a collider for any leftovers
	if (wallStart != NO_WALL_FOUND)
	{
		int wallWidth = (wallEnd - wallStart) + 1;
		StoreWallColliderData(wallStart, row, wallWidth, 1);
	}
}

void GameLevel::GenerateColumnColliders(int col)
{
	int wallStart = NO_WALL_FOUND;
	int wallEnd = NO_WALL_FOUND;

	for (int row = 0; row < m_Height; ++row)
	{
		bool doesWallNeedCollider = m_WallNeedsCollider[row][col];

		if (!doesWallNeedCollider) continue;

		if (wallStart == NO_WALL_FOUND)
		{
			if (doesWallNeedCollider)
			{
				wallStart = row;
				wallEnd = row;
			}
		}
		else
		{
			if (doesWallNeedCollider)
			{
				wallEnd = row;
			}
			// create a collider if the end of the wall has been found
			else if (!doesWallNeedCollider)
			{
				int wallHeight = (wallEnd - wallStart) + 1;
				StoreWallColliderData(col, wallStart, 1, wallHeight);

				wallStart = NO_WALL_FOUND;
				wallEnd = NO_WALL_FOUND;
			}
		}
	}

	// create a collider for any leftovers
	if (wallStart != NO_WALL_FOUND)
	{
		int wallHeight = (wallEnd - wallStart) + 1;
		StoreWallColliderData(col, wallStart, 1, wallHeight);
	}
}

void GameLevel::GenerateBufferObjects()
{
	GenerateTileVertices();
	PackBufferObjects();
}

void GameLevel::GenerateTileVertices()
{
	m_TotalNumVertices = m_Width * m_Height * m_NumVerticesPerTile;
	m_pTileVertices = new std::vector<Vertex>();

	for (int row = 0; row < m_Height; ++row)
	{
		for (int col = 0; col < m_Width; ++col)
		{
			char tileType = m_LevelLayout[row * m_Width + col];
			if (tileType == TILE_NOTHING)
			{
				m_TotalNumVertices -= m_NumVerticesPerTile;
				continue;
			}

			glm::vec2 topLeft(col * TILE_SIZE_WIDTH, row * TILE_SIZE_HEIGHT);
			glm::vec2 topRight = topLeft + glm::vec2(TILE_SIZE_WIDTH, 0.0f);
			glm::vec2 bottomLeft = topLeft + glm::vec2(0.0f, TILE_SIZE_HEIGHT);
			glm::vec2 bottomRight = topLeft + glm::vec2(TILE_SIZE_WIDTH, TILE_SIZE_HEIGHT);

			glm::vec2 texTopLeft;
			switch (tileType)
			{
				case TILE_WALL:
					texTopLeft = CalculateTopLeftTexCoord(TILE_WALL_TEXTURE_INDEX);
					break;
				case TILE_FLOOR:
					std::random_device rd;
					std::mt19937 engine(rd());
					std::uniform_int_distribution<int> randomFloorTile(1, 100);

					int generatedFloorTile = randomFloorTile(engine);
					if (generatedFloorTile < FLOOR_TILE_VARIATION_COUNT)
					{
						texTopLeft = CalculateTopLeftTexCoord(TILE_FLOOR0_TEXTURE_INDEX + generatedFloorTile);
					}
					else
					{
						texTopLeft = CalculateTopLeftTexCoord(TILE_FLOOR0_TEXTURE_INDEX);
					}
						
					break;
			}

			glm::vec2 texTopRight = texTopLeft + glm::vec2(TILE_TEXTURE_SIZE_WIDTH, 0.0f);
			glm::vec2 texBottomLeft = texTopLeft + glm::vec2(0.0f, TILE_TEXTURE_SIZE_HEIGHT);
			glm::vec2 texBottomRight = texTopLeft + glm::vec2(TILE_TEXTURE_SIZE_WIDTH, TILE_TEXTURE_SIZE_HEIGHT);

			glm::vec2 normalizeTexCoord(1 / TILESET_TEXTURE_SIZE_WIDTH, 1 / TILESET_TEXTURE_SIZE_HEIGHT);
			
			texTopLeft *= normalizeTexCoord;
			texTopRight *= normalizeTexCoord;
			texBottomLeft *= normalizeTexCoord;
			texBottomRight *= normalizeTexCoord;

			m_pTileVertices->push_back(Vertex(topLeft, texTopLeft));
			m_pTileVertices->push_back(Vertex(topRight, texTopRight));
			m_pTileVertices->push_back(Vertex(bottomLeft, texBottomLeft));

			m_pTileVertices->push_back(Vertex(bottomLeft, texBottomLeft));
			m_pTileVertices->push_back(Vertex(topRight, texTopRight));
			m_pTileVertices->push_back(Vertex(bottomRight, texBottomRight));
		}
	}
}

void GameLevel::PackBufferObjects()
{
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_TotalNumVertices * sizeof(Vertex), m_pTileVertices->data(), GL_STATIC_DRAW);
}

glm::vec2 GameLevel::CalculateTopLeftTexCoord(int textureIndex)
{
	int texturesPerRow = (int)(TILESET_TEXTURE_SIZE_WIDTH / TILE_SIZE_WIDTH);

	int textureColumn = textureIndex % texturesPerRow;
	int textureRow = textureIndex / texturesPerRow;

	return glm::vec2((float)(textureColumn) * TILE_SIZE_WIDTH, (float)(textureRow) * TILE_SIZE_HEIGHT);
}

bool GameLevel::IsValidLevelLayout()
{
	if (strlen(m_LevelLayout) != m_Width * m_Height)
	{
		fprintf(stderr, "Invalid level layout. Dimensions do not match. WIDTH: %d, HEIGHT: %d, TOTAL: %d\n", m_Width, m_Height, strlen(m_LevelLayout));
		return false;
	}

	for (unsigned int i = 0; i < strlen(m_LevelLayout); ++i)
	{
		if (!IsValidCharacter(m_LevelLayout[i]))
		{
			fprintf(stderr, "Invalid level layout. Invalid character: %c\n", m_LevelLayout[i]);
			return false;
		}
	}

	return true;
}

bool GameLevel::IsValidCharacter(char character)
{
	switch (character)
	{
		case TILE_NOTHING:
			return true;
		case TILE_WALL:
			return true;
		case TILE_FLOOR:
			return true;
	}
	return false;
}

int GameLevel::GetWidth()
{
	return m_Width;
}

int GameLevel::GetHeight()
{
	return m_Height;
}

float GameLevel::GetWidthInPixels()
{
	return (float)m_Width * TILE_SIZE_WIDTH;
}

float GameLevel::GetHeightInPixels()
{
	return (float)m_Height * TILE_SIZE_HEIGHT;
}

std::vector<CollisionAABB>* GameLevel::GetWallCollisionObjects()
{
	return &m_WallColliders;
}