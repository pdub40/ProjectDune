#include "ContentGenerator.h"

//================================================================================================================================
// Constructors - Start

//----------------------------------------------------------------
// Constructor - Base
//----------------------------------------------------------------
AContentGenerator::AContentGenerator()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Constructors - End
//================================================================================================================================
// Virtual Functions - Start

//----------------------------------------------------------------
// Run
//----------------------------------------------------------------
void AContentGenerator::Run(uint64 seed, AGridWalker* grid)
{
	StartGeneration();

	while (IsRunning())
	{
		Init(seed, grid);
		Generate();
		Finalize();

		if (IsValidGeneration())
		{
			break;
		}
		else
		{
			Reset();
		}
	}
}

//----------------------------------------------------------------
// Init
//----------------------------------------------------------------
void AContentGenerator::Init(uint64 seed, AGridWalker* grid)
{
	SeedGenerator(seed);
	GridRef = grid;
	SaveTiles();
}

//----------------------------------------------------------------
// Reset
//----------------------------------------------------------------
void AContentGenerator::Reset()
{
	RestoreTiles();
}

//----------------------------------------------------------------
// Is Valid Generation
//----------------------------------------------------------------
bool AContentGenerator::IsValidGeneration()
{
	return true;
}

//----------------------------------------------------------------
// Is Valid Neighbor Of Type
//----------------------------------------------------------------
bool AContentGenerator::IsValidNeighborOfType(int x, int y, ETileType type)
{
	return (GridRef->GetTile(x, y) == type);
}

// Virtual Functions - End
//================================================================================================================================
// Public Funtions - Start

//----------------------------------------------------------------
// Seed Generator
//----------------------------------------------------------------
void AContentGenerator::SeedGenerator(uint64 seed)
{
	if (!UseSpecifiedSeed && Seed != seed)
	{
		RNG.Seed(seed);
		Seed = seed;
	}
}

//----------------------------------------------------------------
// Get Grid Ref
//----------------------------------------------------------------
AGridWalker* AContentGenerator::GetGridRef()
{
	return GridRef;
}

// Public Funtions - End
//================================================================================================================================
// Protected Funtions - Start

//----------------------------------------------------------------
// Start Generation
//----------------------------------------------------------------
void AContentGenerator::StartGeneration()
{
	ShouldAbort = false;
}

//----------------------------------------------------------------
// Abort Generation
//----------------------------------------------------------------
void AContentGenerator::AbortGeneration()
{
	ShouldAbort = true;
}

//----------------------------------------------------------------
// Is Running
//----------------------------------------------------------------
bool AContentGenerator::IsRunning()
{
	return !ShouldAbort;
}

//----------------------------------------------------------------
// Save Tiles
//----------------------------------------------------------------
void AContentGenerator::SaveTiles()
{
	BackupTiles.Reset();
	BackupTiles = GridRef->GetTiles();
}

//----------------------------------------------------------------
// Restore Tiles
//----------------------------------------------------------------
void AContentGenerator::RestoreTiles()
{
	GridRef->SetTiles(BackupTiles);
	GridRef->UpdateMeshToTileState();
}

//----------------------------------------------------------------
// Count Neighbors
//----------------------------------------------------------------
int AContentGenerator::CountNeighbors(size_t index, ETileType typeToCount, int neighborhoodRadius)
{
	const int gridWidth = GridRef->GetGridWidth();
	const int gridHeight = GridRef->GetGridHeight();

	const int x = index % gridWidth;
	const int y = index / gridWidth;

	int numNeighbors = 0;
	const int startX = x - neighborhoodRadius;
	const int endX = x + neighborhoodRadius;
	const int startY = y - neighborhoodRadius;
	const int endY = y + neighborhoodRadius;

	for (int iy = startY; iy <= endY; ++iy)
	{
		for (int ix = startX; ix <= endX; ++ix)
		{
			if (ix == x && iy == y)
				continue;

			//if ((ix <= 0 || ix >= gridWidth - 1) || (iy <= 0 || iy >= gridHeight - 1))
			if ((ix < 0 || ix > gridWidth - 1) || (iy < 0 || iy > gridHeight - 1))
				continue;

			if (IsValidNeighborOfType(ix, iy, typeToCount))
				++numNeighbors;
		}
	}

	return numNeighbors;
}

//----------------------------------------------------------------
// Get Neighbor Ids Of Type
//----------------------------------------------------------------
std::vector<size_t> AContentGenerator::GetNeighborIdsOfType(size_t index, ETileType typeToCount, int neighborhoodRadius)
{
	std::vector<size_t> neighborIds;
	
	const int gridWidth = GridRef->GetGridWidth();
	const int gridHeight = GridRef->GetGridHeight();

	const int x = index % gridWidth;
	const int y = index / gridWidth;

	int numNeighbors = 0;
	const int startX = x - neighborhoodRadius;
	const int endX = x + neighborhoodRadius;
	const int startY = y - neighborhoodRadius;
	const int endY = y + neighborhoodRadius;

	for (int iy = startY; iy <= endY; ++iy)
	{
		for (int ix = startX; ix <= endX; ++ix)
		{
			if (ix == x && iy == y)
				continue;

			//if ((ix <= 0 || ix >= gridWidth - 1) || (iy <= 0 || iy >= gridHeight - 1))
			if ((ix < 0 || ix > gridWidth - 1) || (iy < 0 || iy > gridHeight - 1))
				continue;

			if (IsValidNeighborOfType(ix, iy, typeToCount))
				neighborIds.push_back(GridRef->GetIndex(ix, iy));
		}
	}

	return neighborIds;
}

// Protected Funtions - End
//================================================================================================================================
