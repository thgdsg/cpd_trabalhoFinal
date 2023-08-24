#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <regex>
#include <cmath>

using namespace std;


//  Nodo trie
struct NodoTrie{ 
     struct NodoTrie *filho[26];
     // ehFim eh uma flag que mostra se o nodo é o nodo final na trie
     bool ehFim;
     int ID = 0;
}; 

struct listaID{
    int ID;
    listaID *prox;
};

struct NodoTrie2
{ 
    struct NodoTrie2 *filho[26];
    bool ehFim;
    listaID *IDs = NULL;
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

struct NodoTrie2 *getNode2(void)
{
    struct NodoTrie2 *pNode =  new NodoTrie2;
 
    pNode->ehFim = false;
 
    for (int i = 0; i < 26; i++)
        pNode->filho[i] = NULL;
 
    return pNode;
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
 
void insert2(struct NodoTrie2 *root, string key, int num)
{
    struct NodoTrie2 *pCrawl = root;
 
    for (int i = 0; i < key.length(); i++)
    {
        int index = key[i] - 'a';
        if (!pCrawl->filho[index])
            pCrawl->filho[index] = getNode2();
        pCrawl = pCrawl->filho[index];
    }
    pCrawl->ehFim = true;

    listaID *IDs = pCrawl->IDs;
    while (IDs != NULL && IDs->ID != num){
        IDs = IDs->prox; 
    }

    if (IDs == NULL){
        listaID *IDnovo = new listaID;
        IDnovo->ID = num;
        IDnovo->prox = pCrawl->IDs;
        pCrawl->IDs = IDnovo;
    }

}

// Devolve o nodo se estiver presente na trie
NodoTrie *search(struct NodoTrie *raiz, string key)
{
    struct NodoTrie *percorre = raiz;
 
    for (int i = 0; i < key.length(); i++)
    {
        int index = key[i] - 'a';
        if (!percorre->filho[index])
            return NULL;
        percorre = percorre->filho[index];
    }

    return percorre;
}

listaID *search2(struct NodoTrie2 *root, string key)
{
    struct NodoTrie2 *pCrawl = root;
 
    for (int i = 0; i < key.length(); i++)
    {
        int index = key[i] - 'a';
        if (!pCrawl->filho[index]){
            return NULL;
        }
        pCrawl = pCrawl->filho[index];
    }

    return pCrawl->IDs;
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
    int contador = 20;
    float maiorVal = 5.0;

    usuario *percorre = tabelaU[chave];
    listaEncadeada *percorredor;
    listaEncadeada *anterior = NULL;

    while(percorre != NULL){
        if(percorre->UID == UID){
            cout << "ID DO USUARIO: " << percorre->UID << endl;
            cout << "NUM DE AVALIACOES DO USUARIO: " << percorre->totalAvaliacoes << endl;
            cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
            cout << "JOGADORES QUE O USUARIO AVALIOU: " << endl;

            while(contador > 0){
                percorredor = percorre->ratings;
                while(percorredor != NULL){
                    if(percorredor->avaliacao == maiorVal){
                        if(contador > 0){
                            cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
                            cout << "AVALIACAO DO USUARIO PARA ESSE JOGADOR: " << percorredor->avaliacao << endl;
                            buscaJogador(percorredor->ID, tabelaJ, tamanhoJ);
                            contador--;
                        }
                    }
                    percorredor = percorredor->prox;
                }
                maiorVal = maiorVal - 0.5;
                if(maiorVal == 0)
                    break;
            }
            break;
        }
        percorre = percorre->prox;
    }
    if(percorre == NULL)
        cout << "usuario nao encontrado" << endl;
}

void search_all(NodoTrie *root, jogador *tabela[], int tamanho){
    if (root->ehFim){
        cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
        buscaJogador(root->ID, tabela, tamanho);
    }
    for (int i = 0; i < 26; i++){
        if (root->filho[i])
            search_all(root->filho[i], tabela, tamanho);
    }
}

void buscaNome(string prefix, NodoTrie *root, jogador *tabela[], int tamanho){
    NodoTrie *busca = search(root, prefix);

    if (!busca) {
        cout << "Nenhum jogador foi encontrado" << endl;
    } else {
        search_all(busca, tabela, tamanho);
    }

}

void buscaTags(string tagstring, NodoTrie2 *root, jogador *tabela[], int tamanho){
    const regex AZ("[^abcdefghijklmnopqrstuvwxyz]");

    stringstream ss(tagstring);
    string tag;
    getline(ss, tag, '\''); //string pré aspas jogada fora

    if (tag == ""){
        getline(ss, tag, '\'');
        transform(tag.begin(), tag.end(), tag.begin(), ::tolower);
        tag = regex_replace(tag, AZ, "");

        listaID *busca = search2(root, tag);
        listaID *resultado = NULL;
        while (busca!=NULL){
            listaID *IDnovo = new listaID;
            IDnovo->ID = busca->ID;
            IDnovo->prox = resultado;
            resultado = IDnovo;
            busca = busca->prox;
        }

        getline(ss, tag, '\''); //espaço entre as aspas

        while (tag == " "){
            getline(ss, tag, '\''); 
            transform(tag.begin(), tag.end(), tag.begin(), ::tolower);
            tag = regex_replace(tag, AZ, "");

            listaID *busca2 = search2(root, tag);
            if (busca2 != NULL){
                listaID *resultadoAux = resultado;
                listaID *resultadoPreAux = NULL;
                while (resultadoAux!=NULL){
                    busca = busca2;
                    while (busca!=NULL && busca->ID!=resultadoAux->ID){
                        busca = busca->prox;
                    }

                    if (busca==NULL){
                        if (resultadoPreAux == NULL){
                            if (resultadoAux->prox == NULL) {
                                resultado = NULL;
                            } else {
                                resultado = resultadoAux->prox;
                            }
                        } else {
                            resultadoPreAux->prox = resultadoAux->prox;
                        }
                    } else {
                        resultadoPreAux = resultadoAux;
                    }
                    resultadoAux = resultadoAux->prox;
                }

                getline(ss, tag, '\''); //espaço entre as aspas
            } else {
                resultado = NULL;
                break;
            }
        }

        if (resultado == NULL){
            cout << "Nenhum jogador foi encontrado!" << endl;
        } else {
            while (resultado!=NULL){
                cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
                buscaJogador(resultado->ID, tabela, tamanho);
                resultado = resultado->prox;
            }
        }

    } else
        cout << "Nenhum jogador foi encontrado!" << endl;
    cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
}

void top(int N, string posicao, jogador *tabela[], int tamanho){
    int achados[N] = {0}, maior;
    float maior_rating;

    for (int i = 0; i < N; i++){
        maior_rating = 0;
        maior = 0;
        for (int j = 0; j < tamanho; j++){
            if (tabela[j]!=NULL){
                jogador *P = tabela[j];
                while (P != NULL){
                    if (P->numAvaliacoes >= 1000) {
                        if (P->dados.find(posicao) != string::npos){ //verifica se a posição faz parte das posições do jogador
                            if (P->ratings->avaliacao > maior_rating){
                                int k = 0;
                                while (k < N && achados[k]!= P->ID){
                                    k++;
                                }
                                if (k==N){
                                    maior_rating = P->ratings->avaliacao;
                                    maior = P->ID;
                                }
                            }
                        }
                    }
                    P = P->prox;
                }
            }
        }
        if (maior == 0 && i==0){
            cout << "Nenhum jogador foi encontrado!" << endl;
            break;
        } else if (maior == 0){
            cout << "Mais nenhum jogador foi encontrado!" << endl;
            break;
        }
        achados[i] = maior;
        cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
        buscaJogador(maior, tabela, tamanho);
        
    }
    
}