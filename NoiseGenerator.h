#pragma once

#include "CoreMinimal.h"
#include "ContentGenerator.h"
#include "NoiseGenerator.generated.h"

//================================================================================================================================
// Noise Generator - Start

UCLASS()
class PROJECTDUNE_API ANoiseGenerator : public AContentGenerator
{
	GENERATED_BODY()
	
public: // Function Overrides

	virtual void Init(uint64 seed, AGridWalker* grid) override;

protected: // Virtual Functions

	virtual float CalculateNoise(size_t index);
	
public: // Functions
	
	float GenerateNoise(size_t index, float currentNoise = 0.f);

protected: // Functions

	float GetPerlinNoise(float x, float y);
	float GetPerlinNoise(float x, float y, TArray<uint64> seed);
	float GetPerlinNoise(float x, float y, float inputRange, unsigned int octaves, float persistence, TArray<uint64> seed);
	float GetPerlinNoise_Internal(float x, float y, float inputRange, uint64 seed);

	FVector2D GetGradient(int x, int y, uint64 seed);
	float GetGradientDot(int gridX, int gridY, float x, float y, uint64 seed);
	
	float CalculateLayerScalar(size_t index);

protected: // Variables

	UPROPERTY(EditAnywhere, category = "Generator|Noise", meta = (AllowPrivateAccess = true))
	float InputRange = 1.f;

	UPROPERTY(EditAnywhere, category = "Generator|Noise", meta = (AllowPrivateAccess = true))
	unsigned int Octaves = 1;

	UPROPERTY(EditAnywhere, category = "Generator|Noise", meta = (AllowPrivateAccess = true))
	float Persistence = 0.5f;

	/** Scalar used to modify internal noise score. Value 0.f will zero out noise. */
	UPROPERTY(EditAnywhere, category = "Generator|Noise", meta = (AllowPrivateAccess = true))
	float Weight = 1.0f;

	/** Scalar used to modify external noise score. Value 1.f won't affect noise. */
	UPROPERTY(EditAnywhere, category = "Generator|Noise", meta = (AllowPrivateAccess = true))
	float LayerScalar = 1.0f;

	/** If true, applies noise value as scalar rather than summation. */
	UPROPERTY(EditAnywhere, category = "Generator|Noise", meta = (AllowPrivateAccess = true))
	bool UseNoiseAsLayerScalar = false;

	TArray<uint64> OctaveSeeds;
};

// Noise Generator - End
//================================================================================================================================
