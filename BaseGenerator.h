#pragma once

#include "CoreMinimal.h"
#include "GrammarGenerator.h"
#include "LandGenerator.h"
#include "BaseGenerator.generated.h"

//================================================================================================================================
// Structs - Start

//----------------------------------------------------------------
// Structures
//----------------------------------------------------------------
UENUM(BlueprintType)
enum class EStructure
{
	Invalid					UMETA(DisplayName = "Invalid"),
	ConstructionSite		UMETA(DisplayName = "Construction Site"),
	PowerStation			UMETA(DisplayName = "Power Station"),
	ResourceSilo			UMETA(DisplayName = "Resource Silo"),
	Factory					UMETA(DisplayName = "Factory"),
	Barracks				UMETA(DisplayName = "Barracks"),
	Turret					UMETA(DisplayName = "Turret")
};

//----------------------------------------------------------------
// Structure Sizes
//----------------------------------------------------------------
USTRUCT(BlueprintType)
struct FStructureSizes
{
	GENERATED_BODY();

public: // Variables

	UPROPERTY(EditAnywhere) FVector2D ConstructionSite = FVector2D(3.f, 3.f);
	UPROPERTY(EditAnywhere) FVector2D PowerStation = FVector2D(2.f, 3.f);
	UPROPERTY(EditAnywhere) FVector2D ResourceSilo = FVector2D(2.f, 2.f);
	UPROPERTY(EditAnywhere) FVector2D Factory = FVector2D(3.f, 3.f);
	UPROPERTY(EditAnywhere) FVector2D Barracks = FVector2D(2.f, 3.f);
	UPROPERTY(EditAnywhere) FVector2D Turret = FVector2D(1.f, 1.f);
};

// Structs - End
//================================================================================================================================
// Base Generator - Start

UCLASS()
class PROJECTDUNE_API ABaseGenerator : public AGrammarGenerator
{
	GENERATED_BODY()
	
protected: // Function Overrides

	virtual void Init(uint64 seed, AGridWalker* grid) override;
	virtual void InterpretCharacter(TCHAR character) override;
	virtual void Reset() override;

public: // Functions

	void BuildStucture(EStructure type = EStructure::Invalid);

private: // Variables

	UPROPERTY(EditAnywhere, category = "Generator|Base", meta = (AllowPrivateAccess = true))
	FStructureSizes StructureSizes;

	UPROPERTY(EditAnywhere, category = "Generator|Base", meta = (AllowPrivateAccess = true))
	TArray<EStructure> StructureQueue;

	UPROPERTY(EditAnywhere, category = "Generator|Base", meta = (AllowPrivateAccess = true))
	ALandGenerator* LandGeneratorRef;

	UPROPERTY(EditAnywhere, category = "Generator|Base", meta = (AllowPrivateAccess = true))
	int PathStep = 1;

	UPROPERTY(EditAnywhere, category = "Generator|Base", meta = (AllowPrivateAccess = true))
	bool IsEnemy = false;

	size_t CurrentStructureIndex = 0;
};

// Base Generator - End
//================================================================================================================================
