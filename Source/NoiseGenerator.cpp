#include "NoiseGenerator.h"
#include "MathHelper.h"

//================================================================================================================================
// Function Overrides - Start

//----------------------------------------------------------------
// Init
//----------------------------------------------------------------
void ANoiseGenerator::Init(uint64 seed, AGridWalker* grid)
{
	AContentGenerator::Init(seed, grid);

	for (size_t i = 0; i < Octaves; ++i)
	{
		OctaveSeeds.Add(RNG.Random());
	}
}

// Function Overrides - End
//================================================================================================================================
// Virtual Functions - Start

//----------------------------------------------------------------
// Calculate Noise
//----------------------------------------------------------------
float ANoiseGenerator::CalculateNoise(size_t index)
{
	const float x = GridRef->GetWorldX(index);
	const float y = GridRef->GetWorldY(index);

	const float noise = GetPerlinNoise(x, y);
	const float smoothedNoise = MathHelper::SmootherStep(0.f, 1.f, noise);

	return smoothedNoise * Weight;
}

// Virtual Functions - End
//================================================================================================================================
// Public Functions - Start

//----------------------------------------------------------------
// Generate Noise
//----------------------------------------------------------------
float ANoiseGenerator::GenerateNoise(size_t index, float currentNoise)
{
	currentNoise += CalculateNoise(index);
	currentNoise *= CalculateLayerScalar(index);

	return currentNoise;
}

// Public Functions - End
//================================================================================================================================
// Protected Functions - Start

//----------------------------------------------------------------
// Get Layer Scalar
//----------------------------------------------------------------
float ANoiseGenerator::CalculateLayerScalar(size_t index)
{
	if (UseNoiseAsLayerScalar)
		return CalculateNoise(index);

	return LayerScalar;
}

//----------------------------------------------------------------
// Get Perlin Noise
//----------------------------------------------------------------
float ANoiseGenerator::GetPerlinNoise(float x, float y)
{
	float inputRange = InputRange;
	float persistence = Persistence;
	float octaves = Octaves;
	
	if (octaves <= 0)
		return 0.f;
	
	float noise = 0.f;
	float localNoise = 0.f;
	int currentAmplitude = 1;
	int totalAmplitude = 0;
	
	for (int i = (octaves - 1); i >= 0; --i)
	{
		totalAmplitude += currentAmplitude;
	
		localNoise = GetPerlinNoise_Internal(x, y, inputRange, Seed);
		noise += localNoise * currentAmplitude;
	
		currentAmplitude *= persistence;
		inputRange *= 2;
	}
	
	noise /= totalAmplitude;
	
	return noise;
}

//----------------------------------------------------------------
// Get Perlin Noise - Multi-seed
//----------------------------------------------------------------
float ANoiseGenerator::GetPerlinNoise(float x, float y, TArray<uint64> seed)
{
	float inputRange = InputRange;
	float persistence = Persistence;
	float octaves = Octaves;

	if (octaves <= 0)
		return 0.f;

	float noise = 0.f;
	float localNoise = 0.f;
	int currentAmplitude = 1;
	int totalAmplitude = 0;

	for (int i = (octaves - 1); i >= 0; --i)
	{
		totalAmplitude += currentAmplitude;

		localNoise = GetPerlinNoise_Internal(x, y, inputRange, seed[i]);
		noise += localNoise * currentAmplitude;

		currentAmplitude *= persistence;
		inputRange *= 2;
	}

	noise /= totalAmplitude;

	return noise;
}

//----------------------------------------------------------------
// Get Perlin Noise - Complex, Multi-seed
//----------------------------------------------------------------
float ANoiseGenerator::GetPerlinNoise(float x, float y, float inputRange, unsigned int octaves, float persistence, TArray<uint64> seed)
{
	if (octaves <= 0)
		return 0.f;

	float noise = 0.f;
	float localNoise = 0.f;
	int currentAmplitude = 1;
	int totalAmplitude = 0;

	for (int i = (octaves - 1); i >= 0; --i)
	{
		totalAmplitude += currentAmplitude;

		localNoise = GetPerlinNoise_Internal(x, y, inputRange, seed[i]);
		noise += localNoise * currentAmplitude;

		currentAmplitude *= persistence;
		inputRange *= 2;
	}

	noise /= totalAmplitude;

	return noise;
}

//----------------------------------------------------------------
// Get Perlin Noise - Internal
//----------------------------------------------------------------
float ANoiseGenerator::GetPerlinNoise_Internal(float x, float y, float inputRange, uint64 seed)
{
	float maxX = (GridRef->GetGridWidth() * GridRef->GetTileSize());
	float maxY = (GridRef->GetGridHeight() * GridRef->GetTileSize());

	x = (x / maxX) * inputRange;
	y = (y / maxY) * inputRange;

	const int left = (int)floor(x);
	const int right = left + 1;
	const int top = (int)floor(y);
	const int bottom = top + 1;
	
	const float weightX = x - (float)left;
	const float weightY = y - (float)top;
	
	const float topLeft = GetGradientDot(left, top, x, y, seed);
	const float topRight = GetGradientDot(right, top, x, y, seed);
	const float lerpTop = MathHelper::SmootherStep(topLeft, topRight, weightX);
	
	const float bottomLeft = GetGradientDot(left, bottom, x, y, seed);
	const float bottomRight = GetGradientDot(right, bottom, x, y, seed);
	const float lerpBottom = MathHelper::SmootherStep(bottomLeft, bottomRight, weightX);
	
	const float noise = MathHelper::SmootherStep(lerpTop, lerpBottom, weightY);
	const float normalizedNoise = FMath::GetMappedRangeValueClamped(FVector2D(-1, 1), FVector2D(0, 1), noise);
	
	return normalizedNoise;
}

//----------------------------------------------------------------
// Get Gradient
//----------------------------------------------------------------
FVector2D ANoiseGenerator::GetGradient(int x, int y, uint64 seed)
{
	const unsigned w = 8 * sizeof(unsigned);
	const unsigned s = w / 2;

	x *= seed;
	y *= seed;

	x *= 3284157443;
	y ^= x << s | x >> (w - s);
	y *= 1911520717;
	x ^= y << s | y >> (w - s);
	x *= 2048419325;

	float rand = x * (UE_PI / ~(~0u >> 1));

	return FVector2D(cos(rand), sin(rand));
}

//----------------------------------------------------------------
// Get Gradient Dot
//----------------------------------------------------------------
float ANoiseGenerator::GetGradientDot(int gridX, int gridY, float x, float y, uint64 seed)
{
	FVector2D gradient = GetGradient(gridX, gridY, seed);

	const float deltaX = x - (float)gridX;
	const float deltaY = y - (float)gridY;

	return ((deltaX * gradient.X) + (deltaY * gradient.Y));
}

// Protected Functions - End
//================================================================================================================================
