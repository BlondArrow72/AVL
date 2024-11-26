#pragma once

template<class T>
class AVL {
public:
    struct Node {
        Node* left;
        Node* right;
        T value;
        int height;

        Node(T v) : left(nullptr), right(nullptr), value(v), height(1) {}
    };

    AVL() {
        root = nullptr;
        numItems = 0;
    }

    ~AVL() {
        clear();
    }

    const Node* getRootNode() const {
        return root;
    }

    bool insert(T item) {
        return insertHelper(root, item);
    }

    bool remove(T item) {
        bool retVal = removeHelper(root, item);
        rebalance(root);
        return retVal;
    }

    bool contains(T item) const {
        return containsHelper(root, item);
    }

    void clear() {
        clearHelper(root);
        root = nullptr;
        numItems = 0;
    }

    int size() const {
        return numItems;
    }

private:
    Node* root;
    int numItems;

    bool insertHelper(Node* &tree, T item) {
        if (tree == nullptr) {
            // case where tree is empty
            Node* newItem = new Node(item);
            tree = newItem;
            numItems++;
            root = newItem;
            return true;
        }
        if (tree->value == item) {
            // case where item already in tree
            return false;
        }
        else if (tree->value > item) {
            // case where item less than current node
            if (tree->left == nullptr) {
                // case where item needs to be inserted as left leaf
                Node* newItem = new Node(item);
                tree->left = newItem;
                numItems++;
                updateHeight(tree);
                return true;
            }
            else { // recurse left
                bool retVal = insertHelper(tree->left, item);
                if (retVal) {
                    rebalance(tree);
                }
                return retVal;
            }
        }
        else {
            // case where item greater than current node
            if (tree->right == nullptr) {
                // case where item needs to be inserted as right leaf
                Node* newItem = new Node(item);
                tree->right = newItem;
                numItems++;
                updateHeight(tree);
                return true;
            }
            else { // recurse right
                bool retVal = insertHelper(tree->right, item);
                if (retVal) {
                    rebalance(tree);
                }
                return retVal;
            }
        }
    }

    bool removeHelper(Node* &tree, T item) {
        if (tree == nullptr) {
            // case where tree is empty
            return false;
        }
        if (tree->value == item) {
            // case where found item to remove
            if ((tree->left == nullptr) && (tree->right == nullptr)) {
                // remove leaf node
                delete tree;
                numItems--;
                tree = nullptr;
            }
            else if (tree->left == nullptr) {
                // remove node with right child
                Node* temp = tree;
                tree = tree->right;
                delete temp;
                numItems--;
            }
            else if (tree->right == nullptr) {
                //remove node with left child
                Node* temp = tree;
                tree = tree->left;
                delete temp;
                numItems--;
            }
            else {
                // remove node with two children
                Node* inOrderPredecessor = tree->left;
                while (inOrderPredecessor->right != nullptr) {
                    inOrderPredecessor = inOrderPredecessor->right;
                }
                tree->value = inOrderPredecessor->value;
                removeHelper(tree->left, inOrderPredecessor->value);
            }
            return true;
        }
        else if (tree->value > item) {
            // recurse left
            bool retVal = removeHelper(tree->left, item);
            if (retVal) {
                rebalance(tree);
            }
            return retVal;
        }
        else { // recurse right
            bool retVal = removeHelper(tree->right, item);
            if (retVal) {
                rebalance(tree);
            }
            return retVal;
        }
    }

    bool containsHelper(Node* tree, T item) const {
        if (tree == nullptr) {
            // traversed to leaf
            return false;
        }
        else if (tree->value == item) {
            // found item
            return true;
        }
        else if (tree->value > item) {
            // recurse left
            return containsHelper(tree->left, item);
        }
        else { // recurse right
            return containsHelper(tree->right, item);
        }
    }

    void clearHelper(Node* tree) {
        if (tree == nullptr) {
            // tree is empty
            return;
        }
        clearHelper(tree->left);
        clearHelper(tree->right);
        delete tree;
    }

    int max(int val1, int val2) {
	    if (val1 > val2) {
		    return val1;
	    }
        else {
		    return val2;
	    }
    }

    int getHeight(Node* &tree) {
	    if (tree == nullptr) {
		    // if nullptr, height 0
            return 0;
	    }
        else {
		    return tree->height;
	    }
    }

    void updateHeight(Node* &tree) {
        // don't update height of nullptr
        if (tree == nullptr) {
            return;
        }

	    // get height of children
	    int leftHeight = getHeight(tree->left);
	    int rightHeight = getHeight(tree->right);

	    // tree height is height of tallest child + 1
	    tree->height = max(leftHeight, rightHeight) + 1;
    }

    void promoteLeft(Node* &tree) {
	    // promote left child
	    Node* temp = tree;
	    tree = temp->left;
	    temp->left = tree->right;
	    tree->right = temp;

	    // update heights
	    updateHeight(temp);
	    updateHeight(tree);
    }

    void promoteRight(Node* &tree) {
	    // promote right child
	    Node* temp = tree;
	    tree = temp->right;
	    temp->right = tree->left;
	    tree->left = temp;

	    // update heights
	    updateHeight(temp);
	    updateHeight(tree);
    }

    void rebalance(Node* &tree) {
        // don't rebalance a null tree
        if (tree == nullptr) {
            return;
        }

	    // get height of children
	    int leftHeight = getHeight(tree->left);
	    int rightHeight = getHeight(tree->right);

	    // check for right or left imbalance
	    int imbalance = leftHeight - rightHeight;

	    if (imbalance > 1) {
		    // left imbalance
		    Node* leftNode = tree->left;

		    // get heights of leftNode children
		    int leftLeftHeight = getHeight(leftNode->left);
		    int leftRightHeight = getHeight(leftNode->right);

		    if (leftLeftHeight > leftRightHeight) {
			    // left-left imbalance
                promoteLeft(tree);
		    }
		    else {
                // left-right imbalance
			    promoteRight(tree->left);
			    promoteLeft(tree);
		    }
	    }
	    else if (imbalance < -1) {
		    // right imbalance
            Node* rightNode = tree->right;
		
		    // get heights of rightNode children
		    int rightLeftHeight = getHeight(rightNode->left);
		    int rightRightHeight = getHeight(rightNode->right);

		    if (rightLeftHeight > rightRightHeight) {
			    // right-left imbalance
                promoteLeft(tree->right);
			    promoteRight(tree);
		    }
		    else { // right-right imbalance
			    promoteRight(tree);
		    }
	    }
	    updateHeight(tree);
    }
};
