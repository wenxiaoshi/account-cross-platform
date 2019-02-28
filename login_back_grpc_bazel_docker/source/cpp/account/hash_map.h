//
// Created by melon on 2019/2/6.
//

#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;

namespace my_struct{

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
    };

    class HashMap{
    public:
        HashMap(int size);
        ~HashMap();
        bool insert(const string& key, const string& value);
        bool remove(const string& key);
        string& find(const string& key);
        void clean();
        std::string list();
    private:
        int hashfunc(const string& key);
        HashNode ** mTable;
        int mSize;
        string strnull;
    };

}