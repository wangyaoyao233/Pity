#include "main.h"
#include "Mathf.h"

float Mathf::Lerp(float from, float to, float t)
{
	if (t < 0)
	{
		t = 0;
	}
	if (t > 1)
	{
		t = 1;
	}

	return t * to + (1 - t) * from;
}