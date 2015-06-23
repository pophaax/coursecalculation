#include <math.h>
#include "CourseMath.h"

double CourseMath::degreeToRadian(const double degrees) const
{
	return degrees * M_PI / 180;
}


double CourseMath::radianToDegree(const double radians) const
{
	return radians / M_PI * 180;
}


double CourseMath::calculateBTW(const PositionModel boat, const PositionModel waypoint) const
{
	return 0.0;
}


double CourseMath::calculateDTW(const PositionModel boat, const PositionModel waypoint) const
{
	return 0.0;
}