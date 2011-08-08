#include "BloomFilter.hpp"
const unsigned long BloomFilter::m_pocketSize = LONG_BIT;

/***********************************************************/
/******************* PROVIDED FUNCTIONS ********************/
/***********************************************************/

void BloomFilter::init(unsigned long length) {
    ////////////// Write your code below  ////////////////////////
    //	m_length = length;
    m_length = (unsigned long)((2.0*length)/log(2))+1;
    m_pockets = (unsigned long)(ceil(double(m_length)/m_pocketSize));
    m_tickBook.resize(m_pockets);
    unsigned long i;
    for (i=0; i< m_pockets; i++) {
        m_tickBook[i] = 0;
    }
}

unsigned long BloomFilter::hash1(const Key& key) {
    unsigned long hash = 5381;
    unsigned int i=0;
    for (i=0; i< key.length(); i++) {
        hash = ((hash << 5) + hash) + key.c_str()[i]; /* hash * 33 + c */
    }

    double d_hash = (double) hash;

    d_hash *= (0.5*(sqrt(5)-1));
    d_hash -= floor(d_hash);
    d_hash *= (double)m_length;

    return (unsigned long)floor(d_hash);
}

unsigned long BloomFilter::hash2(const Key& key) {
   unsigned long hash = 0;
   unsigned int i=0;
   for (i=0; i< key.length(); i++) {
       hash = key.c_str()[i] + (hash << 6) + (hash << 16) - hash;
   }
   long double d_hash = (long double) hash;

   d_hash *= (0.5*(sqrtl(5)-1));
   d_hash = d_hash/10.0 - floorl(d_hash/10.0);
   d_hash *= (long double)m_length;

   return (unsigned long)floorl(d_hash);
}


bool BloomFilter::testExist(const Key& key, bool verbose) {
    if (exist(key)) {
        if (verbose) {
            cout<<"Key "<< key<<" is in the set"<<endl;
        }
        return true;
    } else {
        if (verbose) {
            cout<<"Key "<< key<<" is not in the set"<<endl;
        }
        return false;
    }
}

void BloomFilter::dump() {
    cout << "m_length: " << m_length << endl;
    cout<<m_pockets<<" Pockets: ";
    unsigned long i;
    for (i=0; i< m_pockets; i++) {
        cout<< m_tickBook[i]<<" ";
    }
    cout<<endl;
}

/***********************************************************/
/****************  TO BE COMPLETED FUNCTIONS ***************/
/***********************************************************/

// Given a key, sets both bits to 1.

void BloomFilter::add(const Key& key) {
    countAdd++;
    ////////////// Write your code below  ////////////////////////

    bloom_data d;
    getBloomData(key,&d); // populate struct
    
    m_tickBook[d.pocket1] |= d.sig1;
    m_tickBook[d.pocket2] |= d.sig2;
}

// Given a key, this function sets the required bits to 0.

void BloomFilter::del(const Key& key) {
    countDelete++;
    ////////////// Write your code below  ////////////////////////
 
    bloom_data d;
    getBloomData(key,&d); // populate struct

    m_tickBook[d.pocket1] &= ~d.sig1;
    m_tickBook[d.pocket2] &= ~d.sig2;

}

// Checks whether given a key, tests whether the two given bits
// are both 1.

bool BloomFilter::exist(const Key& key) {
    countFind++;
    ////////////// Write your code below  ////////////////////////

    bloom_data d;
    getBloomData(key,&d); // populate struct

    return (m_tickBook[d.pocket1] & d.sig1) == d.sig1 && (m_tickBook[d.pocket2] & d.sig2) == d.sig2;

}


/* Given a bloom_data struct and a key, this function populates 
 * the struct with the required data, such as the pocket, and which
 * bit within the pocket is the one we are interested in. It also
 * computes the binary signatures. The result is placed in the 
 * bloom_data b struct.
 */

void BloomFilter::getBloomData(const Key& key, bloom_data *b){
    unsigned long h1 = hash1(key);
    unsigned long h2 = hash2(key);

    b->pocket1 = h1 / m_pocketSize;
    b->offset1 = h1 - b->pocket1*m_pocketSize;

    b->pocket2 = h2 / m_pocketSize;
    b->offset2 = h2 - b->pocket2*m_pocketSize;

    b->sig1 = 1 << b->offset1;
    b->sig2 = 1 << b->offset2;
}

