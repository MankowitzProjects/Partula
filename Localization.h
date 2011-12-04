/* 
 * File:   Localization.h
 * Author: daniel
 *
 * Created on December 3, 2011, 9:43 PM
 */

#ifndef LOCALIZATION_H
#define	LOCALIZATION_H

class Localization {
public:
    Localization();
    Localization(const Localization& orig);
    virtual ~Localization();
    
    void initialisePosition();
    
    void EstimateResourceSiteLocations();
    
    void takeMeasurement();
    
    void updateParticles();
   
private:

};

#endif	/* LOCALIZATION_H */

