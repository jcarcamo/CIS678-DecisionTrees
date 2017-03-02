/*
 *  DecisionTreeMaker.cpp
 *
 *  Created on: Feb 24, 2017
 *      Author: jcarcamo
 */

#include "DecisionTreeMaker.h"

DecisionTreeMaker::DecisionTreeMaker() 
{

}

DecisionTreeMaker::~DecisionTreeMaker() 
{

}

void DecisionTreeMaker::parseFileWithHeaders(std::string examples_file_wh) 
{
	std::string line;
        std::ifstream inputFile (examples_file_wh);
        if (inputFile.is_open())
	{
		std::cout << "With Headers" << std::endl;
        	while ( getline (inputFile,line) )
        	{
			std::cout << line << std::endl;
		}
	} else {
                std::cerr << "Unable to open input file!" << std::endl;
        }
	inputFile.close();
}

void DecisionTreeMaker::parseFileNoHeaders(std::string examples_file_nh)
{
        std::string line;
        std::ifstream inputFile (examples_file_nh);
        if (inputFile.is_open())
        {
                std::cout << "No Headers" << std::endl;
		while ( getline (inputFile,line) )
        	{
	                std::cout << line << std::endl;
                }
        } else {
                std::cerr << "Unable to open input file!" << std::endl;
        }
	inputFile.close();
}

void DecisionTreeMaker::id3(){
	this->tree.setName("Forecast");
	this->tree.setRule("None");

	Node child1("Wind","Sunny");
	Node child2("Yes","Cloudy");
	Node child3("Airtemp","Rainy");

	Node child1a("Yes","Strong");
	Node child1b("Water","Weak");

	Node child1b1("No","Cold");
	Node child1b2("Yes","Moderate");
	Node child1b3("No","Warm");

	Node child3a("No","Cool");
	Node child3b("Wind","Warm");

	Node child3b1("Yes","Strong");
	Node child3b2("No","Weak");

	child1b.addChild(child1b1);
	child1b.addChild(child1b2);
	child1b.addChild(child1b3);

	child3b.addChild(child3b1);
	child3b.addChild(child3b2);

	child1.addChild(child1a);
	child1.addChild(child1b);

	child3.addChild(child3a);
	child3.addChild(child3b);


	this->tree.addChild(child1);
	this->tree.addChild(child2);
	this->tree.addChild(child3);


	std::cout << "Coming Soon, meanwhile see this beautiful tree:" <<std::endl;
	this->tree.printNode("|-");
}

void DecisionTreeMaker::createDecisionTree(std::string examples_file_path, int type) {
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	switch(type){
		case 0:
			this->parseFileWithHeaders(examples_file_path);
			//this->id3();
			break;
		case 1:
			this->parseFileNoHeaders(examples_file_path);
			this->id3();
			break;
		default:
			std::cerr << "Undefined type of example file!" << std::endl;
			break;
	}
	
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
	std::string execTimeFileName = "./execTime.csv";
	std::cout << execTimeFileName << "(in microseconds)" << duration << std::endl;
	/*
	std::string execTimeFileName = "./execTime.csv";
	std::ofstream resultFile(execTimeFileName, std::ios_base::app);
	if(resultFile.is_open())
	{
		std::cout << execTimeFileName << "(in microseconds)" << duration << std::endl;
		resultFile << duration << std::endl;
		resultFile.close();
	} else {
		std::cout << execTimeFileName << "(in microseconds)" << duration << std::endl;
	}
	*/
}

