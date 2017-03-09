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
	if(this->examples) // p3 not null means p2 was freed by std::realloc
		std::free(this->examples);
}

std::vector<std::string> DecisionTreeMaker::split(const char *str, char c)
{
	std::vector<std::string> result;

	do
	{
		const char *begin = str;

		while(*str != c && *str)
			str++;

		std::string item(std::string(begin, str));
		if (!item.empty() && item[item.size() - 1] == '\r')
			item.erase(item.size() - 1);
		result.push_back(item);
	} while (0 != *str++);

	return result;
}

int DecisionTreeMaker::findFeatureValueIndex(std::vector<std::string> feature, std::string featureValue)
{
	int index = -1;
	auto it = std::find(feature.begin(), feature.end(), featureValue);
	if (it != feature.end())
	{
	  index = std::distance(feature.begin(), it);
	}

	return index;
}

void DecisionTreeMaker::printExamples()
{
        unsigned int i,j;
        unsigned int cols = features.size()+1;
        unsigned int rows = this->numberOfExamples;
        for (i=0; i < rows; i++) {
        		for (j=0; j < cols; j++)
                        std::cout << this->examples[i*cols+j] << ",";
                std::cout << std::endl;
        }
}

void DecisionTreeMaker::parseFileWithHeaders(std::string examples_file_wh) 
{
	std::string line;
	std::ifstream inputFile (examples_file_wh);
	if (inputFile.is_open())
	{
		std::cout << "With Headers" << std::endl;
		bool isData = false;
		int headValue = 0;
		int dataToRead = 0;
		int lineIndex = 0;
		while ( getline (inputFile,line) )
		{
			std::vector<std::string> splitted = this->split(line.c_str(), ',');
			if(!isData)
			{
				lineIndex = 0;
				dataToRead++;
				headValue = atoi(splitted.at(0).c_str());
				isData = true;
				if(dataToRead == EXAMPLES)
				{
					this->numberOfExamples = headValue;
					this->examples = (int*)std::malloc(this->numberOfExamples*(this->features.size() + 1)*sizeof(int));
				}

			} else {
				switch(dataToRead){
					case CLASSES:{
						for (std::string lineElement:splitted)
						{
							this->classes.push_back(lineElement);
						}
						isData = false;
						break;
					}
					case FEATURES:{
						std::string name(splitted.at(0));
						std::vector< std::string >feature;
						for(unsigned int i = 2; i < splitted.size(); i++)
						{
							feature.push_back(splitted.at(i));
						}
						this->features.push_back(feature);
						if(lineIndex < headValue-1)
						{
							lineIndex++;
						} else {
							isData = false;
						}

						break;
					}
					case EXAMPLES:{
						int featureValue;
						unsigned int cols = this->features.size() + 1;
						for(unsigned int j = 0; j < splitted.size(); j++)
						{

							if(j != splitted.size()-1)
							{
								featureValue = this->findFeatureValueIndex(this->features.at(j),splitted.at(j));

							} else {
								//This looks for the oracle answer index in classes vector
								featureValue = this->findFeatureValueIndex(this->classes,splitted.at(j));
							}
							this->examples[lineIndex*cols+j] = featureValue;
						}
						if(lineIndex < headValue - 1)
						{
							lineIndex++;
						} else {
							isData = false;
						}
						break;
					}
					default:{
						break;
					}
				}
			}
		}
		inputFile.close();

		std::cout << "CLASSES" << std::endl;
		for(std::string _class:this->classes)
		{
			std::cout << _class << "\t";
		}
		std::cout << "\nFEATURES" << std::endl;
		for(std::vector< std::string > feature:this->features)
		{
			for(std::string featureValue:feature){
				std::cout << featureValue << "\t";
			}
			std::cout << std::endl;
		}
		std::cout << "\nEXAMPLES" << std::endl;
		this->printExamples();
	} else {
		std::cerr << "Unable to open input file!" << std::endl;
		exit(1);
	}
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


	std::cout << "ID3 Coming Soon, meanwhile see this beautiful tree:" <<std::endl;
	this->tree.printNode("|-");

	std::cout << "Now as a JSON" << std::endl;
	std::string json = JSON::producer<Node>::convert(this->tree);
	std::cout << json << std::endl;
}

void DecisionTreeMaker::createDecisionTree(std::string examples_file_path, int type) {
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	switch(type){
	case 0:
		this->parseFileWithHeaders(examples_file_path);
		//this->id3();
		break;
	case 1:
		//this->parseFileNoHeaders(examples_file_path);
		//this->id3();
		break;
	default:
		std::cerr << "Undefined type of example file!" << std::endl;
		break;
	}

	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
	std::string execTimeFileName = "./execTime.csv";
	std::cout << execTimeFileName << "(in microseconds) " << duration << std::endl;
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

