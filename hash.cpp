#include "hash.h"

using namespace std;

int main(){
    string buffer;
    jogador player;
    rating rank;
    NodoTrie *nomesPai = getNode();
    int tamanhoM, tamanhoR, chave, temp;
    int min = 0, max = 0, media = 0, vazio = 0;
    int media2 = 0, encontrados = 0, total = 0;
    bool f = false;
    listaEncadeada *percorre;

    tamanhoM = 10000;
    jogador *tabelaJog[tamanhoM] = {NULL};

    ifstream &rate = abreArq("minirating.csv");
    ifstream &tags = abreArq("tags.csv");
    ifstream &jogadores = abreArq("players.csv");
    ofstream saidapadrao ("tabelafinal.txt", ios_base::out|ios_base::trunc);

    // Criando uma tabela hash para os jogadores
    getline(jogadores, buffer);
    cout << "Criando tabela hash para os jogadores!" << endl;
    while(getline(jogadores, buffer, ',')){
        player.ID = stoi(buffer);                   //passa as informações do jogador para uma variavel auxiliar
        getline(jogadores, player.nome, ',');
        getline(jogadores, player.dados, '\n');
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

    // Criando uma tabela hash para o rating dos jogadores
    tamanhoR = 10000;
    rating *tabelaRating[tamanhoR] = {NULL};

    getline(rate, buffer);
    cout << "Criando tabela hash para os ratings!" << endl;
    while(getline(rate, buffer, ',')){
        getline(rate, buffer, ',');
        rank.ID = stoi(buffer);
        getline(rate, buffer, '\n');

        listaEncadeada *tempL = new listaEncadeada;
        tempL->avaliacao = stof(buffer);
        tempL->prox = NULL;
        rank.rat = tempL;
        rank.prox = NULL;

        chave = calculaChave(rank.ID, tamanhoR);

        rating *novo;
        if(tabelaRating[chave] == NULL){                //se vazio, posição da tabela = ponteiro para novo rating
            novo = new rating;
            novo->ID = rank.ID;
            novo->rat = rank.rat;
            novo->prox = NULL;
            tabelaRating[chave] = novo;
        }
        else{
            temp = rank.ID;
            novo = tabelaRating[chave];
            // Checando pra ver se ja existe rating do jogador
            while(novo->prox != NULL){
                if(novo->ID == temp){
                    f = true;
                    percorre = novo->rat;
                    novo->numAvaliacoes += 1;
                    while(percorre->prox != NULL){
                        percorre = percorre->prox;
                    }
                    percorre->prox = tempL;
                }
                novo = novo->prox;
            }
            // TODO: MODIFICAR A TABELA HASH DOS RATINGS PARA SEREM A MEDIA DO RATING CADA JOGADOR
            if(f == false){
                novo = new rating;
                novo->ID = rank.ID;
                novo->rat = rank.rat;
                novo->prox = tabelaRating[chave];
                tabelaRating[chave] = novo;
            }
        }
    }

    cout << "Criando trie para os nomes!" << endl;
    regex r("\\s");
    string name;
    for (int i = 0; i < tamanhoM; i++) {
        if (tabelaJog[i] != NULL){
            jogador *j = tabelaJog[i];
            while (j->prox != NULL){
                name = j->nome;
                transform(name.begin(), name.end(), name.begin(), ::tolower);
                name = regex_replace(name, r, "");
                insert(nomesPai, name, j->ID);
                j = j->prox;
            }
        }
    }

    int penis = search(nomesPai, "cristiano");
    cout << name << endl;
    cout << penis << endl;
    return 0;
}