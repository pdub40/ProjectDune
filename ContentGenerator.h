#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridWalker.h"
#include "RandomNumberGenerator.h"
#include "ContentGenerator.generated.h"

//================================================================================================================================
// Content Generator - Start

UCLASS()
class PROJECTDUNE_API AContentGenerator : public AActor
{
	GENERATED_BODY()
	
public:	// Constructors

	AContentGenerator();

public: // Virtual Functions

	virtual void Run(uint64 seed, AGridWalker* grid);

	virtual void Init(uint64 seed, AGridWalker* grid);
	virtual void Generate() {}
	virtual void Finalize() {}
	virtual void Reset();
	virtual bool IsValidGeneration();

	virtual bool IsValidNeighborOfType(int x, int y, ETileType type);

public: // Functions

	void SeedGenerator(uint64 seed);
	AGridWalker* GetGridRef();

protected: // Functions

	void StartGeneration();
	void AbortGeneration();
	bool IsRunning();
	void SaveTiles();
	void RestoreTiles();
	int CountNeighbors(size_t index, ETileType typeToCount, int neighborhoodRadius = 1);
	std::vector<size_t> GetNeighborIdsOfType(size_t index, ETileType typeToCount, int neighborhoodRadius = 1);

protected: // Variables

	UPROPERTY(EditAnywhere, category = "Generator|Seed", meta = (AllowPrivateAccess = true))
	bool UseSpecifiedSeed = false;

	UPROPERTY(EditAnywhere, category = "Generator|Seed", meta = (AllowPrivateAccess = true))
	uint64 Seed = 17;

	bool ShouldAbort = false;
	TArray<ETileType> BackupTiles;

	AGridWalker* GridRef = nullptr;
	RandomNumberGenerator RNG;
};

// Content Generator - End
//================================================================================================================================
