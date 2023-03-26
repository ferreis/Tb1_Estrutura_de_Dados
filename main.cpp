#include <iostream>

using namespace std;

struct NoCompromisso {
    //int h.comeco,m.comeco,h.fim,m.fim;
    int hInicio, hFinal;
    int mInicio, mFinal;
    char *texto;
};

struct NoData {
    unsigned int dia, mes, ano;
    NoData *eloA, *eloP;
    NoCompromisso *comeco, *fim;
};

struct Data {
    NoData *comeco, *fim;
};

bool anoBissexto(int ano) {
    return ((ano % 4 == 0) && ((!(ano % 100 == 0)) ||
                               (ano % 400 == 0)));
}

void mostrarAgenda(Data lst, char frase[]) {
    NoData *aux;

    cout << frase << ": ";
    aux = lst.comeco;
    while (aux != NULL) {
        cout << aux->dia << "/" << aux->mes << "/" << aux->ano << "  ";
        aux = aux->eloP;
    }
    cout << endl;
}

NoData *buscarData(Data lst, int d, int m, int a) {
    NoData *aux = lst.comeco;
    while (aux != NULL) {
        if (aux->dia == d && aux->mes == m && aux->ano == a) return aux;
        aux = aux->eloP;
    }
    return NULL;
}

int compararData(NoData *data1, NoData *data2) {
    int D_anos = data1->ano - data2->ano;
    int D_meses = data1->mes - data2->mes;
    int D_dias = data1->dia - data2->dia;

    if (D_anos != 0) return D_anos;
    if (D_meses != 0) return D_meses;
    return D_dias;
}

void inicializarLista(Data &lst) {
    lst.comeco = NULL;
    lst.fim = NULL;
}

bool verificarAgenda(Data &lst, int d, int m, int a) {
    NoData *aux = lst.comeco;
    while (aux != NULL) {
        if (aux->dia == d && aux->mes == m && aux->ano == a) {
            return true;
        }
        aux = aux->eloP;
    }
    return false;
}

bool inserirData(Data &lst, int d, int m, int a) {

    NoData *novaData = new NoData;
    if (novaData == NULL) return false;

    novaData->dia = d;
    novaData->mes = m;
    novaData->ano = a;

    novaData->eloA = NULL;
    novaData->eloP = NULL;

    if (lst.comeco == NULL) {
        lst.comeco = novaData;
        lst.fim = novaData;
        return true;
    }
    if (a < lst.comeco->ano && m < lst.comeco->mes && d < lst.comeco->dia) {
        novaData->eloP = lst.comeco;
        lst.comeco->eloA = novaData;
        lst.comeco = novaData;
        return true;
    }
    if (a > lst.comeco->ano && m > lst.comeco->mes && d > lst.comeco->dia) {
        lst.fim->eloP = novaData;
        novaData->eloA = lst.fim;
        lst.fim = novaData;
        return true;
    }
    NoData *aux = lst.comeco;
    while (aux != NULL) {
        if (compararData(novaData, aux) <= 0) { // novaData é anterior ou igual a aux
            if (aux->eloA != NULL) { // se aux não é o primeiro elemento da lista
                novaData->eloA = aux->eloA;
                aux->eloA->eloP = novaData;
            } else { // se aux é o primeiro elemento da lista
                lst.comeco = novaData;
            }
            novaData->eloP = aux;
            aux->eloA = novaData;
            return true; // a data foi inserida com sucesso
        }
        aux = aux->eloP;
    }

// se a função chegou até aqui, a nova data é posterior a todas as datas da lista
    lst.fim->eloP = novaData;
    novaData->eloA = lst.fim;
    lst.fim = novaData;
    return true;
}

bool retirarData(Data &lst, int d, int m, int a) {
    NoData *aux, *ant, *prox;

    aux = buscarData(lst, d, m, a);
    if (aux == NULL) return false; // Valor não encontrado

    ant = aux->eloA;
    prox = aux->eloP;

    // Remover o primeiro ou unico
    if (aux == lst.comeco) {
        lst.comeco = prox;
        if (aux == lst.fim) lst.fim = prox;
        else prox->eloA = NULL;
    } else {
        ant->eloP = aux->eloP;
        if (aux == lst.fim) lst.fim = ant;
        else prox->eloA = ant;
    }
    delete aux;
    return true;
}

bool
adicionarCompromisso(Data &lst, int d, int m, int a, int horaInicio, int minutoInicio, int horaFinal, int minutoFinal,
                     char *texto) {
    NoData *data = buscarData(lst, d, m, a);
    if (data == NULL) { // Data não encontrada na lista
        if (!inserirData(lst, d, m, a)) return false; // Não foi possível inserir a data na lista
        data = lst.comeco;
        while (data != NULL && (data->dia != d || data->mes != m || data->ano != a)) {
            data = data->eloP;
        }
    }

    // Criar nova estrutura de compromisso
    NoCompromisso *novoCompromisso = new NoCompromisso;
    if (novoCompromisso == NULL) return false;

    novoCompromisso->hInicio = horaInicio;
    novoCompromisso->mInicio = minutoInicio;
    novoCompromisso->hFinal = horaFinal;
    novoCompromisso->mFinal = minutoFinal;
    novoCompromisso->texto = texto;

    data->fim = NULL;

    // Adicionar novo compromisso à lista de compromissos da data correspondente
    if (data->comeco == NULL) {
        data->comeco = novoCompromisso;
        data->fim = novoCompromisso;
    } else {
        data->fim = novoCompromisso;
        data->fim = novoCompromisso;
    }

    return true;
}

int main() {
    int d, m, a;
    Data agenda;
    bool encerrar = 0;
    inicializarLista(agenda);
    while (encerrar == 0) {

        //Inserir a data e quando inserir a data perguntar se quer btoar um compromisso, se sim chamar a funcao de
        //inserir compromisso naquela data
        cout << "dia: ";
        cin >> d;
        cout << "mes: ";
        cin >> m;
        cout << "ano: ";
        cin >> a;

        int dias[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (anoBissexto(a)) dias[2] = 29;

        if (m >= 1 && m <= 12 && a > 1900 && d > 0 && d <= dias[m]) {
            if (verificarAgenda(agenda, d, m, a) == true) {

            } else {
                inserirData(agenda, d, m, a);
            }
            //retirarData(agenda, 24,3,2020);
            mostrarAgenda(agenda, "Datas");
        } else {
            cout << "data invalida" << endl;
        }
        cout << "deseja encerrar?[1] ";
        cin >> encerrar;
        system("cls");
    }
    return 0;
}
