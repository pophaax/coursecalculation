#ifndef __COURSEMATH_H__
#define __COURSEMATH_H__

#include "../models/PositionModel.h"

class CourseMath {
public:
	double anglesDifference(const double angle1, const double angle2) const;
	double limitAngleRange(double angle) const;
	double degreeToRadian(const double degrees) const;
	double radianToDegree(const double radians) const;
	double calculateBTW(const PositionModel boat, const PositionModel waypoint) const;
	double calculateDTW(const PositionModel boat, const PositionModel waypoint) const;
};

#endif
