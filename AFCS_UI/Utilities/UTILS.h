#pragma once

inline double DegreesToRadians(double DEGREES)
{
	return DEGREES * 0.0174532925;
}

inline int FeetPerSecondToFeetPerMinute(double FEET_PER_SECOND)
{
	return FEET_PER_SECOND * 60;
}

inline double RadiansToDegrees(double RADIANS)
{
	return RADIANS * 57.2957795;
}

inline int Sign(double VALUE)
{
	if (VALUE >= 0) return 1;
	else return -1;
}