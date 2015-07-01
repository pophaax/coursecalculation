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

bool CourseCalculation::continuePort()
{
	bool continueDirection = false;
	double sectorBegin = m_trueWindDirection - m_tackAngle;
	double sectorEnd = m_trueWindDirection + m_sectorAngle;

	if (m_courseMath.isAngleInSector(m_bearingToWaypoint, sectorBegin, sectorEnd))
		continueDirection = true;

	return continueDirection;
}

bool CourseCalculation::continueStarboard()
{
	bool continueDirection = false;
	double sectorBegin = m_trueWindDirection + m_tackAngle;
	double sectorEnd = m_trueWindDirection - m_sectorAngle;

	if (m_courseMath.isAngleInSector(m_bearingToWaypoint, sectorBegin, sectorEnd))
		continueDirection = true;

	return continueDirection;
}

double CourseCalculation::calculateStarboardCTS()
{
	return m_courseMath.limitAngleRange(m_trueWindDirection + m_tackAngle);
}

double CourseCalculation::calculatePortCTS()
{
	return m_courseMath.limitAngleRange(m_trueWindDirection - m_tackAngle);
}

void CourseCalculation::calculateCourseToSteer(PositionModel boat, PositionModel waypoint)
{
	m_distanceToWaypoint = m_courseMath.calculateDTW(boat, waypoint);
	m_bearingToWaypoint = m_courseMath.calculateBTW(boat, waypoint);

	calculateTack();

	if (m_tack && !m_previousIterationTack) {
		// We just ended up in tack position
		m_courseToSteer = determineFirstCTS();
	}
	else if(m_tack)
	{
		if (m_goingStarboard) {
			if (!continueStarboard()) {
				m_goingStarboard = false;
				m_courseToSteer = calculatePortCTS();
			} else {
				m_courseToSteer = calculateStarboardCTS();
			}
		}
		else if (!m_goingStarboard) {
			if (!continuePort()) {
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

