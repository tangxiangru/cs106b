/*
 * CS 106B Trailblazer
 * This file declares the functions you will write in this assignment.
 * 
 * !!! DO NOT MODIFY THIS FILE !!!
 * !!! DO NOT MODIFY THIS FILE !!!
 * !!! DO NOT MODIFY THIS FILE !!!
 *
 * !!!IF YOU MODIFY THIS FILE, YOU *WILL* LOSE POINTS ON YOUR GRADE!!!
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @author Chris Piech, Marty Stepp, based on past code by Keith Schwarz
 * @version 2016/11/30 (initial version for 14au)
 */

#ifndef _trailblazer_h
#define _trailblazer_h

#include "basicgraph.h"
#include "set.h"
#include "vector.h"
#include "Color.h"
#include "RoadGraph.h"

/*
 * The path-searching algorithms you must implement.
 */

Vector<Vertex*> breadthFirstSearch(RoadGraph& graph, Vertex* start, Vertex* end);
Vector<Vertex*> dijkstrasAlgorithm(RoadGraph& graph, Vertex* start, Vertex* end);
Vector<Vertex*> aStar(RoadGraph& graph, Vertex* start, Vertex* end);
Vector<Vertex*> alternativeRoute(RoadGraph& graph, Vertex* start, Vertex* end);

#endif

/*
 * !!! DO NOT MODIFY THIS FILE !!!
 * !!! DO NOT MODIFY THIS FILE !!!
 * !!! DO NOT MODIFY THIS FILE !!!
 */

