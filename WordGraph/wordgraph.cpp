//
//  wordgraph.cpp
//  WordGraph
//
//  Created by Anubhav on 10/16/14.
//  Copyright (c) 2014 AnubhavAshok. All rights reserved.
//

#include "wordgraph.h"
#include <boost/format.hpp>
#include <boost/optional.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <json/json.h>
#include <fstream>

using namespace std;

void WordGraph::printGraph()
{
    for (auto graphPair: wordGraph) {
        int idx = graphPair.first;
        // std::pair<std::map<int, int>, std::map<int, int>>
        auto val = graphPair.second;
        auto prev = val.first;
        auto next = val.second;
        for (auto p: prev) {
            std::string word = wordVector[p.first];
            int count = p.second;
            cout << (boost::format("(%s, %d)") % word % count).str();
        }
        if (prev.size() > 0) {
            cout << "--->";
        }
        cout << wordVector[idx];
        if (next.size() > 0) {
            cout << "<---";
        }
        for (auto p: next) {
            std::string word = wordVector[p.first];
            int count = p.second;
            cout << (boost::format("(%s, %d)") % word % count).str();
        }
        cout << endl << "-******-" << endl;
    }
}

void WordGraph::indexDoc(std::string docString)
{
    std::vector<std::string> strs;
    // Split strings by space or new line
    boost::split(strs, docString, boost::is_any_of(" \n"));
    boost::optional<int> prev;
    
    for (std::string word: strs) {
        
        int idx = 0;
        
        // Add word to wordVector and wordIndex
        if (!wordIndex.count(word)) {
            idx = (int) wordVector.size();
            wordVector.push_back(word);
            wordIndex[word] = idx;
        } else {
            idx = wordIndex[word];
        }
        
        // Set links
        if (prev) {
            int prevIdx = *prev;
            
            // set current word's prev to prev
            std::map<int, int>& myPrev = wordGraph[idx].first;
            if(!myPrev.count(prevIdx)) {
                myPrev[prevIdx] = 1;
            } else {
                myPrev[prevIdx]++;
            }
            
            // set prev's next to current word
            std::map<int, int>& prevsNext = wordGraph[prevIdx].second;
            if(!prevsNext.count(idx)) {
                prevsNext[idx] = 1;
            } else {
                prevsNext[idx]++;
            }
        }
        prev = idx;
    }
}

void WordGraph::outputToOrientDBFormat(boost::filesystem::path p)
{
    /*
    {
        nodes:[
            {
                "word": "Mary",
                "outgoing": [
                    {"type": "prev", "word": "a", "count":1},
                    {"type": "next", "word": "had", "count"},
                ]
            }
        ]
     }
     
     */
    
    
    ofstream out(p.string());
    Json::Value value;
    Json::Value nodes;
    for (std::string word: wordVector) {
        int idx = wordIndex[word];
        auto wordNode = wordGraph[idx];
        
        Json::Value node;
        node["word"] = word;
        
        Json::Value outgoing;
        // Add all prev relationships
        for (auto p: wordNode.first) {
            int idx = p.first;
            std::string prevWord = wordVector[idx];
            int count = p.second;
            Json::Value rel;
            rel["type"] = "prev";
            rel["word"] = prevWord;
            rel["count"] = count;
            outgoing.append(rel);
        }
        
        // Add all next relationships
        for (auto p: wordNode.second) {
            int idx = p.first;
            std::string nextWord = wordVector[idx];
            int count = p.second;
            Json::Value rel;
            rel["type"] = "next";
            rel["word"] = nextWord;
            rel["count"] = Json::Value(count);
            outgoing.append(rel);
        }
        
        node["outgoing"] = outgoing;
        nodes.append(node);
    }
    value["nodes"] = nodes;

    
    out << value << endl;
}
