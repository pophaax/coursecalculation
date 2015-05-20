#include "CourseCalculation.h"
#include <math.h>

const double PI = 3.1415926;
const double radiusOfEarth = 6371.0;
const double radDegree = 180.0;

CourseCalculation::CourseCalculation() {
	m_PREVIOUS_ITERATION_TACK = false;
}

CourseCalculation::~CourseCalculation() {
}

void CourseCalculation::decimalToRadian(double boatLat, double boatLong, double wpLat, double wpLong) {

	m_deltaLatitudeRadians 	= (wpLat-boatLat)*PI/radDegree;
	m_deltaLongitudeRadians = (wpLong-boatLong)*PI/radDegree;
	m_boatLatitudeInRadian 	= boatLat*PI/radDegree;
	m_waypointLatitudeInRadian = wpLat*PI/radDegree;
}

void CourseCalculation::calculateBTW(double boatLat, double boatLong, double wpLat, double wpLong) {

	decimalToRadian(boatLat, boatLong, wpLat, wpLong);

		double y_coordinate = sin(m_deltaLongitudeRadians)
							* cos(m_waypointLatitudeInRadian);

		double x_coordinate = cos(m_boatLatitudeInRadian)
							* sin(m_waypointLatitudeInRadian)
							- sin(m_boatLatitudeInRadian)
							* cos(m_waypointLatitudeInRadian)
							* cos(m_deltaLongitudeRadians);

		double bearingToWaypoint = atan2(y_coordinate, x_coordinate) / PI * 180;

		if (bearingToWaypoint < 0) {
			bearingToWaypoint = 360 + bearingToWaypoint;
		}

	this->m_bearingToWaypoint = bearingToWaypoint;
}

void CourseCalculation::calculateDTW(double boatLat, double boatLong, double wpLat, double wpLong) {

	decimalToRadian(boatLat, boatLong, wpLat, wpLong);

		double a = sin(m_deltaLatitudeRadians/2)
				* sin(m_deltaLatitudeRadians/2)
				+ cos(m_boatLatitudeInRadian)
				* cos(m_waypointLatitudeInRadian)
				* sin(m_deltaLongitudeRadians/2)
				* sin(m_deltaLongitudeRadians/2); 			

		double b = 2 * atan2(sqrt(a), sqrt(1 - a));
		double distanceToWaypoint = radiusOfEarth * b * 1000;
	
	this->m_distanceToWaypoint = distanceToWaypoint;
}

int CourseCalculation::determineFirstCTS() {

	int courseToSteer = 0;
	int port = countDown();
	int starboard = countUp();
		if (port > starboard) {
			courseToSteer = calculatePortCTS();
			m_GOING_STARBOARD = false;
		} 
		else if (starboard >= port) {
			courseToSteer = calculateStarboardCTS();
			m_GOING_STARBOARD = true;
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

void CourseCalculation::calculateCTS() {

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

bool CourseCalculation::calcUp() {

	int tmp_TWD = m_TWD;
	int tmp_BWP = round(m_bearingToWaypoint);
	bool tack = false;

	for (int i = 0; i < m_TACK_ANGLE; i++) {

		if (tmp_TWD == tmp_BWP) {
			tack = true;
		}

		tmp_TWD++;

		if (tmp_TWD == 360) {
			tmp_TWD = 0;
		}

	}

	return tack;
}

bool CourseCalculation::calcDown() {

	int tmp_TWD = m_TWD;
	int tmp_BWP = round(m_bearingToWaypoint);
	bool tack = false;

	for (int i = 0; i < m_TACK_ANGLE; i++) {

		if (tmp_TWD == tmp_BWP) {
			tack = true;
		}

		tmp_TWD--;

		if (tmp_TWD == -1) {
			tmp_TWD = 359;
		}

	}

	return tack;
}

bool CourseCalculation::calculateTACK() {

	if (calcUp() || calcDown()) {
		m_TACK = true;
	}

	else {
		m_TACK = false;
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
