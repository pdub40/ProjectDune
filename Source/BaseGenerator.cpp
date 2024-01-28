#include "BaseGenerator.h"

//================================================================================================================================
// Function Overrides - Start

//----------------------------------------------------------------
// Init
//----------------------------------------------------------------
void ABaseGenerator::Init(uint64 seed, AGridWalker* grid)
{
	AGrammarGenerator::Init(seed, grid);

	CurrentStructureIndex = 0;

	const std::vector<std::vector<size_t>> candidates = LandGeneratorRef->GetBaseCandidates();
	const size_t numCandidates = candidates.size();
	size_t index = 0;
	int largestCandidateSize = 0;
	int largestCandidateIndex = -1;

	for (size_t i = 0; i < numCandidates; ++i)
	{
		if (candidates[i].size() > largestCandidateSize)
		{
			largestCandidateSize = candidates[i].size();
			largestCandidateIndex = i;
		}
	}

	index = candidates[largestCandidateIndex][largestCandidateSize / 2];

	GridRef->SetCurrentIndex(index);
	LandGeneratorRef->RemoveCandidate(largestCandidateIndex);
}

//----------------------------------------------------------------
// Interpret Character
//----------------------------------------------------------------
void ABaseGenerator::InterpretCharacter(TCHAR character)
{
	switch (character)
	{
	case 's': { GridRef->PaintTile(GridRef->GetCurrentIndex(), ETileType::State_Dead); break; }
	case 'm': { GridRef->MoveForward(true, ETileType::State_Dead); break; }
	case '+': { GridRef->TurnLeft(); break; }
	case '-': { GridRef->TurnRight(); break; }
	case '[': { GridRef->PushLocation(); break; }
	case ']': { GridRef->PopLocation(); break; }
	case 'c': { BuildStucture(EStructure::ConstructionSite); break; }
	case 'p': { BuildStucture(EStructure::PowerStation); break; }
	case 'r': { BuildStucture(EStructure::ResourceSilo); break; }
	case 'f': { BuildStucture(EStructure::Factory); break; }
	case 'b': { BuildStucture(EStructure::Barracks); break; }
	case 't': { BuildStucture(EStructure::Turret); break; }
	case 'q': { BuildStucture(); break; }
	}
}

//----------------------------------------------------------------
// Reset
//----------------------------------------------------------------
void ABaseGenerator::Reset()
{
	AGrammarGenerator::Reset();

	CurrentStructureIndex = 0;
}

//----------------------------------------------------------------
// Build Structure
// - CRUNCH CODE: NEEDS CLEAN UP
//----------------------------------------------------------------
void ABaseGenerator::BuildStucture(EStructure type)
{
	float width = 0;
	float height = 0;

	GridRef->PushLocation();
	GridRef->SetDirection(1);

	if (type == EStructure::Invalid)
	{
		if (CurrentStructureIndex >= StructureQueue.Num())
		{
			return;
		}

		type = StructureQueue[CurrentStructureIndex];
		++CurrentStructureIndex;
	}

	for (int i = 0; i < PathStep; ++i)
		GridRef->MoveForward(true, ETileType::State_Dead);

	switch (type)
	{
		case EStructure::ConstructionSite:
		{
			width = StructureSizes.ConstructionSite.X;
			height = StructureSizes.ConstructionSite.Y;
			break;
		}
		case EStructure::PowerStation:
		{
			width = StructureSizes.PowerStation.X;
			height = StructureSizes.PowerStation.Y;
			break;
		}
		case EStructure::ResourceSilo:
		{
			width = StructureSizes.ResourceSilo.X;
			height = StructureSizes.ResourceSilo.Y;
			break;
		}
		case EStructure::Factory:
		{
			width = StructureSizes.Factory.X;
			height = StructureSizes.Factory.Y;
			break;
		}
		case EStructure::Barracks:
		{
			width = StructureSizes.Barracks.X;
			height = StructureSizes.Barracks.Y;
			break;
		}
		case EStructure::Turret:
		{
			width = StructureSizes.Turret.X;
			height = StructureSizes.Turret.Y;
			break;
		}
	}

	const int x = GridRef->GetCurrentX();
	const int y = GridRef->GetCurrentY();

	if (width > 0)
		width -= 1;

	const int left = FMath::Floor(width / 2);
	const int right = width - left;

	const int startX = y - left;
	const int endX = y + right;
	const int startY = x + 1;
	const int endY = x + height;

	int index = 0;

	for (int iy = startY; iy <= endY; ++iy)
	{
		for (int ix = startX; ix <= endX; ++ix)
		{
			index = GridRef->GetIndex(iy, ix);

			if (type == EStructure::Turret)
				GridRef->PaintTile(index, ETileType::Unit_Turret);
			else
				GridRef->PaintTile(index, ETileType::State_Dead);
		}
	}

	GridRef->PopLocation();
}

// Function Overrides - End
//================================================================================================================================
