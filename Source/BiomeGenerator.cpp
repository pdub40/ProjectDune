#include "BiomeGenerator.h"

//================================================================================================================================
// Function Overrides - Start

//----------------------------------------------------------------
// Generate
//----------------------------------------------------------------
void ABiomeGenerator::Init(uint64 seed, AGridWalker* grid)
{
	AContentGenerator::Init(seed, grid);

	NoiseGrid.Reset();

	const size_t numGenerators = NoiseGenerators.Num();
	for (size_t i = 0; i < numGenerators; ++i)
	{
		NoiseGenerators[i]->Init(RNG.Random(), grid);
	}
}

//----------------------------------------------------------------
// Generate
//----------------------------------------------------------------
void ABiomeGenerator::Generate()
{
	int noise = 0;

	const size_t numTiles = GridRef->GetNumTiles();
	const size_t numGenerators = NoiseGenerators.Num();

	for (size_t iTile = 0; iTile < numTiles; ++iTile)
	{
		noise = 0;

		for (size_t iGen = 0; iGen < numGenerators; ++iGen)
		{
			noise = NoiseGenerators[iGen]->GenerateNoise(iTile, noise);
		}

		SetBiome(iTile, noise);
		NoiseGrid.Add(noise);
	}
}

//----------------------------------------------------------------
// Is Valid Generation
//----------------------------------------------------------------
bool ABiomeGenerator::IsValidGeneration()
{
	int numIceTiles = 0;
	int numHighDesertTiles = 0;
	size_t numTiles = GridRef->GetNumTiles();

	for (size_t i = 0; i < numTiles; ++i)
	{
		if (GridRef->GetTile(i) == ETileType::Biome_Ice || GridRef->GetTile(i) == ETileType::Biome_Water)
			++numIceTiles;

		else if (GridRef->GetTile(i) == ETileType::Biome_HighDesert)
			++numHighDesertTiles;
	}

	if (numIceTiles < MinIceTiles || numIceTiles > MaxIceTiles)
		return false;

	if (numHighDesertTiles < MinHighDesertTiles || numHighDesertTiles > MaxHighDesertTiles)
		return false;

	return true;
}

// Function Overrides - End
//================================================================================================================================
// Public Functions - Start

//----------------------------------------------------------------
// Get Noise At Index
//----------------------------------------------------------------
float ABiomeGenerator::GetNoiseAtIndex(size_t index)
{
	return NoiseGrid[index];
}

// Public Functions - End
//================================================================================================================================
// Private Functions - Start

//----------------------------------------------------------------
// Get Biome
//----------------------------------------------------------------
ETileType ABiomeGenerator::GetBiome(int index, float noise)
{
	if (noise > Thresholds.LowDesert)
		return ETileType::Biome_LowDesert;

	if (noise > Thresholds.Elevation6)
		return ETileType::Biome_Elevation_6;

	if (noise > Thresholds.Elevation5)
		return ETileType::Biome_Elevation_5;

	if (noise > Thresholds.Elevation4)
		return ETileType::Biome_Elevation_4;

	if (noise > Thresholds.Elevation3)
		return ETileType::Biome_Elevation_3;

	if (noise > Thresholds.Elevation2)
		return ETileType::Biome_Elevation_2;

	if (noise > Thresholds.Elevation1)
		return ETileType::Biome_Elevation_1;

	if (noise > Thresholds.HighDesert)
		return ETileType::Biome_HighDesert;

	if (noise > Thresholds.Ice)
		return ETileType::Biome_Ice;

	if (noise > Thresholds.Water)
		return ETileType::Biome_Water;

	return ETileType::Empty;
}

//----------------------------------------------------------------
// Set Biome
//----------------------------------------------------------------
void ABiomeGenerator::SetBiome(int index, float noise)
{
	const ETileType biome = GetBiome(index, noise);
	GridRef->PaintTile(index, biome);
}

// Private Functions - End
//================================================================================================================================
