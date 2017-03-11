/*
* Feature.h
*
*  Created on: Mar 11, 2017
*      Author: jcarcamo
*/

#ifndef FEATURE_H_
#define FEATURE_H_

#include <string>
#include <vector>
class Feature
{
public:
	Feature();
	virtual ~Feature();
	void setName(std::string name);
	void setValues(std::vector< std::string > values);

	std::string getName();
	std::vector< std::string > getValues();

private:
	std::string name;
	std::vector< std::string > values;

};
#endif /* FEATURE_H_ */
