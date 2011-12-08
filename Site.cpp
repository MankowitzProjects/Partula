#include "Site.h"

Site sites[NUM_SITE];

/** \brief The default constructor
 *
 * \param void
 *
 */
Feature::Feature(void)
{
    lenLeft  = 0.0;
    lenRight = 0.0;
    rad      = 0.0;
}

/** \brief the copy constructor.
 *         construct the feature by giving another feature.
 *
 * \param wp const Feature&
 *
 */
Feature::Feature(const Feature &feature)
{
    this->lenLeft  = feature.lenLeft;
    this->lenRight = feature.lenRight;
    this->rad      = feature.rad;
}

/** \brief set the values
 *
 * \param lenLeft double
 * \param lenRight double
 * \param rad double
 * \return void
 *
 */
void Feature::set(double lenLeft, double lenRight, double rad)
{
    this->lenLeft  = lenLeft;
    this->lenRight = lenRight;
    this->rad      = rad;
}

/** \brief reloaded operator =
 *
 * \param feature const Feature&
 * \return const Feature&
 *
 */
const Feature &Feature::operator=(const Feature &feature)
{
    this->lenLeft  = feature.lenLeft;
    this->lenRight = feature.lenRight;
    this->rad      = feature.rad;

    return *this;
}

/** \brief reloaded operator ==
 *
 * \param feature const Feature&
 * \return bool
 *
 */
bool Feature::operator==(const Feature &feature) const
{
    return (   (abs(this->lenLeft  - feature.lenLeft)  < DIV_SITE_LENGTH)
            && (abs(this->lenRight - feature.lenRight) < DIV_SITE_LENGTH)
            && (abs(this->rad      - feature.rad)      < DIV_SITE_RADIAN));
}

/** \brief reloaded operator !=
 *
 * \param wp const WayPoint&
 * \return bool
 *
 */
bool Feature::operator!=(const Feature &feature) const
{
    return !(*this == feature);
}

/** \brief default constructor
 *
 * \param void
 *
 */
Site::Site(void)
{
    init(0);
}

/** \brief init a site
 *
 * \param idSite int
 * \return void
 *
 */
void Site::init(int idSite)
{
    this->idSite = idSite;
    bVisited = false;
}

/** \brief finalize a site
 *
 * \param void
 * \return void
 *
 */
void Site::fin(void)
{
    ;
}

/** \brief test if the site is visited
 *
 * \param void
 * \return bool
 *
 */
bool Site::isVisited(void)
{
    return bVisited;
}

/** \brief set the site as visited
 *
 * \param void
 * \return void
 *
 */
void Site::setVisited(void)
{
    bVisited = true;
}

/** \brief Calculate the radian of right hand side angle and the edge in front.
 *
 * \param lenFront double&
 * \param radRight double&
 * \param lenLeft const double&
 * \param rad const double&
 * \param lenRight const double&
 * \return void
 *
 */
inline void calRadLen(double &lenFront, double &radRight, const double &lenLeft, const double &rad, const double &lenRight)
{
    double BE = 0.0;

    BE = sin(rad)*lenLeft;

    radRight = atan2(BE, (lenRight - cos(rad)*lenLeft));

    lenFront = BE/sin(radRight);
}

/** \brief Calculate the feature of a site from the measurements
 *
 * \param feature Feature&
 * \param measure const SITE_MEASUREMENT&
 * \return void
 *
 */
void calFeatures(Feature &feature, const SITE_MEASUREMENT &measure)
{
    double thetaOne;

    calRadLen(feature.lenLeft, thetaOne, measure.rngLeft, measure.radLeft, measure.rngMid);
    calRadLen(feature.lenRight, feature.rad, measure.rngRight, measure.radRight, measure.rngMid);

    feature.rad += thetaOne;
}



/** \brief initialize the sites
 *
 * \param void
 * \return void
 *
 */
void initSites(void)
{
    for (int i = 0; i < NUM_SITE; ++i)
    {
        sites[i].init(i);
    }

    // add information
    sites[1].readyPoint.x = 53;
    sites[1].readyPoint.y = 24;
    sites[1].dotLeft.x = 0;
    sites[1].dotLeft.y = 24;
    sites[1].dotMid.x = 0;
    sites[1].dotMid.y = 0;
    sites[1].dotRight.x = 53;
    sites[1].dotRight.y = 0;
    
    sites[2].readyPoint.x = 50;
    sites[2].readyPoint.y = 189;
    sites[2].dotLeft.x = 20;
    sites[2].dotLeft.y = 239;
    sites[2].dotMid.x = 0;
    sites[2].dotMid.y = 184;
    sites[2].dotRight.x = 12;
    sites[2].dotRight.y = 129;
    
    sites[3].readyPoint.x = 115;
    sites[3].readyPoint.y = 190;
    sites[3].dotLeft.x = 160;
    sites[3].dotLeft.y = 202;
    sites[3].dotMid.x = 142;
    sites[3].dotMid.y = 220;
    sites[3].dotRight.x = 94;
    sites[3].dotRight.y = 220;
    
    sites[4].readyPoint.x = 221;
    sites[4].readyPoint.y = 181;
    sites[4].dotLeft.x = 186;
    sites[4].dotLeft.y = 150;
    sites[4].dotMid.x = 0;
    sites[4].dotMid.y = 0;
    sites[4].dotRight.x = 245;
    sites[4].dotRight.y = 208;
    
    sites[5].readyPoint.x = 172;
    sites[5].readyPoint.y = 43;
    sites[5].dotLeft.x = 260;
    sites[5].dotLeft.y = 49;
    sites[5].dotMid.x = 190;
    sites[5].dotMid.y = 84;
    sites[5].dotRight.x = 136;
    sites[5].dotRight.y = 63;

}
