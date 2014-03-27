/****************************************************************************
 Copyright (c) 2013-2014 George Guy
 Copyright (c) 2013-2014 Casey Loufek
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

/****************************************************************************
 This is a general purpose positive-weighted A* pathfinding implementation for
 Box2D. Refer to http://en.wikipedia.org/wiki/A_star for more
 information on A*.
 
 USAGE:
 
 b2Fixture* pawn = // A fixture from the body representing the agent that
 // will follow the path. If the agent's body is composed of many small
 // fixtures, I recommend adding a large sensor fixture encompassing all of
 // those smaller fixtures to use as the pawn. By default, static fixtures that
 // would collide with the pawn are considered blocked. Any dynamic fixture
 // that the pawn would collide with is instead adds weight equal to its
 // body's mass to the region. Adding weight to a region means that the distance
 // across it is treated as being that much longer.
 
 b2Vec2 goal = // A point in space to which you want to pathfind.
 
 float courtesy = // A multiplier applied to the weights of all regions
 // searched. With a higher courtesy, the pathfinder will try harder to find
 // paths around fixtures with dynamic bodies and fixtures whose category bits
 // have in common with the heavy bits given to the pathfinder.
 
 short limit = // A cap on the size of any of the data structures used by the
 // pathfinder. This prevents the algorithm from taking forever, or too long,
 // if it can't find a path. Depending on the size of the maps and bodies you're
 // dealing with, start with a value like 512 or so and tweak it for your
 // purposes until it doesn't lag. A negative value means there's no limit, but
 // that's not recommended for release versions.
 
 uint16 blockedBits = // A bitvector compared against the category bits of each
 // fixture found in any region searched during pathfinding. If any fixture has
 // category bits in common with the pathfinder's blocked bits, its region is
 // considered blocked, even if the fixture is dynamic or wouldn't normally
 // collide with the pawn.
 
 uint16 heavyBits = // Similar to the blocked bits, only instead of making a
 // region blocked if a fixture in it has bits in common with the pawn's
 // category bits, the pathfinder will add weight to the region equal to the
 // mass of the "heavy" fixture's body.
 
 // Once you've figured out all those values, go ahead and construct the pathfinder:
 b2Pathfinder pathfinder = b2Pathfinder(pawn, goal, courtesy, limit, blockedBits, heavyBits);
 
 // Then, have it find the path:
 pathfinder.aStarFindPath();
 
 // Then, you can retrieve the path to the destination, if any has been found:
 b2Pathfinder::Path path = pathfinder.tracePathFromDestination();
 
 // If the pathfinder doesn't find a path to the destination, you may still find
 // it useful to retrieve other data about what it found:
 if( path.size() == 0 )
 {
     // Path to the region found with lowest (path length from start) + (Euclidian distance to goal):
     path = pathfinder.tracePathFromBest();
 
     // Path to the region with the highest (path length from start) - (Euclidian distance to goal):
     path = pathfinder.tracePathFromInteresting();
 
     // Path to the region with the highest (path length from start) + (Euclidian distance to goal):
     path = pathfinder.tracePathFromFar();
 
     // Path to the last region searched before hitting the limit:
     path = pathfinder.tracePathFromFinal();
 }
 ****************************************************************************/


#ifndef __ZombiesWillInfectYourFamily__b2AStarFindPath__
#define __ZombiesWillInfectYourFamily__b2AStarFindPath__

#include "Box2D/Common/b2Math.h"
#include "Box2D/Collision/b2Collision.h"
#include <vector>
#include <map>
#include <set>

class b2Fixture;

struct b2Pathfinder
{
protected:
    
    /*
     * A vertex represents a "closed" point that has already been explored in
     * the current pathfinding attempt.
     */
    struct vertex
    {
        /*
         * The closed vertices for the A* implementation are stored in an
         * ExploredMap, which maps vertices to their distances from the origin.
         *
         * The unique_ptr is a kluge made necessary by C++11's
         * reduced support for incomplete types. You see, in C++98, the
         * following code was perfectly legal:
         *
         * struct foo
         * {
         *     std::set<foo>::const_iterator bar;
         * };
         *
         * However, in C++11, the above code will cause the following compiler
         * error:
         *
         * "Field has incomplete type 'value_type' (aka 'foo')"
         *
         * All this means, for our purposes, is that whenever we want to create
         * a new vertex, we must create it with a unique_ptr wrapper, for
         * example:
         *
         * vertex v = unique_ptr<vertex>( new vertex( x, y, distance, previous );
         *
         * Whenever we must to access an attribute of a vertex wrapped in a
         * unique_ptr, we use the dereference operator:
         *
         * float distance = (*v).distance;
         *
         * If we put the vertex in an ExploredSet and retrieve an iterator to
         * it, we must dereference twice in order to retrieve the vertex -- once
         * for the iterator, and once for the unique_ptr:
         *
         * float distance = (**v).distance;
         */
        typedef std::set<std::unique_ptr<vertex>> ExploredSet;
        
        /*
         * The x and y coordinates of a vertex are stored as ints because they
         * measure discrete steps away from the origin and not points in space.
         */
        int x;
        int y;
        
        /*
         * This vertex's distance from the origin down the shortest
         * path yet computed.
         */
        float distance;
        
        /*
         * Points to the vertex found before this vertex in the path on which
         * this vertex is found.
         */
        ExploredSet::const_iterator previous;
        
        /*
         * Default constructor. Note that (0, 0) represents the position of the
         * origin from which the pathfinding attempt is being made.
         */
        vertex() : x(0), y(0), distance(0) {};
        ~vertex() {};
        
        vertex(const vertex& v) :
        x(v.x), y(v.y),
        distance(v.distance),
        previous(v.previous)
        {};
        
        vertex(vertex&& v) :
        x(std::move(v.x)),
        y(std::move(v.y)),
        distance(std::move(v.distance)),
        previous(std::move(v.previous))
        {};
        
        /*
         * Constructor. Given the origin of the pathfinding attempt and the
         * location currently being searched, defines the x and y coordinates by
         * the number of steps away from origin that location is on a grid
         * aligned with origin where each square is of a size equal to that of
         * origin.
         *
         * location: the location of the vertex to be defined in space. Location
         * must be the same size as origin, and be grid-aligned with origin.
         *
         * origin: the region occupied by the entity that is attempting to
         * pathfind.
         *
         * p and d: initialize "previous" and "distance", respectively.
         */
        vertex(const b2AABB& location, const b2AABB& origin, ExploredSet::const_iterator p, float d) :
        previous(p),
        distance(d)
        {
            const b2Vec2 locationCenter = location.GetCenter();
            const b2Vec2 originCenter = origin.GetCenter();
            const b2Vec2 distance = locationCenter - originCenter;
            const b2Vec2 size = b2Vec2(origin.upperBound.x - origin.lowerBound.x, origin.upperBound.y - origin.lowerBound.y);
            x = distance.x ? floorf((distance.x / size.x) + 0.5f) : 0;
            y = distance.y ? floorf((distance.y / size.y) + 0.5f) : 0;
            
            b2Assert(x < SHRT_MAX && y < SHRT_MAX && x > SHRT_MIN && y > SHRT_MIN);
        };
        
        /*
         * Converts this vertex to a point in space, using origin as a reference
         * point.
         */
        b2Vec2 toVector(const b2AABB& origin) const
        {
            const b2Vec2 size = b2Vec2(origin.upperBound.x - origin.lowerBound.x, origin.upperBound.y - origin.lowerBound.y);
            return (origin.GetCenter() + b2Vec2(x * size.x, y * size.y));
        }
        
        /*
         * Returns a unique hash value for this vertex based on its coordinates.
         */
        unsigned long int hash() const
        {
            unsigned long int X = x >= 0 ? 2 * x : -2 * x - 1;
            unsigned long int Y = y >= 0 ? 2 * y : -2 * y - 1;
            return X >= Y ? (X * X) + X + Y : X + (Y * Y);
        }
        
        /*
         * compares two vertices based on their hashes -- required for storing
         * vertices in a set.
         */
        bool operator<(const vertex& other) const
        {
            return hash() < other.hash();
        }
        
        /*
         * When comparing vertices, always make sure that you are comparing
         * the actual vertices, and not unique_pointers or
         * ExploredSet::iterators. Remember that although Comparing
         * unique_pointers with "<" or ">" operators calls
         * through to the contained object's operator, "==" and "!=" compare
         * pointer values.
         */
        bool operator==(const vertex& other) const
        {
            return x == other.x && y == other.y;
        }
        
        bool operator!=(const vertex& other) const
        {
            return x != other.x || y != other.y;
        }
        
        vertex& operator=(const vertex& other)
        {
            if( this == &other ) return *this;
            x = other.x;
            y = other.y;
            previous = other.previous;
            distance = other.distance;
            return *this;
        }
    };
    
    /*
     * A spot represents a region that is in the "open" or "unexplored" list,
     * and thus has yet to be checked for obstacles.
     */
    struct spot
    {
        /*
         * The length of the shortest path yet found from the origin to this
         * spot.
         */
        float min_distance;
        
        /*
         * The Euclidian distance from this spot to the goal.
         */
        float heuristic;
        
        /*
         * The physical region referred to by this spot. Unlike with the simple
         * coordinates on the vertex, the region must be stored as a full AABB,
         * because a space on the unexplored list has not yet been region-
         * queried for obstacles.
         */
        b2AABB region;
        vertex::ExploredSet::const_iterator previous;
        
        spot(b2AABB r, float md, float h, vertex::ExploredSet::const_iterator p) :
        region(r), min_distance(md), heuristic(h), previous(p)
        {
        };
        
        /*
         * The keystone of A*: allows the the spots to be sorted by the sums of
         * their path distances from the origin and their Euclidian distances
         * from the destination.
         */
        bool operator<(const spot& other) const
        {
            return min_distance + heuristic < other.min_distance + other.heuristic;
        }
        
        bool operator==(const spot& other) const
        {
            b2AABB r = region;
            r.Combine(other.region);
            return r.GetPerimeter() <= 1.5f * region.GetPerimeter();
        }
        
        spot& operator=(const spot& other)
        {
            if( this == &other ) return *this;
            this->region = other.region;
            this->min_distance = other.min_distance;
            this->heuristic = other.heuristic;
            this->previous = other.previous;
            return *this;
        }
    };
    
    struct BestRecord
    {
        vertex::ExploredSet::const_iterator best;
        float record;
        
        BestRecord(vertex::ExploredSet::const_iterator b, float r) : best(b), record(r) {};
        BestRecord(const BestRecord& b) : best(b.best), record(b.record) {};
        BestRecord(const BestRecord&& b) : best(std::move(b.best)), record(std::move(b.record)) {};
        
        BestRecord& operator=( const BestRecord& b )
        {
            if(this == &b) return *this;
            this->best = b.best;
            this->record = b.record;
            return *this;
        }
        
        void judgeBest( const vertex::ExploredSet::const_iterator contestant, const float heuristic )
        {
            const float result = (**contestant).distance + heuristic;
            
            if(result <= record)
            {
                best = contestant;
                record = result;
            }
        }
        
        void judgeInteresting( const vertex::ExploredSet::const_iterator contestant, const float heuristic )
        {
            const float result = (**contestant).distance - heuristic;
            
            if(result >= record)
            {
                best = contestant;
                record = result;
            }
        }
        
        void judgeFar( const vertex::ExploredSet::const_iterator contestant, const float heuristic )
        {
            const float result = (**contestant).distance + heuristic;
            
            if(result >= record)
            {
                best = contestant;
                record = result;
            }
        }
    };
    
    b2Fixture* pawn;
    
public:
    typedef std::vector<b2Vec2> Path;
    
    const b2Vec2 goal;
    const float courtesy;
    short limit;
    const uint16 blockedBits;
    const uint16 heavyBits;
    
    BestRecord bestExplored;
    BestRecord interestingExplored;
    BestRecord farExplored;
    
    vertex finalVertex;
    vertex destinationVertex;
    
    vertex::ExploredSet explored;
    std::set<vertex> blocked;
    std::multiset<spot> unexplored;
    
    b2Pathfinder(b2Fixture* p, const b2Vec2& g, float c, short l, uint16 b, uint16 h) :
    pawn(p),
    goal(g),
    courtesy(c),
    limit(l),
    blockedBits(b),
    heavyBits(),
    bestExplored(BestRecord(explored.end(), 0)),
    interestingExplored(BestRecord(explored.end(), 0)),
    farExplored(BestRecord(explored.end(), 0))
    {
    }
    
    bool aStarFindPath();
    Path traceFromDestination();
    Path traceFromBest();
    Path traceFromInteresting();
    Path traceFromFar();
    Path traceFromFinal();
    static float pathLength(const Path& path);
    
protected:
    Path tracePath(const vertex& end);
};

#endif /* defined(__ZombiesWillInfectYourFamily__b2AStarFindPath__) */