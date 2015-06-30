#include "CourseCalculation.h"
#include <math.h>
#include "CourseMath.h"


CourseCalculation::CourseCalculation() {
	m_PREVIOUS_ITERATION_TACK = false;
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
		m_GOING_STARBOARD = false;
	}
	else
	{
		courseToSteer = starboardCTS;
		m_GOING_STARBOARD = true;
	}

	return courseToSteer;
}

bool CourseCalculation::continuePort()
{
	bool continueDirection = false;
	double sectorBegin = m_TWD - m_TACK_ANGLE;
	double sectorEnd = m_TWD + m_SECTOR_ANGLE;

	if (m_courseMath.isAngleInSector(m_bearingToWaypoint, sectorBegin, sectorEnd))
		continueDirection = true;

	return continueDirection;
}

bool CourseCalculation::continueStarboard()
{
	bool continueDirection = false;
	double sectorBegin = m_TWD + m_TACK_ANGLE;
	double sectorEnd = m_TWD - m_SECTOR_ANGLE;

	if (m_courseMath.isAngleInSector(m_bearingToWaypoint, sectorBegin, sectorEnd))
		continueDirection = true;

	return continueDirection;
}

double CourseCalculation::calculateStarboardCTS()
{
	return m_courseMath.limitAngleRange(m_TWD + m_TACK_ANGLE);
}

double CourseCalculation::calculatePortCTS()
{
	return m_courseMath.limitAngleRange(m_TWD - m_TACK_ANGLE);
}

void CourseCalculation::calculateCTS(PositionModel boat, PositionModel waypoint)
{
	m_distanceToWaypoint = m_courseMath.calculateDTW(boat, waypoint);
	m_bearingToWaypoint = m_courseMath.calculateBTW(boat, waypoint);

	double cts = 0;
	calculateTack();

	if (m_TACK == true) {
		// We are going against the wind direction
		if (m_PREVIOUS_ITERATION_TACK == false) {
			// We just ended up in tack position
			cts = determineFirstCTS();
		} 
		else {
			// We are still in tack position
			if (m_GOING_STARBOARD) {
				// Wind is coming from port but we are going in starboard direction relative to the wind
				if (!continueStarboard()) {
					// CTS should switch to port side
					m_GOING_STARBOARD = false;
					cts = calculatePortCTS();
				} else {
					// CTS should remain about the same
					cts = calculateStarboardCTS();
				}
			}
			else if (!m_GOING_STARBOARD) {
				// Wind is coming from starboard but we are going in port direction relative to the wind
				if (!continuePort()) {
					// CTS should switch to starboard side
					m_GOING_STARBOARD = true;
					cts = calculateStarboardCTS();
				} else {
					// CTS should remain about the same
					cts = calculatePortCTS();
				}
			}
		}
	}
	else {
		// We are not going against the wind direction
		cts = m_bearingToWaypoint;
	}

	m_CTS = cts;
	m_PREVIOUS_ITERATION_TACK = m_TACK;

}

bool CourseCalculation::calculateTack()
{
	double minTackAngle = m_TWD - m_TACK_ANGLE;
	double maxTackAngle = m_TWD + m_TACK_ANGLE;
	m_TACK = m_courseMath.isAngleInSector(m_bearingToWaypoint, minTackAngle, maxTackAngle);
	return m_TACK;
}


void CourseCalculation::setTWD(double degrees) {
	m_TWD = degrees;
}

void CourseCalculation::setBTW(double degrees) {
	m_bearingToWaypoint = degrees;
}

void CourseCalculation::setTACK_ANGLE(double degrees) {
	m_TACK_ANGLE = degrees;
}

void CourseCalculation::setSECTOR_ANGLE(double degrees) {
	m_SECTOR_ANGLE = degrees;
}

double CourseCalculation::getCTS() {
	return m_CTS;
}

double CourseCalculation::getBTW() {
	return m_bearingToWaypoint;
}

double CourseCalculation::getDTW() {
	return m_distanceToWaypoint;
}

double CourseCalculation::getTWD() {
	return m_TWD;
}

bool CourseCalculation::getTACK() {
	return m_TACK;
}

bool CourseCalculation::getGOING_STARBOARD() {
	return m_GOING_STARBOARD;
}

