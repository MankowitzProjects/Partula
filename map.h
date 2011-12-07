#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include <stdlib.h>

#include "mapGrid.h"
#include "EasyBMP.h"

#include <iostream>

using namespace std;

// maybe can be measured in cm

#define DEFAULT_MAP_WIDTH  500
#define DEFAULT_MAP_LENGTH 500

class Map
{
public:

    Map(void);

    void init(void);
    void init(unsigned int width, unsigned int length);
    void init(const char *filename);

    inline void      setGridType(int col, int row, TYPE_GRID type);
    inline TYPE_GRID getGridType(int col, int row);

    inline void setGridVisited(int col, int row);
    inline void setGridVisit(int col, int row, bool visited);
    inline bool isGridVisited(int col, int row);

    inline bool isGoodToVisit(int col, int row);
    inline bool isGoodToSearch(int col, int row);

    inline void setGridSerached(int col, int row);
    inline void clrGridSearched(int col, int row);
    inline void setGridSearch(int col, int row, bool searched);
    inline bool isGridSearched(int col, int row);

    unsigned int length;    /**< the length of the map, Y, row */
    unsigned int width;     /**< the width of the map,  X, col */

    // inflate the edges to make the robot move safely
    void inflateEdges(int inflation);

    void fin(void);

    void outToBmp(char *filename);
    void readFromBmp(const char *filename);

private:
    MapGrid *pMap;
};


/** \brief A default initialise function
 *
 * \param void
 * \return void
 *
 */
void Map::init(void)
{
    init(DEFAULT_MAP_WIDTH, DEFAULT_MAP_LENGTH);
}

/** \brief The initialise function
 *
 * \param width unsigned int
 * \param length unsigned int
 * \return void
 *
 */
void Map::init(unsigned int width, unsigned int length)
{
    if (NULL != pMap)
    {
        free(pMap);
    }

    this->width  = width;
    this->length = length;

    pMap = (MapGrid *)malloc(width*length*sizeof(MapGrid));

    for (unsigned int i = 0; i < width*length; ++i)
    {
        (pMap + i)->init();
    }
}

/** \brief The initialise function, initialise with a given file name
 *
 * \param filename const char*
 * \return void
 *
 */
void Map::init(const char *filename)
{
    readFromBmp(filename);
}

/** \brief The default finalise function
 *
 * \param void
 * \return void
 *
 */
void Map::fin(void)
{
    if (NULL != pMap)
    {
        free(pMap);
    }
}

/** \brief The default constructor
 *
 * \param void
 *
 */
Map::Map(void)
{
    width  = 0;
    length = 0;

    pMap = NULL;
}

/** \brief Set the type of the grid
 *
 * \param col int
 * \param row int
 * \param type TYPE_GRID
 * \return void
 *
 */
inline void Map::setGridType(int col, int row, TYPE_GRID type)
{
    // currently not handling expanding map
    if ((col >= width) || (row >= length))
    {
        return;
    }
    else
    {
        (pMap + row*length + col)->type = type;
    }
}

/** \brief Set the type of the grid
 *
 * \param col int
 * \param row int
 * \return TYPE_GRID
 *
 */
inline TYPE_GRID Map::getGridType(int col, int row)
{
    // currently not handling expanding map
    if ((col >= width) || (row >= length))
    {
        return TYPE_GRID_UNKNOWN;
    }
    else
    {
        return (pMap + row*length + col)->type;
    }
}

/** \brief Set the grid as visited
 *
 * \param col int
 * \param row int
 * \return void
 *
 */
inline void Map::setGridVisited(int col, int row)
{
    setGridVisit(col, row, true);
}

/** \brief Set the gird as been searched
 *
 * \param col int
 * \param row int
 * \return void
 *
 */
inline void Map::setGridSerached(int col, int row)
{
    setGridSearch(col, row, true);
}

/** \brief Clear the searched status
 *
 * \param col int
 * \param row int
 * \return void
 *
 */
inline void Map::clrGridSearched(int col, int row)
{
    setGridSearch(col, row, false);
}

/** \brief Set the search status of a grid
 *
 * \param col int
 * \param row int
 * \param searched bool
 * \return void
 *
 */
inline void Map::setGridSearch(int col, int row, bool searched)
{
    if ((col >= width) || (row >= length))
    {
        return;
    }
    else
    {
        (pMap + row*length + col)->isSearched = searched;
    }
}

/** \brief Return true if the grid is searched
 *
 * \param col int
 * \param row int
 * \return bool
 *
 */
inline bool Map::isGridSearched(int col, int row)
{
    if ((col >= width) || (row >= length))
    {
        return false;
    }
    else
    {
        return (pMap + row*length + col)->isSearched;
    }
}

/** \brief Set the visit state of the grid
 *
 * \param col int
 * \param row int
 * \param visited bool
 * \return void
 *
 */
inline void Map::setGridVisit(int col, int row, bool visited)
{
    if ((col >= width) || (row >= length))
    {
        return;
    }
    else
    {
        (pMap + row*length + col)->isVisited = visited;
    }
}

/** \brief Return true if the grid is visited
 *
 * \param col int
 * \param row int
 * \return bool
 *
 */
inline bool Map::isGridVisited(int col, int row)
{
    if ((col >= width) || (row >= length))
    {
        return false;
    }
    else
    {
        return (pMap + row*length + col)->isVisited;
    }
}

/** \brief return true if the grid is is safe to travel and not visited
 *
 * \param col int
 * \param row int
 * \return bool
 *
 */
inline bool Map::isGoodToVisit(int col, int row)
{
    return (TYPE_GRID_SAFE == getGridType(col, row));
}

inline bool Map::isGoodToSearch(int col, int row)
{
    return ((TYPE_GRID_SAFE == getGridType(col, row)) && (!isGridSearched(col, row)));
}

/** \brief Plot the map to a BMP file
 *
 * \param filename char*
 * \return void
 *
 */
void Map::outToBmp(char *filename)
{
    BMP outImg;             /* the output image */
    unsigned int col = 0;   /* the loop counter for columns */
    unsigned int row = 0;   /* the loop counter for rows */

    TYPE_GRID type;         /* the temp variable for type */

    // invalid map
    if ((0 == length) && (0 == width))
    {
        return;
    }

    // set the attributes of the image
    outImg.SetBitDepth(24);
    outImg.SetSize(width, length);

    // retrieve the map information from the map data
    for (col = 0; col < width; ++col)
    {
        for (row = 0; row < length; ++row)
        {
            type = getGridType(col, row);

            // colour the image pixel according to the grid type
            switch (type)
            {
                case TYPE_GRID_UNKNOWN:
                {
                    outImg(col, row)->Red   = 171;
                    outImg(col, row)->Green = 171;
                    outImg(col, row)->Blue  = 171;
                    break;
                }
                case TYPE_GRID_BARRIER:
                {
                    outImg(col, row)->Red   = 0;
                    outImg(col, row)->Green = 0;
                    outImg(col, row)->Blue  = 0;
                    break;
                }
                case TYPE_GRID_SAFE:
                {
                    if (isGridVisited(col, row))
                    {
                        // set to the colour of visited
                        outImg(col, row)->Red   = 0x7F;
                        outImg(col, row)->Green = 0xFF;
                        outImg(col, row)->Blue  = 0x00;
                    }
                    else
                    {
                        // set to the colour of unvisited but known
                        outImg(col, row)->Red   = 0x69;
                        outImg(col, row)->Green = 0x8B;
                        outImg(col, row)->Blue  = 0x22;
                    }

                    break;
                }
                case TYPE_GRID_UNSAFE:
                {
                    outImg(col, row)->Red   = 238;
                    outImg(col, row)->Green = 0;
                    outImg(col, row)->Blue  = 0;
                    break;
                }
                default:
                {
                    outImg(col, row)->Red   = 127;
                    outImg(col, row)->Green = 127;
                    outImg(col, row)->Blue  = 127;
                }
            }
        }
    }

    // save the image file
    outImg.WriteToFile(filename);
}

/** \brief Read a bmp file as map
 *
 * \param filename const char*
 * \return void
 *
 */
void Map::readFromBmp(const char *filename)
{
    init();

    BMP inImg;      /* the input image */

    int col = 0;   /* the loop counter for columns */
    int row = 0;   /* the loop counter for rows */

    if (NULL == filename)
    {
        cout << "Error with image name" << endl;
        return;
    }

    inImg.ReadFromFile(filename);

    // set the attributes of the image
    width  = inImg.TellWidth();
    length = inImg.TellHeight();

    // invalid map
    if ((0 == length) && (0 == width))
    {
        cout << "Error with image size" << endl;
        return;
    }

    init(width, length);

    // retrieve the map information from the map data
    for (col = 0; col < width; ++col)
    {
        for (row = 0; row < length; ++row)
        {
            setGridVisit(col, row, false);

            // colour the image pixel according to the grid type
            if (   (inImg(col, row)->Red   == 171)
                && (inImg(col, row)->Green == 171)
                && (inImg(col, row)->Blue  == 171))
            {
                setGridType(col, row, TYPE_GRID_UNKNOWN);
            }
            else if (   (inImg(col, row)->Red   == 0)
                     && (inImg(col, row)->Green == 0)
                     && (inImg(col, row)->Blue  == 0))
            {
                setGridType(col, row, TYPE_GRID_BARRIER);
            }
            else if (   (inImg(col, row)->Red   == 0x7F)
                     && (inImg(col, row)->Green == 0xFF)
                     && (inImg(col, row)->Blue  == 0x00))
            {
                setGridType(col, row, TYPE_GRID_SAFE);
                setGridVisited(col, row);
            }
            else if (   (inImg(col, row)->Red   == 0x69)
                     && (inImg(col, row)->Green == 0x8B)
                     && (inImg(col, row)->Blue  == 0x22))
            {
                setGridType(col, row, TYPE_GRID_SAFE);
            }
            else if (  (inImg(col, row)->Red   == 238)
                    && (inImg(col, row)->Green == 0)
                    && (inImg(col, row)->Blue  == 0))
            {
                setGridType(col, row, TYPE_GRID_UNSAFE);
            }
            else
            {
                setGridType(col, row, TYPE_GRID_UNKNOWN);
            }
        }
    }
}

#endif // MAP_H_INCLUDED
