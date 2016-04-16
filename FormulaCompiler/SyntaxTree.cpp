#include "SyntaxTree.h"
#include <cmath>
#include <iostream>



Node::Node()
	{
	mIsNumber = true;
	mLabel = "0";
	mHeight = 0;
	mParent = nullptr;
	mRightChildren = nullptr;
	mLeftChildren = nullptr;
	}

Node::Node(const bool & isNumber, const std::string & label)
	{
	mIsNumber = isNumber;
	mLabel = label;
	if (isNumber)
		{
		mHeight = 0;
		}
	else
		{
		mHeight = 1;
		}
	mParent = nullptr;
	mRightChildren = nullptr;
	mLeftChildren = nullptr;
	}


Node::~Node()
	{
	delete mLeftChildren;
	delete mRightChildren;
	mLeftChildren = nullptr;
	mRightChildren = nullptr;
	};

void Node::setPatent(Node* parent)
	{
	mParent = parent;
	}

Node* Node::getParent()
	{
	return mParent;
	}

void Node::setLeftChildren(Node* leftChildren)
	{
	mLeftChildren = leftChildren;
	recountHeight();
	}

Node* Node::getLeftChildren()
	{
	if (mLeftChildren != nullptr)
		{
		return mLeftChildren;
		}
	else
		{
		return this;
		}
	}

void Node::setRightChildren(Node* rightChildren)
	{
	mRightChildren = rightChildren;
	recountHeight();
	}

Node* Node::getRightChildren()
	{
	if (mRightChildren != nullptr)
		{
		return mRightChildren;
		}
	else
		{
		return this;
		}
	}

Node* Node::InsertParent(const std::string & label)
	{
	if ((this->getParent())->getRightChildren() == this)
		{
		Node* newParent = new Node(false, label);
		newParent->setLeftChildren(this);
		(this->getParent())->setRightChildren(newParent);
		newParent->setPatent(this->getParent());
		this->setPatent(newParent);
		return newParent;
		}
	return this;
	}

Node* Node::InsertRightChildren(const std::string & label)
	{
	if ((this->getParent() == nullptr) || ((this->getParent())->getRightChildren() == this))
		{
		Node* newRightChildren = new Node(false, label);
		newRightChildren->setLeftChildren(this->getRightChildren());
		(this->getRightChildren())->setPatent(newRightChildren);
		this->setRightChildren(newRightChildren);
		newRightChildren->setPatent(this);
		return (this->getRightChildren());
		}
	return this;
	}

size_t Node::getHeight() const
	{
	if (mIsNumber)
		{
		return 0;
		}
	else
		{
		return mHeight;
		}
	}

void Node::recountHeight()
	{
	if (mIsNumber)
		{
		mHeight = 0;
		}
	else
		{
		mHeight = (((getLeftChildren()->getHeight()) + (getRightChildren()->getHeight())) + 1);
		}
	}

bool Node::IsNumber() const
	{
	return mIsNumber;
	}

std::string Node::getLabel() const
	{
	return mLabel;
	}

long double Node::count()
	{
	if (mHeight == 1)
		{
		long double LeftVal(0.0), RightVal(0.0);
		std::string label(mLabel);
		LeftVal = std::stold((this->getLeftChildren())->getLabel());
		RightVal = std::stold((this->getRightChildren())->getLabel());
		mHeight = 0;
		mIsNumber = true;
		if (label != "root")
			{
			(this->getParent())->recountHeight();
			}
		(this->getLeftChildren())->~Node();
		(this->getRightChildren())->~Node();
		this->setLeftChildren(nullptr);
		this->setRightChildren(nullptr);
		if (label == "-")
			{
			return setLabel(LeftVal - RightVal);
			}
		else if (label == "+")
			{
			return setLabel(LeftVal + RightVal);
			}
		else if (label == "root")
			{
			return setLabel(LeftVal + RightVal);
			}
		else if (label == "(")
			{
			return setLabel(LeftVal + RightVal);
			}
		else if (label == "*")
			{
			return setLabel(LeftVal * RightVal);
			}
		else if (label == "/")
			{
			return setLabel(LeftVal / RightVal);
			}
		else if (label == "^")
			{
			return setLabel(std::pow(LeftVal, RightVal));
			}
		}
	return 0;
	}

long double Node::setLabel(const long double & label)
	{
	mLabel = std::to_string(label);
	return label;
	}

SyntaxTree::SyntaxTree()
	{
	Node* root = new Node(false, "root");
	mRoot = root;
	Node* LeftChildren = new Node(true, "0");
	mRoot->setLeftChildren(LeftChildren);
	mParenthesesSum = 0;
	}

SyntaxTree::~SyntaxTree()
	{
	delete mRoot;
	mRoot = nullptr;
	}

void SyntaxTree::BuildSyntaxTree(const std::string formula)
	{
	Node* index = mRoot;
	std::string number;
	std::string PFormula;
	bool POn = false;
	for each (char sym in formula)
		{
		if (sym == '(')
			{
			mParenthesesSum++;
			if(POn)
				{
				PFormula += sym;
				}
			POn = true;
			continue;
			}
		else if (sym == ')')
			{
			mParenthesesSum--;
			if (mParenthesesSum == 0)
				{
				mParenthesesLocal.push_back(index);
				SyntaxTree* InsideTree = new SyntaxTree;
				InsideTree->BuildSyntaxTree(PFormula);

				Node* RightChildren = new Node(false, "(");
				RightChildren->setPatent(index);
				index->setRightChildren(RightChildren);
				index = index->getRightChildren();
				Node* LeftChildren = new Node(true, "0");
				index->setLeftChildren(LeftChildren);

				((InsideTree->getRoot())->getRightChildren())->setPatent(index);
				index->setRightChildren((InsideTree->getRoot())->getRightChildren());

				PFormula.clear();
				POn = false;
				continue;
				}
			}
		if (POn)
			{
			PFormula += sym;
			}
		else
			{
			if (((sym >= '0') && (sym <= '9')) || (sym == ' '))
				{
				if ((sym != ' '))
					{
					number += sym;
					}
				}
			else
				{
				if (!(number.empty()))
					{
					Node* RightChildren = new Node(true, number);
					index->setRightChildren(RightChildren);
					number.clear();
					}
				std::string strSym;
				strSym += sym;
				if (sym != '^')
					{
					while (!(rate[strSym] > rate[index->getLabel()]))
						{
						index = (index->getParent());
						}
					}
				else
					{
					while (!(rate[strSym] >= rate[index->getLabel()]))
						{
						index = (index->getParent());
						}
					}
				index = (index->InsertRightChildren(strSym));
				}
			}
		}
	if (!(number.empty()))
		{
		Node* RightChildren = new Node(true, number);
		index->setRightChildren(RightChildren);
		number.clear();
		}
	}



Node* SyntaxTree::FindLeaf()
	{
	Node* finder = mRoot;
	finder->recountHeight();
	while (finder->getHeight() > 1)
		{
		if (((finder->getLeftChildren())->getHeight()) > ((finder->getRightChildren())->getHeight()))
			{
			finder = (finder->getLeftChildren());
			}
		else
			{
			finder = (finder->getRightChildren());
			}
		finder->recountHeight();
		}
	return finder;
	}

Node * SyntaxTree::getRoot()
	{
	return mRoot;
	}

long double SyntaxTree::Count()
	{
	mRoot->recountHeight();
	if ((mRoot->getHeight()) == 0)
		{
		return std::stold(mRoot->getLabel());
		}
	else
		{
		FindLeaf()->count();
		return Count();
		}
	}

