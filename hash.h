#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <bits/stdc++.h>

using namespace std;


// Trie node 
struct NodoTrie 
{ 
     struct NodoTrie *filho[26];
     // isEndOfWord is true if the node 
     // represents end of a word 
     bool ehFim;
     int ID = 0;
}; 

struct jogador{
    int ID;
    string nome;
    string dados;
    jogador *prox;
};

struct rating{
    int ID;
    int numAvaliacoes = 0;
    listaEncadeada *rat;
    rating *prox;
};

struct listaEncadeada{
    float avaliacao;
    listaEncadeada *prox;
};

// Returns new trie node (initialized to NULLs)
struct NodoTrie *getNode(void)
{
    static struct NodoTrie *pNode =  new NodoTrie;
 
    pNode->ehFim = false;
 
    for (int i = 0; i < 26; i++)
        pNode->filho[i] = NULL;
 
    return pNode;
}
 
// If not present, inserts key into trie
// If the key is prefix of trie node, just
// marks leaf node
void insert(struct NodoTrie *root, string key, int num)
{
    struct NodoTrie *pCrawl = root;
 
    for (int i = 0; i < key.length(); i++)
    {
        int index = key[i] - 'a';
        if (!pCrawl->filho[index])
            pCrawl->filho[index] = getNode();
        pCrawl = pCrawl->filho[index];
    }
 
    // mark last node as leaf
    pCrawl->ehFim = true;
    pCrawl->ID = num;
}
 
// Returns true if key presents in trie, else
// false
int search(struct NodoTrie *root, string key)
{
    struct NodoTrie *pCrawl = root;
 
    for (int i = 0; i < key.length(); i++)
    {
        int index = key[i] - 'a';
        if (!pCrawl->filho[index])
            return pCrawl->ID;
 
        pCrawl = pCrawl->filho[index];
    }

    return pCrawl->ID;
}

ifstream& abreArq(string arqnome){
    ifstream *arq = 0;

    for(;;){

        arq = new ifstream(arqnome.c_str());
        if (arq->good()){
            arq->seekg(0);
            cerr << arqnome << " Aberto com sucesso!" << endl;
            break;
        }
        cerr << arqnome << " Nao foi aberto!" << endl;
        delete arq;
    }
    return *arq;
}

int calculaChave(int ID, int tamanho){
    int i = ((int)pow((ID / 1000), 2.0) + ID) % tamanho;
    return i;
}
