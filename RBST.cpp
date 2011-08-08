#include "RBST.hpp"
/***********************************************************/
/******************* PROVIDED FUNCTIONS ********************/
/***********************************************************/
int RBST::add(const Key& key, bool verbose) {
    unsigned int oldSize = m_size;
	RBSTNode *newNode = new RBSTNode(key);
    m_head = randomAdd(m_head, newNode);
	if (m_size > oldSize) {
        if (verbose) {
            cout<<"Node "<<key<< " is added into the tree."<<endl;
        }
        return 1;
    } else {
        if (verbose) {
            cout<<"Node "<<key<< " is already in the tree."<<endl;
        }
        return 0;
    }
    if (verbose) cout<<endl;
};

int RBST::del(const Key& key, bool verbose) {
    unsigned oldSize= m_size;
    m_head = del(m_head, key);
    if (m_size < oldSize) {
        if (verbose) {
            cout<<"Node "<<key<< " is deleted from the tree."<<endl;
        }
        return 1;
    } else {
        if (verbose) {
            cout<< "Node "<<key<< " is not in the tree."<<endl;
        }
        return 0;
    }
};

int RBST::find(const Key& key, bool verbose) {
    RBSTNode* ret = find(m_head, key);
    if (ret == NULL) {
        if (verbose) {
            cout<< "Node "<<key<< " is not in the tree."<<endl;
        }
        return 0;
    } else {
        if (verbose) {
            cout<<"Node "<<key<< " is in the tree."<<endl;
        }
        return 1;
    }
};

int RBST::dump(char sep) {
    int ret = dump(m_head, sep);
    cout<<"SIZE: " <<ret<<endl;
    return ret;
};

int RBST::dump(RBSTNode* target, char sep) {
    if (target == NULL) {
        return 0;
    }
    int ret = dump(target->left(), sep);
    cout<< *target<<sep;
    ret ++;
    ret += dump(target->right(), sep);
    return ret;
};

/***********************************************************/
/****************  TO BE COMPLETED FUNCTIONS ***************/
/***********************************************************/


/////////////////////////////////////////////////////////////
/////////////////////  ADD FUNCTIONS ////////////////////////
/////////////////////////////////////////////////////////////

RBSTNode*  RBST::rightRotate(RBSTNode* target) {
    RBSTNode *l = target->left();
    target->setLeft(l->right());
    l->setRight(target);
    return l;
};

RBSTNode*  RBST::leftRotate(RBSTNode* target) {
    RBSTNode *r = target->right();
    target->setRight(r->left());
    r->setLeft(target); 
    return r;
};

RBSTNode* RBST::addRoot(RBSTNode* target, RBSTNode* newNode) {
    countAdd++;
    
    if(target == NULL){
        return newNode;
    }

    int comparison = newNode->compare(*target);

    if(comparison < 0){
        target->setLeft( addRoot( target->left(), newNode) ); 
        return rightRotate(target);
    }
    else{
        target->setRight( addRoot( target->right(), newNode) );
        return leftRotate(target);
    }
};


/* Performs a random insertion. Sometimes uses root-insert to
 * insert into the root, and otherwise peforms a normal binary
 * tree insertion.
 */
RBSTNode* RBST::randomAdd(RBSTNode* target, RBSTNode* newNode) {
    countAdd++;
    ////////////// Write your code below  ////////////////////////

    if(target == NULL){
		m_size++;
		newNode->setSize(1);
		return newNode;
    }

    int r = rand() % target->getSize() + 1; 
    
    int comparison = newNode->compare(*target);

    if(comparison == 0) return target;

    if(r == 1){
        newNode->setSize( target->getSize() + 1);
        m_size++;
        return addRoot(target,newNode);
    }

    if(comparison < 0){
        target->setLeft( randomAdd(target->left(),newNode) );
    }
    else if(comparison > 0){
        target->setRight( randomAdd(target->right(),newNode) );
    }

    target->setSize(target->getSize() + 1);

    return target;
};

/////////////////////////////////////////////////////////////
/////////////////////  DEL FUNCTIONS ////////////////////////
/////////////////////////////////////////////////////////////


// Recursively deletes node with key 'key', and returns the new head
// of the subtree.

RBSTNode* RBST::del(RBSTNode* target, const Key& key) {
    countDelete++;

	if (target == NULL) return NULL;
	
    int comparison = key.compare(*target);
	
	if (comparison == 0) { 			// Equal case
     	target = deleteNode(target);
     	m_size --;
    }
    else if (comparison < 0) 		// key < target
    	target->setLeft(del(target->left(),key));
    else							// key > target
    	target->setRight(del(target->right(),key));

    return target;
};

RBSTNode* RBST::deleteNode(RBSTNode* target) {
	if (target->right() == NULL)
		return target->left(); 		// 0 or 1 child case
	else if (target->left() == NULL)
		return target->right();		// 1 child case
	
	// 2 children case
	// get the minimal node from right subtree, replace what we 
    // want to delete with it.

	else {
        RBSTNode *min = findMin(target->right());
		min->setRight(removeMinLeaf(target->right() , min));
		min->setLeft(target->left());
		return min; // new head
	}
};

// Recursively remove the mininum element

RBSTNode* RBST::removeMinLeaf(RBSTNode* target , RBSTNode* min) {
	if (target->compare(*min) == 0) return min->right();
	else
		target->setLeft(removeMinLeaf(target->left(),min));
		return target;
};

// Finds the leftmost element of subtree t
RBSTNode* RBST::findMin(RBSTNode* t){
    RBSTNode* ret = t;
    while(ret->left() != NULL){
        ret = ret->left();
    }
	return ret;
}


/////////////////////////////////////////////////////////////
/////////////////////  FIND FUNCTIONS ///////////////////////
/////////////////////////////////////////////////////////////

// Straightforward binary search

RBSTNode* RBST::find(RBSTNode* target, const Key& key) {
    countFind++;
    ////////////// Write your code below  ////////////////////////

    if(target == NULL) return NULL;

    int comparison = key.compare(*target);

    if(comparison < 0){
       return find(target->left(),key);
    }
    else if(comparison == 0){
       return target;
    }
    else{
        return find(target->right(),key);
    }
   
    return NULL;
}

