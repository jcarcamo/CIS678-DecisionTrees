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
	if(this->examples)
		std::free(this->examples);
}

Node DecisionTreeMaker::getTree(){
    return this->tree;
}
TreeCount DecisionTreeMaker::countTree(Node node)
{
    TreeCount tc;

    if (node.getChildren().empty())
    {
        tc.leaveCount = 1;
    }
    else
    {
        tc.nodeCount = 1;
        for ( Node child:node.getChildren() ){
            TreeCount childTc = countTree(child);
            tc.nodeCount += childTc.nodeCount;
            tc.leaveCount += childTc.leaveCount;
        }
    }
    
    return tc;
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

long DecisionTreeMaker::findValueIndex(std::vector<std::string> feature, std::string featureValue)
{
	long index = -1;
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
				headValue = atol(splitted.at(0).c_str());
				isData = true;
				if(dataToRead == EXAMPLES)
				{
					this->numberOfExamples = headValue;
					this->examples = (long*)std::malloc(this->numberOfExamples*(this->features.size() + 1)*sizeof(long));
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
						Feature feature;
						feature.setName(splitted.at(0));

						std::vector< std::string >currentFeature;
						for(unsigned int i = 2; i < splitted.size(); i++)
						{
							currentFeature.push_back(splitted.at(i));
						}
						feature.setValues(currentFeature);
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
								featureValue = this->findValueIndex(this->features.at(j).getValues(),splitted.at(j));

							} else {
								//This looks for the oracle answer index in classes vector
								featureValue = this->findValueIndex(this->classes,splitted.at(j));
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
		for(Feature feature:this->features)
		{
			std::cout << feature.getName() << ":" << std::endl;
			for(std::string featureValue:feature.getValues()){
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

double DecisionTreeMaker::findSetEntropy(long* S, unsigned long size) {
	unsigned long cols = this->features.size() + 1; //because the last column is actually the oracle
	unsigned long oracle = this->features.size();
	std::vector<int> classCount(this->classes.size(), 0);
	for (unsigned long i = 0; i < size; i++)
	{
		unsigned long row = S[i];
		classCount[this->examples[row*cols + oracle]] += 1;
	}

	double entropy = 0.0;
	double probability = 0.0;
	for (unsigned long i = 0; i < classCount.size(); i++)
	{
		if (classCount[i] != 0)
		{
			probability = classCount[i] / (float)size;
			entropy -= (probability)*std::log2(probability);
		}
		else {
			probability = 0.0;
			entropy -= probability;
		}
	}

	return entropy;
}

bool DecisionTreeMaker::areAllExamplesFromOneClass(long* S, unsigned long size)
{
	bool resultsFromOneClass = false;
	unsigned long cols = this->features.size() + 1; //because the last column is actually the oracle
	unsigned long oracle = this->features.size();
	std::vector<int> classCount(this->classes.size(), 0);
	for (unsigned long i = 0; i < size; i++)
	{
		unsigned long row = S[i];
		classCount[this->examples[row*cols + oracle]] += 1;
	}
	
	for (unsigned long i = 0; i < classCount.size() && !resultsFromOneClass; i++)
	{
		if (classCount[i] == size) {
			resultsFromOneClass = true;
		}
	}
	
	return resultsFromOneClass;
}

long DecisionTreeMaker::getTheMostCommonClassIndex(long* S, unsigned long size)
{
	long mostCommonOneClassIndex = -1;
	unsigned long cols = this->features.size() + 1; //because the last column is actually the oracle
	unsigned long oracle = this->features.size();
	std::vector<int> classCount(this->classes.size(), 0);
	for (unsigned long i = 0; i < size; i++)
	{
		unsigned long row = S[i];
		classCount[this->examples[row*cols + oracle]] += 1;
	}
	
	int maxClassCount = 0;
	for (unsigned long i = 0; i < classCount.size(); i++)
	{
		if ( maxClassCount < classCount[i] ) {
			maxClassCount = classCount[i];
			mostCommonOneClassIndex = i;
		}
	}

	return mostCommonOneClassIndex;
}

bool DecisionTreeMaker::hasFeatureBeenTested(long* testedAttributes, unsigned long sizeOfTestedAttributes, long attribute) 
{
	bool attributeFound = false;
	unsigned long i;
	for (i = 0; i < sizeOfTestedAttributes; i++)
	{
		if (testedAttributes[i] == attribute) 
		{
			attributeFound = true;
		}
	}
	return attributeFound;
}

long DecisionTreeMaker::getMaxInformationGainIndexAttribute(long* S, unsigned long sizeOfS, long* testedAttributes, unsigned long sizeOfTestedAttributes)
{
	double setEntropy = this->findSetEntropy(S, sizeOfS);
	long cols = this->features.size() + 1; //because the last col is the oracle
	
	long maxIGindex = -1;
	double maxIG = -1000.0; //it could be that information gain is negative, so we choose an absurd value so this won't affect us
	double IG = 0.0;

	for (unsigned long featureIndex = 0; featureIndex < cols - 1; featureIndex++)
	{
		if (!this->hasFeatureBeenTested(testedAttributes, sizeOfTestedAttributes, featureIndex)) {
			long sizeOfValues = this->features[featureIndex].getValues().size();
			double sumIG = 0.0;
			for (unsigned long valueIndex = 0; valueIndex < sizeOfValues; valueIndex++)
			{
				long sizeOfSv = 0;
				for (unsigned long sIndex = 0; sIndex < sizeOfS; sIndex++)
				{
					unsigned long row = S[sIndex];
					if (this->examples[row*cols + featureIndex] == valueIndex)
					{
						sizeOfSv++;
					}
					
				}
				long* Sv = (long*)std::malloc(sizeOfSv * sizeof(long));
				for (unsigned long sIndex = 0, svIndex = 0; sIndex < sizeOfS; sIndex++)
				{
					unsigned long row = S[sIndex];
					if (this->examples[row*cols + featureIndex] == valueIndex)
					{
						Sv[svIndex] = row;
						svIndex++;
					}
				}
				sumIG -= (sizeOfSv/(float)sizeOfS)*this->findSetEntropy(Sv, sizeOfSv);
				std::free(Sv);
			}
			IG = setEntropy + sumIG;
			if (maxIG < IG) 
			{
				maxIG = IG;
				maxIGindex = featureIndex;
			}

		}
	}
	return maxIGindex;
}

Node DecisionTreeMaker::id3(long* S, unsigned long sizeOfS, long* testedAttributes, unsigned long sizeOfTestedAttributes)
{
	Node node;
	unsigned long cols = this->features.size() + 1; //because the last column is actually the oracle
	unsigned long oracle = this->features.size();
	if (this->areAllExamplesFromOneClass(S, sizeOfS)) //all examples in S are of the same class
	{
		long classIndex = this->examples[S[0] * cols + oracle];
		node.setName(this->classes.at(classIndex));
		node.setIndex(classIndex);
	} 
	else if (sizeOfTestedAttributes == this->features.size()) //there are no more attributes to test
	{
		long classIndex = this->getTheMostCommonClassIndex(S, sizeOfS);
		node.setName(this->classes.at(classIndex));
		node.setIndex(classIndex);
	}
	else 
	{
		long chosenAttribute = this->getMaxInformationGainIndexAttribute(S, sizeOfS, testedAttributes, sizeOfTestedAttributes);
		
		long* currentTestedAttributes = (long*)std::malloc((sizeOfTestedAttributes+1) * sizeof(long));
		for (long i = 0; i < sizeOfTestedAttributes; i++)
		{
			currentTestedAttributes[i] = testedAttributes[i];
		}
		currentTestedAttributes[sizeOfTestedAttributes] = chosenAttribute;
		unsigned long currentSizeOfTestedAttributes = sizeOfTestedAttributes + 1;

		node.setName(this->features.at(chosenAttribute).getName());
		node.setIndex(chosenAttribute);
		long sizeOfSv = 0;
		for (int i = 0; i < this->features.at(chosenAttribute).getValues().size(); i++)
		{
			sizeOfSv = 0;
			for (unsigned long sIndex = 0; sIndex < sizeOfS; sIndex++)
			{
				unsigned long row = S[sIndex];
				if (this->examples[row*cols + chosenAttribute] == i)
				{
					sizeOfSv++;
				}

			}
			long* Sv = (long*)std::malloc(sizeOfSv * sizeof(long));
			for (unsigned long sIndex = 0, svIndex = 0; sIndex < sizeOfS; sIndex++)
			{
				unsigned long row = S[sIndex];
				if (this->examples[row*cols + chosenAttribute] == i)
				{
					Sv[svIndex] = row;
					svIndex++;
				}
			}
			
			Node childNode = id3(Sv, sizeOfSv, currentTestedAttributes, currentSizeOfTestedAttributes);
			childNode.setRule(this->features.at(chosenAttribute).getValues()[i]);
			node.addChild(childNode);
			std::free(Sv);
		}
		std::free(currentTestedAttributes);
	}
	
	return node;
}

void DecisionTreeMaker::saveTree(int fileType)
{
	if (!this->tree.isEmpty())
	{
		switch (fileType)
		{
			case JSON: {
				std::string fileName = "./decision_tree.json";
				std::string json = JSON::producer<Node>::convert(this->tree);
				std::ofstream resultFile(fileName, std::ios_base::app);
				if (resultFile.is_open())
				{
					resultFile << json << std::endl;
					resultFile.close();
				}
				else {
					std::cerr << "Error saving the tree file!" << std::endl;
				}
				break;
			}
			case ASCII: {
				break;
			}
			default: {
				break;
			}
		}
	}
	else 
	{
		std::cerr << "No tree has been created yet, Are you sure you already calledDecisionTreeMaker::createDecisionTree(std::string examples_file_path, int type) " << std::endl;
	}
}

void DecisionTreeMaker::createDecisionTree(std::string examples_file_path, int type) {
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	switch(type){
	case 0:
		this->parseFileWithHeaders(examples_file_path);
		//validate that examples have already been loaded, a minor safeguard
		if (this->examples) {
			long* S;
			S = (long*)std::malloc(this->numberOfExamples*sizeof(long));
			for (long i = 0; i < this->numberOfExamples; i++) 
			{
				S[i] = i;
			}
			this->tree = this->id3(S, this->numberOfExamples,NULL, 0);
			std::cout << "Regular Print:" << std::endl;
			this->tree.printNode("|-");

			std::cout << "JSON String" << std::endl;
			std::string json = JSON::producer<Node>::convert(this->tree);
			std::cout << json << std::endl;
		}
		break;
	case 1:
		//TODO
		break;
	default:
		std::cerr << "Undefined type of example file!" << std::endl;
		break;
	}

	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
	std::cout << "Time (in microseconds)," << duration << std::endl;
	
}

