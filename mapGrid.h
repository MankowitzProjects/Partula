#ifndef MAPGRID_H_INCLUDED
#define MAPGRID_H_INCLUDED

typedef enum
{
    TYPE_GRID_UNKNOWN = 0,  // unknown grid type
    TYPE_GRID_BARRIER,      // the grid is a part of barrier
    TYPE_GRID_SAFE,         // the grid is safe to travel
    TYPE_GRID_UNSAFE        // the grid is close to the barrier

} TYPE_GRID;

class MapGrid
{
public:
    void init(void);
    TYPE_GRID type;         /**< the type of the grid */
    bool      isVisited;    /**< is the gird been visited */
    bool      isSearched;   /**< is the grid been searched */
    inline void markType(TYPE_GRID type);
};

/** \brief The default initialse function of the grid
 *
 * \param void
 * \return void
 *
 */
void MapGrid::init(void)
{
    type = TYPE_GRID_UNKNOWN;
    isVisited = false;
}

/** \brief mark the type of the grid
 *
 * \param type TYPE_GRID
 * \return void
 *
 */
inline void MapGrid::markType(TYPE_GRID type)
{
    if (TYPE_GRID_UNKNOWN == this->type)
    {
        this->type = type;
    }
}

#endif // MAPGRID_H_INCLUDED
