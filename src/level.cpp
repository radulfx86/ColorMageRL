#include "level.h"
#include <fstream>

typedef struct Room
{
    Vec2i start;
    Vec2i size;
} Room;


LevelData LevelData::generate(Vec2i levelSize, int numSubLevels)
{
    if ( levelSize.x > 100 || levelSize.y > 100 )
    {
        exit(99);
    }
    Vec2i offset{-levelSize.x/2, -levelSize.y/2};
    (void)numSubLevels;
    const unsigned int seed = 1423123;
    std::srand(seed);
    LevelData level;
    std::vector<Room> rooms;
    std::bitset<MAX_WIDTH * MAX_WIDTH> levelDataOuter(false);
    std::bitset<MAX_WIDTH * MAX_WIDTH> levelDataInner(false);

    int maxNumRooms = 30;
    int maxTries = 5;
    bool foundSpace = true;
    const Vec2i maxRoomSize{20, 20};
    const Vec2i minRoomSize{4, 4};
    const int randScaleX = RAND_MAX / (levelSize.x + minRoomSize.x);
    const int randScaleY = RAND_MAX / (levelSize.y + minRoomSize.y);
    const int randRoomScaleX = RAND_MAX / (maxRoomSize.x - minRoomSize.y);
    const int randRoomScaleY = RAND_MAX / (maxRoomSize.y - minRoomSize.y);
    for (int numRooms = 0; foundSpace && numRooms < maxNumRooms; ++numRooms)
    {
        std::bitset<MAX_WIDTH * MAX_WIDTH> tmpLevel(false);
        std::bitset<MAX_WIDTH * MAX_WIDTH> tmpLevelInner(false);
        // extracted separately to keep order for deterministic handling using seeded random
        int startRandX;
        int startRandY;
        int numTries = 0;
        foundSpace = true;
        do
        {
            // rooms always leave a distance of 1 to the border
            // only for innerData
            // outer data profits being able to use (x,y) +-(1,1) on every point without test against borders
            startRandX = 1 + std::rand() / randScaleX;
            startRandY = 1 + std::rand() / randScaleY;
            foundSpace = not levelDataOuter.test(startRandY * levelSize.x + startRandX);
            printf("room[%d] try[%d] points: %d %d space? %d\n", numRooms, numTries, startRandX, startRandY, foundSpace);
            ++numTries;
        } while ((not foundSpace) and (numTries < maxTries));
        if (numTries >= maxTries)
        {
            printf("did not find start point for room %d\n", numRooms);
            foundSpace = false;
        }
        if (foundSpace)
        {
            int roomWidth = minRoomSize.x + std::rand() / randRoomScaleX;
            int roomHeight = minRoomSize.y + std::rand() / randRoomScaleY;
	    roomWidth = maxRoomSize.x;
	    roomHeight = maxRoomSize.y;
            if ( levelSize.x <= roomWidth + startRandX )
            {
                printf("reduce width from %d to %d\n", roomWidth, levelSize.x - startRandX - 1);
                roomWidth = levelSize.x - startRandX - 1;
            }
            if ( levelSize.y <= roomHeight + startRandY )
            {
                roomHeight = levelSize.y - startRandY - 1;
            }
            if (roomWidth <= minRoomSize.x || roomHeight <= minRoomSize.y )
            {
                continue;
            }
            printf("room width: %d height: %d\n", roomWidth, roomHeight);
            int h = 0;
            for (h = 0; h < roomHeight; ++h)
            {
                int w = 0;
                //printf("test w for h = %d: %d %d\n",h, levelDataInner.test(startRandY + h) *levelSize.x + (startRandX + w), w < roomWidth);
                for (w = 0;
                     (not levelDataOuter.test((startRandY + h) * levelSize.x + (startRandX + w))) && w < roomWidth;
                     ++w)
                {
                //    printf("data at %d %d: %d %d\n", startRandX + w, startRandY + h, levelDataInner.test((startRandY + h) * levelSize.x + (startRandX + w)), levelDataOuter.test((startRandY + h) * levelSize.x + (startRandX + w)));
                }
                if (w < minRoomSize.x)
                {
                    printf("max height for room[%d]: %d < %d\n", numRooms, h, roomHeight);
                    roomHeight = h;
                    break;
                }
                if (w < roomWidth)
                {
                    printf("max width for room[%d]: %d < %d\n", numRooms, w, roomWidth);
                    roomWidth = w;
                }
                for (int w = 0; w < roomWidth; ++w)
                {
                    // set room as used
                    tmpLevelInner.set((startRandY + h) * levelSize.x + (startRandX + w));
                    // set outer perimeter of room as well
                    for (int i = -1; i < 2; ++i)
                    {
                        for (int j = -1; j < 2; ++j)
                        {
                            tmpLevel.set((startRandY + h + i) * levelSize.x + (startRandX + w + j));
                        }
                    }
                }
            }
            if (h > minRoomSize.y)
            {
                // found room of size roomWidth, roomHeight
                rooms.push_back(Room{Vec2i{startRandX, startRandY}, Vec2i{roomWidth, roomHeight}});
            	levelDataOuter |= tmpLevel;
            	levelDataInner |= tmpLevelInner;
            }
        }
    }
    // connect rooms
    for (unsigned int idxRoom = 0; idxRoom < rooms.size() - 1; ++idxRoom)
    {
	    int idxRoomNext = idxRoom + 1;
	    int tNext = 0;
	    int dNext = 100;
	    for ( idxRoomNext = idxRoom; idxRoomNext < (int)rooms.size(); ++idxRoomNext)
	    {
		    if ( idxRoomNext == (int)idxRoom )
		    {
			    continue;
		    }
		    int x2 = rooms[idxRoomNext].start.x - rooms[idxRoom].start.x;
		    x2 *= x2;
		    int y2 = rooms[idxRoomNext].start.y - rooms[idxRoom].start.y;
		    y2 *= y2;
		    int dNextTmp = sqrt(x2 + y2);
		    if ( dNextTmp <  dNext )
		    {
			    dNext = dNextTmp;
			    tNext = idxRoomNext;
		    }

	    }
	    idxRoomNext = tNext;
	    printf("connect room %u with %d\n", idxRoom, idxRoomNext);
	    unsigned int roomScaleAX = RAND_MAX / rooms[idxRoom].size.x;
	    unsigned int roomScaleAY = RAND_MAX / rooms[idxRoom].size.y;
	    unsigned int pointRoomAX = rooms[idxRoom].start.x + std::rand() / roomScaleAX;
	    unsigned int pointRoomAY = rooms[idxRoom].start.y + std::rand() / roomScaleAY;
	    unsigned int roomScaleBX = RAND_MAX / rooms[idxRoomNext].size.x;
	    unsigned int roomScaleBY = RAND_MAX / rooms[idxRoomNext].size.y;
	    unsigned int pointRoomBX = rooms[idxRoomNext].start.x + std::rand() / roomScaleBX;
	    unsigned int pointRoomBY = rooms[idxRoomNext].start.y + std::rand() / roomScaleBY;
	    int dx = (int)pointRoomBX - (int)pointRoomAX;
	    int dy = (int)pointRoomBY - (int)pointRoomAY;
	    int tx = dx > 0 ? 1 : dx < 0 ? -1
		    : 0;
	    int ty = dy > 0 ? 1 : dy < 0 ? -1
		    : 0;
	    /*
	    printf("connect - walk from %d %d to %d %d by %d %d\n",
			    pointRoomAX, pointRoomAY,
			    pointRoomBX, pointRoomBY,
			    tx, ty);
			    */
	    unsigned int y = pointRoomAY;
	    unsigned int x = pointRoomAX;
	    for (; y != pointRoomBY; y += ty)
	    {
		    levelDataInner.set(y * levelSize.x + x);
	    }
	    for (; x != pointRoomBX; x += tx)
	    {
		    levelDataInner.set(y * levelSize.x + x);
	    }
    }
    printf("LEVEL: %d x %d\n", levelSize.x, levelSize.y);
    if ( levelSize.x > 100 || levelSize.y > 100 )
    {
        exit(97);
    }
    // add stairs
    for ( Room room : rooms )
    {
        int roomScale = RAND_MAX / 2;
        if ( rand() > RAND_MAX / roomScale )
        {
            unsigned int roomScaleAX = RAND_MAX / room.size.x;
            unsigned int roomScaleAY = RAND_MAX / room.size.y;
            int x = room.start.x + std::rand() / roomScaleAX;
            int y = room.start.y + std::rand() / roomScaleAY;

            LevelTileType tileType = STAIRS_UP;
            if (rand() > RAND_MAX / 2)
            {
                tileType = STAIRS_DOWN;
            }

            level.tileData[Vec2i{2 * x + offset.x, 2 * y + offset.y}] = {tileType, 0, true};
            level.tileData[Vec2i{2 * x + offset.x + 1, 2 * y + offset.y}] = {tileType, 0, true};
            level.tileData[Vec2i{2 * x + offset.x + 1, 2 * y + offset.y + 1}] = {tileType, 0, true};
            level.tileData[Vec2i{2 * x + offset.x, 2 * y + offset.y + 1}] = {tileType, 0, true};
        }
    }
    for (int y = 0; y < levelSize.y; ++y)
    {
        for (int x = 0; x < levelSize.x; ++x)
        {
            if (not levelDataInner.test(y * levelSize.x + x))
            {
                level.tileData[Vec2i{2 * x + offset.x, 2 * y + offset.y}] = {WALL, 0, true};
                level.tileData[Vec2i{2 * x + offset.x + 1, 2 * y + offset.y}] = {WALL, 0, true};
                level.tileData[Vec2i{2 * x + offset.x + 1, 2 * y + offset.y + 1}] = {WALL, 0, true};
                level.tileData[Vec2i{2 * x + offset.x, 2 * y + offset.y + 1}] = {WALL, 0, true};
                printf("XX");
            }
            else
            {
                printf("  ");
            }
        }
        printf("\n");
    }
    // starting point

    unsigned int roomScaleAX = RAND_MAX / rooms[0].size.x;
    unsigned int roomScaleAY = RAND_MAX / rooms[0].size.y;
    int x = rooms[0].start.x + std::rand() / roomScaleAX;
    int y = rooms[0].start.y + std::rand() / roomScaleAY;
    level.entryPoint = Vec2i{x,y};


    printf("generated %lu rooms\n", rooms.size());
    for ( Room &room : rooms )
    {
        printf("room {%d,%d} size {%d,%d}\n", room.start.x, room.start.y, room.size.x, room.size.y);
    }

    LevelData::updateAutoTilerOrientation(level);
    return level;
}

LevelData LevelData::load(std::string path, Vec2i offset)
{
    LevelData level;
    int w = 0;
    int h = 0;
    try
    {
        std::ifstream istr(path, std::ios_base::in);
        int y = 0;
        for (std::string line; std::getline(istr, line); ++y)
        {
            printf("y: %2d ", y);
            for (int x = 0; x < (int)line.length(); ++x)
            {
                printf(">(%d,%d)<", x + offset.x, y + offset.y);
                if (x > w)
                {
                    w = x;
                }
                switch (line[x])
                {
                case 'W':
                    level.tileData[Vec2i{2 * x + offset.x, 2 * y + offset.y}] = {WALL, 0, true};
                    level.tileData[Vec2i{2 * x + offset.x + 1, 2 * y + offset.y}] = {WALL, 0, true};
                    level.tileData[Vec2i{2 * x + offset.x + 1, 2 * y + offset.y + 1}] = {WALL, 0, true};
                    level.tileData[Vec2i{2 * x + offset.x, 2 * y + offset.y + 1}] = {WALL, 0, true};
                    break;
                case 'D':
                    level.tileData[Vec2i{2 * x + offset.x, 2 * y + offset.y}] = {DOOR, 0, false};
                    level.tileData[Vec2i{2 * x + offset.x + 1, 2 * y + offset.y}] = {DOOR, 0, false};
                    level.tileData[Vec2i{2 * x + offset.x + 1, 2 * y + offset.y + 1}] = {DOOR, 0, false};
                    level.tileData[Vec2i{2 * x + offset.x, 2 * y + offset.y + 1}] = {DOOR, 0, false};
                    break;
                default:
                    break;
                }
            }
            printf("\n");
            if (y > h)
            {
                h = y;
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    LevelData::updateAutoTilerOrientation(level);

    return level;
}

void LevelData::updateAutoTilerOrientation(LevelData &level)
{
    // index in tile-set to be used
    // indices correspond to the sum of neighbor positions
    //  1		1/2		2
    //  1/2		x		2/8
    //   4		4/8     8
    for (std::pair<const Vec2i, LevelTile> &tile : level.tileData)
    {
        int x = tile.first.x;
        int y = tile.first.y;
        int cnt = 0;
        cnt += (level.tileData.find(Vec2i{x, y - 1}) != level.tileData.end()) && (level.tileData.find(Vec2i{x - 1, y - 1}) != level.tileData.end()) && (level.tileData.find(Vec2i{x - 1, y}) != level.tileData.end())
                   ? 4
                   : 0;
        cnt += (level.tileData.find(Vec2i{x, y - 1}) != level.tileData.end()) && (level.tileData.find(Vec2i{x + 1, y - 1}) != level.tileData.end()) && (level.tileData.find(Vec2i{x + 1, y}) != level.tileData.end())
                   ? 8
                   : 0;
        cnt += (level.tileData.find(Vec2i{x, y + 1}) != level.tileData.end()) && (level.tileData.find(Vec2i{x - 1, y + 1}) != level.tileData.end()) && (level.tileData.find(Vec2i{x - 1, y}) != level.tileData.end())
                   ? 1
                   : 0;
        cnt += (level.tileData.find(Vec2i{x, y + 1}) != level.tileData.end()) && (level.tileData.find(Vec2i{x + 1, y + 1}) != level.tileData.end()) && (level.tileData.find(Vec2i{x + 1, y}) != level.tileData.end())
                   ? 2
                   : 0;
        tile.second.orientation = cnt;
        if (tile.second.type != WALL)
        {
            // tile.second.orientation = 0;
        }
        printf("LEVEL %2d %2d -> %d\n", x, y, cnt);
    }
    printf("LEVEL size: %ld\n", level.tileData.size());
}
