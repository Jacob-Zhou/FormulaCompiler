#pragma once
#include <string>
#include <memory>
#include <vector>
#include <limits>
#include <map>

const int WRONG_FROMULA = 1;
const int PARENTHESE_UNMARCH = 2;
const int EMPTY_FROMULA = 3;
const int ZERO_ZERO_ERROR = 4;

class Node
{
public:
	Node();
	Node(const bool& isNumber, const std::string& label);
	virtual ~Node();

	void setPatent(Node* parent);
	Node* getParent();

	void setLeftChildren(Node* leftChildren);
	Node* getLeftChildren();

	void setRightChildren(Node* rightChildren);
	Node* getRightChildren();

	Node* InsertParent(const std::string& label);
	Node* InsertRightChildren(const std::string& label);

	size_t getHeight() const;
	void recountHeight();

	bool IsNumber() const;

	std::string getLabel() const;

	long double count();

private:

	long double setLabel(const long double& label);
	bool mIsNumber;

	std::string mLabel;
	size_t mHeight;
	Node* mParent;
	Node* mLeftChildren;
	Node* mRightChildren;
};

class SyntaxTree
{
public:
	SyntaxTree();
	~SyntaxTree();

	void BuildSyntaxTree(const std::string formula);

	Node* FindLeaf();
	Node* getRoot();

	bool IsCountable() const;
	long double Count();

public:
	std::map<std::string, long long> rate
	{
		{ "root", 0 },
		{ "-", 1 },
		{ "+", 1 },
		{ "*", 2 },
		{ "/", 2 },
		{ "%", 2 },
		{ "^", 3 },
		{ "p", 4 },
		{ "n", 4 },
		{ "(", 5 },
	};
private:
	Node* mRoot;
	void deleteTree();
	void ErrorHandle(const int& message);
	std::string mStrFormula;
	std::vector<Node*> mParenthesesLocal;
	size_t mParenthesesSum;
	bool mIsCountable;
};

