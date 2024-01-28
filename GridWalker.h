#pragma once

#include "CoreMinimal.h"
#include "GridGenerator.h"
#include "GridWalker.generated.h"

//================================================================================================================================
// Grid Walker - Start

UCLASS()
class PROJECTDUNE_API AGridWalker : public AGridGenerator
{
	GENERATED_BODY()
	
protected: // Function Overrides

	virtual void PostGridInit() override;

public: // Functions

	void ResetWalker(bool randomDirection = true, int direction = 0);

	bool MoveForward(bool shouldPaintTile = false, ETileType tileType = ETileType::State_Active);
	void TurnLeft();
	void TurnRight();

	void PushLocation();
	void PopLocation();

	int GetDirection();
	int GetCurrentIndex();
	int GetCurrentX();
	int GetCurrentY();
	void SetCurrentIndex(int index);
	void SetDirection(int index);

private: // Variables

	int CurrentIndex = 0;
	int DirectionIndex = 0;
	int Directions[kNumDirections] = { 0, 0, 0, 0 };
	int StepSize = 1;

	TArray<int> LocationStack;
	TArray<int> DirectionStack;
};

// Grid Walker - End
//================================================================================================================================
