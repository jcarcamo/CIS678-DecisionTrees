#include "DecisionTreeMaker.h"

int main()
{

	DecisionTreeMaker dtm;
	dtm.createDecisionTree("/home/jcarcamo/git/CIS678-DecisionTrees/data/fishing.data",0);
        std::cout<<"Press any key to continue"<<std::endl;
	getchar();
	dtm.createDecisionTree("/home/jcarcamo/git/CIS678-DecisionTrees/data/fishing.data",1);
}
