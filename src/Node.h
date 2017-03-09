/*
 * Node.h
 *
 *  Created on: Mar 2, 2017
 *      Author: jcarcamo
 */

#ifndef NODE_H_
#define NODE_H_

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <json_adapter.h>

class Node {
public:
	Node();
	Node(std::string name, std::string rule);
	bool operator==(const Node &other) const;

	virtual ~Node();

	void setName(std::string name);
	void setRule(std::string rule);
	void addChild(Node child);
	void removeChild(Node child);

	std::vector<Node> getChildren();
	std::string getName();
	std::string getRule();

	//Needed for esj to work
	void serialize(JSON::Adapter& adapter);

	void printNode(std::string depth);

private:
	std::string name;
	std::string rule;
	std::vector<Node> children;

};

#endif /* NODE_H_ */
