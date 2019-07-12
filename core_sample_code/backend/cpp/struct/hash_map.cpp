//
// Created by melon on 2019/2/6.
//

#include "hash_map.h"

using namespace my_struct;

HashMap::HashMap(int size):mSize(size){
    mTable = new HashNode*[size];
    for(int i=0; i<mSize; ++i){
        mTable[i] = NULL;
    }
    strnull = "";
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

bool HashMap::insert(const string& key, const string& value)
{
    int index = hashfunc(key)%mSize;
    HashNode *node = new HashNode(key, value);
    HashNode *head = mTable[index];
    while (head){
        if (strcmp(node->mKey.data(),head->mKey.data()) == 0){
            head->mValue = node->mValue;
            return true;
        }
        head = head->next;
    }
    node->next = mTable[index];
    mTable[index] = node;
    return true;
}

bool HashMap::remove(const string &key)
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

string& HashMap::find(const string& key)
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

int HashMap::hashfunc(const string& key){
    int hash = 0;
    for(int i=0; i<key.length(); ++i){
        hash = hash << 7^key[i];
    }
    return (hash & 0x7FFFFFFF);
}

void HashMap::clean(){
    for(int i=0; i<mSize; ++i){
        delete[] mTable[i];
        mTable[i] = NULL;
    }
    delete[] mTable;
    mTable = NULL;

}

std::string HashMap::list(){
    string source;
    for(int i=0; i<mSize; ++i){
        HashNode* node = mTable[i];
        while (node){
            source += node->mKey + " : " + node->mValue + "\n";
            node = node->next;
        }
    }
    return source;
}