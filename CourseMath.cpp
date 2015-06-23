#include <math.h>
#include "CourseMath.h"

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
	//const double piRadDegree = 180.0;
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