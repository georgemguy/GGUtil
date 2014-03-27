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

#include "b2AStarFindPath.h"
#include <limits> // for numeric_limits
#include <utility> // for pair
#include <algorithm>
#include <iterator>
#include "Box2D/Box2D.h"

#define VERBOSE 0

using namespace std;

typedef enum
{
    South,
    SouthEast,
    East,
    NorthEast,
    North,
    NorthWest,
    West,
    SouthWest,
    COUNT,
    NO_WAY = -1
} Direction;

const b2Vec2 wayVectors[ COUNT ] =
{
    b2Vec2(0,-1),
    b2Vec2(1,-1),
    b2Vec2(1,0),
    b2Vec2(1,1),
    b2Vec2(0,1),
    b2Vec2(-1,1),
    b2Vec2(-1,0),
    b2Vec2(-1,-1)
};

const b2Vec2 WayToVector( int way )
{
    return wayVectors[way % COUNT];
}

/*
 * SearchQuery is the region query used to check whether a given region is occupied within the
 * physics simulation.
 */
class SearchQuery : public b2QueryCallback
{
public:
    /*
     * A fixture from the body representing the agent for which we are
     * pathfinding.
     *
     * Any fixture of a static body that "searcher" would collide with is
     * considered to block "searcher" for purposes of pathfinding.
     *
     * Any fixture of a dynamic body that "searcher" would collide with does not
     * block "searcher" but is given a weight within the pathfinding equal to
     * its mass.
     */
    b2Fixture* searcher;
    
    /*
     * The first fixture found that fulfills the criteria for blocking a region.
     */
    b2Fixture* blocker;
    
    /*
     * The
     */
    const uint16 blockedBits;
    const uint16 heavyBits;
    float weight;
    
    SearchQuery(b2Fixture* s, uint16 bb, uint16 hb) :
    searcher(s), blockedBits(bb), heavyBits(hb), weight(0), blocker(NULL)
    {
        
    }
    
    bool ReportFixture(b2Fixture* fixture)
    {
        if(fixture->GetBody() != searcher->GetBody() &&
           fixture->GetBody()->IsActive() &&
           b2ContactFilter().ShouldCollide(searcher, fixture))
        {
            const b2Filter& filter = fixture->GetFilterData();
            if( ( ! fixture->IsSensor() && fixture->GetBody()->GetType() == b2_staticBody ) ||
               filter.categoryBits & blockedBits )
            {
                blocker = fixture;
            }
            else if(!fixture->IsSensor() || filter.categoryBits & heavyBits)
            {
                b2MassData massData;
                fixture->GetMassData(&massData);
                weight += massData.mass;
            }
        }
        
        return !blocker;//keep going to find all fixtures in the query area
    }
};

b2Pathfinder::Path b2Pathfinder::traceFromFar()
{
    if(farExplored.best == explored.end()) return Path();
    return tracePath(**farExplored.best);
}

b2Pathfinder::Path b2Pathfinder::traceFromBest()
{
    if(bestExplored.best == explored.end()) return Path();
    return tracePath(**bestExplored.best);
}

b2Pathfinder::Path b2Pathfinder::traceFromInteresting()
{
    if(interestingExplored.best == explored.end()) return Path();
    return tracePath(**interestingExplored.best);
}

b2Pathfinder::Path b2Pathfinder::traceFromDestination()
{
    if(destinationVertex.x != 0 || destinationVertex.y != 0)
    {
        Path ret = tracePath(destinationVertex);
        ret.insert(ret.begin(), goal);
        return ret;
    }
    return Path();
}

b2Pathfinder::Path b2Pathfinder::traceFromFinal()
{
    if(finalVertex.x != 0 || finalVertex.y != 0)
    {
        Path ret = tracePath(finalVertex);
        return ret;
    }
    return Path();
}

b2Pathfinder::Path b2Pathfinder::tracePath(const vertex& end)
{
    Path path;
    
    for(vertex point = end; ; point = **point.previous)
    {
        path.push_back(point.toVector(pawn->GetAABB(0)));
        if(path.size() > explored.size() + 1)
        {
            b2Assert(false);
        }
        
        if( point.previous == explored.end() ) break;
    }
    
    return path;
}

bool b2Pathfinder::aStarFindPath()
{
    b2Assert(!explored.size());
    
    const b2World* world = pawn->GetBody()->GetWorld();
    b2AABB sourceBox = pawn->GetAABB(0);
    const spot source = spot( sourceBox, 0, b2Distance(goal, sourceBox.GetCenter()), explored.end() );
    
    const b2Vec2 difference = goal - source.region.GetCenter();
    b2AABB goalRegion = source.region;
    goalRegion.lowerBound += difference;
    goalRegion.upperBound += difference;
    
    b2AABB goalPoint;
    goalPoint.lowerBound = goal;
    goalPoint.upperBound = goal;
    
    unexplored.insert(source);
    
    //vertex::ExploredSet::const_iterator best = explored.end();
    bestExplored = BestRecord(explored.end(), source.heuristic * source.heuristic);
    interestingExplored = BestRecord(explored.end(), source.heuristic * -source.heuristic);
    farExplored = BestRecord(explored.end(), 0);
    
    while (!unexplored.empty())
    {
        const spot here = *unexplored.begin();
        unexplored.erase(unexplored.begin());
        
        b2Assert(here.region.upperBound.y == here.region.upperBound.y);
        const float value = here.min_distance + here.heuristic;
        
        pair<vertex::ExploredSet::const_iterator, bool> insertionResult = explored.insert(unique_ptr<vertex>(new vertex(here.region, source.region, here.previous, value)));
        const vertex::ExploredSet::const_iterator vertexU = insertionResult.first;
        
        if( bestExplored.best == explored.end() ) bestExplored = BestRecord(vertexU, value);
        else bestExplored.judgeBest(vertexU, here.heuristic);
        
        if( interestingExplored.best == explored.end() ) interestingExplored = BestRecord(vertexU, value);
        else interestingExplored.judgeInteresting(vertexU, here.heuristic);
        
        if( farExplored.best == explored.end() ) farExplored = BestRecord(vertexU, value);
        else farExplored.judgeFar(vertexU, here.heuristic);
        
        for( int w = South; w < COUNT; w++ )
        {
            b2Vec2 to = WayToVector(w);
            
            to *=
            w % 2 ? b2Distance(source.region.lowerBound, source.region.upperBound) :
            w % 4 ? source.region.upperBound.x - source.region.lowerBound.x :
            source.region.upperBound.y - source.region.lowerBound.y;
            
            b2AABB v = here.region;
            v.lowerBound += to;
            v.upperBound += to;
            b2Assert(v.upperBound.y == v.upperBound.y);
            
            float weight = (v.GetCenter() - here.region.GetCenter()).Length();
            b2Assert(weight == weight);
            float distance_through_u = here.min_distance + weight;
            
            unique_ptr<vertex> vertexV = unique_ptr<vertex>(new vertex(v, source.region, vertexU, distance_through_u));
            
            if(((**vertexU).previous == explored.end() ||
                *vertexV != **(**vertexU).previous) &&
               blocked.find(*vertexV) == blocked.end() )
            {
                spot next = spot(v, distance_through_u, b2Distance(v.GetCenter(), goal), vertexU);
                
                if((limit >= 0 && (unexplored.size() > limit ||
                   explored.size() > limit ||
                   blocked.size() > limit)) ||
                   next.region.Contains(goalPoint))
                {
                    vector<spot> candidates;
                    candidates.push_back(next);
                    spot prime = next;
                    
                    for(multiset<spot>::const_iterator s = unexplored.begin(); s != unexplored.end(); s++)
                    {
                        if( s->min_distance + s->heuristic <= prime.min_distance + prime.heuristic )
                        {
                            candidates.push_back(*s);
                        }
                    }
                    
                    prime = candidates[rand() % candidates.size()];
                    if(prime.previous != explored.end()) finalVertex = **prime.previous;
                }
                
                if( next.region.Contains(goalPoint) )
                {
                    destinationVertex = *vertexV;
                    return true;
                }
                
                multiset<spot>::iterator unexplored_match = unexplored         .end();
                pair<multiset<spot>::iterator, multiset<spot>::iterator> same_range = unexplored.equal_range(next);
                for(multiset<spot>::iterator same = same_range.first; same != same_range.second; same++)
                {
                    if( *same == next ) unexplored_match = same;
                }
                
                vertex::ExploredSet::iterator explored_match = explored.find(vertexV);
                if( explored_match != explored.end())
                {
                    
                }
                else if( unexplored_match != unexplored.end() )
                {
                    if( distance_through_u < unexplored_match->min_distance )
                    {
                        unexplored.erase(unexplored_match);
                        unexplored.insert(next);
                    }
                }
                else
                {
                    SearchQuery regionQuery = SearchQuery(pawn, blockedBits, heavyBits);
                    world->QueryAABB(&regionQuery, v);
                    if( !regionQuery.blocker )
                    {
                        next.heuristic += regionQuery.weight * courtesy;
                        unexplored.insert(next);
                    }
                    else
                    {
                        blocked.insert(*vertexV);
                    }
                }
            }
        }
        
        if(!unexplored.size() ||
           ((limit >= 0) && (unexplored.size() > limit ||
           explored.size() > limit ||
           blocked.size() > limit)))
        {
#if VERBOSE
            b2Log("b2AStarFindPath() -- pathfinding exhausted; limit = %u, unexplored = %u, explored = %u, blocked = %u", limit, unexplored.size(), explored.size(), blocked.size());
#endif
            return false;
        }
    }
    
    return false;
}

float b2Pathfinder::pathLength(const b2Pathfinder::Path& path)
{
    float pathLength = 0.0f;
    for(b2Pathfinder::Path::const_iterator p = path.begin(); p != path.end(); p++)
    {
        b2Pathfinder::Path::const_iterator q = p + 1;
        
        if( q != path.end() )
        {
            float distance = b2Distance(*q, *p);
            pathLength += distance;
        }
    }
    
    return pathLength;
}
