#ifndef _LEVEL_H_
#define _LEVEL_H_
#include "types.h"
#include "ecs.h"

#include <vector>
#include <map>
#include <queue>
#include <set>

class LevelData
{
public:
    enum LevelTileType
    {
        WALL,
        DOOR,
        STAIRS_UP,
        STAIRS_DOWN,
        NUM_LEVEL_TILE_TYPES
    };
    struct LevelTile
    {
        LevelTileType type;
        int orientation;
        bool solid;
    };
    int getTileIndex(Vec2i pos)
    {
        if ( tileData.find(pos) == tileData.end() )
        {
            return -1;
        }
        return tileData[pos].orientation;
    }

    LevelData() {}
    static LevelData load(std::string path, Vec2i offset);
    
    static LevelData generate(Vec2i levelSize, int numSubLevels);

    static void updateAutoTilerOrientation(LevelData &level);

    bool intersects(const Bounds &b)
    {
        return (tileData.find(Vec2i{(int)b.pos.x, (int)b.pos.y}) != tileData.end() && tileData[Vec2i{(int)b.pos.x, (int)b.pos.y}].solid)
                || (tileData.find(Vec2i{(int)(b.pos.x + b.size.x), (int)b.pos.y}) != tileData.end() && tileData[Vec2i{(int)(b.pos.x + b.size.x), (int)b.pos.y}].solid)
                || (tileData.find(Vec2i{(int)(b.pos.x + b.size.x), (int)(b.pos.y + b.size.y)}) != tileData.end() && tileData[Vec2i{(int)(b.pos.x + b.size.x), (int)(b.pos.y + b.size.y)}].solid)
                || (tileData.find(Vec2i{(int)b.pos.x, (int)(b.pos.y + b.size.y)}) != tileData.end() && tileData[Vec2i{(int)b.pos.x, (int)(b.pos.y + b.size.y)}].solid);
    }
    std::vector<std::pair<Vec2i, LevelTile>> getTilesInBounds(const Bounds &b)
    {
        std::vector<std::pair<Vec2i, LevelTile>> tiles;
        for ( int dist = 0; dist < 16; ++dist )
        {
            for ( int x = -dist; x < dist - 1; ++x )
            {
                Vec2i tmp{(int)(b.pos.x + x), (int)(b.pos.y - dist)};
                if ( tileData.find(tmp) != tileData.end() )
                {
                    tiles.push_back(std::pair<Vec2i, LevelTile>(tmp, tileData[tmp]));
                }
                Vec2i tmp2{(int)(b.pos.x + x + 1), (int)(b.pos.y + dist - 1)};
                if ( tileData.find(tmp2) != tileData.end() )
                {
                    tiles.push_back(std::pair<Vec2i, LevelTile>(tmp2, tileData[tmp2]));
                }
                Vec2i tmp3{(int)(b.pos.x - dist), (int)(b.pos.y + x + 1)};
                if ( tileData.find(tmp3) != tileData.end() )
                {
                    tiles.push_back(std::pair<Vec2i, LevelTile>(tmp3, tileData[tmp3]));
                }
                Vec2i tmp4{(int)(b.pos.x + dist - 1), (int)(b.pos.y + x)};
                if ( tileData.find(tmp4) != tileData.end() )
                {
                    tiles.push_back(std::pair<Vec2i, LevelTile>(tmp4, tileData[tmp4]));
                }

            }
        }
        #if 0
        Vec2i min = {(int)b.pos.x - offset.x, (int)b.pos.y - offset.y};
        Vec2i max = {(int)std::ceil(b.pos.x + b.size.x - offset.x), (int)(std::ceil(b.pos.y + b.size.y - offset.y))};
        for ( int x = min.x; x < max.x ; ++x )
        {
            for ( int y = min.y; y < max.y; ++y )
            {
                Vec2i tmp{x,y};
                if ( tileData.find(tmp) != tileData.end() )
                {
                    tiles.push_back(std::pair<Vec2i, LevelTile>(tmp, tileData[tmp]));
                }
            }
        }
        #endif
        printf("number of active tiles: %lu of %lu\n", tiles.size(), tileData.size());
        return tiles;
    }
    std::vector<Vec2> getPathTo(Vec2i start, Vec2i end)
    {
        struct WeightedVec2i{
            Vec2i pos;
            int distanceSquared;
            std::vector<Vec2i> path;
        };
        std::vector<Vec2i> path;
        std::set<Vec2i> seen;
        auto cmp = [&](WeightedVec2i l, WeightedVec2i r) { return l.distanceSquared > r.distanceSquared; };
        std::priority_queue<WeightedVec2i, std::vector<WeightedVec2i>, decltype(cmp)> pq(cmp);

        auto getWeightedVec2i = [] (Vec2i pos, Vec2i target, std::vector<Vec2i> path) -> WeightedVec2i
        {
            path.push_back(pos);
            return WeightedVec2i{pos, (target.x - pos.x) * (target.x - pos.x) + (target.y - pos.y) * (target.y - pos.y), path};
        };

        pq.push(getWeightedVec2i(start, end, {}));
        do
        {
            WeightedVec2i el = pq.top();
            pq.pop();
            printf("test path of length %lu to pos %d %d (target %d %d) pq size: %lu\n", el.path.size(), el.pos.x, el.pos.y, end.x, end.y, pq.size());
            if (end == el.pos)
            {
                el.path.push_back(el.pos);
                path = el.path;
                break;
            }
            for (int x = -1; x < 2; ++x)
            {
                for (int y = -1; y < 2; ++y)
                {
                    if ( not (x == 0 || y == 0) )
                        continue;
                    Vec2i next{el.pos.x + x, el.pos.y + y};
                    if (tileData.find(next) != tileData.end())
                    {
                        continue;
                    }
                    std::vector<Vec2i> tmp = el.path;
                    pq.push(getWeightedVec2i(next, end, tmp));
                }
            }
        } while (not pq.empty());
        std::vector<Vec2> vPath;
        for ( auto p : path )
        {
            vPath.push_back(Vec2{(float)p.x,(float)p.y});
        }
        return vPath;
    }



    Vec2 getEntryPoint() const
    {
        return {(float)entryPoint.x, (float)entryPoint.y};
    }
private:
    std::map<Vec2i, LevelTile> tileData;
    Vec2i entryPoint;
};

#endif // _LEVEL_H_