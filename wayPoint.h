#ifndef WAYPOINT_H_INCLUDED
#define WAYPOINT_H_INCLUDED

#include "math_ext.h"

#include "priList.h"

class WayPoint
{
public:

    int x;  /**< the x coordinate */
    int y;  /**< the y coordinate */

    WayPoint *parent;   /**< point to it's parent */
    list_head node;     /**< use the uiNum as f(n) */

    /** \brief default constructor
     *
     * \param void
     *
     */
    WayPoint(void)
    {
        init(0, 0);
    }

    /** \brief construct the way point by giving coordinates
     *
     * \param x int
     * \param y int
     *
     */
    WayPoint(int x, int y)
    {
        init(x, y);
    }

    /** \brief initialse the way point by giving coordinates
     *
     * \param x int
     * \param y int
     * \return void
     *
     */
    void init(int x, int y)
    {
        this->x = x;
        this->y = y;
        parent = this;
        INIT_LIST_HEAD(&node);
    }

    /** \brief construct the way point by giving another way point
     *
     * \param wp const WayPoint&
     *
     */
    WayPoint(const WayPoint &wp)
    {
        this->x = wp.x;
        this->y = wp.y;
        this->parent = wp.parent;
        this->node   = wp.node;
    }

    /** \brief reloaded operator =
     *
     * \param wp const WayPoint&
     * \return const WayPoint&
     *
     */
    const WayPoint& operator=(const WayPoint &wp)
    {
        this->x = wp.x;
        this->y = wp.y;
        this->parent = wp.parent;
        this->node = wp.node;

        return *this;
    }

    /** \brief reloaded operator ==
     *
     * \param wp const WayPoint&
     * \return bool
     *
     */
    bool operator==(const WayPoint &wp) const
    {
        return ((this->x == wp.x) && (this->y == wp.y));
    }

    /** \brief reloaded operator !=
     *
     * \param wp const WayPoint&
     * \return bool
     *
     */
    bool operator!=(const WayPoint &wp) const
    {
        return ((this->x != wp.x) || (this->y != wp.y));
    }

    /** \brief test if two points are connected
     *
     * \param wp const WayPoint&
     * \return bool
     *
     */
    bool isConnected(const WayPoint &wp)
    {
        return (   ((abs(this->x - wp.x) <= 1) && (abs(this->y - wp.y) <= 1))
                && (!((this->x != wp.x) && (this->y != wp.y))));
    }
};

#endif // WAYPOINT_H_INCLUDED
