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
	dtm.createDecisionTree(filePath,0);
    //std::cout<<"Press any key to continue"<<std::endl;
	//getchar();
	//dtm.createDecisionTree(filePath,1);
	std::cout << "Finished. Press any key to exit" << std::endl;
	getchar();
}
