#include "CourseCalculation.h"
#include <math.h>
#include "models/PositionModel.h"
#include "models/WaypointModel.h"
#include "utility/Utility.h"


CourseCalculation::CourseCalculation() :
	m_tack(false),
	m_goingStarboard(false)
{
	m_previousIterationTack = false;
	m_distanceToWaypoint = 100000.0;
}

CourseCalculation::~CourseCalculation() {
}

void CourseCalculation::determineTackDirection()
{
	double sectorBegin = m_trueWindDirection - m_tackAngle;
	double sectorEnd = m_trueWindDirection;
	if (Utility::isAngleInSector(m_bearingToWaypoint, sectorBegin, sectorEnd))
		m_goingStarboard = false;

	sectorBegin = m_trueWindDirection;
	sectorEnd = m_trueWindDirection + m_tackAngle;
	if (Utility::isAngleInSector(m_bearingToWaypoint, sectorBegin, sectorEnd))
		m_goingStarboard = true;
}

bool CourseCalculation::continueDirection(double waypointRadius) const
{
	bool continueDirection = false;

	if (m_goingStarboard)
	{
		double sectorBegin = m_trueWindDirection - m_sectorAngle;
		double sectorEnd = m_trueWindDirection + m_tackAngle;
		if (Utility::isAngleInSector(m_bearingToWaypoint, sectorBegin, sectorEnd))
			continueDirection = true;

		sectorBegin = m_trueWindDirection - m_tackAngle;
		sectorEnd = m_trueWindDirection - m_sectorAngle;
		if (Utility::isAngleInSector(m_bearingToWaypoint, sectorBegin, sectorEnd))
		{
			double distance = distanceFromWaypointToTackSector(waypointRadius);
			if (m_distanceToWaypoint < distance)
				continueDirection = true;
		}
	}
	else
	{
		double sectorBegin = m_trueWindDirection - m_tackAngle;
		double sectorEnd = m_trueWindDirection + m_sectorAngle;
		if (Utility::isAngleInSector(m_bearingToWaypoint, sectorBegin, sectorEnd))
			continueDirection = true;

		sectorBegin = m_trueWindDirection + m_sectorAngle;
		sectorEnd = m_trueWindDirection + m_tackAngle;
		if (Utility::isAngleInSector(m_bearingToWaypoint, sectorBegin, sectorEnd))
		{
			double distance = distanceFromWaypointToTackSector(waypointRadius);
			if (m_distanceToWaypoint < distance)
				continueDirection = true;
		}
	}

	return continueDirection;
}

double CourseCalculation::distanceFromWaypointToTackSector(double waypointRadius) const
{
	double angleBetweenBTWAndTS =
		Utility::angleDifference(m_trueWindDirection, m_bearingToWaypoint) - m_sectorAngle;

	return waypointRadius / sin(Utility::degreeToRadian(angleBetweenBTWAndTS));
}

double CourseCalculation::calculateTackCTS() const
{
	int directionMult = 1;
	if (m_goingStarboard)
		directionMult = -1;

	return Utility::limitAngleRange(m_trueWindDirection - (directionMult * m_tackAngle));
}

double CourseCalculation::calculateCourseToSteer(PositionModel boat, WaypointModel waypoint,
	double trueWindDirection)
{
	m_trueWindDirection = trueWindDirection;
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
	return m_courseToSteer;
}

void CourseCalculation::calculateTack()
{
	double minTackAngle = m_trueWindDirection - m_tackAngle;
	double maxTackAngle = m_trueWindDirection + m_tackAngle;
	m_tack = Utility::isAngleInSector(m_bearingToWaypoint, minTackAngle, maxTackAngle);
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

