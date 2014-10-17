//
//  wordgraph.h
//  WordGraph
//
//  Created by Anubhav on 10/16/14.
//  Copyright (c) 2014 AnubhavAshok. All rights reserved.
//

#ifndef __WordGraph__wordgraph__
#define __WordGraph__wordgraph__

#include <iostream>
#include <vector>
#include <map>
#include <boost/filesystem.hpp>


class WordGraph {
public:
    // methods
    // add a document to the wordgraph
    void indexDoc(std::string docString);
    void printGraph();
    void outputToOrientDBFormat(boost::filesystem::path p);
    
private:
    // map of word idx to prev words and
    std::map<int, std::pair<std::map<int, int>, std::map<int, int>>> wordGraph;
    
    // map of all words
    std::map<std::string, int> wordIndex;
    
    // reverse map
    std::vector<std::string> wordVector;
    
};


#endif /* defined(__WordGraph__wordgraph__) */
