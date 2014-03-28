/* Author: rlunden */

#include <iostream>
#include <math.h>

using namespace std;

/**---------------------------------------
* 	USAGE:
* 
* 	RudderCalculation rc;
* 
* 	rc.setTACK_ANGLE(45);
* 	rc.setSECTOR_ANGLE(5);
* 	rc.setTWD(true_wind_direction);
* 
* 	rc.calculateBWP(boatLat,boatLong,waypointLat, waypointLong);
* 	rc.calculateDWP(boatLat,boatLong,waypointLat, waypointLong);
* 	rc.calculateTACK();
* 	rc.calculateCTS();
*----------------------------------------*/

class RudderCalculation {
 
private:
  /* Course to steer */
  double m_CTS;
  
  /* Bearing to waypoint */
  double m_BWP;
  
  /* Distance to waypoint */
  double m_DWP;
  
  /* True wind direction */
  double m_TWD;
  
  /* Has to tack */
  bool m_TACK;
  
  /* Angle around true wind direction in which the boat has to tack */
  double m_TACK_ANGLE;
  
  /* Angle around true wind direction in which the boat will be turning side when tacking */
  double m_SECTOR_ANGLE;
  
  /* These member variables do not have getters and will only be used inside the class */
  /* -------------------------- */
  double m_deltaLatitudeRadians;
  double m_deltaLongitudeRadians;
  double m_latitudeRadians;
  double m_wpLatitudeRadians;
  double m_longitudeRadians;
  double m_wpLongitudeRadians;
  bool m_GOING_STARBOARD;
  
  /* Did we tack last iteration? */
  bool m_PREVIOUS_ITERATION_TACK;
  /* -------------------------- */
  
  /* These methods will only be used inside the class */
  /* -------------------------- */
  /* Calculates if the boat has to tack, which it needs if bearing to waypoint is close to true wind direction */
  bool calculateTACK();
  void radianConversion(double  boatLat, double boatLong, double wpLat, double wpLong);
  int countUp();
  int countDown();
  bool calcUp();
  bool calcDown();
  int determineFirstCTS();
  bool continueStarboard();
  bool continuePort();
  double calculateStarboardCTS();
  double calculatePortCTS();
  /* -------------------------- */
  
public:
  
  /* Constructor */
  RudderCalculation();
  
  /* Destructor */
  ~RudderCalculation();
  
  /* Calculates the bearing to waypoint angle using the boat's coordinates and the waypoint's coordinates */
  void calculateBWP(double  boatLong, double boatLat, double wpLong, double wpLat);
  
  /* Calculates the distance in meters to waypoint using the boat's coordinates and the waypoint's coordinates */
  void calculateDWP(double  boatLat, double boatLong, double wpLat, double wpLong);
  
  /* Calculates course to steer which is the direction the boat is supposed to sail */
  void calculateCTS();
  
  /* Sets true wind direction */
  void setTWD(double degrees);
  
  /* Sets bearing to waypoint */
  void setBWP(double degrees);
  
  /* Sets TACK_ANGLE */
  void setTACK_ANGLE(double degrees);
  
  /* Sets SECTOR_ANGLE */
  void setSECTOR_ANGLE(double degrees);
  
  /* Gets the distance to waypoint */
  double getDWP();

  /* Gets course to steer */
  double getCTS();
  
  /* Gets bearing to waypoint */
  double getBWP();
  
  /* Gets true wind direction */
  double getTWD();
  
  /* Gets wheter the boat has to tack */
  bool getTACK();
  
};
