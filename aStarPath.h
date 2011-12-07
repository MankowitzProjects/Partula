#ifndef ASTARPATH_H_INCLUDED
#define ASTARPATH_H_INCLUDED

#include <vector>
#include <math.h>

#include "wayPoint.h"
#include "map.h"

#include "priList.h"

#include "math_ext.h"

/** \brief Calculate the estimate distance from a point to goal point
 *
 * \param start const WayPoint&
 * \param goal const WayPoint&
 * \return int
 *
 */
inline int as_calHeuristic(const WayPoint &start, const WayPoint &goal)
{
    return int(dist(double(start.x), double(start.y), double(goal.x), double(goal.y)) * 10.0);
}

/** \brief Calculate g(n)
 *         g(n) represents the cost of the path from the starting point to any vertex n
 *         The distance here is 10 times lager, for we are using a trick in next function
 *
 * \param
 * \param
 * \return
 *
 */
inline int as_calGn(const WayPoint &start, const WayPoint &goal)
{
    return int(dist(double(start.x), double(start.y), double(goal.x), double(goal.y)) * 10.0);
}

/** \brief Calculate the movement cost of two points
 *         If two points are in the same horizon or vertical line,
 *         then the cost should be 1, if they are in diagonal, then
 *         it should be sqrt(2), to make it run faster, we just use
 *         10 and 14 to replace 1 and sqrt(2), respectively.
 *
 * \param
 * \param
 * \return
 *
 */
#if 0
inline int as_calMvCost(const int &startX, const int &startY,
                        const int &goalX,  const int &goalY)
{
    if ((startX == goalX) || (startY == goalY))
    {
        return 10;
    }
    else
    {
        return 14;
    }
}
#else
inline int as_calMvCost(const int &startX, const int &startY,
                        const int &goalX,  const int &goalY)
{
    return 10;
}
#endif

/** \brief Calculate the movement cost of two points
 *         If two points are in the same horizon or vertical line,
 *         then the cost should be 1, if they are in diagonal, then
 *         it should be sqrt(2), to make it run faster, we just use
 *         10 and 14 to replace 1 and sqrt(2), respectively.
 *
 * \param start const WayPoint&
 * \param goal const WayPoint&
 * \return int
 *
 */
inline int as_calMvCost(const WayPoint *start, const WayPoint *goal)
{
    return as_calMvCost(start->x, start->y, goal->x, goal->y);
}

/** \brief Get the entry of the given linked list node
 *
 * \param node list_head*
 * \return WayPoint*
 *
 */
inline WayPoint *as_getEntry(list_head *node)
{
    return list_entry(node, WayPoint, node);
}

/** \brief Clear the memory used by the linked list nodes
 *
 * \param pDot WayPoint*
 * \return void
 *
 */
inline static void as_clearList(WayPoint *pDot)
{
    WayPoint *pos = NULL;
    WayPoint *n   = NULL;

    list_for_each_entry_safe(pos, n, &(pDot->node), node)
    {
        delete pos;
    }
}

/** \brief get the entry of a node by given its coordinates
 *
 * \param x int
 * \param y int
 * \param list WayPoint*
 * \return WayPoint*
 *
 */
WayPoint *as_getEntry(int x, int y, WayPoint *list)
{
    WayPoint *pos = NULL;

    // iterate the list to find the node entry
    list_for_each_entry(pos, &(list->node), node)
    {
        // compare the coordinates
        if ((pos->x == x) && (pos->y == y))
        {
            return pos;
        }
    }

    // not found
    return NULL;
}

/** \brief The A* algorithm for finding path
 *
 * \param start WayPoint&   the start way point
 * \param goal WayPoint&    the goal way point
 * \param map Map&          the map used for searching
 * \param wpList vector<WayPoint>&  the result
 * \return void
 *
 */
void as_findPath(WayPoint &start, WayPoint &goal, Map &map, vector<WayPoint> &wpList)
{
    //cout << "find_path: s_x: " << start.x << " s_y: " << start.y << " g_x: " << goal.x << " g_y: " << goal.y << endl;

    // if start and goal are connected directly
    if (start.isConnected(goal))
    {
        wpList.push_back(start);

        if (start != goal)
        {
            wpList.push_back(goal);
        }

        return;
    }

    // create the OPEN set
    /** This is a list of all the nodes that have been visited (in an iteration
     *  of the search) and have been assigned a cost. The node that has the
     *  lowest cost will be used to perform the next iteration of the search. */
    WayPoint openList;

    // create the CLOSE set
    // This is the list of nodes whose neighbors have been visited.
    WayPoint closeList;

    WayPoint *curDot = NULL;     // a temp variable
    WayPoint *nbrDot = NULL;     // the temp variable for neighbors

    // the start dot should be included in the OPEN set
    curDot = new WayPoint(start);
    list_pri_add(&(curDot->node), &(openList.node));

    int i    = 0;   // variable for loop columns
    int j    = 0;   // variable for loop rows
    int cost = 0;   // the cost of movement from staring point
    int f    = 0;   // the sum of all cost

    // lowest rank in OPEN is not the GOAL
    while (*as_getEntry(openList.node.prev) != goal)
    {
        // current = remove lowest rank item from OPEN
        curDot = as_getEntry(openList.node.prev);

        list_del(&(curDot->node));
        // add current to CLOSED
        list_pri_add(&(curDot->node), &(closeList.node));

        // for neighbors of current
        // only search four points
        for (i = curDot->x - 1; i <= curDot->x + 1; ++i)
        {
            for (j = curDot->y - 1; j <= curDot->y + 1; ++j)
            {
                // is the diagonal point
                if ((i != curDot->x) && (j != curDot->y))
                {
                    continue;
                }

                // is current point
                if ((i == curDot->x) && (j == curDot->y))
                {
                    continue;
                }

                // is not an accessible point
                if (!map.isGoodToVisit(i, j))
                {
                    continue;
                }

                // if neighbor in OPEN
                if (nbrDot = as_getEntry(i, j, &openList))
                {
                    // calculate the cost for passing current point
                    //cost = curDot->node.num + as_calMvCost(curDot, nbrDot) + as_calHeuristic(*nbrDot, goal);
                    cost = curDot->node.num + as_calMvCost(curDot, nbrDot);

                    // and cost less than g(neighbor), 说明从当前点经过更好
                    if (cost < nbrDot->node.num)
                    {
                        // remove neighbor from OPEN, because new path is better
                        list_del(&(nbrDot->node));

                        nbrDot->node.num = cost;
                        nbrDot->parent = curDot;

                        // add neighbor to OPEN
                        // list_pri_add(&(nbrDot->node), &(openList.node));
                    }
                }
                // if neighbor in CLOSE set
                else if (nbrDot = as_getEntry(i, j, &closeList))
                {
                    // calculate the cost for passing current point
                    // cost = curDot->node.num + as_calMvCost(curDot, nbrDot) + as_calHeuristic(*nbrDot, goal);;
                    cost = curDot->node.num + as_calMvCost(curDot, nbrDot);

                    // and cost less than g(neighbor)
                    if (cost < nbrDot->node.num)
                    {
                        // remove neighbor from CLOSE, because new path is better
                        list_del(&(nbrDot->node));

                        nbrDot->node.num = cost;
                        nbrDot->parent = curDot;

                        // add neighbor to OPEN
                        list_pri_add(&(nbrDot->node), &(openList.node));
                    }
                }
                else
                {
                    // create a new way point
                    nbrDot = new WayPoint(i, j);

                    // calculate the cost
                    cost = curDot->node.num + as_calMvCost(curDot, nbrDot);

                    // set g(neighbor) to cost
                    nbrDot->node.num = cost;

                    // set neighbor's parent to current
                    nbrDot->parent = curDot;

                    // add neighbor to OPEN
                    list_pri_add(&(nbrDot->node), &(openList.node));
                }
            }
        }
    }

    wpList.push_back(start);

    vector<WayPoint>::iterator it;

    it = wpList.end();
    int xx = 0;

    // reconstruct reverse path from goal to start by following parent pointers
    for (curDot = as_getEntry(openList.node.prev); *curDot != start; curDot = curDot->parent)
    {
        it = wpList.end();
        it -=xx ;

        //cout << "~x: " << curDot->x << " y: " << curDot->y << endl;
        // store the point to way point list
        wpList.insert(it, *curDot);
        // wpList.push_back(*curDot);
        xx++;
    }

    // clear the lists
    as_clearList(&openList);
    as_clearList(&closeList);
}

#endif // ASTARPATH_H_INCLUDED
