#pragma once

#include "CoreMinimal.h"
#include "ContentGenerator.h"
#include "AutomataGenerator.generated.h"

//================================================================================================================================
// Automata Generator - Start

UCLASS()
class PROJECTDUNE_API AAutomataGenerator : public AContentGenerator
{
	GENERATED_BODY()
	
public: // Function Overrides

	virtual void Init(uint64 seed, AGridWalker* grid) override;
	virtual void Generate() override;
	virtual void Finalize() override;

	virtual bool IsValidNeighborOfType(int x, int y, ETileType type) override;

protected: // Virtual Functions

	virtual void Salt(float chance, ETileType saltType, ETileType typeToReplace);
	virtual void UpdateAutomataAtIndex(size_t index, ETileType replacementType, ETileType typeToReplace) {}
	virtual void FinalizeAutomataAtIndex(size_t index, ETileType replacementType, ETileType typeToReplace) {}

protected: // Functions

	ETileType GetSnapshotTile(int x, int y);
	ETileType GetSnapshotTile(size_t index);

protected: // Variables

	UPROPERTY(EditAnywhere, category = "Generator|Automata", meta = (AllowPrivateAccess = true))
	float SaltChance = 15.f;

	UPROPERTY(EditAnywhere, category = "Generator|Automata", meta = (AllowPrivateAccess = true))
	int IterationLimit = 12;

	UPROPERTY(EditAnywhere, category = "Generator|Automata", meta = (AllowPrivateAccess = true))
	ETileType ReplacementType = ETileType::Empty;

	UPROPERTY(EditAnywhere, category = "Generator|Automata", meta = (AllowPrivateAccess = true))
	ETileType TypeToReplace = ETileType::Empty;

	TArray<ETileType> Snapshot;
	float StepInterval = 0.125f;
};

// Automata Generator - End
//================================================================================================================================
