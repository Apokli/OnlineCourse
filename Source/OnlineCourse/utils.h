#pragma once

bool IsFloatZero(float Value)
{
	return fabs(Value) < FLT_EPSILON;
}