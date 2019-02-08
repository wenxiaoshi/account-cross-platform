//
// Created by melon on 2019/2/6.
//

#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;

class HashNode{
public:
    string  mKey;
    string  mValue;
    HashNode *next;

    HashNode(string key, string value){
        mKey   = key;
        mValue = value;
        next = NULL;
    }
    ~HashNode(){
    }
    HashNode& operator=(const HashNode& node){
        if(this == &node) return *this;
        mKey = node.mKey;
        mValue = node.mValue;
        next = node.next;
        return *this;
    }
};

class HashMap{
public:
    HashMap(int size);
    ~HashMap();
    bool HMInsert(const string& key, const string& value);
    bool HMDelete(const string& key);
    string& HMFind(const string& key);
    string& operator[](const string& key);
private:
    int hashfunc(const string& key);
    HashNode ** mTable;
    int mSize;
    string strnull;
};

HashMap::HashMap(int size):mSize(size){
    mTable = new HashNode*[size];
    for(int i=0; i<mSize; ++i){
        mTable[i] = NULL;
    }
    strnull = "NULL";
}

HashMap::~HashMap(){
    for(int i=0; i<mSize; ++i){
        HashNode *curNode = mTable[i];
        while(curNode){
            HashNode *temp = curNode;
            curNode =curNode->next;
            delete temp;
        }
    }
    delete mTable;
}

bool HashMap::HMInsert(const string& key, const string& value)
{
    int index = hashfunc(key)%mSize;
    HashNode *node = new HashNode(key, value);
    node->next = mTable[index];
    mTable[index] = node;
    return true;
}

bool HashMap::HMDelete(const string &key)
{
    int index = hashfunc(key)%mSize;
    HashNode *node = mTable[index];
    HashNode *prev = NULL;
    while(node){
        if(key == node->mKey){
            if(NULL == prev){
                mTable[index] = node->next;
            }else{
                prev->next = node->next;
            }
            delete node;
            return true;
        }
        prev = node;
        node = node->next;
    }
    return false;
}

string& HashMap::HMFind(const string& key)
{
    int index = hashfunc(key)%mSize;
    if(NULL == mTable[index]){
        return strnull;
    }else{
        HashNode *node = mTable[index];
        while(node){
            if(key == node->mKey){
                return node->mValue;
            }
            node = node->next;
        }
    }
    return strnull;
}

string& HashMap::operator[](const string& key)
{
    return HMFind(key);
}

int HashMap::hashfunc(const string& key){
    int hash = 0;
    for(int i=0; i<key.length(); ++i){
        hash = hash << 7^key[i];
    }
    return (hash & 0x7FFFFFFF);
}