#ifndef __COURSECALCULATION_H__
#define __COURSECALCULATION_H__

#include "../models/PositionModel.h"
#include "CourseMath.h"

class CourseCalculation {

private:

	/* Has to tack */
	bool m_tack;

	/* Did we tack last iteration? */
	bool m_previousIterationTack;

	/* Check for the boat's direction*/
	bool m_goingStarboard;

	/* Course to steer */
	double m_courseToSteer;

	/* Bearing to waypoint */
	double m_bearingToWaypoint;

	/* Distance to waypoint */
	double m_distanceToWaypoint;

	/* True wind direction */
	double m_trueWindDirection;

	/* Angle around true wind direction in which the boat has to tack */
	double m_tackAngle;

	/* Angle around true wind direction in which the boat will be turning side when tacking */
	double m_sectorAngle;
		
	/* Difference between waypoint latitude and boat latitude turned into radians */
	double m_deltaLatitudeRadians;

	/* Difference between waypoint longitude and boat longitude turned into radians */
	double m_deltaLongitudeRadians;

	/* The boat's latitude value in radian */
	double m_boatLatitudeInRadian;

	/* The waypoint's latitude value in radian */
	double m_waypointLatitudeInRadian;
	
	CourseMath m_courseMath;

	/* Converts the longitude/latitude decimal values into radians */
//	void decimalToRadian(double boatLongitude, double boatLatitude, double waypointLongitude, double waypointLatitude);

	/* Calculates if the boat has to tack, which it needs if bearing to waypoint is close to true wind direction */
	bool calculateTack();
	
	/* */
	bool calcUp();

	/* */
	bool calcDown();

	/* */
	bool continueStarboard();

	/* */
	bool continuePort();

	/* */
	int countUp();

	/* */
	int countDown();

	/* */
	int determineFirstCTS();
	
	/* */
	double calculateStarboardCTS();

	/* */
	double calculatePortCTS();

public:

	/* Constructor */
	CourseCalculation();

	/* Destructor */
	~CourseCalculation();

	/* Calculates the bearing to waypoint angle using the boat's coordinates and the waypoint's coordinates */
//	void calculateBTW(double boatLat, double boatLong, double wpLat, double wpLong);

	/* Calculates the distance in meters to waypoint using the boat's coordinates and the waypoint's coordinates */
//	void calculateDTW(double boatLat, double boatLong, double wpLat, double wpLong);

	/* Calculates course to steer which is the direction the boat is supposed to sail */
	void calculateCTS(PositionModel boat, PositionModel waypoint);

	/* Sets true wind direction */
	void setTWD(double degrees);

	/* Sets bearing to waypoint */
	void setBTW(double degrees);

	/* Sets TACK_ANGLE */
	void setTACK_ANGLE(double degrees);

	/* Sets SECTOR_ANGLE */
	void setSECTOR_ANGLE(double degrees);

	/* Gets the distance to waypoint */
	double getDTW();

	/* Gets course to steer */
	double getCTS();

	/* Gets bearing to waypoint */
	double getBTW();

	/* Gets true wind direction */
	double getTWD();

	/* Gets wheter the boat has to tack */
	bool getTACK();

	bool getGOING_STARBOARD();
};

#endif