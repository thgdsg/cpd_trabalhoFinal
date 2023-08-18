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

struct listaEncadeada{
    float avaliacao;
    // APENAS UTILIZAR ID NA HORA DE MANIPULAR USUARIO!
    int ID = 0;
    listaEncadeada *prox;
};

struct jogador{
    int ID;
    int numAvaliacoes = 0;

    listaEncadeada *ratings = NULL;
    string nome;
    string dados;
    jogador *prox;
};

struct usuario{
    int UID;
    int totalAvaliacoes = 1;
    listaEncadeada *ratings;
    usuario *prox;
};

// Returns new trie node (initialized to NULLs)
struct NodoTrie *getNode(void)
{
    struct NodoTrie *pNode =  new NodoTrie;
 
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
        //cout << key[i];
    }
    //cout << endl;
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
            return 0;
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

void buscaJogador(int ID, jogador *tabela[], int tamanho){
    int chave = calculaChave(ID, tamanho);
    jogador *percorre = tabela[chave];
    while(percorre != NULL || percorre->ID != ID){
        if(percorre->ID == ID){
            cout << percorre->ID << "," << percorre->numAvaliacoes << "," << percorre->ratings->avaliacao << endl;
            cout << percorre->ID << "," << percorre->nome << "," << percorre->dados << endl;
        }
        percorre = percorre->prox;
    }
    if(percorre == NULL)
        cout << "valor nao encontrado" << endl;
}

/* NAO UTILIZAR
void buscaRating(int ID, rating *tabela[], int tamanho){
    int chave = calculaChave(ID, tamanho);
    rating *percorre = tabela[chave];
    while(percorre != NULL || percorre->ID != ID){
        if(percorre->ID == ID){
            cout << percorre->ID << "," << percorre->numAvaliacoes << "," << percorre->rat->avaliacao << endl;
        }
        percorre = percorre->prox;
    }
    if(percorre == NULL)
        cout << "valor nao encontrado" << endl;
}*/

void buscaUser(int UID, usuario *tabela[], int tamanho){
    int chave = calculaChave(UID, tamanho);
    usuario *percorre = tabela[chave];
    listaEncadeada *percorredor;
    bool f = false;
    while(percorre != NULL){
        percorredor = percorre->ratings;
        if(percorre->UID == UID){
            f = true;
            cout << percorre->UID << "," << percorre->totalAvaliacoes << endl;
            while(percorredor != NULL){
                cout << percorredor->avaliacao << "," << percorredor->ID << endl;
                percorredor = percorredor->prox;
            }
        }
        percorre = percorre->prox;
    }
    if(f == false)
        cout << "usuario nao encontrado" << endl;
}