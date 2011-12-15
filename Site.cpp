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

#include "conf.h"

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
    sites[SITE_1].readyPoint.y = 115;
    sites[SITE_1].readyPoint.x = 190;
    sites[SITE_1].dotLeft.y = 160;
    sites[SITE_1].dotLeft.x = 202;
    sites[SITE_1].dotMid.y = 142;
    sites[SITE_1].dotMid.x = 220;
    sites[SITE_1].dotRight.y = 94;
    sites[SITE_1].dotRight.x = 220;

    sites[SITE_2].readyPoint.y = 50;
    sites[SITE_2].readyPoint.x = 189;
    sites[SITE_2].dotLeft.y = 20;
    sites[SITE_2].dotLeft.x = 229;
    sites[SITE_2].dotMid.y = 0;
    sites[SITE_2].dotMid.x = 184;
    sites[SITE_2].dotRight.y = 12;
    sites[SITE_2].dotRight.x = 129;

    sites[SITE_3].readyPoint.y = 53;
    sites[SITE_3].readyPoint.x = 24;
    sites[SITE_3].dotLeft.y = 0;
    sites[SITE_3].dotLeft.x = 23;
    sites[SITE_3].dotMid.y = 0;
    sites[SITE_3].dotMid.x = 0;
    sites[SITE_3].dotRight.y = 53;
    sites[SITE_3].dotRight.x = 0;

    sites[SITE_4].readyPoint.y = 172;
    sites[SITE_4].readyPoint.x = 43;
    sites[SITE_4].dotLeft.y = 260;
    sites[SITE_4].dotLeft.x = 49;
    sites[SITE_4].dotMid.y = 190;
    sites[SITE_4].dotMid.x = 84;
    sites[SITE_4].dotRight.y = 136;
    sites[SITE_4].dotRight.x = 63;

    sites[SITE_5].readyPoint.y = 221;
    sites[SITE_5].readyPoint.x = 181;
    sites[SITE_5].dotLeft.y = 186;
    sites[SITE_5].dotLeft.x = 150;
    sites[SITE_5].dotMid.y = 245;
    sites[SITE_5].dotMid.x = 150;
    sites[SITE_5].dotRight.y = 245;
    sites[SITE_5].dotRight.x = 208;
}
