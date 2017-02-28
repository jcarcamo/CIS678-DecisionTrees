/*
 * DecisionTreeMaker.h
 *
 * Created on: Feb 24, 2017
 *      Author: jcarcamo
 */

#ifndef DECISIONTREEMAKER_H_
#define DECISIONTREEMAKER_H_

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <deque>
#include <vector>
#include <chrono>

// for simple analysis, and analyzedDocumentsSWData.csv for SW analysis.
//
// Additionally execution times are stored in execTime.csv and execTimeSW.csv for
//
// Example:
// 		ComplexityAnalyzer ca(unit,slidingWindowSize);
//		ca.analyzeDocument(document, type);
//		ca.analyzeDocumentSlidingWindows(document, type);

class DecisionTreeMaker {
public:
	DecisionTreeMaker();
	
        // Performs a simple analysis of a document based in the type of the document
	// inputs:
	//		std:string examples_file_path,
	//		int type: 0 examples with headers 
	//			  1 without headers
	void createDecisionTree(std::string examples_file_path,int type);

	virtual ~DecisionTreeMaker();

private:

	// Uses std::replace_if to replace invalid characters with white spaces
	// Uses std::transform to replace all the chars in the string to lowercase
	// inputs:
	//		std::string str: String to be modified
        void parseFileWithHeaders(std::string examples_file_wh);
        // Uses std::replace_if to replace invalid characters with white spaces
        // Uses std::transform to replace all the chars in the string to lowercase
        // inputs:
        //              std::string str: String to be modified        
	void parseFileNoHeaders(std::string examples_file_nh);
	
};

#endif /* DECISIONTREEMAKER_H_ */

