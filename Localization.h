/*
 * File:   Localization.h
 * Author: daniel
 *
 * Created on December 3, 2011, 9:43 PM
 */

#ifndef LOCALIZATION_H
#define	LOCALIZATION_H
#include "Pose.h"

class Localization {

public:
    Localization();
    ~Localization();

    void identifySite(FREQUENCY frequency);

    void initializePosition(double x, double y, double theta);

    void EstimateResourceSiteLocations();

    void takeMeasurement();

    void updateParticles();

    //static void* sonarThread(void* param);
    //Pose pose(0,0,0);

private:

};

#endif	/* LOCALIZATION_H */

