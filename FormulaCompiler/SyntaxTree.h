#pragma once
#include <string>
#include <memory>
#include <vector>
#include <limits>
#include <map>

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

		long double Count();

	public:
		std::map<std::string, long long> rate
			{
					{"root", 0},
					{"-", 1},
					{"+", 1},
					{"*", 2},
					{"/", 2},
					{"^", 3},
					{"(", 4},
			};
	private:
		Node* mRoot;
		std::vector<Node*> mParenthesesLocal;
		size_t mParenthesesSum;
	};

