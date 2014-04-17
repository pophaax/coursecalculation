#ifndef __COURSECALCULATION_H__
#define __COURSECALCULATION_H__

#include <math.h>


class CourseCalculation {

private:

	/* Has to tack */
	bool m_TACK;

	/* Did we tack last iteration? */
	bool m_PREVIOUS_ITERATION_TACK;

	/* Check for the boat's direction*/
	bool m_GOING_STARBOARD;

	/* Course to steer */
	double m_CTS;

	/* Bearing to waypoint */
	double m_bearingToWaypoint;

	/* Distance to waypoint */
	double m_distanceToWaypoint;

	/* True wind direction */
	double m_TWD;

	/* Angle around true wind direction in which the boat has to tack */
	double m_TACK_ANGLE;

	/* Angle around true wind direction in which the boat will be turning side when tacking */
	double m_SECTOR_ANGLE;
		
	/* Difference between waypoint latitude and boat latitude turned into radians */
	double m_deltaLatitudeRadians;

	/* Difference between waypoint longitude and boat longitude turned into radians */
	double m_deltaLongitudeRadians;

	/* The boat's latitude value in radian */
	double m_boatLatitudeInRadian;

	/* The waypoint's latitude value in radian */
	double m_waypointLatitudeInRadian;
	

	/* Converts the longitude/latitude decimal values into radians */
	void decimalToRadian(double boatLongitude, double boatLatitude, double waypointLongitude, double waypointLatitude);

	/* Calculates if the boat has to tack, which it needs if bearing to waypoint is close to true wind direction */
	bool calculateTACK();
	
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
	void calculateBTW(double boatLong, double boatLat, double wpLong, double wpLat);

	/* Calculates the distance in meters to waypoint using the boat's coordinates and the waypoint's coordinates */
	void calculateDTW(double boatLat, double boatLong, double wpLat, double wpLong);

	/* Calculates course to steer which is the direction the boat is supposed to sail */
	void calculateCTS();

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

};

#endif