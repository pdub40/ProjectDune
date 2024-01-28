#include "GridGenerator.h"
#include "Components/InstancedStaticMeshComponent.h"

//================================================================================================================================
// Constructors - Start

//----------------------------------------------------------------
// Constructor - Base
//----------------------------------------------------------------
AGridGenerator::AGridGenerator()
{
	PrimaryActorTick.bCanEverTick = false;

	// setup grid mesh
	GridMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("GridMesh"));
	SetRootComponent(GridMesh);
	GridMesh->SetMobility(EComponentMobility::Movable);
	GridMesh->SetCollisionProfileName("BlockAll");
}

// Constructors - End
//================================================================================================================================
// Virtual Functions - Start

//----------------------------------------------------------------
// Init
//----------------------------------------------------------------
void AGridGenerator::Init(uint64 seed)
{
	RNG.Seed(seed);
}

// Virtual Functions - End
//================================================================================================================================
// Public Functions - Start

//----------------------------------------------------------------
// Generate
//----------------------------------------------------------------
void AGridGenerator::Generate()
{
	CreateGrid();
	PostGridInit();
}

//----------------------------------------------------------------
// Clear Grid
//----------------------------------------------------------------
void AGridGenerator::ClearGrid()
{
	GridMesh->ClearInstances();
	GridMesh->SetWorldLocation(FVector::ZeroVector);
	Tiles.Reset();
}

//----------------------------------------------------------------
// Paint Tile
//----------------------------------------------------------------
void AGridGenerator::PaintTile(int index, ETileType tileType)
{
	if (!IsValidIndex(index))
		return;

	Tiles[index] = tileType;

	switch (tileType)
	{
	case ETileType::Empty: { SetMeshInstanceColor(index, Black); break; }

	case ETileType::State_Dead: { SetMeshInstanceColor(index, Black); break; }
	case ETileType::State_Active: { SetMeshInstanceColor(index, White); break; }

	case ETileType::Biome_Water: { SetMeshInstanceColor(index, LightBlue1); break; }
	case ETileType::Biome_Ice: { SetMeshInstanceColor(index, LightBlue5); break; }
	case ETileType::Biome_HighDesert: { SetMeshInstanceColor(index, Red15); break; }

	case ETileType::Biome_Elevation_1: { SetMeshInstanceColor(index, Purple1); break; }
	case ETileType::Biome_Elevation_2: { SetMeshInstanceColor(index, Purple2); break; }
	case ETileType::Biome_Elevation_3: { SetMeshInstanceColor(index, Purple3); break; }
	case ETileType::Biome_Elevation_4: { SetMeshInstanceColor(index, Purple4); break; }
	case ETileType::Biome_Elevation_5: { SetMeshInstanceColor(index, Purple5); break; }
	case ETileType::Biome_Elevation_6: { SetMeshInstanceColor(index, Purple6); break; }

	case ETileType::Biome_LowDesert: { SetMeshInstanceColor(index, Red1); break; }
	case ETileType::Biome_Rock: { SetMeshInstanceColor(index, DarkDarkBrown); break; }
	case ETileType::Biome_SolidLand: { SetMeshInstanceColor(index, DarkBrown); break; }

	case ETileType::Unit_Turret: { SetMeshInstanceColor(index, Red); break; }
	}
}

//----------------------------------------------------------------
// Is Valid Index ?
//----------------------------------------------------------------
bool AGridGenerator::IsValidIndex(int index)
{
	return (index > -1 && index < GetGridSize());
}

//----------------------------------------------------------------
// Is Valid Move ?
//----------------------------------------------------------------
bool AGridGenerator::IsValidMove(int fromIndex, int toIndex)
{
	// validate index
	if (!IsValidIndex(fromIndex) || !IsValidIndex(toIndex))
		return false;

	// check left edge
	if ((GetGridX(fromIndex) == 0) && (GetGridX(toIndex) == GridWidth - 1))
		return false;

	// check right edge
	if ((GetGridX(fromIndex) == GridWidth - 1) && (GetGridX(toIndex) == 0))
		return false;

	return true;
}

//----------------------------------------------------------------
// Set Tiles
//----------------------------------------------------------------
void AGridGenerator::SetTiles(TArray<ETileType> tiles)
{
	Tiles.Reset();
	Tiles = tiles;
}

//----------------------------------------------------------------
// Update Mesh To Tile State
//----------------------------------------------------------------
void AGridGenerator::UpdateMeshToTileState()
{
	size_t numTiles = Tiles.Num();

	for (size_t i = 0; i < numTiles; ++i)
	{
		PaintTile(i, Tiles[i]);
	}
}

//----------------------------------------------------------------
// Get Index
//----------------------------------------------------------------
int AGridGenerator::GetIndex(int x, int y)
{
	return y * GridWidth + x;
}

//----------------------------------------------------------------
// Get Midpoint Index
//----------------------------------------------------------------
int AGridGenerator::GetMidpointIndex()
{
	return (GridHeight / 2) * GridWidth + (GridWidth / 2);
}

//----------------------------------------------------------------
// Get Grid Size
//----------------------------------------------------------------
int AGridGenerator::GetGridSize()
{
	return GridWidth * GridHeight;
}

//----------------------------------------------------------------
// Get Grid Width
//----------------------------------------------------------------
int AGridGenerator::GetGridWidth()
{
	return GridWidth;
}

//----------------------------------------------------------------
// Get Grid Height
//----------------------------------------------------------------
int AGridGenerator::GetGridHeight()
{
	return GridHeight;
}

//----------------------------------------------------------------
// Get Grid Index
//----------------------------------------------------------------
int AGridGenerator::GetGridIndex(FVector location)
{
	float x = location.X;
	float y = location.Y;

	x = FMath::Floor(x / TileSize);
	y = FMath::Floor(y / TileSize);

	return GetIndex((int)x, (int)y);
}

//----------------------------------------------------------------
// Get Grid X
//----------------------------------------------------------------
float AGridGenerator::GetGridX(size_t index)
{
	return (float)(index % GridWidth);
}

//----------------------------------------------------------------
// Get Grid Y
//----------------------------------------------------------------
float AGridGenerator::GetGridY(size_t index)
{
	return (float)(index / GridWidth);
}

//----------------------------------------------------------------
// Get World X
//----------------------------------------------------------------
float AGridGenerator::GetWorldX(size_t index)
{
	return (float)(GetGridX(index) * TileSize);
}

//----------------------------------------------------------------
// Get World Y
//----------------------------------------------------------------
float AGridGenerator::GetWorldY(size_t index)
{
	return (float)(GetGridY(index) * TileSize);
}

//----------------------------------------------------------------
// Get Number of Tiles
//----------------------------------------------------------------
size_t AGridGenerator::GetNumTiles()
{
	return Tiles.Num();
}

//----------------------------------------------------------------
// Get Tiles
//----------------------------------------------------------------
TArray<ETileType> AGridGenerator::GetTiles()
{
	return Tiles;
}

//----------------------------------------------------------------
// Get Tile Size
//----------------------------------------------------------------
float AGridGenerator::GetTileSize()
{
	return TileSize;
}

//----------------------------------------------------------------
// Get Tile
//----------------------------------------------------------------
ETileType AGridGenerator::GetTile(size_t index)
{
	return Tiles[index];
}

//----------------------------------------------------------------
// Get Tile
//----------------------------------------------------------------
ETileType AGridGenerator::GetTile(int x, int y)
{
	size_t index = GetIndex(x, y);

	return Tiles[index];
}

//----------------------------------------------------------------
// Set Tile
//----------------------------------------------------------------
void AGridGenerator::SetTile(int index, ETileType type)
{
	Tiles[index] = type;
}

// Public Functions - End
//================================================================================================================================
// Private Functions - Start

//----------------------------------------------------------------
// Create Grid
//----------------------------------------------------------------
void AGridGenerator::CreateGrid()
{
	ClearGrid();

	FVector location(0.0f, 0.0f, 0.0f);

	// create grid of instanced static mesh cubes
	for (int y = 0; y < GridHeight; ++y)
	{
		for (int x = 0; x < GridWidth; ++x)
		{
			location.Set((float)x * TileSize, (float)y * TileSize, 0.f);
			GridMesh->AddInstance(FTransform(location), true);
			Tiles.Add(ETileType::Empty);
			PaintTile(GetIndex(x, y), ETileType::Empty);
		}
	}
}

//----------------------------------------------------------------
// Set Mesh Instance Color
//----------------------------------------------------------------
void AGridGenerator::SetMeshInstanceColor(int index, FVector color)
{
	GridMesh->SetCustomDataValue(index, 0, color.X, true);
	GridMesh->SetCustomDataValue(index, 1, color.Y, true);
	GridMesh->SetCustomDataValue(index, 2, color.Z, true);
}

// Private Functions - End
//================================================================================================================================
