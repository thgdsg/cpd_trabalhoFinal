#include "hash.h"

using namespace std;

int main(){
    string buffer;
    jogador player;
    NodoTrie *nomesPai = getNode();
    int tamanhoM, tamanhoU, chave, temp;
    int chaveU, keyU;
    bool f = false;
    listaEncadeada *percorre;

    tamanhoM = 10000;
    jogador *tabelaJog[tamanhoM] = {NULL};

    ifstream &rate = abreArq("minirating.csv");
    ifstream &tags = abreArq("tags.csv");
    ifstream &jogadores = abreArq("players.csv");
    ofstream saidapadrao ("tabelafinal.txt", ios_base::out|ios_base::trunc);

    const regex aspas("[\"]");
    // Criando uma tabela hash para os jogadores
    getline(jogadores, buffer);
    cout << "Criando tabela hash para os jogadores!" << endl;
    while(getline(jogadores, buffer, ',')){
        player.ID = stoi(buffer);                   //passa as informações do jogador para uma variavel auxiliar
        getline(jogadores, player.nome, ',');
        getline(jogadores, buffer, '\n');
        buffer = regex_replace(buffer, aspas, "");
        player.dados = buffer;
        player.prox = NULL;
        
        chave = calculaChave(player.ID, tamanhoM);

        jogador *novo = new jogador;
        novo->ID = player.ID;
        novo->nome = player.nome;
        novo->dados = player.dados;
        if(tabelaJog[chave] == NULL){                //se vazio, posição da tabela = ponteiro para novo jogador
            novo->prox = NULL;
            tabelaJog[chave] = novo;
        }
        else{                                       //senão, novo jogador aponta para o primeiro jogador e se torna o primeiro jogador da lista
            novo->prox = tabelaJog[chave];
            tabelaJog[chave] = novo;
        }
    }

    // Adicionando o rating dos jogadores na tabela hash
    tamanhoU = 10000;
    jogador *percorreTabela;
    int avaliacao;
    usuario *tabelaUsuario[tamanhoU] = {NULL};

    getline(rate, buffer);
    cout << "Criando tabela hash para os usuarios e adicionando os ratings na tabela hash dos jogadores!" << endl;
    while(getline(rate, buffer, ',')){
        chaveU = stoi(buffer);
        getline(rate, buffer, ',');
        player.ID = stoi(buffer);
        getline(rate, buffer, '\n');

        listaEncadeada *tempL = new listaEncadeada;
        tempL->avaliacao = stof(buffer);
        tempL->prox = NULL;
        player.ratings = tempL;
        player.prox = NULL;

        chave = calculaChave(player.ID, tamanhoM);
        percorreTabela = tabelaJog[chave];
        while(percorreTabela->ID != player.ID)
            percorreTabela = percorreTabela->prox;
        
        percorreTabela->numAvaliacoes += 1;
        if(percorreTabela->ratings == NULL)
            percorreTabela->ratings = player.ratings;
        else{
            player.ratings->prox = percorreTabela->ratings;
            percorreTabela->ratings = player.ratings;
        }
        // criando hash de usuarios
        keyU = calculaChave(chaveU, tamanhoU);
        listaEncadeada *ratingsUser = new listaEncadeada;
        ratingsUser->avaliacao = player.ratings->avaliacao;
        ratingsUser->ID = player.ID;
        ratingsUser->prox = NULL;
        usuario *newUser;
        if(tabelaUsuario[keyU] == NULL){                //se vazio, posição da tabela = ponteiro para novo usuario
            newUser = new usuario;
            newUser->UID = chaveU;
            newUser->ratings = ratingsUser;
            newUser->prox = NULL;
            tabelaUsuario[keyU] = newUser;
        }
        else{
            newUser = tabelaUsuario[keyU];
            // Checando pra ver se ja existe rating do jogador
            while(newUser != NULL){
                if(newUser->UID == chaveU){
                    f = true;
                    newUser->totalAvaliacoes += 1;
                    ratingsUser->prox = newUser->ratings;
                    newUser->ratings = ratingsUser;

                }
                newUser = newUser->prox;
            }
            if(f == false){
                newUser = new usuario;
                newUser->UID = chaveU;
                newUser->ratings = ratingsUser;
                newUser->prox = tabelaUsuario[keyU];
                tabelaUsuario[keyU] = newUser;
            }
            f = false;
        }
        //cout << newUser->UID << "," << newUser->totalAvaliacoes << endl;
    }

    // Transformando a tabela hash de ratings numa tabela hash de media de ratings, depois colocando como atributo do jogador
    double mediaRank = 0;
    listaEncadeada *anteriorList;
    for (int i = 0; i < tamanhoM; i++) {
        if (tabelaJog[i] != NULL){
            jogador *P = tabelaJog[i];
            while (P != NULL){
                listaEncadeada *newList = new listaEncadeada;
                newList->prox = NULL;
                while(P->ratings != NULL){
                    //cout << P->ID << endl;
                    mediaRank = mediaRank + P->ratings->avaliacao;
                    anteriorList = P->ratings;
                    P->ratings = P->ratings->prox;
                    delete anteriorList;
                }
                newList->avaliacao = (float)(mediaRank / P->numAvaliacoes);
                P->ratings = newList;

                P = P->prox;
                mediaRank = 0;
            }
        }
    }

    cout << "Criando trie para os nomes!" << endl;
    const regex AZ("[^abcdefghijklmnopqrstuvwxyz]");
    string name;
    for (int i = 0; i < tamanhoM; i++) {
        if (tabelaJog[i] != NULL){
            jogador *j = tabelaJog[i];
            while (j != NULL){
                name = j->nome;
                transform(name.begin(), name.end(), name.begin(), ::tolower);
                name = regex_replace(name, AZ, "");
                insert(nomesPai, name, j->ID);
                j = j->prox;
            }
        }
    }

    int denis = search(nomesPai, "aleksanderfoosns");
    cout << denis << endl;
    denis = search(nomesPai, "nicolaecarnat");
    cout << denis << endl;
    denis = search(nomesPai, "neymardasilvasantosjunior");
    cout << denis << endl;
    denis = search(nomesPai, "cristianoronaldodossantosaveiro");
    cout << denis << endl;
    
    cout << "BUSCA USER: ";
    buscaUser(11923, tabelaUsuario, tamanhoU);
    cout << "BUSCA JOGADOR: ";
    buscaJogador(231747, tabelaJog, tamanhoM);
    //buscaUser(11923, tabelaUsuario, tamanhoU);
    return 0;
}