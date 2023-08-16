#include "hash.h"

using namespace std;

int main(){
    string buffer;
    jogador player;
    rating rank;
    NodoTrie *nomesPai = getNode();
    int tamanhoM, tamanhoR, chave, temp;
    int min = 0, max = 0;
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
    cout << "Criando tabela hash para a media dos ratings!" << endl;
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
            while(novo != NULL){
                if(novo->ID == temp){
                    f = true;
                    novo->numAvaliacoes += 1;
                    tempL->prox = novo->rat;
                    novo->rat = tempL;


                    /*percorre = tempL;
                    while(percorre != NULL){
                        cout << percorre->avaliacao << " ";
                        percorre = percorre->prox;
                    }
                    cout << endl;*/
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
            f = false;
        }
    }

    // Transformando numa tabela hash de media de ratings
    double mediaRank = 0;
    for (int i = 0; i < tamanhoR; i++) {
        if (tabelaRating[i] != NULL){
            rating *R = tabelaRating[i];
            while (R != NULL){
                listaEncadeada *newList = new listaEncadeada;
                newList->prox = NULL;
                while(R->rat != NULL){
                    mediaRank = mediaRank + R->rat->avaliacao;
                    R->rat = R->rat->prox;
                }
                newList->avaliacao = (float)(mediaRank / R->numAvaliacoes);
                R->rat = newList;
                R = R->prox;
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

    int penis = search(nomesPai, "aleksanderfoosns");
    cout << penis << endl;
    penis = search(nomesPai, "nicolaecarnat");
    cout << penis << endl;
    penis = search(nomesPai, "neymardasilvasantosjunior");
    cout << penis << endl;
    penis = search(nomesPai, "cristianoronaldodossantosaveiro");
    cout << penis << endl;
    
    buscaRating(231747, tabelaRating, tamanhoR);
    return 0;
}