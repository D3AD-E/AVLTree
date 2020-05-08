#pragma once
#include <string>
#include <iostream>
template <class Key, class Info>
class AVLtree
{
private:
	struct Node
	{
		Node* left;
		Node* right;
		Key key;
		Info info;
	};
public:
	AVLtree()
	{
		root = nullptr;
	}
	~AVLtree()
	{
		clear(root);
	}
	AVLtree(const AVLtree& other)
	{
		root = copyNode(other.root);
	}
	AVLtree& operator =(const AVLtree& other)
	{
		clear();
		root = copyNode(other.root);
	}
	
	bool remove(const Key& key)
	{
		return remove(key, root);;
	}
	bool getInfo(const Key& key, Info& info)
	{
		Node* node = find(key, root);
		if (node)
		{
			info = node->info;
			return true;
		}
		else
			return false;
	}
	bool replace(const Key& key, const Info& info)
	{
		Node* toplace = find(key, root);
		if (toplace)
		{
			toplace->info = info;
			return true;
		}
		return false;
	}
	bool contains(const Key& key)
	{
		if (find(key, root))
			return true;
		else
			return false;
	}
	bool insert(const Key& key, const Info& info)
	{
		bool placed;
		root = insert(key, info, root, placed);
		if (!placed)
			return false;
		return true;
	}
	bool isEmpty()
	{
		if (root == nullptr)
			return true;
		return false;
	}

	void finsert(const Key& key, const Info& info)
	{
		root = finsert(key, info, root);
	}
	void clear()
	{
		clear(root);
		root = nullptr;
	}

	void print()
	{
		if (root)
		{
			print(root, "", false);
			std::cout << "Number of elements =" << count(root) << std::endl;
			std::cout << "Balance factor of root=" << balFactor(root) << std::endl;
		}
		else
		{
			std::cout << "Tree is empty" << std::endl;
		}
	}
	void inorder()
	{
		if (root)
		{
			std::cout << "Key " << "\t" << "Info" << std::endl;
			inorder(root);
		}
		else
		{
			std::cout << "Tree is empty" << std::endl;
		}
	}
	void preorder()
	{
		if (root)
		{
			std::cout << "Key " << "\t" << "Info" << std::endl;
			preorder(root);
		}
		else
		{
			std::cout << "Tree is empty" << std::endl;
		}
	}
	void postorder()
	{
		if (root)
		{
			std::cout << "Key " << "\t" << "Info" << std::endl;
			postorder(root);
		}
		else
		{
			std::cout << "Tree is empty" << std::endl;
		}
	}

	int count()
	{
		return count(root);
	}

private:

	int height(Node* leaf)
	{
		int h = 0;
		if (leaf)
		{
			int height_left = height(leaf->left);
			int height_right = height(leaf->right);
			int max_height = (height_left < height_right) ? height_right : height_left;
			h = max_height + 1;
		}
		return h;
	}
	int balFactor(Node* leaf)
	{
		if (!leaf)
			return 0;
		int hl = height(leaf->left);
		int hr = height(leaf->right);
		int diff = hl - hr;
		return diff;
	}
	int count(Node* leaf)
	{
		int c = 0;
		if (leaf)
		{
			int cl = count(leaf->left);
			int cr = count(leaf->right);
			c = cl+cr + 1;
		}
		return c;
	}

	void clear(Node* leaf)
	{
		if (leaf != nullptr)
		{
			clear(leaf->left);
			clear(leaf->right);
			delete leaf;
			leaf = nullptr;
		}
	}
	void print(const Node* node, const std::string& prefix, bool isLeft)
	{
		if (node != nullptr)
		{
			std::cout << prefix;
			if (node == root)
				std::cout << "h--";
			else
				std::cout << (isLeft ? "l-- " : "r-- ");

			std::cout << node->key << std::endl;

			print(node->right, prefix + (isLeft ? "    " : "|   "), false);
			print(node->left, prefix + (isLeft ? "    " : "|   "), true);
		}

	}
	void inorder(Node* leaf)
	{
		if (!leaf)
			return;
		inorder(leaf->left);
		std::cout << leaf->key << "\t" << leaf->info << std::endl;
		inorder(leaf->right);
	}
	void preorder(Node* leaf) 
	{
		if (!leaf)
			return;
		std::cout << leaf->key << "\t" << leaf->info << std::endl;
		preorder(leaf->left);
		preorder(leaf->right);
	}
	void postorder(Node* leaf) 
	{
		if (!leaf)
			return;
		postorder(leaf->left);
		postorder(leaf->right);
		std::cout << leaf->key << "\t" << leaf->info << std::endl;
	}
	void findParent(Node* leaf, Node* pred, Node* curr, Node*& parent)
	{
		if (!curr)
			return;
		if (curr == leaf)
		{
			parent = pred;
			return;
		}
		else
		{
			findParent(leaf, curr, curr->left, parent);
			findParent(leaf, curr, curr->right, parent);
		}
	}

	bool remove(const Key& key, Node* leaf)
	{
		Node* parent = root;
		leaf = find(key, leaf, parent);
		if (root == nullptr || leaf == nullptr)
			return false;
		if (root->key == key && root->left == nullptr && root->right == nullptr)
		{
			delete root;
			root = nullptr;
			return true;
		}
		else
		{
			if ((leaf->left == nullptr) || (leaf->right == nullptr))
			{
				Node* temp = leaf->left ? leaf->left :leaf->right;
				if (temp == nullptr)
				{
					if (parent->left == leaf)
					{
						parent->left = nullptr;
					}
					else if(parent->right == leaf)
					{
						parent->right = nullptr;
					}
					else
					{
						findParent(leaf, root, root, parent);

						if (parent->left == leaf)
						{
							parent->left = nullptr;
						}
						else if (parent->right == leaf)
						{
							parent->right = nullptr;
						}
					}
					delete leaf;
					leaf = nullptr;
					if (parent == root)
						root = balance(root);
					else
						parent = balance(parent);
					return true;
				}
				else
				{
					leaf->key = temp->key;
					leaf->info = temp->info;
					leaf->left = nullptr;
					leaf->right = nullptr;
					delete temp;
					temp = nullptr;
					if (parent == root)
						root = balance(root);
					else
						parent = balance(parent);
					return true;
				}
			}
			else
			{
				Node* temp = minValueNode(leaf->right);
				leaf->key = temp->key;
				leaf->info = temp->info;
				remove(temp->key, leaf->right);
			}
		}
		return true;
	}

	Node* minValueNode(Node* node)
	{
		Node* current = node;

		/* loop down to find the leftmost leaf */
		while (current->left !=nullptr)
			current = current->left;

		return current;
	}
	Node* insert(const Key& key, const Info& info, Node* leaf, bool& placed)
	{
		if (leaf == nullptr)
		{
			leaf = new Node;
			leaf->key = key;
			leaf->info = info;
			leaf->left = nullptr;
			leaf->right = nullptr;
			placed = true;
		}
		else if (key > leaf->key)
		{
			leaf->right = insert(key, info, leaf->right, placed);
			leaf = balance(leaf);
		}
		else if(key <  leaf->key)
		{
			leaf->left = insert(key, info, leaf->left, placed);
			leaf = balance(leaf);
		}
		else if (key == leaf->key)
		{
			placed = false;
		}
		return leaf;
	}
	Node* finsert(const Key& key, const Info& info, Node* leaf)
	{
		if (leaf == nullptr)
		{
			leaf = new Node;
			leaf->key = key;
			leaf->info = info;
			leaf->left = nullptr;
			leaf->right = nullptr;
		}
		else if (key > leaf->key)
		{
			leaf->right = finsert(key, info, leaf->right);
			leaf = balance(leaf);
		}
		else if (key < leaf->key)
		{
			leaf->left = finsert(key, info, leaf->left);
			leaf = balance(leaf);
		}
		else if (key == leaf->key)
		{
			leaf->info = info;
		}
		return leaf;
	}
	Node* find(const Key& key, Node* leaf)
	{
		if (leaf == nullptr)
			return nullptr;
		if (leaf->key == key)
			return leaf;
		if (leaf->key > key)
			return find(key, leaf->left);
		else
			return find(key, leaf->right);
	}
	Node* find(const Key& key, Node* leaf, Node*& parent)
	{
		if (leaf == nullptr)
			return nullptr;
		if (leaf->key == key)
			return leaf;
		parent = leaf;
		if (leaf->key > key)
		{
			return find(key, leaf->left, parent);
		}
		else
		{
			return find(key, leaf->right, parent);
		}
	}
	Node* balance(Node* leaf)
	{
		if (!leaf)
			return leaf;
		int diff = balFactor(leaf);
		if (diff > 1)
		{
			if (balFactor(leaf->left) > 0)
				leaf = rotation_rr(leaf);
			else
			{
				leaf = rotation_lr(leaf);
			}
		}
		else if (diff < -1)
		{
			if (balFactor(leaf->right) > 0)
				leaf = rotation_rl(leaf);
			else
			{
				leaf = rotation_ll(leaf);
			}
		}
		return leaf;
	}
	Node* rotation_rr(Node* parent)
	{
		Node* temp;
		temp = parent->left;
		parent->left = temp->right;
		temp->right = parent;
		return temp;
	}
	Node* rotation_ll(Node* parent)
	{
		Node* temp;
		temp = parent->right;
		parent->right = temp->left;
		temp->left = parent;
		return temp;
	}
	Node* rotation_lr(Node* parent)
	{
		Node* temp;
		temp = parent->left;
		parent->left = rotation_ll(temp);
		return rotation_rr(parent);
	}
	Node* rotation_rl(Node* parent)
	{
		Node* temp;
		temp = parent->right;
		parent->right = rotation_rr(temp);
		return rotation_ll(parent);
	}
	Node* copyNode(Node* leaf)
	{
		if (leaf != nullptr)
		{
			Node* left = copyNode(leaf->left);
			Node* right = copyNode(leaf->right);
			Node* newnode = new Node;
			newnode->left = left;
			newnode->right = right;
			newnode->key = leaf->key;
			newnode->info = leaf->info;
			return newnode;
		}
		else
		{
			return nullptr;
		}
	}

	Node* root;
};

