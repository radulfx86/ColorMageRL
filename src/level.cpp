#include "level.h"

LevelData LevelData::generate(unsigned long seed)
{
    LevelData level;

    return level;
}

LevelData LevelData:load(std::string path, Vec2i offset)
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
                    level.data[Vec2i{2 * x + offset.x, 2 * y + offset.y}] = {WALL, 0, true};
                    level.data[Vec2i{2 * x + offset.x + 1, 2 * y + offset.y}] = {WALL, 0, true};
                    level.data[Vec2i{2 * x + offset.x + 1, 2 * y + offset.y + 1}] = {WALL, 0, true};
                    level.data[Vec2i{2 * x + offset.x, 2 * y + offset.y + 1}] = {WALL, 0, true};
                    break;
                case 'D':
                    level.data[Vec2i{2 * x + offset.x, 2 * y + offset.y}] = {DOOR, 0, false};
                    level.data[Vec2i{2 * x + offset.x + 1, 2 * y + offset.y}] = {DOOR, 0, false};
                    level.data[Vec2i{2 * x + offset.x + 1, 2 * y + offset.y + 1}] = {DOOR, 0, false};
                    level.data[Vec2i{2 * x + offset.x, 2 * y + offset.y + 1}] = {DOOR, 0, false};
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

    // index in tile-set to be used
    // indices correspond to the sum of neighbor positions
    //  1		1/2		2
    //  1/2		x		2/8
    //   4		4/8     8
    for (std::pair<const Vec2i, LevelTile> &tile : level.data)
    {
        int x = tile.first.x;
        int y = tile.first.y;
        int cnt = 0;
        cnt += (level.data.find(Vec2i{x, y - 1}) != level.data.end()) && (level.data.find(Vec2i{x - 1, y - 1}) != level.data.end()) && (level.data.find(Vec2i{x - 1, y}) != level.data.end())
                   ? 4
                   : 0;
        cnt += (level.data.find(Vec2i{x, y - 1}) != level.data.end()) && (level.data.find(Vec2i{x + 1, y - 1}) != level.data.end()) && (level.data.find(Vec2i{x + 1, y}) != level.data.end())
                   ? 8
                   : 0;
        cnt += (level.data.find(Vec2i{x, y + 1}) != level.data.end()) && (level.data.find(Vec2i{x - 1, y + 1}) != level.data.end()) && (level.data.find(Vec2i{x - 1, y}) != level.data.end())
                   ? 1
                   : 0;
        cnt += (level.data.find(Vec2i{x, y + 1}) != level.data.end()) && (level.data.find(Vec2i{x + 1, y + 1}) != level.data.end()) && (level.data.find(Vec2i{x + 1, y}) != level.data.end())
                   ? 2
                   : 0;
        tile.second.orientation = cnt;
        if (tile.second.type != WALL)
        {
            // tile.second.orientation = 0;
        }
        printf("LEVEL %2d %2d -> %d\n", x, y, cnt);
    }
    printf("LEVEL size: %ld\n", level.data.size());

    return level;
}