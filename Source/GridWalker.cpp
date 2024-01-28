#include "GridWalker.h"

//================================================================================================================================
// Function Overrides - Start

//----------------------------------------------------------------
// Post Grid Init
//----------------------------------------------------------------
void AGridWalker::PostGridInit()
{
	Directions[0] = -GridWidth;
	Directions[1] = 1;
	Directions[2] = GridWidth;
	Directions[3] = -1;

	ResetWalker();
}

// Function Overrides - Start
//================================================================================================================================
// Protected Functions - Start

//----------------------------------------------------------------
// Reset
//----------------------------------------------------------------
void AGridWalker::ResetWalker(bool randomDirection, int directionIndex)
{
	CurrentIndex = GetMidpointIndex();

	if (randomDirection)
		DirectionIndex = RNG.Random(kNumDirections);
	else
		DirectionIndex = directionIndex;
}

//----------------------------------------------------------------
// Move Forward
//----------------------------------------------------------------
bool AGridWalker::MoveForward(bool shouldPaintTile, ETileType tileType)
{
	for (int i = 0; i < StepSize; ++i)
	{
		int deltaIndex = CurrentIndex;

		CurrentIndex += Directions[DirectionIndex];

		if (IsValidMove(deltaIndex, CurrentIndex))
		{
			if (shouldPaintTile)
				PaintTile(CurrentIndex, tileType);
		}
		else
		{
			return false;
		}
	}

	return true;
}

//----------------------------------------------------------------
// Turn Left
//----------------------------------------------------------------
void AGridWalker::TurnLeft()
{
	--DirectionIndex;

	if (DirectionIndex < 0)
		DirectionIndex = kNumDirections - 1;
}

//----------------------------------------------------------------
// Turn Right
//----------------------------------------------------------------
void AGridWalker::TurnRight()
{
	++DirectionIndex;

	if (DirectionIndex >= kNumDirections)
		DirectionIndex = 0;
}

//----------------------------------------------------------------
// Push Location
//----------------------------------------------------------------
void AGridWalker::PushLocation()
{
	LocationStack.Push(CurrentIndex);
	DirectionStack.Push(DirectionIndex);
}

//----------------------------------------------------------------
// Pop Location
//----------------------------------------------------------------
void AGridWalker::PopLocation()
{
	CurrentIndex = LocationStack.Pop();
	DirectionIndex = DirectionStack.Pop();
}

//----------------------------------------------------------------
// Get Direction
//----------------------------------------------------------------
int AGridWalker::GetDirection()
{
	return Directions[DirectionIndex];
}

//----------------------------------------------------------------
// Get Current Index
//----------------------------------------------------------------
int AGridWalker::GetCurrentIndex()
{
	return CurrentIndex;
}

//----------------------------------------------------------------
// Get Current X
//----------------------------------------------------------------
int AGridWalker::GetCurrentX()
{
	return GetGridX(CurrentIndex);
}

//----------------------------------------------------------------
// Get Current Y
//----------------------------------------------------------------
int AGridWalker::GetCurrentY()
{
	return GetGridY(CurrentIndex);
}

//----------------------------------------------------------------
// Set Current Index
//----------------------------------------------------------------
void AGridWalker::SetCurrentIndex(int index)
{
	CurrentIndex = index;
}

//----------------------------------------------------------------
// Set Direction
//----------------------------------------------------------------
void AGridWalker::SetDirection(int index)
{
	DirectionIndex = index;
}

// Protected Functions - End
//================================================================================================================================
