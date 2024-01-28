#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridWalker.h"
#include "ContentGenerator.h"
#include "RandomNumberGenerator.h"
#include "WorldGenerator.generated.h"

//================================================================================================================================
// World Generator - Start

UCLASS()
class PROJECTDUNE_API AWorldGenerator : public AActor
{
	GENERATED_BODY()
	
public:	// Constructors

	AWorldGenerator();

protected: // Function Overrides

	virtual void BeginPlay() override;

public: // Functions

	UFUNCTION(BlueprintCallable)
	void Generate();

protected: // Functions

	void SeedWorld();

private: // Variables

	UPROPERTY(EditAnywhere, category = "World", meta = (AllowPrivateAccess = true))
	AGridWalker* Grid = nullptr;

	UPROPERTY(EditAnywhere, category = "World", meta = (AllowPrivateAccess = true))
	TArray<AContentGenerator*> ContentGenerators;

	UPROPERTY(EditAnywhere, category = "World|Seed", meta = (AllowPrivateAccess = true))
	bool UseSpecifiedSeed = false;

	UPROPERTY(EditAnywhere, category = "World|Seed", meta = (AllowPrivateAccess = true))
	int Seed = 17;

	RandomNumberGenerator RNG;
};

// World Generator - End
//================================================================================================================================
