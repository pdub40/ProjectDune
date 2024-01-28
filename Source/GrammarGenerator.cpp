#include "GrammarGenerator.h"

//================================================================================================================================
// Function Overrides - Start

void AGrammarGenerator::Init(uint64 seed, AGridWalker* grid)
{
	AContentGenerator::Init(seed, grid);

	GridRef->ResetWalker(false, 1);
	State = SeedState;
}

//----------------------------------------------------------------
// Generate
//----------------------------------------------------------------
void AGrammarGenerator::Generate()
{
	ConstructString(IterationLimit);
	InterpretString();
}

//----------------------------------------------------------------
// Reset
//----------------------------------------------------------------
void AGrammarGenerator::Reset()
{
	AContentGenerator::Reset();

	State = SeedState;
}

// Function Overrides - End
//================================================================================================================================
// Protected Functions - Start

//----------------------------------------------------------------
// Open Processing Stack
//----------------------------------------------------------------
void AGrammarGenerator::OpenProcessingStack()
{
	FString string;
	ProcessingStack.Push(string);
	IsProcessingStackOpen = true;
}

//----------------------------------------------------------------
// Appened Processing Stack
//----------------------------------------------------------------
void AGrammarGenerator::AppendProcessingStack(TCHAR character)
{
	ProcessingStack.Last().AppendChar(character);
}

//----------------------------------------------------------------
// Close Processing Stack
//----------------------------------------------------------------
void AGrammarGenerator::CloseProcessingStack()
{
	FString string = ProcessingStack.Pop();
	InterpretProcessingStack(string);

	if (ProcessingStack.IsEmpty())
		IsProcessingStackOpen = false;
}

// Protected Functions - End
//================================================================================================================================
// Private Functions - Start

//----------------------------------------------------------------
// Construct String
//----------------------------------------------------------------
void AGrammarGenerator::ConstructString(int iterationLimit)
{
	FString newState;
	size_t stateLength = State.Len();
	const size_t numRules = Rules.Num();
	float randNum = 0.f;

	bool shouldTerminate = false;
	bool hasIterationLimit = iterationLimit != kInvalidNum;
	int numIterations = 0;

	while (!shouldTerminate)
	{
		if (hasIterationLimit && (numIterations >= iterationLimit))
			break;

		{ // iteration - start

			shouldTerminate = true;
			newState.Empty();
			stateLength = State.Len();

			// for string size
			for (size_t iState = 0; iState < stateLength; ++iState)
			{
				randNum = RNG.NormalizedRandom();

				// for num rules
				for (size_t iRules = 0; iRules < numRules; ++iRules)
				{
					// if predecessor matches, append successor
					if (State[iState] == Rules[iRules].Predecessor[0])
					{
						// check weighted random
						if (randNum < Rules[iRules].Weight)
						{
							newState.Append(Rules[iRules].Successor);
							shouldTerminate = false;
							break;
						}
						else
						{
							randNum -= Rules[iRules].Weight;
						}
					}

					// if no predecessor matches, carry over constant
					else if (iRules >= numRules - 1)
					{
						newState.AppendChar(State[iState]);
					}
				}
			}

			State = newState;

		} // iteration - end

		++numIterations;
	}
}

//----------------------------------------------------------------
// Interpret String
//----------------------------------------------------------------
void AGrammarGenerator::InterpretString()
{
	const size_t stateLength = State.Len();
	TCHAR character = '0';

	for (size_t i = 0; i < stateLength; ++i)
	{
		if (ShouldAbort)
			return;

		character = State[i];

		if (character == OpenProcessingStackCharacter[0])
			OpenProcessingStack();

		else if (character == CloseProcessingStackCharacter[0])
			CloseProcessingStack();

		else if (IsProcessingStackOpen)
			AppendProcessingStack(character);

		else
			InterpretCharacter(State[i]);
	}
}

// Private Functions - End
//================================================================================================================================
