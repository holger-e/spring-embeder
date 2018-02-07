#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/BarycenterHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/basic/simple_graph_alg.h>
#include <iostream>
#include <vector>

using namespace ogdf;

/*int r[] = {
    0, 1, 2, 3, 4, 5, 5, 6, 7, 8, 9, 9, 10, 10, 11, 12, 12,
    13, 14, 14, 15, 16, 17, 18, 18, 19, 19, 20, 21, 22, 22,
    22, 23, 23, 23, 23, 24, 25, 26, 27, 27, 27, 28, 29, 29,
    29, 30, 30, 31, 31, 31, 32, 33, 33, 34, 34, 35, 35, 35,
    35, 0, 1, 2, 3, 5, 6, 7, 8, 10, 11, 12, 14, 15, 16, 18,
    19, 20, 21, 22, 23, 25, 27, 29, 30, 31, 32, 33, 34, 35, -1
};*/

//int r [] = {}; // levels of vertices
std::vector<int> r;

int main()
{
    int n = 2277;
    // parse input
    for (int i = 0; i < 2277; ++i){
        int x;
        std::cin >> x;
        r.push_back(x);
    }
    
    
    Graph G;
    GraphAttributes GA(G,
      GraphAttributes::nodeGraphics |
      GraphAttributes::edgeGraphics |
      GraphAttributes::nodeLabel |
      GraphAttributes::edgeStyle |
      GraphAttributes::nodeStyle |
      GraphAttributes::nodeTemplate);
    // "unix-history-time.gml"
    if (!GraphIO::read(GA, G, "bigGraph.gml", GraphIO::readGML)) {
        std::cerr << "Could not load bigGraph.gml" << std::endl;
        return 1;
    }
    
    if (!isSimpleUndirected(G)){
        std::cout << "!!!" << std::endl;
        makeSimpleUndirected(G);
    }
        
    
    NodeArray<int> rank(G);
    int i = 0;
    for(node v : G.nodes)
        rank[v] = r[i++];
    SugiyamaLayout SL;
    SL.setCrossMin(new BarycenterHeuristic);//BarycenterHeuristic
    SL.arrangeCCs(false);
    OptimalHierarchyLayout *ohl = new OptimalHierarchyLayout;
    ohl->layerDistance(30.0);
    ohl->nodeDistance(25.0);
    ohl->weightBalancing(0.7);
    SL.setLayout(ohl);
    SL.call(GA, rank);
    std::cout << "Number of crossings: " << SL.numberOfCrossings() << std::endl;
    // "output-unix-history-hierarchical-ranking.svg"
    GraphIO::write(GA, "layering_bigGraph.gml", GraphIO::writeGML);//drawSVG
    /*std::vector<node> NL;
    G.allNodes(&NL);
    for (node v : NL){
        std::cout << GA.x(v) << "," << std::endl;
    }*/
    return 0;
}
