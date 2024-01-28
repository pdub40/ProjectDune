#pragma once

#include "CoreMinimal.h"
#include "AutomataGenerator.h"
#include "LandGenerator.generated.h"

//================================================================================================================================
// Land Generator - Start

UCLASS()
class PROJECTDUNE_API ALandGenerator : public AAutomataGenerator
{
	GENERATED_BODY()
	
public: // Function Overrides

	virtual void UpdateAutomataAtIndex(size_t index, ETileType replacementType, ETileType typeToReplace) override;
	virtual void FinalizeAutomataAtIndex(size_t index, ETileType replacementType, ETileType typeToReplace) override;
	virtual bool IsValidGeneration() override;

public: // Functions

	std::vector<std::vector<size_t>>& GetBaseCandidates();
	bool RemoveCandidate(size_t indexToRemove);

private: // Variables

	UPROPERTY(EditAnywhere, category = "Generator|Automata|Land", meta = (AllowPrivateAccess = true))
	float GrowthChance = 50.f;

	UPROPERTY(EditAnywhere, category = "Generator|Automata|Land", meta = (AllowPrivateAccess = true))
	int MinLandQuantity = 2;

	UPROPERTY(EditAnywhere, category = "Generator|Automata|Land", meta = (AllowPrivateAccess = true))
	int MaxLandQuantity = 7;

	UPROPERTY(EditAnywhere, category = "Generator|Automata|Land", meta = (AllowPrivateAccess = true))
	int MaxLandSize = 1000;

	UPROPERTY(EditAnywhere, category = "Generator|Automata|Land|Base", meta = (AllowPrivateAccess = true))
	int MinBaseQuantity = 2;

	UPROPERTY(EditAnywhere, category = "Generator|Automata|Land|Base", meta = (AllowPrivateAccess = true))
	int MinBaseSize = 50;

	UPROPERTY(EditAnywhere, category = "Generator|Automata|Land|Base", meta = (AllowPrivateAccess = true))
	int MinBaseDistance = 50;

	std::vector<std::vector<size_t>> BaseCandidates;
};

// Land Generator - End
//================================================================================================================================
