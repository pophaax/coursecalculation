#include "CourseCalculation.h"
#include <math.h>
#include "CourseMath.h"


CourseCalculation::CourseCalculation() {
	m_previousIterationTack = false;
	m_distanceToWaypoint = 100000.0;
}

CourseCalculation::~CourseCalculation() {
}

void CourseCalculation::determineTackDirection()
{
	double sectorBegin = m_trueWindDirection;
	double sectorEnd = m_trueWindDirection + m_tackAngle;
	if (m_courseMath.isAngleInSector(m_bearingToWaypoint, sectorBegin, sectorEnd))
		m_goingStarboard = true;

	sectorBegin = m_trueWindDirection;
	sectorEnd = m_trueWindDirection - m_tackAngle;
	if (m_courseMath.isAngleInSector(m_bearingToWaypoint, sectorBegin, sectorEnd))
		m_goingStarboard = false;
}

bool CourseCalculation::continueDirection(const double waypointRadius) const
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
		double distance = distanceFromWaypointToTackSector(waypointRadius);
		if (m_distanceToWaypoint < distance)
			continueDirection = true;
	}

	return continueDirection;
}

double CourseCalculation::distanceFromWaypointToTackSector(const double waypointRadius) const
{
	double angleBetweenBTWAndTS =
		m_courseMath.angleDifference(m_trueWindDirection, m_bearingToWaypoint) - m_sectorAngle;

	return waypointRadius / sin(m_courseMath.degreeToRadian(angleBetweenBTWAndTS));
}

double CourseCalculation::calculateTackCTS() const
{
	int directionMult = 1;
	if (m_goingStarboard)
		directionMult = -1;

	return m_courseMath.limitAngleRange(m_trueWindDirection - (directionMult * m_tackAngle));
}

void CourseCalculation::calculateCourseToSteer(PositionModel boat, WaypointModel waypoint)
{
	m_distanceToWaypoint = m_courseMath.calculateDTW(boat, waypoint.positionModel);
	m_bearingToWaypoint = m_courseMath.calculateBTW(boat, waypoint.positionModel);

	calculateTack();

	if (m_tack && !m_previousIterationTack)
		determineTackDirection();

	if(m_tack)
	{
		if (!continueDirection(waypoint.radius))
			m_goingStarboard = !m_goingStarboard;

		m_courseToSteer = calculateTackCTS();
	}
	else
	{
		m_courseToSteer = m_bearingToWaypoint;
	}

	m_previousIterationTack = m_tack;
}

void CourseCalculation::calculateTack()
{
	double minTackAngle = m_trueWindDirection - m_tackAngle;
	double maxTackAngle = m_trueWindDirection + m_tackAngle;
	m_tack = m_courseMath.isAngleInSector(m_bearingToWaypoint, minTackAngle, maxTackAngle);
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

