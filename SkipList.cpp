#include "SkipList.hpp"

/***********************************************************/
/********************** PROVIDED FUNCTIONS *****************/
/***********************************************************/
void SkipList::init(int maxHeight) {
    m_maxHeight = maxHeight;
    m_head = new SkipListNode("", m_maxHeight);
    srand( time(NULL) );
}

SkipList::~SkipList() {
    while (m_head != NULL) {
        SkipListNode* head = m_head;
        m_head = m_head->nextAtLevel(0);
        delete head;
    }
}


int SkipList::add(const Key& key, bool verbose){
    if (find(key, false) != 0) {
        if (verbose) {
            cout<< "Calling add: " << "Node "<<key<<" is already in the list."<<endl;
        }
        return 0;
    }

    SkipListNode* newNode = new SkipListNode(key, randHeight());
    if (verbose) {
        cout<< "Calling add: " << "Add new node "<<*newNode<<" with height "<<newNode->height()<<endl;
    }
    int ret = add (m_head, newNode, m_maxHeight-1);
    if (ret == 0 ){
        return ret;
    }
    return 1;
}

int SkipList::find(const Key &key, bool verbose) {
    SkipListNode* ret = find (m_head, key, m_maxHeight-1) ;
    if (ret == NULL) {
        if (verbose) {
            cout<< "Calling find: " << "Node "<<key<<" is not in the list"<<endl;
        }
        return 0;
    } else {
        if (verbose) {
            cout<< "Calling find: " << "Node "<<key<<" is in the list"<<endl;
        }
        return 1;
    }

}
int SkipList::del(const Key& key, bool verbose) {
    if (key.length() == 0){
      return 1;
    }
    SkipListNode* toBeDeleted  = del(m_head, key, m_maxHeight-1);

    if (toBeDeleted == NULL) {
        if (verbose) {
            cout<< "Node "<<key<<" is not in the list"<<endl;
        }
    } else {
        delete toBeDeleted;
        if (verbose) {
            cout<< "Node "<<key<<" is deleted from the list"<<endl;
        }
    }
    return 1;
}

void SkipList::dump(char sep) {
    int length = -1;
    cout<<"Current List: ";
    for ( SkipListNode* iter = m_head; (iter != NULL); iter=iter->nextAtLevel(0)) {
        length++;
        cout << string(*iter)<<" ";
        cout <<"("<< iter->height() <<":";
        unsigned int i;
        for (i=0; i< iter->height(); i++) {
            if (iter->nextAtLevel(i)) {
                cout<<" "<<i<<":"<<*(iter->nextAtLevel(i));
                cout.flush();
            }
        }
        cout<<")"<<sep;
    }
    cout<<length<<" nodes in total."<<endl;
}


/***********************************************************/
/***************  TO BE COMPLETED FUNCTIONS ****************/
/***********************************************************/

/////////////////////////////////////////////////////////////
/////////////////////  ADD FUNCTIONS ////////////////////////
/////////////////////////////////////////////////////////////


/*
 * Generate a random number j in range 
 * 1 <= j <= height with P(j) = 1/(2**j)
 */

unsigned int SkipList::randHeight() {
    int t = rand() + 1;
    int j = 2;
    unsigned int i = 1;
    double result;

    for(i = 1; i < this->m_maxHeight; i++){
        result = (double)(RAND_MAX) / double(j);
        if( t > result ) break; 
        j += j;
    }
    return i;
}


int SkipList::add(SkipListNode* target, SkipListNode* newNode, unsigned int level) {
    if (target->nextAtLevel(level) != NULL &&
            (*target->nextAtLevel(level)) < *newNode) {
        countAdd++;
    }
    ////////////// Write your code below  ////////////////////////
    
    SkipListNode* t = target->nextAtLevel(level); 
    if(t == NULL || newNode->compare(*t) < 0){
        if( level < newNode->height() ){
            newNode->setNextAtLevel(level,t);
            target->setNextAtLevel(level,newNode);
        }
        if(level > 0) add(target,newNode,level-1);
        return 1;
    }
    add(t,newNode,level);
    return 1; 
}

/////////////////////////////////////////////////////////////
/////////////////////  FIND FUNCTION ////////////////////////
/////////////////////////////////////////////////////////////
SkipListNode* SkipList::find(SkipListNode* target, const Key& key, unsigned int level) {
    if (target->nextAtLevel(level) != NULL && *(target->nextAtLevel(level)) < key) {
        countFind++;
    }
    ////////////// Write your code below  ////////////////////////

    if( target == NULL) return NULL;

    if( target->compare(key) == 0 ){
        return target;
    }

    SkipListNode* newTarget = target->nextAtLevel(level);

    if(newTarget == NULL || key.compare( *newTarget ) < 0){
        if(level == 0)
            return NULL;
        else
            return find(target,key,level-1);
    }
    return find(newTarget,key,level);
}

    
/////////////////////////////////////////////////////////////
/////////////////////  DEL FUNCTION ////////////////////////
/////////////////////////////////////////////////////////////
SkipListNode* SkipList::del(SkipListNode* target, const Key& key, unsigned int level) {
    if (target->nextAtLevel(level) != NULL && *(target->nextAtLevel(level)) < key) {
        countDelete++;
    }
    ////////////// Write your code below  ////////////////////////

    if(target == NULL) return NULL;

    SkipListNode *t = target->nextAtLevel(level);

    
    if(t == NULL){
        if(level == 0) return NULL;
        else return del(target,key,level-1);
    }
    
    int comparison = key.compare(*t);

    if(level == 0){
        if(t == NULL || comparison < 0) return NULL;
    }

    // The key is smaller than the node
    if( comparison < 0){
        return del(target,key,level-1);
    }

    // We found the node
    else if( comparison == 0 ){ 
        target->setNextAtLevel(level, t->nextAtLevel(level) );
        if(level == 0) return t;
        return del(target,key,level-1);
    }

    // The key is greater than the node
    
    else{
        return del(t,key,level);
    }

}
