/*
 * Node.cpp
 *
 *  Created on: Mar 2, 2017
 *      Author: jcarcamo
 */

#include "Node.h"

Node::Node(){
	this->name = "";
	this->rule = "";
}

Node::Node(std::string name, std::string rule, unsigned long index)
{
	this->name = name;
	this->rule = rule;
	this->index = index;
}

Node::~Node()
{

}

bool Node::operator == (const Node &other) const
{
   if( this->name == other.name && this->rule == other.rule && this->index == other.index )
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

long Node::getIndex()
{
	return this->index;
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

void Node::setIndex(long index)
{
	this->index = index;
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

void Node::printNode(std::string depth)
{
	std::cout << depth << "Name: " << this->name<<std::endl;
	std::cout << depth << "Rule: " << this->rule << std::endl;
	std::cout << depth << "Index: " << this->index << std::endl;
	depth = depth + "-";
	for(Node node : this->children)
	{
		node.printNode(depth);
	}
}

bool Node::isEmpty() 
{
	bool empty = false;
	if ( this->name == "" && this->rule == "" && this->children.empty()) {
		empty = true;
	}
	return empty;
}

void Node::serialize(JSON::Adapter& adapter)
{
	JSON::Class root(adapter, "Node");
	JSON_E(adapter, name);
	JSON_E(adapter, rule);
	JSON_E(adapter, index);
	JSON_T(adapter, children);
}
