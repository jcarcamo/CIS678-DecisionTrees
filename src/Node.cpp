/*
 * Node.cpp
 *
 *  Created on: Mar 2, 2017
 *      Author: jcarcamo
 */

#include "Node.h"

Node::Node(){

}

Node::Node(std::string name, std::string rule)
{
	this->name = name;
	this->rule = rule;
}

Node::~Node()
{

}

bool Node::operator == (const Node &other) const
{
   if( this->name == other.name && this->rule == other.rule )
     return true;
   else
     return false;
}

std::string Node::getName()
{
	return this->name;
}

std::string Node::getRule()
{
	return this->rule;
}

std::vector<Node> Node::getChildren()
{
	return this->children;
}

void Node::setName(std::string name)
{
	this->name = name;
}

void Node::setRule(std::string rule)
{
	this->rule = rule;
}

void Node::addChild(Node child)
{
	this->children.push_back(child);
}

void Node::removeChild(Node child)
{
	std::vector<Node>::iterator itr = std::remove(this->children.begin(),this->children.end(),child);
	this->children.erase(itr, this->children.end());
}

void Node::printNode(std::string depth){
	std::cout << depth << "Name: " << this->name<<std::endl;
	std::cout << depth << "Rule: " << this->rule << std::endl;
	depth = depth + "-";
	for(Node node : this->children)
	{
		node.printNode(depth);
	}
}
