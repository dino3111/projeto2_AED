//
// Algoritmos e Estruturas de Dados --- 2025/2026
//
// Joaquim Madeira - Nov 2025
//
// GraphDominatingSets - Computing Vertex Dominating Sets for UNDIRECTED graphs
//

// Student Name : Maria Moreira Mané
// Student Number : 125102
// Student Name : Claudino José Martins
// Student Number : 127368

/*** COMPLETE THE GraphIsDominatingSet FUNCTION ***/
/*** COMPLETE THE GraphMinDominatingSet FUNCTION ***/
/*** COMPLETE THE GraphMinWeightDominatingSet FUNCTION ***/

#include "GraphDominatingSets.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "IndicesSet.h"
#include "instrumentation.h"

//
// TO BE COMPLETED
//
// Check if the given set is a dominating set for the graph
// Return 1 if true, or 0 otherwise
//
// A dominating set is a set of graph vertices such that every other
// graph vertex not in the set is adjacent to a graph vertex in the set
//
int GraphIsDominatingSet(const Graph* g, IndicesSet* vertSet) {
  assert(g != NULL);
  assert(GraphIsDigraph(g) == 0);
  assert(IndicesSetIsEmpty(vertSet) == 0);

  // Get all vertices in the graph
  IndicesSet* allVertices = GraphGetSetVertices(g);
  int v = IndicesSetGetFirstElem(allVertices);
  
  // Check each vertex in the graph
  while (v != -1) {
    // If vertex v is not in the dominating set
    if (!IndicesSetContains(vertSet, v)) {
      // It must be adjacent to at least one vertex in the dominating set
      int isDominated = 0;
      IndicesSet* adjacents = GraphGetSetAdjacentsTo(g, v);
      
      int neighbor = IndicesSetGetFirstElem(adjacents);
      while (neighbor != -1) {
        if (IndicesSetContains(vertSet, neighbor)) {
          isDominated = 1;
          break;
        }
        neighbor = IndicesSetGetNextElem(adjacents);
      }
      
      IndicesSetDestroy(&adjacents);
      
      if (!isDominated) {
        IndicesSetDestroy(&allVertices);
        return 0;
      }
    }
    v = IndicesSetGetNextElem(allVertices);
  }
  
  IndicesSetDestroy(&allVertices);
  return 1;
}

//
// TO BE COMPLETED
//
// Compute a MIN VERTEX DOMINATING SET of the graph
// using an EXHAUSTIVE SEARCH approach
// Return the/a dominating set
//
IndicesSet* GraphComputeMinDominatingSet(const Graph* g) {
  assert(g != NULL);
  assert(GraphIsDigraph(g) == 0);

  unsigned int range = GraphGetVertexRange(g);
  IndicesSet* currentSet = IndicesSetCreateEmpty(range);
  IndicesSet* bestSet = NULL;
  IndicesSet* validVertices = GraphGetSetVertices(g);

  // Exhaustive search: iterate through all possible subsets of vertices
  do {
    // Optimization: skip if currentSet contains invalid vertices
    if (!IndicesSetIsSubset(currentSet, validVertices)) continue;
    
    // Optimization: skip empty sets (cannot be dominating)
    if (IndicesSetIsEmpty(currentSet)) continue;

    // Optimization: pruning by cardinality - skip if already larger than best
    if (bestSet != NULL && IndicesSetGetNumElems(currentSet) >= IndicesSetGetNumElems(bestSet)) {
       continue;
    }

    if (GraphIsDominatingSet(g, currentSet)) {
      if (bestSet == NULL || IndicesSetGetNumElems(currentSet) < IndicesSetGetNumElems(bestSet)) {
        if (bestSet != NULL) IndicesSetDestroy(&bestSet);
        bestSet = IndicesSetCreateCopy(currentSet);
      }
    }
  } while (IndicesSetNextSubset(currentSet));

  IndicesSetDestroy(&currentSet);
  IndicesSetDestroy(&validVertices);
  
  if (bestSet == NULL) {
      return IndicesSetCreateEmpty(range);
  }
  return bestSet;
}

//
// TO BE COMPLETED
//
// Compute a MIN WEIGHT VERTEX DOMINATING SET of the graph
// using an EXHAUSTIVE SEARCH approach
// Return the dominating set
//
IndicesSet* GraphComputeMinWeightDominatingSet(const Graph* g) {
  assert(g != NULL);
  assert(GraphIsDigraph(g) == 0);

  unsigned int range = GraphGetVertexRange(g);
  IndicesSet* currentSet = IndicesSetCreateEmpty(range);
  IndicesSet* bestSet = NULL;
  double minWeight = -1.0;
  
  double* vertexWeights = GraphComputeVertexWeights(g);
  IndicesSet* validVertices = GraphGetSetVertices(g);

  // Exhaustive search: iterate through all possible subsets
  do {
    if (!IndicesSetIsSubset(currentSet, validVertices)) continue;
    if (IndicesSetIsEmpty(currentSet)) continue;

    // Calculate the total weight of the current subset
    double currentWeight = 0.0;
    int v = IndicesSetGetFirstElem(currentSet);
    while (v != -1) {
      if (vertexWeights[v] != -1.0) {
          currentWeight += vertexWeights[v];
      }
      v = IndicesSetGetNextElem(currentSet);
    }

    // Optimization: pruning by weight - skip if already heavier than best
    if (bestSet != NULL && currentWeight >= minWeight) {
        continue;
    }

    if (GraphIsDominatingSet(g, currentSet)) {
      if (bestSet == NULL || currentWeight < minWeight) {
        if (bestSet != NULL) IndicesSetDestroy(&bestSet);
        bestSet = IndicesSetCreateCopy(currentSet);
        minWeight = currentWeight;
      }
    }
  } while (IndicesSetNextSubset(currentSet));

  IndicesSetDestroy(&currentSet);
  IndicesSetDestroy(&validVertices);
  free(vertexWeights);

  if (bestSet == NULL) {
      return IndicesSetCreateEmpty(range);
  }
  return bestSet;
}

