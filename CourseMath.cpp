#include "CourseMath.h"
#include <cmath>
#include "models/PositionModel.h"


bool CourseMath::isAngleInSector(const double angle, const double sectorAngle1,
	const double sectorAngle2) const
{
	double diff1 = angleDifference(angle, sectorAngle1);
	double diff2 = angleDifference(angle, sectorAngle2);
	double sectorDiff = angleDifference(sectorAngle1, sectorAngle2);

	if (diff1 < sectorDiff && diff2 < sectorDiff)
		return true;
	else
		return false;
}


double CourseMath::angleDifference(const double angle1, const double angle2) const
{
	const double fullRevolution = 360;
	
	double diff = std::abs(limitAngleRange(angle1) - limitAngleRange(angle2));
	if (diff > fullRevolution/2) diff = fullRevolution - diff;

	return diff;
}


double CourseMath::limitAngleRange(double angle) const
{
	const double fullRevolution = 360;
	const double minAngle = 0;

	while (angle < minAngle)
		angle += fullRevolution;

	while (angle >= minAngle + fullRevolution)
		angle -= fullRevolution;

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

	bearingToWaypoint = limitAngleRange(bearingToWaypoint);

	return bearingToWaypoint;
}


double CourseMath::calculateDTW(const PositionModel boat, const PositionModel waypoint) const
{
	const double radiusOfEarth = 6371.0;

	double deltaLatitudeRadians = degreeToRadian(waypoint.latitude - boat.latitude);
	double boatLatitudeInRadian = degreeToRadian(boat.latitude);
	double waypointLatitudeInRadian = degreeToRadian(waypoint.latitude);
	double deltaLongitudeRadians = degreeToRadian(waypoint.longitude - boat.longitude);

	double a = sin(deltaLatitudeRadians/2)
			* sin(deltaLatitudeRadians/2)
			+ cos(boatLatitudeInRadian)
			* cos(waypointLatitudeInRadian)
			* sin(deltaLongitudeRadians/2)
			* sin(deltaLongitudeRadians/2); 			

	double b = 2 * atan2(sqrt(a), sqrt(1 - a));
	double distanceToWaypoint = radiusOfEarth * b * 1000;
	
	return distanceToWaypoint;
}