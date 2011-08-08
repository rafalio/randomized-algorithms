#ifndef RBST_HPP_
#define RBST_HPP_

#include <iostream>
#include <fstream>
#include <assert.h>
#include <vector>
#include <math.h>
#include <cstdlib>
#include "Key.hpp"
#include "DataStructure.hpp"

#define VERB false

using namespace std;

class RBSTNode;
class RBSTNode: public Key {
public:
    RBSTNode(const Key& key):Key(key),m_left(NULL),m_right(NULL) { }
    virtual ~RBSTNode() {}

    string getKey() {
        return *this;
    }
 
    string setKey(const Key& key) {
        *this = key;
        return Key(key);
    }

    RBSTNode* left() {
        return m_left;
    }
    RBSTNode* right() {
        return m_right;
    }

    RBSTNode* setLeft (RBSTNode* left) {
        m_left = left;
        return this;
    }
    RBSTNode* setRight (RBSTNode* right) {
        m_right =right;
        return this;
    }

    int getSize(void){
        return size;
    }

    void setSize(int s){
        size = s;
    }

private:
    RBSTNode() {}
    RBSTNode* m_left;
    RBSTNode* m_right;

	int size;
};


class RBST : public DataStructure {
public:
    RBST():m_head(NULL),m_size(0){};
    virtual ~RBST() {};

    //ADD FUNCTIONS
    int add(const Key& key, bool verbose=VERB);

    //DEL FUNCTIONS
    int del(const Key& key, bool verbose=VERB);

    //FIND FUNCTIONS
    int find(const Key& key, bool verbose =VERB);


    //DUMP FUNCTIONS
    int dump(char sep = ' ');
    int dump(RBSTNode* target, char sep);

	void dotDump();
    void dotDump(RBSTNode* target, ofstream *fi );

private:
    RBSTNode* randomAdd(RBSTNode* target, RBSTNode* newNode);
    RBSTNode* addRoot(RBSTNode* target,  RBSTNode* newNode);
    RBSTNode*  rightRotate(RBSTNode* target);
    RBSTNode*  leftRotate(RBSTNode* target);
    RBSTNode* del(RBSTNode* target, const Key& key);
    RBSTNode* removeMinLeaf(RBSTNode* target , RBSTNode* min);

	RBSTNode* findMin(RBSTNode* t);
	RBSTNode* deleteNode(RBSTNode* target);

    RBSTNode* find(RBSTNode* target, const Key& key);

    RBSTNode* m_head;
    unsigned int m_size;
};

#endif /*RBST_HPP_*/
