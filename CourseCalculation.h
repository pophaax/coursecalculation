#ifndef __COURSECALCULATION_H__
#define __COURSECALCULATION_H__

#include "../models/PositionModel.h"
#include "../models/WaypointModel.h"
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
	
	CourseMath m_courseMath;

	/* Calculates if the boat has to tack, which it needs if bearing to waypoint is close to true wind direction */
	void calculateTack();

	double distanceFromWaypointToSector(const double waypointRadius) const;
	
	bool continueDirection(const double waypointRadius) const;

	void determineTackDirection();
	
	double calculateTackCTS() const;

public:

	/* Constructor */
	CourseCalculation();

	/* Destructor */
	~CourseCalculation();

	/* Calculates course to steer which is the direction the boat is supposed to sail */
	void calculateCourseToSteer(PositionModel boat, WaypointModel waypoint);

	/* Sets true wind direction */
	void setTrueWindDirection(double degrees);

	/* Sets bearing to waypoint */
	void setBearingToWaypoint(double degrees);

	/* Sets TACK_ANGLE */
	void setTackAngle(double degrees);

	/* Sets SECTOR_ANGLE */
	void setSectorAngle(double degrees);

	/* Gets the distance to waypoint */
	double getDTW();

	/* Gets course to steer */
	double getCTS();

	/* Gets bearing to waypoint */
	double getBTW();

	/* Gets true wind direction */
	double getTWD();

	/* Gets wheter the boat has to tack */
	bool getTack();

	bool getGoingStarboard();
};

#endif