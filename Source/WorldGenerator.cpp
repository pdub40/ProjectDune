#include "WorldGenerator.h"

//================================================================================================================================
// Constructors - Start

//----------------------------------------------------------------
// Constructor - Base
//----------------------------------------------------------------
AWorldGenerator::AWorldGenerator()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Constructors - End
//================================================================================================================================
// Function Overrides - Start

//----------------------------------------------------------------
// Begin Play
//----------------------------------------------------------------
void AWorldGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	if (Grid == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("World Gen had null reference to Grid!"));
		return;
	}

	Generate();
}

// Function Overrides - End
//================================================================================================================================
// Public Functions - Start

//----------------------------------------------------------------
// Generate
//----------------------------------------------------------------
void AWorldGenerator::Generate()
{
	SeedWorld();

	Grid->Init(RNG.Random());
	Grid->Generate();

	const size_t numGenerators = ContentGenerators.Num();
	for (size_t i = 0; i < numGenerators; ++i)
	{
		ContentGenerators[i]->Run(RNG.Random(), Grid);
	}
}

// Public Overrides - End
//================================================================================================================================
// Protected Functions - Start

//----------------------------------------------------------------
// Seed World
//----------------------------------------------------------------
void AWorldGenerator::SeedWorld()
{
	if (UseSpecifiedSeed)
	{
		RNG.Seed(Seed);
	}
	else
	{
		RNG.RandomizeSeed();
	}
}

// Protected Functions - End
//================================================================================================================================
