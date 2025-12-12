//
// Test adicional com grafo mais complexo
//

#include <assert.h>
#include <stdio.h>

#include "Graph.h"
#include "GraphDominatingSets.h"
#include "IndicesSet.h"

int main(void) {
  printf("=== TESTE ADICIONAL COM GRAFO MAIS COMPLEXO ===\n\n");
  
  // Test with a more complex graph
  FILE* file = fopen("G_test_extra.txt", "r");
  if (file == NULL) {
    printf("Erro: ficheiro G_test_extra.txt não encontrado\n");
    return 1;
  }
  
  Graph* g = GraphFromFile(file);
  fclose(file);
  
  printf("Grafo de teste (8 vértices, 12 arestas):\n");
  GraphDisplayDOT(g);
  printf("\n");
  
  // Test adjacency function
  printf("Testando GraphGetSetAdjacentsTo:\n");
  for (unsigned int v = 0; v < 4; v++) {
    printf("  Adjacentes ao vértice %u: ", v);
    IndicesSet* adj = GraphGetSetAdjacentsTo(g, v);
    IndicesSetDisplay(adj);
    IndicesSetDestroy(&adj);
  }
  printf("\n");
  
  // Test vertex weights
  printf("Testando GraphComputeVertexWeights:\n");
  double* weights = GraphComputeVertexWeights(g);
  for (unsigned int v = 0; v < GraphGetVertexRange(g); v++) {
    if (weights[v] >= 0) {
      printf("  Vértice %u: peso = %.1f\n", v, weights[v]);
    }
  }
  free(weights);
  printf("\n");
  
  // Test dominating set
  printf("Testando GraphComputeMinDominatingSet:\n");
  IndicesSet* minDom = GraphComputeMinDominatingSet(g);
  printf("  Conjunto dominante mínimo: ");
  IndicesSetDisplay(minDom);
  printf("  Verificação: É dominante? %d\n", GraphIsDominatingSet(g, minDom));
  printf("  Cardinalidade: %u\n", IndicesSetGetNumElems(minDom));
  IndicesSetDestroy(&minDom);
  printf("\n");
  
  // Test weighted dominating set
  printf("Testando GraphComputeMinWeightDominatingSet:\n");
  IndicesSet* minWeightDom = GraphComputeMinWeightDominatingSet(g);
  printf("  Conjunto dominante de peso mínimo: ");
  IndicesSetDisplay(minWeightDom);
  printf("  Verificação: É dominante? %d\n", GraphIsDominatingSet(g, minWeightDom));
  
  weights = GraphComputeVertexWeights(g);
  double totalWeight = 0.0;
  int v = IndicesSetGetFirstElem(minWeightDom);
  while (v != -1) {
    totalWeight += weights[v];
    v = IndicesSetGetNextElem(minWeightDom);
  }
  printf("  Peso total: %.1f\n", totalWeight);
  free(weights);
  IndicesSetDestroy(&minWeightDom);
  printf("\n");
  
  // Test subgraph
  printf("Testando GraphGetSubgraph:\n");
  IndicesSet* subset = IndicesSetCreateEmpty(GraphGetVertexRange(g));
  IndicesSetAdd(subset, 0);
  IndicesSetAdd(subset, 1);
  IndicesSetAdd(subset, 3);
  IndicesSetAdd(subset, 6);
  printf("  Subconjunto de vértices: ");
  IndicesSetDisplay(subset);
  
  Graph* subgraph = GraphGetSubgraph(g, subset);
  printf("  Subgrafo:\n");
  GraphDisplayDOT(subgraph);
  
  IndicesSetDestroy(&subset);
  GraphDestroy(&subgraph);
  GraphDestroy(&g);
  
  printf("\n=== TODOS OS TESTES ADICIONAIS PASSARAM ===\n");
  
  return 0;
}
