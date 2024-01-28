#include "LandGenerator.h"

//================================================================================================================================
// Function Overrides - Start

//----------------------------------------------------------------
// Update Automata At Index
//----------------------------------------------------------------
void ALandGenerator::UpdateAutomataAtIndex(size_t index, ETileType replacementType, ETileType typeToReplace)
{
	const ETileType type = GetSnapshotTile(index);

	const int n = CountNeighbors(index, replacementType);

	if (type == typeToReplace)
	{
		if (n > 0 && RNG.Chance(GrowthChance))
			GridRef->PaintTile(index, replacementType);
	}
}

//----------------------------------------------------------------
// Finalize Automata At Index
//----------------------------------------------------------------
void ALandGenerator::FinalizeAutomataAtIndex(size_t index, ETileType replacementType, ETileType typeToReplace)
{
	const ETileType type = GetSnapshotTile(index);

	const int n = CountNeighbors(index, replacementType);

	if (type == typeToReplace)
	{
		if (n > 3)
			GridRef->PaintTile(index, replacementType);
	}
}

//----------------------------------------------------------------
// Is Valid Generation
//----------------------------------------------------------------
bool ALandGenerator::IsValidGeneration()
{
	TQueue<size_t> idQueue;
	std::vector<bool> wasExplored;
	std::vector<size_t> neighborIds;
	std::vector<std::vector<size_t>> landSets;
	size_t idToCheck = 0;
	size_t numNeighbors = 0;
	size_t neighborId = 0;
	size_t numTiles = GridRef->GetNumTiles();

	wasExplored.resize(numTiles, false);

	for (size_t iTile = 0; iTile < numTiles; ++iTile)
	{
		if (wasExplored[iTile])
			continue;

		if (GridRef->GetTile(iTile) != ETileType::Biome_SolidLand)
			continue;

		landSets.resize(landSets.size() + 1);
		wasExplored[iTile] = true;
		idQueue.Enqueue(iTile);

		while (!idQueue.IsEmpty())
		{
			idQueue.Dequeue(idToCheck);

			landSets[landSets.size() - 1].push_back(idToCheck);

			neighborIds.clear();

			// TODO: cleanup this code ------------------- start
			if (GridRef->IsValidMove(idToCheck, idToCheck + 1))
				neighborIds.push_back(idToCheck + 1);
			if (GridRef->IsValidMove(idToCheck, idToCheck - 1))
				neighborIds.push_back(idToCheck - 1);
			if (GridRef->IsValidMove(idToCheck, idToCheck + GridRef->GetGridWidth()))
				neighborIds.push_back(idToCheck + GridRef->GetGridWidth());
			if (GridRef->IsValidMove(idToCheck, idToCheck - GridRef->GetGridWidth()))
				neighborIds.push_back(idToCheck - GridRef->GetGridWidth());

			if (GridRef->IsValidMove(idToCheck, idToCheck + GridRef->GetGridWidth() + 1))
				neighborIds.push_back(idToCheck + GridRef->GetGridWidth() + 1);
			if (GridRef->IsValidMove(idToCheck, idToCheck + GridRef->GetGridWidth() - 1))
				neighborIds.push_back(idToCheck + GridRef->GetGridWidth() - 1);
			if (GridRef->IsValidMove(idToCheck, idToCheck - GridRef->GetGridWidth() + 1))
				neighborIds.push_back(idToCheck - GridRef->GetGridWidth() + 1);
			if (GridRef->IsValidMove(idToCheck, idToCheck - GridRef->GetGridWidth() - 1))
				neighborIds.push_back(idToCheck - GridRef->GetGridWidth() - 1);
			// TODO: cleanup this code ------------------- end

			numNeighbors = neighborIds.size();
			for (size_t iNeighbor = 0; iNeighbor < numNeighbors; ++iNeighbor)
			{
				neighborId = neighborIds[iNeighbor];

				if (!wasExplored[neighborId] && GridRef->GetTile(neighborId) == ETileType::Biome_SolidLand)
				{
					wasExplored[neighborId] = true;
					idQueue.Enqueue(neighborId);
				}
			}
		}
	}

	BaseCandidates.clear();
	size_t numLands = landSets.size();

	// 1) check quantity
	UE_LOG(LogTemp, Warning, TEXT("Land Count: %d"), landSets.size());
	if (numLands < MinLandQuantity || numLands > MaxLandQuantity)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed Quantity Check!"));
		return false;
	}

	// 2) check size
	size_t numBaseCandidates = 0;
	for (size_t iLand = 0; iLand < numLands; ++iLand)
	{
		UE_LOG(LogTemp, Warning, TEXT("Land %d Size: %d"), iLand, landSets[iLand].size());

		if (landSets[iLand].size() >= MinBaseSize)
		{
			++numBaseCandidates;
			BaseCandidates.resize(numBaseCandidates);
			BaseCandidates[numBaseCandidates - 1] = landSets[iLand];
		}

		if (landSets[iLand].size() > MaxLandSize)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed Size Check!"));
			return false;
		}
	}
	
	if (numBaseCandidates < MinBaseQuantity)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed Size Check!"));
		return false;
	}

	// TODO: finish distance check
	// 3) check distance
	//size_t numLargeLands = largeLandSets.size();
	//for (size_t i1 = 0; i1 < numLands - 1; ++i1)
	//{
	//	for (size_t i2 = i1 + 1; i2 < numLands; ++i2)
	//	{
	//		float x1 = GridRef->GetGridX(largeLandSets[i1][0]);
	//		float x2 = GridRef->GetGridX(largeLandSets[i2][0]);
	//		float x = FMath::Abs(x1 - x2);
	//
	//		float y1 = GridRef->GetGridY(largeLandSets[i1][0]);
	//		float y2 = GridRef->GetGridY(largeLandSets[i2][0]);
	//		float y = FMath::Abs(y1 - y2);
	//
	//		if (FMath::Floor(x + y) >= MinBaseDistance)
	//			return true;
	//	}
	//}

	return true;
}

//----------------------------------------------------------------
// Get Base Candidates
//----------------------------------------------------------------
std::vector<std::vector<size_t>>& ALandGenerator::GetBaseCandidates()
{
	return BaseCandidates;
}

//----------------------------------------------------------------
// Remove Candidate
//----------------------------------------------------------------
bool ALandGenerator::RemoveCandidate(size_t indexToRemove)
{
	if (indexToRemove >= 0 && indexToRemove < BaseCandidates.size())
	{
		BaseCandidates.erase(BaseCandidates.begin() + indexToRemove);
		return true;
	}
	
	return false;
}

// Function Overrides - End
//================================================================================================================================
