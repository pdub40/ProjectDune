#pragma once

#include "CoreMinimal.h"
#include "ContentGenerator.h"
#include "NoiseGenerator.h"
#include "BiomeGenerator.generated.h"

//================================================================================================================================
// Structs - Start

//----------------------------------------------------------------
// Biome Threshold
//----------------------------------------------------------------
USTRUCT(BlueprintType)
struct FBiomeThreshold
{
	GENERATED_BODY();

public: // Variables

	UPROPERTY(EditAnywhere) float Water = -1;
	UPROPERTY(EditAnywhere) float Ice = 5;
	UPROPERTY(EditAnywhere) float HighDesert = 10;
	UPROPERTY(EditAnywhere) float Elevation1 = 18;
	UPROPERTY(EditAnywhere) float Elevation2 = 25;
	UPROPERTY(EditAnywhere) float Elevation3 = 30;
	UPROPERTY(EditAnywhere) float Elevation4 = 35;
	UPROPERTY(EditAnywhere) float Elevation5 = 50;
	UPROPERTY(EditAnywhere) float Elevation6 = 55;
	UPROPERTY(EditAnywhere) float LowDesert = 60;
};

// Structs - End
//================================================================================================================================
// Biome Generator - Start

UCLASS()
class PROJECTDUNE_API ABiomeGenerator : public AContentGenerator
{
	GENERATED_BODY()

protected: // Function Overrides

	virtual void Init(uint64 seed, AGridWalker* grid) override;
	virtual void Generate() override;
	virtual bool IsValidGeneration() override;

public: // Functions

	float GetNoiseAtIndex(size_t index);

private: // Functions

	ETileType GetBiome(int index, float noise);
	void SetBiome(int index, float noise);

private: // Variables

	UPROPERTY(EditAnywhere, category = "Generator|Biome", meta = (AllowPrivateAccess = true))
	TArray<ANoiseGenerator*> NoiseGenerators;

	UPROPERTY(EditAnywhere, category = "Generator|Biome", meta = (AllowPrivateAccess = true))
	FBiomeThreshold Thresholds;

	UPROPERTY(EditAnywhere, category = "Generator|Biome|Clamps", meta = (AllowPrivateAccess = true))
	int MinIceTiles = 15;

	UPROPERTY(EditAnywhere, category = "Generator|Biome|Clamps", meta = (AllowPrivateAccess = true))
	int MaxIceTiles = 150;

	UPROPERTY(EditAnywhere, category = "Generator|Biome|Clamps", meta = (AllowPrivateAccess = true))
	int MinHighDesertTiles = 250;

	UPROPERTY(EditAnywhere, category = "Generator|Biome|Clamps", meta = (AllowPrivateAccess = true))
	int MaxHighDesertTiles = 5000;

	TArray<float> NoiseGrid;
};

// Biome Generator - End
//================================================================================================================================
