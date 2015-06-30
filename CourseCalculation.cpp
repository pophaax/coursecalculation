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

void CourseCalculation::calculateCTS(PositionModel boat, PositionModel waypoint)
{
	m_distanceToWaypoint = m_courseMath.calculateDTW(boat, waypoint);
	m_bearingToWaypoint = m_courseMath.calculateBTW(boat, waypoint);

	double cts = 0;
	calculateTack();

	if (m_tack && !m_previousIterationTack) {
		// We just ended up in tack position
		cts = determineFirstCTS();
	}
	else if(m_tack)
	{
		// We are still in tack position
		if (m_goingStarboard) {
			// Wind is coming from port but we are going in starboard direction relative to the wind
			if (!continueStarboard()) {
				// CTS should switch to port side
				m_goingStarboard = false;
				cts = calculatePortCTS();
			} else {
				// CTS should remain about the same
				cts = calculateStarboardCTS();
			}
		}
		else if (!m_goingStarboard) {
			// Wind is coming from starboard but we are going in port direction relative to the wind
			if (!continuePort()) {
				// CTS should switch to starboard side
				m_goingStarboard = true;
				cts = calculateStarboardCTS();
			} else {
				// CTS should remain about the same
				cts = calculatePortCTS();
			}
		}
	}
	else {
		// We are not going against the wind direction
		cts = m_bearingToWaypoint;
	}

	m_courseToSteer = cts;
	m_previousIterationTack = m_tack;

}

bool CourseCalculation::calculateTack()
{
	double minTackAngle = m_trueWindDirection - m_tackAngle;
	double maxTackAngle = m_trueWindDirection + m_tackAngle;
	m_tack = m_courseMath.isAngleInSector(m_bearingToWaypoint, minTackAngle, maxTackAngle);
	return m_tack;
}


void CourseCalculation::setTWD(double degrees) {
	m_trueWindDirection = degrees;
}

void CourseCalculation::setBTW(double degrees) {
	m_bearingToWaypoint = degrees;
}

void CourseCalculation::setTACK_ANGLE(double degrees) {
	m_tackAngle = degrees;
}

void CourseCalculation::setSECTOR_ANGLE(double degrees) {
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

bool CourseCalculation::getTACK() {
	return m_tack;
}

bool CourseCalculation::getGOING_STARBOARD() {
	return m_goingStarboard;
}

