// Cristobal Sciutto's Implementation of Trailblazer
// Section Leader: Shirin Salehi

#include "trailblazer.h"
#include "queue.h"
#include "set.h"
#include "pqueue.h"
#include "point.h"
#include <thread>

using namespace std;

// this makes Path an alias for the type Vector<Vertex*>
typedef Vector<Vertex*> Path;

// the minimum difference for an alternate route
const double SUFFICIENT_DIFFERENCE = 0.2;

/*****************************************************
 Function: pathComputeCost
 -----------------------------------------------------
 ~ Modified from TrailblazerGUI file
 Computes the total cost of a path by iterating through
 it and adding up the costs of each edge.
 *****************************************************/

double pathComputeCost(RoadGraph& graph, Path& path) {
    double result = 0.0;
    for (int i = 1; i < path.size(); i++) {
        Edge* edge = graph.getEdge(path[i - 1], path[i]);
        result += edge->cost;
    }
    return result;
}

/*****************************************************
 Function: breadthFirstSearch
 -----------------------------------------------------
 Tradional BFS implementation according to the pseudo-
 code on the assignment document.
 *****************************************************/

Path breadthFirstSearch(RoadGraph& graph, Vertex* start, Vertex* end) {
    // Basic Setup for BFS
    Queue<Path> todo;
    Set<Vertex*> visited;
    // Create and enqueue initial path
    Path initial;
    initial.add(start);
    todo.enqueue(initial);

    // Main Loop
    while (!todo.isEmpty()) {
        // Iteration Setup
        Path currPath = todo.dequeue();
        Vertex* currNode = currPath[currPath.size() - 1]; // Last Element
        visited.add(currNode);
        currNode->setColor(GREEN);
        // "Pseudo base-case"
        if (currNode == end) return currPath;

        for (Vertex* nextNode : graph.getNeighbors(currNode)) {
            if (!visited.contains(nextNode)) {
                nextNode->setColor(YELLOW);
                Path newPath = currPath;
                newPath.add(nextNode);
                todo.enqueue(newPath);
            }
        }
    }
    // If path not found, return empty path
    Path emptyPath;
    return emptyPath;
}

/*****************************************************
 Function: altAStar
 -----------------------------------------------------
 Strong implmentation for AStar which allows for exe-
 cution of Djukstra, AStar, and ignoring an edge.
 Default values exectues Djikstra.
 If isAStar is true, executes AStar.
 If ignore is not NULL, ignores the edge passed in.

 Same implementation as BFS with a PriorityQueue and
 conditional priorities dependending on parameters
 passed in.
 *****************************************************/

Path altAStar(RoadGraph& graph, Vertex* start, Vertex* end,
              bool isAStar = false, Edge* ignore = NULL) {
    // Basic Setup for PQueue graph searching algorithms
    PriorityQueue<Path> todo;
    Set<Vertex*> visited;
    Path initial;
    initial.add(start);
    todo.enqueue(initial, 0);

    // Heuristic Setup
    double maxSpeed = graph.getMaxRoadSpeed();

    while (!todo.isEmpty()) {
        // Iteration Setup
        Path currPath = todo.dequeue();
        double currPriority = pathComputeCost(graph, currPath);
        Vertex* currNode = currPath[currPath.size() - 1]; // Last Element
        visited.add(currNode);
        currNode->setColor(GREEN);
        // "Pseudo base-case"
        if (currNode == end) return currPath;

        for (Vertex* nextNode : graph.getNeighbors(currNode)) {
            Edge* nextEdge = graph.getEdge(currNode, nextNode);
            // Check if edge that is being analyzed == the ignorable one
            if (!visited.contains(nextNode) && nextEdge != ignore) {
                nextNode->setColor(YELLOW);
                Path newPath = currPath;
                newPath.add(nextNode);

                // Finding cost of traversing next edge
                double crowDist = graph.getCrowFlyDistance(nextNode, end);
                // If isAStar is false, heuristicCost == 0.
                double heuristicCost = (crowDist / maxSpeed) * isAStar;
                double priority = currPriority + nextEdge->cost + heuristicCost;
                todo.enqueue(newPath, priority);
            }
        }
    }
    // If path not found, return empty path
    Path emptyPath;
    return emptyPath;
}

/*****************************************************
 Function: dijkstrasAlgorithm
 -----------------------------------------------------
 Executes the stronger altAStar with correct params for
 dijkstra. See docs for altAStar for more info.
 *****************************************************/

Path dijkstrasAlgorithm(RoadGraph& graph, Vertex* start, Vertex* end) {
    return altAStar(graph, start, end, false);
}

/*****************************************************
 Function: aStar
 -----------------------------------------------------
 Executes the stronger altAStar with correct params for
 aStar. See docs for altAStar for more info.
 *****************************************************/

Path aStar(RoadGraph& graph, Vertex* start, Vertex* end) {
    return altAStar(graph, start, end, true);
}

/*****************************************************
 Function: setFromPath
 -----------------------------------------------------
 Returns set of nodes in a path.
 *****************************************************/

Set<Vertex*> setFromPath(Path& p) {
    Set<Vertex*> s;
    for (Vertex* v : p) s.add(v);
    return s;
}

/*****************************************************
 Function: pathDiff
 -----------------------------------------------------
 Calculates the path difference between the best path
 and a alternative path passed in by creating sets of
 their nodes and using set operations to find the int-
 ersection of nodes. Then, the value is computed using
 the sizes of the distinct sets according to the formula.
 *****************************************************/

double pathDiff(Path& best, Path& alt) {
    Set<Vertex*> bestSet = setFromPath(best);
    Set<Vertex*> altSet = setFromPath(alt);
    Set<Vertex*> inter = bestSet * altSet;

    double inBnotP = bestSet.size() - inter.size();
    double inB = bestSet.size();

    return inBnotP / inB;
}

/*****************************************************
 Function: alternativeRoute
 -----------------------------------------------------
 1. Finds best path using aStar
 2. Loops through each edge on the bestPath, executing
    altAStar by ignoring that edge.
 3. Computes the pathDiff and cost of the new alternative
    path, and if the best, stores it in a variable.
 4. Returns the best alternative path if found, else
    returns an empty path indicating there are no
    alternative paths.
 *****************************************************/

Path alternativeRoute(RoadGraph& graph, Vertex* start, Vertex* end) {
    // Best Path Setup
    Path bestPath = aStar(graph, start, end);
    double bestAltCost = -1; // Initial value for bestAltCost
    Path bestAltPath;

    // Loop over every edge on best path
    for (int i = 1; i < bestPath.size(); i++) {
        // Determine the alternative path
        Edge* ignore = graph.getEdge(bestPath[i - 1], bestPath[i]);
        Path currAltPath = altAStar(graph, start, end, true, ignore); // Switiching to false would use Dijkstra
        // Calculute comparison values
        double diff = pathDiff(bestPath, currAltPath);
        double currAltPathCost = pathComputeCost(graph, currAltPath);
        // If valid, update best alt path or if first alternative cost
        // Also make sure path found is not empty, which satisfy the other conditions, yet is not valid
        if (diff > SUFFICIENT_DIFFERENCE &&
                (currAltPathCost < bestAltCost || bestAltCost == -1) &&
                !currAltPath.isEmpty()) {
            bestAltPath = currAltPath;
            bestAltCost = currAltPathCost;
        }
    }
    return bestAltPath;
}
