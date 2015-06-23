#include <math.h>
#include "CourseMath.h"

double CourseMath::limitAngleRange(double angle) const
{
	double fullRevolution = 360;
	double minAngle = 0;

	if (angle < minAngle) {
		angle += fullRevolution;
	} else if (angle >= minAngle + fullRevolution) {
		angle -= fullRevolution;
	}

	return angle;
}


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
	double boatLatitudeInRadian = degreeToRadian(boat.latitude);
	double waypointLatitudeInRadian = degreeToRadian(waypoint.latitude);
	double deltaLongitudeRadian = degreeToRadian(waypoint.longitude - boat.longitude);

	double y_coordinate = sin(deltaLongitudeRadian)
						* cos(waypointLatitudeInRadian);

	double x_coordinate = cos(boatLatitudeInRadian)
						* sin(waypointLatitudeInRadian)
						- sin(boatLatitudeInRadian)
						* cos(waypointLatitudeInRadian)
						* cos(deltaLongitudeRadian);

	double bearingToWaypointInRadian = atan2(y_coordinate, x_coordinate);
	double bearingToWaypoint = radianToDegree(bearingToWaypointInRadian);

	if (bearingToWaypoint < 0) {
		bearingToWaypoint = 360 + bearingToWaypoint;
	}

	return bearingToWaypoint;
}


double CourseMath::calculateDTW(const PositionModel boat, const PositionModel waypoint) const
{
	return 0.0;
}