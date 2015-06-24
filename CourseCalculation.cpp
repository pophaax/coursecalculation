#include "CourseCalculation.h"
#include <math.h>
#include "CourseMath.h"

const double PI = 3.1415926;
const double radiusOfEarth = 6371.0;
const double radDegree = 180.0;

CourseCalculation::CourseCalculation() {
	m_PREVIOUS_ITERATION_TACK = false;
}

CourseCalculation::~CourseCalculation() {
}


int CourseCalculation::determineFirstCTS() {
 
	int courseToSteer = 0;
	int port = countDown();
	int starboard = countUp();
		if (port > starboard) {
			courseToSteer = calculateStarboardCTS();
			m_GOING_STARBOARD = true;
		} 
		else if (starboard >= port) {
			courseToSteer = calculatePortCTS();
			m_GOING_STARBOARD = false;
		}

	return courseToSteer;
}

bool CourseCalculation::continuePort() {

	int tmp_BWP = round(m_bearingToWaypoint);
	int tmp_TWD = m_TWD;
	int sectorBegin = tmp_TWD - m_TACK_ANGLE;
	bool continueDirection = false;

	if (sectorBegin < 0) {
		sectorBegin += 360;
	}

		for (int i = 0; i < (m_TACK_ANGLE + m_SECTOR_ANGLE); i++) {

			if (tmp_BWP == sectorBegin) {
				continueDirection = true;
				break;
			}

			sectorBegin++;
				if (sectorBegin == 360) {
					sectorBegin = 0;
				}
		}
	return continueDirection;
}

bool CourseCalculation::continueStarboard() {

	int tmp_BWP = round(m_bearingToWaypoint);
	int tmp_TWD = m_TWD;	
	int sectorBegin = tmp_TWD + m_TACK_ANGLE;
	bool continueDirection = false;

	if (sectorBegin >= 360) {
		sectorBegin -= 360;
	}

		for (int i = 0; i < (m_TACK_ANGLE + m_SECTOR_ANGLE); i++) {

			if (tmp_BWP == sectorBegin) {
				continueDirection = true;
				break;
			}

			sectorBegin--;
				if (sectorBegin == -1) {
					sectorBegin = 359;
				}
		}
	return continueDirection;
}

double CourseCalculation::calculateStarboardCTS() {

	double starboardCTS = 0;

		if (m_TWD + m_TACK_ANGLE > 360) {
			starboardCTS = m_TWD + m_TACK_ANGLE - 360;
		} else {
			starboardCTS = m_TWD + m_TACK_ANGLE;
		}
	return starboardCTS;
}

double CourseCalculation::calculatePortCTS() {

	double portCTS = 0;

		if (m_TWD - m_TACK_ANGLE < 0) {
			portCTS = m_TWD - m_TACK_ANGLE + 360;
		} else {
			portCTS = m_TWD - m_TACK_ANGLE;
		}
	return portCTS;
}

void CourseCalculation::calculateCTS(PositionModel boat, PositionModel waypoint) {

	this->m_distanceToWaypoint = m_courseMath.calculateDTW(boat, waypoint);
	this->m_bearingToWaypoint = m_courseMath.calculateBTW(boat, waypoint);

	double cts = 0;
	calculateTACK();

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

int CourseCalculation::countUp() {

	int count = 0;
	int tmp_BWP = round(m_bearingToWaypoint);
	int tmp_TWD = m_TWD;
	bool go = true;


	for (int i = 0; i < m_TACK_ANGLE && go == true; i++) {

		if (tmp_TWD == tmp_BWP) {
			go = false;
		}

		count++;
		tmp_TWD++;

		if (tmp_TWD == 360) {

			tmp_TWD = 0;
		}
	}

	return count;
}

int CourseCalculation::countDown() {

	int count = 0;
	int tmp_BWP = round(m_bearingToWaypoint);
	int tmp_TWD = m_TWD;
	bool go = true;

	for (int i = 0; i < m_TACK_ANGLE && go == true; i++) {

		if (tmp_TWD == tmp_BWP) {
			go = false;
		}

		count++;
		tmp_TWD--;

		if (tmp_TWD == -1) {

			tmp_TWD = 359;
		}
	}

	return count;
}


bool CourseCalculation::calculateTACK()
{
	m_TACK = false;
	double minWindAngle = m_courseMath.limitAngleRange(m_bearingToWaypoint - m_TACK_ANGLE);
	double maxWindAngle = m_courseMath.limitAngleRange(m_bearingToWaypoint + m_TACK_ANGLE);

	if (minWindAngle > maxWindAngle)
	{
		if (m_TWD > minWindAngle || m_TWD < maxWindAngle)
			m_TACK = true;
	}
	else if (m_TWD > minWindAngle && m_TWD < maxWindAngle)
	{
			m_TACK = true;
	}

	return m_TACK;
}

void CourseCalculation::setTWD(double degrees) {
	this->m_TWD = degrees;
}

void CourseCalculation::setBTW(double degrees) {
	this->m_bearingToWaypoint = degrees;
}

void CourseCalculation::setTACK_ANGLE(double degrees) {
	this->m_TACK_ANGLE = degrees;
}

void CourseCalculation::setSECTOR_ANGLE(double degrees) {
	this->m_SECTOR_ANGLE = degrees;
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

