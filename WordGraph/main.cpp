//
//  main.cpp
//  WordGraph
//
//  Created by Anubhav on 10/16/14.
//  Copyright (c) 2014 AnubhavAshok. All rights reserved.
//

#include <iostream>
#include "wordgraph.h"

int main(int argc, const char * argv[])
{

    WordGraph wordGraph;
    
    wordGraph.indexDoc("Mary was a little girl");
    wordGraph.indexDoc("Mary had a little lamb");
    // wordGraph.printGraph();
    wordGraph.outputToOrientDBFormat("/Users/anubhav/Desktop/WordGraph/graph.json");
    return 0;
}

