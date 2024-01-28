#include "AutomataGenerator.h"

//================================================================================================================================
// Function Overrides - Start

//----------------------------------------------------------------
// Init
//----------------------------------------------------------------
void AAutomataGenerator::Init(uint64 seed, AGridWalker* grid)
{
	AContentGenerator::Init(seed, grid);

	Salt(SaltChance, ReplacementType, TypeToReplace);
}

//----------------------------------------------------------------
// Generate
//----------------------------------------------------------------
void AAutomataGenerator::Generate()
{
	const size_t numTiles = GridRef->GetNumTiles();

	for (int iLimit = 0; iLimit < IterationLimit; ++iLimit)
	{
		Snapshot.Reset();
		Snapshot = GridRef->GetTiles();

		for (size_t iTile = 0; iTile < numTiles; ++iTile)
		{
			UpdateAutomataAtIndex(iTile, ReplacementType, TypeToReplace);
		}
	}
}

//----------------------------------------------------------------
// Finalize
//----------------------------------------------------------------
void AAutomataGenerator::Finalize()
{
	const size_t numTiles = GridRef->GetNumTiles();

	for (int iLimit = 0; iLimit < IterationLimit; ++iLimit)
	{
		Snapshot.Reset();
		Snapshot = GridRef->GetTiles();

		for (size_t iTile = 0; iTile < numTiles; ++iTile)
		{
			FinalizeAutomataAtIndex(iTile, ReplacementType, TypeToReplace);
		}
	}
}

//----------------------------------------------------------------
// Is Valid Neighbor Of Type
//----------------------------------------------------------------
bool AAutomataGenerator::IsValidNeighborOfType(int x, int y, ETileType type)
{
	return (GetSnapshotTile(x, y) == type);
}

// Function Overrides - End
//================================================================================================================================
// Virtual Functions - Start

//----------------------------------------------------------------
// Salt
//----------------------------------------------------------------
void AAutomataGenerator::Salt(float chance, ETileType saltType, ETileType typeToReplace)
{
	ETileType type = ETileType::Empty;
	const size_t numTiles = GridRef->GetNumTiles();

	for (size_t i = 0; i < numTiles; ++i)
	{
		type = GridRef->GetTile(i);

		if (type == typeToReplace && RNG.Chance(chance))
		{
			GridRef->PaintTile(i, saltType);
		}
	}
}

// Virtual Functions - End
//================================================================================================================================
// Protected Functions - Start

//----------------------------------------------------------------
// Get Snapshot Tile
//----------------------------------------------------------------
ETileType AAutomataGenerator::GetSnapshotTile(int x, int y)
{
	const int index = GridRef->GetIndex(x, y);

	return GetSnapshotTile(index);
}

//----------------------------------------------------------------
// Get Snapshot Tile
//----------------------------------------------------------------
ETileType AAutomataGenerator::GetSnapshotTile(size_t index)
{
	if (!Snapshot.IsValidIndex(index))
		return ETileType::Empty;

	return Snapshot[index];
}

// Protected Functions - End
//================================================================================================================================
