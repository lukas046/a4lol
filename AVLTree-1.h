#include <iostream>
#include <stdexcept>
// AVL Node Class
template <class K, class V>
class AVLTreeNode {
public:
	// Should have attributes named:
	// parent - pointer to parent 
	AVLTreeNode  * parent, *left, *right;
	// key - node's key
	K key;
	// value - node's value
	V value;
	// height - node's height
	unsigned int height;

	// Constructors ...
	AVLTreeNode(K key, V value);
	AVLTreeNode(AVLTreeNode<K,V> * parent, K key, V value);
	// ~AVLTreeNode();

};

template<class K, class V>
AVLTreeNode<K,V>::AVLTreeNode(K key, V value): key(key), value(value){
	right = nullptr;
	left = nullptr;
	parent = nullptr;
	height = 0;
};

template <class K, class V>
AVLTreeNode<K,V>::AVLTreeNode(AVLTreeNode<K,V> * parent, K key, V value): parent(parent), key(key), value(value){
	left = nullptr;
	right = nullptr;
	height = 0;
};;

// AVL Tree class
template<class K, class V>
class AVLTree
{
private:
	// Tree attributes go here
	// Your tree MUST have a root node named root
	AVLTreeNode<K,V> * root;
	unsigned int s;
	bool insertnode(AVLTreeNode<K,V> * pointer, K k, V v);
	void deletetree(AVLTreeNode<K,V>  * node);
	void rightbal(AVLTreeNode<K,V>  * ptr);
	void leftbal(AVLTreeNode<K,V>  * ptr);
	int balcheck(AVLTreeNode<K,V>  * ptr);
	unsigned int getheight(AVLTreeNode<K,V>  * ptr);
	bool removenode(AVLTreeNode<K,V>  * ptr, K k);
	AVLTreeNode<K,V> * min(AVLTreeNode<K,V> * ptr);
	void traverse_k(AVLTreeNode<K,V> * ptr,std::vector<K> &klist);
	void traverse_v(AVLTreeNode<K,V> * ptr, std::vector<V> &vlist);
	void deletenode(AVLTreeNode<K,V> * ptr);
	int fix_heights(AVLTreeNode<K,V> *ptr);
	AVLTreeNode<K,V> *copying(AVLTreeNode<K,V>* ptr, AVLTreeNode<K,V> * ptr2);

public:
	AVLTree();
	~AVLTree();
	AVLTree(const AVLTree &copy);
	AVLTree &operator=(const AVLTree &copy);
	bool insert(K k, V v);
	bool remove(K k);
	V search(K k);
	std::vector<V> values();
	std::vector<K> keys();
	unsigned int size();
	void* getRoot() const { return root; }; // DO NOT REMOVE
};

// AVL Tree Methods go here
template <class K, class V>
AVLTree<K,V>::AVLTree(){
	root = NULL;
	s = 0;
};

template <class K, class V>
AVLTree<K,V>::~AVLTree(){
	deletenode(this->root);
}

template <class K, class V>
int AVLTree<K,V>::fix_heights(AVLTreeNode<K,V> *ptr)
{
	if (!ptr) return -1;
	unsigned left = fix_heights(ptr->left);
	unsigned right = fix_heights(ptr->right);
	ptr->height = ((left > right) ? left : right)+ 1;
	return ptr->height;
};

template <class K, class V>
AVLTree<K,V>::AVLTree(const AVLTree &copy){
	root = copying(copy.root, nullptr);
	s = copy.s;
}	

template <class K, class V>
AVLTreeNode<K,V> * AVLTree<K,V>::copying(AVLTreeNode<K,V>*ptr, AVLTreeNode<K,V>* ptr2){
	if(ptr == NULL){
		return nullptr;
	}
	AVLTreeNode<K,V> * newcopy = new AVLTreeNode<K,V>(ptr2, ptr->key, ptr->value);
	newcopy->height = ptr->height;
	newcopy->right = copying(ptr->right, newcopy);
	newcopy->left = copying(ptr->left,newcopy);
	return newcopy;
}

template <class K, class V>
AVLTree<K,V> &AVLTree<K,V>::operator=(const AVLTree<K,V> &copy){
	if(this != &copy){
		if(root != NULL){
			deletetree(root);
		}
		root = copying(copy.root,nullptr);
		s = copy.s;
	}
	return *this;
}

template <class K, class V>
void AVLTree<K,V>::deletetree(AVLTreeNode<K,V> * temp){
	if(temp == NULL){
		return;
	}
	deletetree(temp->left);
	deletetree(temp->right);
	delete temp;
}


template<class K, class V>
V AVLTree<K,V>::search(K temp){
	AVLTreeNode<K,V> * ptr = root;
	while(ptr != NULL){
		if(ptr->key == temp){
			return ptr->value;
		}
		if(ptr->key < temp){
			ptr = ptr->right;
		}
		else if(ptr->key > temp){
		ptr = ptr->left;
		}
	}
	throw std::runtime_error("NULL");

	return ptr->value;
}

template<class K, class V>
unsigned int AVLTree<K,V>::size(){
	return s;
}

template<class K, class V>
std::vector<V> AVLTree<K,V>::values(){
	std::vector<V> vect;
	if(!this->root){
		return vect;
	}
	traverse_v(this->root,vect);
	return vect;
}

template<class K, class V>
std::vector<K> AVLTree<K,V>::keys(){
	std::vector<K> vect;
	if(!this->root){
		return vect;
	}
	traverse_k(this->root,vect);
	return vect;
}

template<class K, class V>
bool AVLTree<K,V>::insert(K k, V v){
	if (this->root == NULL){
		this->root = new AVLTreeNode<K,V>(k,v);

		s++;
		return true;
	}
	AVLTreeNode<K,V> * ptr = root;
	bool t = insertnode(ptr,k,v);
	fix_heights(root);
	return t;
}

template<class K, class V>
bool AVLTree<K,V>::remove(K k){
	return removenode(root, k);
}

template <class K, class V>
bool AVLTree<K,V>::removenode(AVLTreeNode<K,V> * ptr, K k){
	bool removing = false;
	if(ptr == NULL){
		removing = false;
	}
	else if(ptr->key < k){
		return removing = removenode(ptr->right,k);
	}
	else if(ptr->key > k){
		return removing = removenode(ptr->left,k);
	}
	else{

		removing = true;
		if(!ptr->left && !ptr->right){
			if(ptr->parent == NULL){
				root = NULL;
			}
			else if(ptr->parent->left == ptr){
				ptr->parent->left = NULL;
			}
			else{
				ptr->parent->right = NULL;
			}
		}
		else if(ptr->left == NULL){
			if(ptr->parent == NULL){
				root = ptr->right;
			}
			else if(ptr->parent->left == ptr){
				ptr->parent->left = ptr->right;
			}
			else{
				ptr->parent->right = ptr->right;
			}
		}
		else if(ptr->right == NULL){
			if(ptr->parent == NULL){
				root = ptr->left;
			}
			else if(ptr->parent->left == ptr){
				ptr->parent->left = ptr->left;
			}
			else{
				ptr->parent->right = ptr->left;
			}
		}
		else{
			AVLTreeNode<K,V> *temp = ptr->right;
			if (temp->left){
				while (temp->left){
					temp = temp->left;
				}

				temp->parent->left = temp->right;
				if (temp->right){	
					temp->parent->left->parent = temp->parent;
				}
				temp->parent->height = getheight(temp->parent);
				temp->right = ptr->right;
				if (temp->right){
					temp->right->parent = temp;
				} 
			}
			temp->left = ptr->left;
			if (temp->left) temp->left->parent = temp;
			temp->parent = ptr->parent;
			
			if (!temp->parent)
			{
				root = temp;
				temp->height = getheight(temp);
			}
			else if (temp->parent->left == ptr)
			{
				temp->parent->left = temp;
			}
			else
			{
				temp->parent->right = temp;
			}
		}
		s--;
		delete ptr;
		ptr = nullptr;
	}
	
	if(removing == true && ptr){
		ptr->height = getheight(ptr);
		int bal = balcheck(ptr);
		if(bal<-1){
			int rightbalance = balcheck(ptr->right);
			if(rightbalance > 0){
				rightbal(ptr->right);
				leftbal(ptr);
			}
			else{
				leftbal(ptr);
			}
		}
		if(bal > 1){
			int leftbalance = balcheck(ptr->left);
			if (leftbalance < 0){

				leftbal(ptr->left);
				rightbal(ptr);
			}
			else{
				rightbal(ptr);
			}
		}
	}
	return removing;
}

template<class K, class V>
AVLTreeNode<K,V>* AVLTree<K,V>::min(AVLTreeNode<K,V>* ptr){
	AVLTreeNode<K,V>* cur = ptr;
	while(cur->left != NULL){
		cur = cur->left;
	}
	return cur;
}

template<class K, class V>
bool AVLTree<K,V>::insertnode(AVLTreeNode <K,V>* pointer, K k, V v){
	bool inserting = false;
	if(pointer->key < k){
		if(pointer->right == NULL){
			s++;
			pointer->right = new AVLTreeNode <K,V>(k,v);

			pointer->right->parent = pointer;
			pointer->height = 0;
			inserting = true;
		}
		else{
			inserting = insertnode(pointer->right, k, v);
		}
	}
	else if(pointer->key > k){
		if(pointer->left == NULL){
			s++;
			pointer->left = new AVLTreeNode <K,V>(k,v);

			pointer->left->parent = pointer;
			pointer->height = 0;
			inserting = true;
			
		}
		else{
			inserting = insertnode(pointer->left, k, v);
		}
	}
	if(inserting == true){
		pointer->height = getheight(pointer);
	}
	int bal = balcheck(pointer);
	if(bal < -1 && pointer->right->key < k){
		leftbal(pointer);
	}
	else if(bal > 1 && pointer->left->key > k){
		rightbal(pointer);
	}
	else if(bal < -1 && pointer->right->key > k){
		rightbal(pointer->right);
		leftbal(pointer);
	}
	else if(bal > 1 && pointer->left->key < k){
		leftbal(pointer->left);
		rightbal(pointer);
	}
	return inserting;
}

template<class K, class V>
unsigned int AVLTree<K,V>::getheight(AVLTreeNode<K,V> * ptr){
	if(ptr == NULL){
		return 0;
	}
	if(ptr->left == NULL && ptr->right == NULL){
		return 0;
	}
	else if(ptr->left == NULL){
		return ptr->right->height+1;
	}
	else if(ptr->right == NULL){
		return ptr->left->height+1;
	}

	else{
		if(ptr->left->height>ptr->right->height){
			return ptr->left->height+1;
		}
		else{
			return ptr->right->height+1;
		}

	}

}

template<class K, class V>
int AVLTree<K,V>::balcheck(AVLTreeNode<K,V> * ptr){
	if(ptr->right == NULL && ptr->left == NULL){
		return 0;
	}
	else if(ptr->right == NULL){
		return ptr->left->height+1;
	}
	else if(ptr->left == NULL){
		return 0 - (ptr->right->height+1);
	}
	else{
		return  ptr->left->height - ptr->right->height;
	}
}

template<class K, class V>
void AVLTree<K,V>::leftbal(AVLTreeNode <K,V>* ptr){
    AVLTreeNode<K,V> * temp1 = ptr;
    AVLTreeNode<K,V> * temp2 = ptr->right;
    temp1->right = temp2->left;
    if (temp1->right) temp1->right->parent = temp1;
    temp2->parent = temp2->parent;
    if (temp2->parent)
    {
        if (temp2->parent->left == temp1) temp2->parent->left = temp2;
        else { temp2->parent->right = temp2;}
    }
    temp2->left = temp1;
    temp1->parent = temp2;
    temp1->height = getheight(temp1);
    temp2->height = getheight(temp2);
    if (root == ptr) root = temp2;
}

template<class K, class V>
void AVLTree<K,V>::rightbal(AVLTreeNode <K,V>* ptr){
	AVLTreeNode<K,V> * node1 = ptr;
	AVLTreeNode<K,V> * node2 = ptr->left;
	node1->left = node2->right;
	if(node1->left != NULL){
		node1->left->parent = node1;
	}
	node2->parent = node1->parent;
	if(node2->parent!= NULL){
		if(node2->parent->left == node1){
			node2->parent->left = node2;
		}
		else{
			node2->parent->right=node2;
		}
	}
	node2->right = node1;
	node1->parent = node2;
	node1->height = getheight(node1);
	node2->height = getheight(node2);
	if(root==ptr){
		root = node2;
	}
}
template<class K, class V>
void AVLTree<K,V>::traverse_k(AVLTreeNode<K,V> * ptr,std::vector<K> &klist){
	if(ptr){
		traverse_k(ptr->left,klist);
		klist.push_back(ptr->key);
		traverse_k(ptr->right,klist);
	}
}
template<class K, class V>
void AVLTree<K,V>::traverse_v(AVLTreeNode<K,V> * ptr, std::vector<V> &vlist){
		if(ptr){
		traverse_v(ptr->left,vlist);
		vlist.push_back(ptr->value);
		traverse_v(ptr->right,vlist);
	}

}



template<class K, class V>
void AVLTree<K,V>::deletenode(AVLTreeNode<K,V> * ptr){
	if(ptr){
		deletenode(ptr->left);
		deletenode(ptr->right);
		delete ptr;
	}
}