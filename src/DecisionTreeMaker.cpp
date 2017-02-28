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

void DecisionTreeMaker::createDecisionTree(std::string examples_file_path, int type) {
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	switch(type){
		case 0:
			this->parseFileWithHeaders(examples_file_path);
			break;
		case 1:
			this->parseFileNoHeaders(examples_file_path);
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

