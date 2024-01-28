#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RandomNumberGenerator.h"
#include "GridGenerator.generated.h"

//================================================================================================================================
// Constants - Start

constexpr size_t kNumDirections = 4;
constexpr int kInvalidNum = -1;

// Constants - End
//================================================================================================================================
// Structs - Start

//----------------------------------------------------------------
// Tile Types
//----------------------------------------------------------------
UENUM(BlueprintType)
enum class ETileType
{
	Invalid					UMETA(DisplayName = "Invalid"),
	Empty					UMETA(DisplayName = "Empty"),

	State_Dead				UMETA(DisplayName = "Dead"),
	State_Active			UMETA(DisplayName = "Active"),

	Biome_Water				UMETA(DisplayName = "Water"),
	Biome_Ice				UMETA(DisplayName = "Ice"),
	Biome_HighDesert		UMETA(DisplayName = "High Desert"),
	Biome_Elevation_1		UMETA(DisplayName = "Elevation 1"),
	Biome_Elevation_2		UMETA(DisplayName = "Elevation 2"),
	Biome_Elevation_3		UMETA(DisplayName = "Elevation 3"),
	Biome_Elevation_4		UMETA(DisplayName = "Elevation 4"),
	Biome_Elevation_5		UMETA(DisplayName = "Elevation 5"),
	Biome_Elevation_6		UMETA(DisplayName = "Elevation 6"),
	Biome_LowDesert		    UMETA(DisplayName = "Low Desert"),
	Biome_Rock				UMETA(DisplayName = "Rock"),

	Biome_SolidLand			UMETA(DisplayName = "Solid Land"),

	Unit_Turret 			UMETA(DisplayName = "Turret")
};

// Structs - End
//================================================================================================================================
// Grid Generator - Start

UCLASS()
class PROJECTDUNE_API AGridGenerator : public AActor
{
	GENERATED_BODY()
	
public:	// Constructors

	AGridGenerator();

public: // Virtual Functions

	virtual void Init(uint64 seed);

protected: // Virtual Functions

	virtual void PostGridInit() {}

public: // Functions

	void Generate();
	void ClearGrid();

	// Tile Management
	void SetTile(int index, ETileType type);
	void PaintTile(int index, ETileType tileType = ETileType::Empty);
	bool IsValidIndex(int index);
	bool IsValidMove(int fromIndex, int toIndex);
	void SetTiles(TArray<ETileType> tiles);
	void UpdateMeshToTileState();

	// Grid Helpers
	int GetIndex(int x, int y);
	int GetMidpointIndex();
	int GetGridSize();
	int GetGridWidth();
	int GetGridHeight();
	int GetGridIndex(FVector location);
	float GetGridX(size_t index);
	float GetGridY(size_t index);
	float GetWorldX(size_t index);
	float GetWorldY(size_t index);
	float GetTileSize();
	size_t GetNumTiles();
	TArray<ETileType> GetTiles();
	ETileType GetTile(size_t index);
	ETileType GetTile(int x, int y);

private: // Functions

	void CreateGrid();
	void SetMeshInstanceColor(int index, FVector color);

protected: // Variables

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Grid|Layout", meta = (AllowPrivateAccess = true))
	float TileSize = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Grid|Layout", meta = (AllowPrivateAccess = true))
	int GridWidth = 125;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Grid|Layout", meta = (AllowPrivateAccess = true))
	int GridHeight = 125;

	TArray<ETileType> Tiles;

	RandomNumberGenerator RNG;

private: // Variables

	UPROPERTY(EditAnywhere, category = "Grid|Layout", meta = (AllowPrivateAccess = true))
	UInstancedStaticMeshComponent* GridMesh;

protected: // Colors ---------------------------------------------------------------------------------------

	inline static FVector Brown = FVector(0.32f, 0.1f, 0.1f);
	inline static FVector LightBrown = FVector(0.88f, 0.75f, 0.64f);
	inline static FVector DarkBrown = FVector(0.15f, 0.04f, 0.04f);
	inline static FVector DarkDarkBrown = FVector(0.1f, 0.02f, 0.02f);

	inline static FVector LightGreen = FVector(0.f, 1.f, 0.f);
	inline static FVector Green = FVector(0.f, 0.5f, 0.f);
	inline static FVector DarkGreen = FVector(0.f, 0.01f, 0.f);

	inline static FVector Yellow = FVector(1.f, 1.f, 0.f);
	inline static FVector DarkYellow = FVector(0.1f, 0.05f, 0.f);
	inline static FVector Orange = FVector(0.3f, 0.05f, 0.f);
	inline static FVector DarkOrange = FVector(0.15f, 0.02f, 0.f);

	inline static FVector BrightBlue = FVector(0.f, 0.f, 1.f);
	inline static FVector Blue = FVector(0.f, 0.f, 0.5f);
	inline static FVector BlueGreen = FVector(0.05f, 0.59f, 0.72f);

	inline static FVector Pink = FVector(1.f, 0.f, 0.4f);
	inline static FVector White = FVector(1.f, 1.f, 1.f);
	inline static FVector Black = FVector(0.f, 0.f, 0.f);

	inline static FVector DarkGray = FVector(0.05f, 0.05f, 0.05f);
	inline static FVector Gray1 = FVector(0.1f, 0.1f, 0.1f);
	inline static FVector Gray2 = FVector(0.2f, 0.2f, 0.2f);
	inline static FVector Gray3 = FVector(0.3f, 0.3f, 0.3f);
	inline static FVector Gray4 = FVector(0.4f, 0.4f, 0.4f);
	inline static FVector Gray5 = FVector(0.5f, 0.5f, 0.5f);
	inline static FVector Gray6 = FVector(0.6f, 0.6f, 0.6f);
	inline static FVector Gray7 = FVector(0.7f, 0.7f, 0.7f);
	inline static FVector Gray8 = FVector(0.8f, 0.8f, 0.8f);
	inline static FVector Gray9 = FVector(0.9f, 0.9f, 0.9f);

	inline static FVector LightBlue5 = FVector(0.5f, 0.5f, 1.f);
	inline static FVector LightBlue1 = FVector(0.1f, 0.1f, 1.f);

	inline static FVector DarkBlue = FVector(0.f, 0.f, 0.05f);
	inline static FVector Blue1 = FVector(0.f, 0.f, 0.1f);
	inline static FVector Blue2 = FVector(0.f, 0.f, 0.2f);
	inline static FVector Blue3 = FVector(0.f, 0.f, 0.3f);
	inline static FVector Blue4 = FVector(0.f, 0.f, 0.4f);
	inline static FVector Blue5 = FVector(0.f, 0.f, 0.5f);
	inline static FVector Blue6 = FVector(0.f, 0.f, 0.6f);
	inline static FVector Blue7 = FVector(0.f, 0.f, 0.7f);
	inline static FVector Blue8 = FVector(0.f, 0.f, 0.8f);
	inline static FVector Blue9 = FVector(0.f, 0.f, 0.9f);

	inline static FVector Purple = FVector(1.f, 0.f, 1.f);
	inline static FVector DarkPurple = FVector(0.05f, 0.f, 0.05f);
	inline static FVector Purple1 = FVector(0.1f, 0.f, 0.1f);
	inline static FVector Purple2 = FVector(0.2f, 0.f, 0.2f);
	inline static FVector Purple3 = FVector(0.3f, 0.f, 0.3f);
	inline static FVector Purple4 = FVector(0.4f, 0.f, 0.4f);
	inline static FVector Purple5 = FVector(0.5f, 0.f, 0.5f);
	inline static FVector Purple6 = FVector(0.6f, 0.f, 0.6f);
	inline static FVector Purple7 = FVector(0.7f, 0.f, 0.7f);
	inline static FVector Purple8 = FVector(0.8f, 0.f, 0.8f);
	inline static FVector Purple9 = FVector(0.9f, 0.f, 0.9f);

	inline static FVector Red = FVector(1.f, 0.f, 0.f);
	inline static FVector DarkRed = FVector(0.05f, 0.f, 0.f);
	inline static FVector Red1 = FVector(0.1f, 0.f, 0.f);
	inline static FVector Red2 = FVector(0.2f, 0.f, 0.f);
	inline static FVector Red3 = FVector(0.3f, 0.f, 0.f);
	inline static FVector Red4 = FVector(0.4f, 0.f, 0.f);
	inline static FVector Red5 = FVector(0.5f, 0.f, 0.f);
	inline static FVector Red15 = FVector(0.15f, 0.f, 0.f);
};

// Grid Generator - End
//================================================================================================================================
