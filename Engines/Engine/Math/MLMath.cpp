#include "MLMath.h"
#include <algorithm>

int MLEngine::Math::Lerp(int a, int b, float t)
{

	t = std::clamp(t, 0.0f, 1.0f);

	float result;

	result = float(a) + t * float(b - a);

	return int(result);
}


float MLEngine::Math::Lerp(float a, float b, float t)
{

	t = std::clamp(t, 0.0f, 1.0f);

	float result;

	result = a + t * (b - a);

	return result;
}
