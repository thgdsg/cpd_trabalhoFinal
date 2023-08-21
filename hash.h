#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <bits/stdc++.h>

using namespace std;


//  Nodo trie
struct NodoTrie 
{ 
     struct NodoTrie *filho[26];
     // ehFim eh uma flag que mostra se o nodo é o nodo final na trie
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

// Retorna um novo nodo da trie
struct NodoTrie *getNode(){
    struct NodoTrie *novo =  new NodoTrie;
 
    novo->ehFim = false;
 
    for (int i = 0; i < 26; i++)
        novo->filho[i] = NULL;
 
    return novo;
}
 
// Se o nodo não estiver presente, insere a KEY recebida na árvore trie
// se a chave for o prefixo de uma outra palavra, marca o nodo da árvore
void insert(struct NodoTrie *raiz, string key, int num)
{
    struct NodoTrie *novo = raiz;
 
    for (int i = 0; i < key.length(); i++)
    {
        int indice = key[i] - 'a';
        if (!novo->filho[indice])
            novo->filho[indice] = getNode();
        novo = novo->filho[indice];
        //cout << key[i];
    }
    //cout << endl;
    // define o a flag do último nodo como true
    novo->ehFim = true;
    novo->ID = num;
}
 
// retorna o ID do jogador caso encontre ele na trie, caso contrário, retorna 0
int search(struct NodoTrie *raiz, string key){
    struct NodoTrie *percorre = raiz;
 
    for (int i = 0; i < key.length(); i++)
    {
        int index = key[i] - 'a';
        if (!percorre->filho[index])
            return 0;
        percorre = percorre->filho[index];
    }

    return percorre->ID;
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
    while(percorre != NULL){
        if(percorre->ID == ID){
            cout << "ID DO JOGADOR: " << percorre->ID << endl;
            cout << "NOME DO JOGADOR: " << percorre->nome << endl;
            cout << "POSICOES DO JOGADOR: " << percorre->dados << endl;
            cout << "NUM DE AVALIACOES DO JOGADOR: " << percorre->numAvaliacoes << endl;
            cout << "MEDIA DE AVALICOES DO JOGADOR: " << percorre->ratings->avaliacao << endl;
            break;
        }
        percorre = percorre->prox;
    }
    if(percorre == NULL)
        cout << "valor nao encontrado" << endl;
}

void buscaUser(int UID, usuario *tabelaU[], jogador *tabelaJ[], int tamanhoU, int tamanhoJ){
    int chave = calculaChave(UID, tamanhoU);
    usuario *percorre = tabelaU[chave];
    listaEncadeada *percorredor;
    bool f = false;
    while(percorre != NULL){
        percorredor = percorre->ratings;
        if(percorre->UID == UID){
            f = true;
            cout << "ID DO USUARIO: " << percorre->UID << endl;
            cout << "NUM DE AVALICOES DO USUARIO: " << percorre->totalAvaliacoes << endl;
            cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
            cout << "JOGADORES QUE O USUARIO AVALIOU: " << endl;
            while(percorredor != NULL){
                buscaJogador(percorredor->ID, tabelaJ, tamanhoJ);
                percorredor = percorredor->prox;
            }
        }
        percorre = percorre->prox;
    }
    if(f == false)
        cout << "usuario nao encontrado" << endl;
}