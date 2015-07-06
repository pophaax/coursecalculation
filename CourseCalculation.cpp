#include "CourseCalculation.h"
#include <math.h>
#include "CourseMath.h"


CourseCalculation::CourseCalculation() {
	m_previousIterationTack = false;
	m_distanceToWaypoint = 100000.0;
}

CourseCalculation::~CourseCalculation() {
}

int CourseCalculation::determineFirstCTS()
{
	int courseToSteer = 0;
	double starboardCTS = calculateStarboardCTS();
	double portCTS = calculatePortCTS();

	double starboardBTWDiff = m_courseMath.angleDifference(starboardCTS, m_bearingToWaypoint);
	double portBTWDiff = m_courseMath.angleDifference(portCTS, m_bearingToWaypoint);

	if (portBTWDiff < starboardBTWDiff)
	{
		courseToSteer = portCTS;
		m_goingStarboard = false;
	}
	else
	{
		courseToSteer = starboardCTS;
		m_goingStarboard = true;
	}

	return courseToSteer;
}
/*
bool CourseCalculation::continueDirection(const double waypointRadius)
{
	bool continueDirection = false;
	int directionMult = 1;
	if (m_goingStarboard)
		directionMult = -1;

	double sectorBegin = m_trueWindDirection - (directionMult * m_tackAngle);
	double sectorEnd = m_trueWindDirection + (directionMult * m_sectorAngle);
	if (m_courseMath.isAngleInSector(m_bearingToWaypoint, sectorBegin, sectorEnd))
		continueDirection = true;

	sectorBegin = m_trueWindDirection + (directionMult * m_sectorAngle);
	sectorEnd = m_trueWindDirection + (directionMult * m_tackAngle);
	if (m_courseMath.isAngleInSector(m_bearingToWaypoint, sectorBegin, sectorEnd))
	{
		double distance = distanceFromWaypointToSector(waypointRadius);
		if (m_distanceToWaypoint < distance)
			continueDirection = true;
	}

	return continueDirection;
}
*/

bool CourseCalculation::continuePort(const double waypointRadius) const
{
	bool continueDirection = false;

	double sectorBegin = m_trueWindDirection - m_tackAngle;
	double sectorEnd = m_trueWindDirection + m_sectorAngle;
	if (m_courseMath.isAngleInSector(m_bearingToWaypoint, sectorBegin, sectorEnd))
		continueDirection = true;

	sectorBegin = m_trueWindDirection + m_sectorAngle;
	sectorEnd = m_trueWindDirection + m_tackAngle;
	if (m_courseMath.isAngleInSector(m_bearingToWaypoint, sectorBegin, sectorEnd))
	{
		double distance = distanceFromWaypointToSector(waypointRadius);
		if (m_distanceToWaypoint < distance)
			continueDirection = true;
	}

	return continueDirection;
}

bool CourseCalculation::continueStarboard(const double waypointRadius) const
{
	bool continueDirection = false;

	double sectorBegin = m_trueWindDirection + m_tackAngle;
	double sectorEnd = m_trueWindDirection - m_sectorAngle;
	if (m_courseMath.isAngleInSector(m_bearingToWaypoint, sectorBegin, sectorEnd))
		continueDirection = true;

	sectorBegin = m_trueWindDirection - m_sectorAngle;
	sectorEnd = m_trueWindDirection - m_tackAngle;
	if (m_courseMath.isAngleInSector(m_bearingToWaypoint, sectorBegin, sectorEnd))
	{
		double distance = distanceFromWaypointToSector(waypointRadius);
		if (m_distanceToWaypoint < distance)
			continueDirection = true;
	}

	return continueDirection;
}

double CourseCalculation::distanceFromWaypointToSector(const double waypointRadius) const
{
	double sectorOrigionToWaypoint = m_courseMath.lengthOfTriangleSide(
		90, waypointRadius, m_sectorAngle);

	double a = m_sectorAngle;
	double b = 180 - m_courseMath.angleDifference(m_bearingToWaypoint, m_trueWindDirection);
	double c = 180 - a - b;
	double intersectionToWP = m_courseMath.lengthOfTriangleSide(a, sectorOrigionToWaypoint, c);

	return intersectionToWP;
}

double CourseCalculation::calculateStarboardCTS()
{
	return m_courseMath.limitAngleRange(m_trueWindDirection + m_tackAngle);
}

double CourseCalculation::calculatePortCTS()
{
	return m_courseMath.limitAngleRange(m_trueWindDirection - m_tackAngle);
}

void CourseCalculation::calculateCourseToSteer(PositionModel boat, WaypointModel waypoint)
{
	m_distanceToWaypoint = m_courseMath.calculateDTW(boat, waypoint.positionModel);
	m_bearingToWaypoint = m_courseMath.calculateBTW(boat, waypoint.positionModel);

	calculateTack();

	if (m_tack && !m_previousIterationTack) {
		// We just ended up in tack position
		m_courseToSteer = determineFirstCTS();
	}
	else if(m_tack)
	{
		if (m_goingStarboard) {
			if (!continueStarboard(waypoint.radius)) {
				m_goingStarboard = false;
				m_courseToSteer = calculatePortCTS();
			} else {
				m_courseToSteer = calculateStarboardCTS();
			}
		}
		else if (!m_goingStarboard) {
			if (!continuePort(waypoint.radius)) {
				m_goingStarboard = true;
				m_courseToSteer = calculateStarboardCTS();
			} else {
				m_courseToSteer = calculatePortCTS();
			}
		}
	}
	else {
		// We are not going against the wind direction
		m_courseToSteer = m_bearingToWaypoint;
	}

	m_previousIterationTack = m_tack;
}

bool CourseCalculation::calculateTack()
{
	double minTackAngle = m_trueWindDirection - m_tackAngle;
	double maxTackAngle = m_trueWindDirection + m_tackAngle;
	m_tack = m_courseMath.isAngleInSector(m_bearingToWaypoint, minTackAngle, maxTackAngle);
	return m_tack;
}


void CourseCalculation::setTrueWindDirection(double degrees) {
	m_trueWindDirection = degrees;
}

void CourseCalculation::setBearingToWaypoint(double degrees) {
	m_bearingToWaypoint = degrees;
}

void CourseCalculation::setTackAngle(double degrees) {
	m_tackAngle = degrees;
}

void CourseCalculation::setSectorAngle(double degrees) {
	m_sectorAngle = degrees;
}

double CourseCalculation::getCTS() {
	return m_courseToSteer;
}

double CourseCalculation::getBTW() {
	return m_bearingToWaypoint;
}

double CourseCalculation::getDTW() {
	return m_distanceToWaypoint;
}

double CourseCalculation::getTWD() {
	return m_trueWindDirection;
}

bool CourseCalculation::getTack() {
	return m_tack;
}

bool CourseCalculation::getGoingStarboard() {
	return m_goingStarboard;
}

