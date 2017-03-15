/*
* Main.cpp
*
*  Created on: Feb 24, 2017
*      Author: jcarcamo
*/
#include "DecisionTreeMaker.h"

int main(int argc, char* argv[])
{
	if (argc != 2) 
	{
		std::cerr << "Incorrect Number of arguments." << std::endl;
		std::cerr << "Usage ./DecisionTrees <Training Examples File>" << std::endl;
		exit(1);
	}
	std::string filePath(argv[1]);
	
	DecisionTreeMaker dtm;
	dtm.createDecisionTree(filePath,dtm.HEADERS);
    TreeCount tc = dtm.countTree(dtm.getTree());
    std::cout << "Stats --------" << std::endl;
    std::cout << "Nodes: " << tc.nodeCount << std::endl;
    std::cout << "Leaves: " << tc.leaveCount << std::endl;
	dtm.saveTree(dtm.JSON);
	std::cout << "Finished. Press any key to exit" << std::endl;
	getchar();
}
