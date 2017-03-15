/*
 * DecisionTreeMaker.h
 *
 * Created on: Feb 24, 2017
 *      Author: jcarcamo
 */

#ifndef DECISIONTREEMAKER_H_
#define DECISIONTREEMAKER_H_

#include "Node.h"
#include "Feature.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cmath>
#include <vector>
#include <iterator>
#include <chrono>
#include <cstdlib>
#include <json_writer.h>

// Example:
// 		DecisionTreeMaker dtm;
//		dtm.createDecisionTree(document, type);

class TreeCount{
public:
    TreeCount(){
        this->nodeCount  = 0;
        this->leaveCount = 0;
    };
    long nodeCount;
    long leaveCount;
};

class DecisionTreeMaker {
public:
	enum fileType {
		ASCII,
		JSON,
	};

	enum {
		HEADERS,
		NOHEADERS,
	};
	DecisionTreeMaker();
	
	// inputs:
	//		std:string examples_file_path,
	//		int type: 0 examples with headers 
	//			  1 without headers
	void createDecisionTree(std::string examples_file_path,int type);
    Node getTree();
	void saveTree(int fileType);
    TreeCount countTree(Node node);
	virtual ~DecisionTreeMaker();

private:
	enum {
		CLASSES=1,
		FEATURES=2,
		EXAMPLES=3,
	};

	void parseFileWithHeaders(std::string examples_file_wh);

	void parseFileNoHeaders(std::string examples_file_nh);
	
	std::vector<std::string> split(const char *str, char c = ' ');
	long findValueIndex(std::vector<std::string> feature, std::string featureValue);
	void printExamples();

	bool areAllExamplesFromOneClass(long* S, unsigned long size);
	bool hasFeatureBeenTested(long* testedAttributes, unsigned long sizeOfTestedAttributes, long attribute);
	
	long getTheMostCommonClassIndex(long* S, unsigned long size);
	long getMaxInformationGainIndexAttribute(long* S, unsigned long sizeOfS, long* testedAttributes, unsigned long sizeOfTestedAttributes);

	double findSetEntropy(long* S, unsigned long size);
	
	Node id3(long* S, unsigned long size, long* testedAttributes, unsigned long sizeOfTestedAttributes);
	
	Node tree;
	std::vector< std::string > classes;
	std::vector< Feature > features;
	long* examples;
	unsigned long numberOfExamples;

};

#endif /* DECISIONTREEMAKER_H_ */

