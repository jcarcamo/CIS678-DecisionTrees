/*
* Feature.cpp
*
*  Created on: Mar 11, 2017
*      Author: jcarcamo
*/
#include "Feature.h"

Feature::Feature()
{
}


Feature::~Feature()
{
}

void Feature::setName(std::string name)
{
	this->name = name;
}

void Feature::setValues(std::vector< std::string > values)
{
	this->values = values;
}

std::string Feature::getName()
{
	return this->name;
}

std::vector< std::string > Feature::getValues()
{
	return this->values;
}