/*
 * File:   Localization.h
 * Author: daniel
 *
 * Created on December 3, 2011, 9:43 PM
 */

#ifndef LOCALIZATION_H
#define	LOCALIZATION_H

#include "debug_tools.h"
#include "Pose.h"
#include "ServoController.h"
#include "SensorController.h"
#include "Action.h"
#include <fstream>
#include "Site.h"

class Localization {

public:
    Localization();
    ~Localization();

    void updateSiteStatus();

    void initializePosition(double x, double y, double theta);

    void turnToFaceResourceSite();

    void takeMeasurements();

    void updateParticle();

    void sonarScan(void);

    static void readSensorData(int sensorValue);

    void moveToResourceSite();
    //Pose pose(0,0,0);

private:
    int timeToTravelToResourceSite;
    double sonarDistanceToSite;
    double predictedDistanceToResourceSite;


    double distanceToSite;
    double angleToSite;

};

bool sonarScanSite(void);
bool irScanSite(void);

#endif	/* LOCALIZATION_H */

