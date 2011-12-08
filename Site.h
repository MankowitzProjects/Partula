#ifndef SITE_H_INCLUDED
#define SITE_H_INCLUDED

#include "wayPoint.h"

#define DIV_SITE_LENGTH 0.3
#define DIV_SITE_RADIAN 0.1

#define NUM_SITE 5

typedef struct
{
    double rngLeft;     /**< the range to the left edge */
    double rngMid;      /**< the range to the centre corner */
    double rngRight;    /**< the range to the right edge */

    double radLeft;     /**< the radian between rngLeft and rngMid */
    double radRight;    /**< the radian between rngMid and rngRight */

} SITE_MEASUREMENT;

class Feature
{
public:

    void set(double lenLeft, double lenRight, double rad);

    Feature(void);
    Feature(const Feature &feature);
    const Feature& operator=(const Feature &feature);
    bool operator==(const Feature &feature) const;
    bool operator!=(const Feature &feature) const;

    double lenLeft;     /**< the length of the left wall */
    double rad;         /**< the radian of the corner */
    double lenRight;    /**< the length of the right wall */
};


class Site
{
public:
    Site(void);
    void init(int idSite);
    void fin(void);

    bool isVisited(void);
    void setVisited(void);

    int      idSite;    /**< The ID of the site */

    bool     bVisited;  /**< The visiting state */
    float    freq;      /**< The frequency of the site */
    Feature  feature;   /**< The feature of the site */

    WayPoint dotLeft;   /**< the coordinates of left edge */
    WayPoint dotMid;    /**< the coordinates of middle corner */
    WayPoint dotRight;  /**< the coordinates of right edge */
    WayPoint readyPoint;
};


class ShapeDectector
{
    void signal(float length, float radian);
    //bool bGetFeature(Feature )
};

extern Site sites[NUM_SITE];

/** \brief Calculate the feature of a site from the measurements
 *
 * \param feature Feature&
 * \param measure const SITE_MEASUREMENT&
 * \return void
 *
 */
void calFeatures(Feature &feature, const SITE_MEASUREMENT &measure);

void initSites(void);

#endif // SITE_H_INCLUDED
