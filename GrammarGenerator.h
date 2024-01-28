#pragma once

#include "CoreMinimal.h"
#include "ContentGenerator.h"
#include "GrammarGenerator.generated.h"

//================================================================================================================================
// Structs - Start

USTRUCT(BlueprintType)
struct FGrammarRule
{
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(EditAnywhere)
	FString Predecessor;

	UPROPERTY(EditAnywhere)
	FString Successor;

	UPROPERTY(EditAnywhere)
	float Weight;
};

// Structs - End
//================================================================================================================================
// Grammar Generator - Start

UCLASS()
class PROJECTDUNE_API AGrammarGenerator : public AContentGenerator
{
	GENERATED_BODY()
	
public: // Function overrides

	virtual void Init(uint64 seed, AGridWalker* grid);
	virtual void Generate() override;
	virtual void Reset() override;

protected: // Virtual Functions

	// OpenProcessingStackCharacter & CloseProcessingStackCharacter characters
	// will NEVER be processed if placed inside this function. They are used
	// to operate ProcessingStack prior to this function being called.
	virtual void InterpretCharacter(TCHAR character) {}

	virtual void InterpretProcessingStack(FString string) {}

protected: // Functions

	void OpenProcessingStack();
	void AppendProcessingStack(TCHAR character);
	void CloseProcessingStack();

private: // Functions

	void ConstructString(int iterationLimit = kInvalidNum);
	void InterpretString();

protected: // Variables

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FString State;

	UPROPERTY(EditAnywhere, category = "Generator|Grammar", meta = (AllowPrivateAccess = true))
	FString SeedState;

	UPROPERTY(EditAnywhere, category = "Generator|Grammar", meta = (AllowPrivateAccess = true))
	int IterationLimit = kInvalidNum;

	UPROPERTY(EditAnywhere, category = "Generator|Grammar", meta = (AllowPrivateAccess = true), meta = (TitleProperty = "Name"))
	TArray<FGrammarRule> Rules;

	UPROPERTY(EditAnywhere, category = "Generator|Grammar|Processing Stack", meta = (AllowPrivateAccess = true))
	FString OpenProcessingStackCharacter = "(";

	UPROPERTY(EditAnywhere, category = "Generator|Grammar|Processing Stack", meta = (AllowPrivateAccess = true))
	FString CloseProcessingStackCharacter = ")";

	TArray<FString> ProcessingStack;
	bool IsProcessingStackOpen = false;
};

// Grammar Generator - End
//================================================================================================================================
