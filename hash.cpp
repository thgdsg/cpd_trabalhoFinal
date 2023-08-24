#include "hash.h"

// Feito por Thomas Schneider Wiederkehr e Thiago dos Santos Goncalves
using namespace std;

int main(){
    // Declarando variáveis que serão utilizadas ao construir as estruturas de dados
    string buffer;
    jogador player;
    NodoTrie *nomesPai = getNode();
    NodoTrie2 *trieTags = getNode2();
    int tamanhoM, tamanhoU, chave, temp;
    int chaveU, keyU;
    bool f = false;
    listaEncadeada *percorre;

    // Criando a tabela hash dos jogadores
    tamanhoM = 10000;
    jogador *tabelaJog[tamanhoM] = {NULL};

    // Abrindo os arquivos necessários
    ifstream &rate = abreArq("rating.csv");
    ifstream &tags = abreArq("tags.csv");
    ifstream &jogadores = abreArq("players.csv");

    cout << "BEM VINDO A BUSCA DE BANCO DE DADOS DE JOGADORES FIFA2021!" << endl;
    cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << "Status:" << endl;
    const regex aspas("[\"]");
    // Criando uma tabela hash para os jogadores
    getline(jogadores, buffer);
    cout << "Criando tabela hash para os jogadores..." << endl;
    while(getline(jogadores, buffer, ',')){
        // Pega as informações da linha conforme o modelo dos dados "sofifa_id,name,player_positions"
        player.ID = stoi(buffer);                   //passa as informações do jogador para uma variavel auxiliar
        getline(jogadores, player.nome, ',');
        getline(jogadores, buffer, '\n');
        buffer = regex_replace(buffer, aspas, "");
        player.dados = buffer;
        player.prox = NULL;
        
        chave = calculaChave(player.ID, tamanhoM);

        // Cria um jogador novo para inserir na tabela hash
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

    // Adicionando o rating dos jogadores na tabela hash associada ao ID de cada jogador, e criando uma tabela hash nova para os usuários
    tamanhoU = 30000;
    jogador *percorreTabela;
    int avaliacao;
    usuario *tabelaUsuario[tamanhoU] = {NULL};

    getline(rate, buffer);
    cout << "Criando tabela hash para os usuarios..." << endl;
    cout << "Adicionando os ratings na tabela hash dos jogadores..." << endl;
    while(getline(rate, buffer, ',')){
        chaveU = stoi(buffer);
        getline(rate, buffer, ',');
        player.ID = stoi(buffer);
        getline(rate, buffer, '\n');

        // Criando uma lista encadeada de avaliações para associar a cada jogador
        listaEncadeada *tempL = new listaEncadeada;
        tempL->avaliacao = stof(buffer);
        tempL->prox = NULL;
        player.ratings = tempL;
        player.prox = NULL;

        chave = calculaChave(player.ID, tamanhoM);
        percorreTabela = tabelaJog[chave];
        // Busca na tabela pelo ID de cada jogador
        while(percorreTabela->ID != player.ID)
            percorreTabela = percorreTabela->prox;
        
        percorreTabela->numAvaliacoes += 1;
        // Se o jogador não possui ratings, o nodo da lista é diretamente associado ao jogador
        if(percorreTabela->ratings == NULL)
            percorreTabela->ratings = player.ratings;
        // Se o jogador possui ratings, o nodo da lista se torna o topo da lista de ratings do jogador
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
            // Checando pra ver se ja existe rating associado ao usuario nessa posição da tabela
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

    cout << "Adicionando a media dos ratings como atributos dos jogadores..." << endl;
    // Transformando a lista de ratings do jogador numa media de ratings total do jogador
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

    cout << "Criando trie para os nomes..." << endl;
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

    cout << "Criando trie para as tags..." << endl;
    string tag;
    getline(tags, buffer);
    while (getline(tags, buffer, ',')){
        getline(tags, buffer, ',');
        int id = stoi(buffer);
        getline(tags, tag, '\n');
        transform(tag.begin(), tag.end(), tag.begin(), ::tolower);
        tag = regex_replace(tag, AZ, "");
        insert2(trieTags, tag, id);
    }
    cout << "Processamento de dados finalizado!" << endl;
    
    cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    /*buscaUser(11923, tabelaUsuario, tabelaJog, tamanhoU, tamanhoM);
    cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    buscaJogador(231747, tabelaJog, tamanhoM);
    cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    buscaUser(1445, tabelaUsuario, tabelaJog, tamanhoU, tamanhoM);*/

    bool saida = false;
    string entrada, comando, bufferEntrada, dados;
    int pos;
    const regex maiusculo("[^ABCDEFGHIJKLMNOPQRSTUVWXYZ]");
    while(saida == false){
        cout << "Digite qual busca deseja realizar agora com seus parametros, ou escreva SAIR para sair" << endl;
        fflush(stdin);
        getline(cin, entrada);
        pos = entrada.find(" ");
        bufferEntrada = entrada.substr(0, pos);
        if (bufferEntrada == "SAIR"){
            saida = true;
        }
        else if(bufferEntrada == "player"){
            bufferEntrada = entrada.substr(pos + 1);
            transform(bufferEntrada.begin(), bufferEntrada.end(), bufferEntrada.begin(), ::tolower);
            bufferEntrada = regex_replace(bufferEntrada, AZ, "");
            buscaNome(bufferEntrada, nomesPai, tabelaJog, tamanhoM);
        }
        else if(bufferEntrada == "user"){
            bufferEntrada = entrada.substr(pos + 1);
            temp = stoi(bufferEntrada);
            buscaUser(temp, tabelaUsuario, tabelaJog, tamanhoU, tamanhoM);
        }
        else if(bufferEntrada == "tags"){
            bufferEntrada = entrada.substr(pos + 1);
            buscaTags(bufferEntrada, trieTags, tabelaJog, tamanhoM);
        }
        bufferEntrada = entrada.substr(0, 3);
        if(bufferEntrada == "top"){
            comando = entrada.substr(sizeof("top") - 1, pos);
            temp = stoi(comando);
            bufferEntrada = entrada.substr(pos + 1);
            transform(bufferEntrada.begin(), bufferEntrada.end(), bufferEntrada.begin(), ::toupper);
            bufferEntrada = regex_replace(bufferEntrada, maiusculo, "");
            top(temp, bufferEntrada, tabelaJog, tamanhoM);
        }
    }
    return 0;
}