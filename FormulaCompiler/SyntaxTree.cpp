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

		if (label == "-" || label == "n")
		{
			return setLabel(LeftVal - RightVal);
		}
		else if (label == "+" || label == "p")
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
		else if (label == "%")
		{
			return setLabel(std::fmodl(LeftVal, RightVal));
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
	mIsCountable = true;
}

SyntaxTree::~SyntaxTree()
{
	deleteTree();
	delete mRoot;
	mRoot = nullptr;
}

void SyntaxTree::BuildSyntaxTree(const std::string formula)
{
	//mStrFormula = formula;
	for each (char sym in formula)
	{
		if (!isblank(sym))
		{
			mStrFormula += sym;
		}
	}
	if (mStrFormula.empty())
	{
		ErrorHandle(EMPTY_FROMULA);
		mIsCountable = false;
		return;
	}
	else if (mStrFormula.find("0^0") != std::string::npos ||
		mStrFormula.find("0/0") != std::string::npos)
	{
		mIsCountable = false;
		ErrorHandle(ZERO_ZERO_ERROR);
		return;
	}
	Node* index = mRoot;
	std::string number;
	std::string PFormula;
	bool POn = false;
	bool rT = false;
	for each (char sym in formula)
	{
		if (sym == '(')
		{
			mParenthesesSum++;
			if (POn)
			{
				PFormula += sym;
			}
			POn = true;
			mIsCountable = false;
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
				mIsCountable = true;
				PFormula.clear();
				POn = false;
				rT = true;
				continue;
			}
		}

		if (POn)
		{
			PFormula += sym;
		}
		else
		{
			if (((sym >= '0') && (sym <= '9')) || (sym == ' ') || (sym == '.'))
			{
				if ((sym != ' '))
				{
					number += sym;
					mIsCountable = true;
				}
			}
			else
			{
				mIsCountable = false;
				bool rightAssociative = false;
				Node* RightChildren;
				std::string strSym;
				strSym += sym;
				if (!(number.empty()))
				{
					RightChildren = new Node(true, number);
					index->setRightChildren(RightChildren);
					number.clear();
				}
				else
				{
					if (!rT)
					{
						if (strSym == "+" || strSym == "-")
						{
							RightChildren = new Node(true, "0");
							rightAssociative = true;
							if (strSym == "+")
							{
								strSym = "p";
							}
							else if (strSym == "-")
							{
								strSym = "n";
							}
							index->setRightChildren(RightChildren);
						}
						else
						{
							mIsCountable = false;
							return ErrorHandle(WRONG_FROMULA);
						}
					}
					else
					{
						rT = false;
					}
				}
				if (rate.find(strSym) == rate.end())
				{
					mIsCountable = false;
					return ErrorHandle(WRONG_FROMULA);
				}
				if (sym != '^' && !rightAssociative)
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
	//mIsCountable = true;
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

bool SyntaxTree::IsCountable() const
{
	return mIsCountable;
}

long double SyntaxTree::Count()
{
	if (mParenthesesSum > 0)
	{
		ErrorHandle(PARENTHESE_UNMARCH);
		return Count();
	}
	else if (IsCountable() && mStrFormula != "")
	{
		mRoot->recountHeight();
		if ((mRoot->getHeight()) == 0)
		{
			return std::stold(mRoot->getLabel());
		}
		else
		{
			Node* leaf = FindLeaf();
			if (leaf->getLabel() == "^" || leaf->getLabel() == "/")
			{
				if (std::stold(leaf->getLeftChildren()->getLabel()) == 0 && std::stold(leaf->getRightChildren()->getLabel()) == 0)
				{
					ErrorHandle(ZERO_ZERO_ERROR);
					mIsCountable = false;
					return NAN;
				}
			}
			leaf->count();
			return Count();
		}
	}
	else
	{
		return NAN;
	}
}

void SyntaxTree::deleteTree()
{
	mRoot->recountHeight();
	delete mRoot;
	mRoot = nullptr;
	mStrFormula = "";
	Node* root = new Node(false, "root");
	mRoot = root;
	Node* LeftChildren = new Node(true, "0");
	mRoot->setLeftChildren(LeftChildren);
	mParenthesesSum = 0;
	mIsCountable = true;
}

void SyntaxTree::ErrorHandle(const int & message)
{
	if (message == WRONG_FROMULA)
	{
		std::cout << "错误： 表达式错误 ";
	}
	else if (message == PARENTHESE_UNMARCH)
	{
		std::cout << "你想输入的是不是	" << mStrFormula << std::string(mParenthesesSum, ')') << "	( Y / N )" << std::endl;
		std::string input;
		std::cin >> input;
		if (input == "Y" || input == "y")
		{
			mStrFormula += std::string(mParenthesesSum, ')');
			std::string formula = mStrFormula;
			deleteTree();
			BuildSyntaxTree(formula);
			mIsCountable = true;
		}
		else
		{
			mIsCountable = false;
		}
		mParenthesesSum = 0;
	}
	else if (message == EMPTY_FROMULA)
	{
		std::cout << "错误： 表达式为空 ";
	}
	else if (message == ZERO_ZERO_ERROR)
	{
		std::cout << "错误： 无法计算 0^0 或 0/0 ";
	}
}

