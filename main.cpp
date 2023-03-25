#include <iostream>
using namespace std;


struct NoData{
    unsigned int dia, mes, ano;
    NoData *eloA, *eloP;
};

struct Data{
    NoData *comeco, *fim;
};

bool anoBissexto( int ano ){
    return ((ano % 4 == 0) && ((!(ano % 100 == 0)) ||
                               (ano % 400 == 0)));
}

void mostrarAgenda(Data lst, char frase[] ){
    NoData *aux;

    cout << frase << ": ";
    aux = lst.comeco;
    while( aux != NULL ){
        cout << aux->dia << "/"<< aux->mes << "/"<< aux->ano << "  ";
        aux = aux->eloP;
    }
    cout << endl;
}

NoData *novaData(int d, int m, int a){
    int dias[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if( anoBissexto(a) ) dias[2] = 29;

    if( m >= 1 && m <= 12 && a > 1900 && d > 0 && d <= dias[m] ){
        NoData *data = new NoData;
        data->dia = d;
        data->mes = m;
        data->ano = a;
        return data;
    }
    return NULL;
}


/*Retorna 0 se data1 == data2, um número positivo se
  data1 > data2, ou negativo se data1 < data2. */
int compararData(NoData *data1, NoData *data2){
    int D_anos = data1->ano - data2->ano;
    int D_meses = data1->mes - data2->mes;
    int D_dias = data1->dia - data2->dia;

    if (D_anos != 0) return D_anos;
    if (D_meses != 0) return D_meses;
    return D_dias;
}

void inicializarLista(Data &lst){
    lst.comeco = NULL;
    lst.fim = NULL;
}

bool inserirData(Data &lst, int d, int m, int a){
    NoData *novaData = new NoData;
    if( novaData == NULL ) return false;

    novaData->dia = d;
    novaData->mes = m;
    novaData->ano = a;

    novaData->eloA = NULL;
    novaData->eloP = NULL;

    if( lst.comeco == NULL ){
        lst.comeco = novaData;
        lst.fim = novaData;
        return true;
    }
    if( a < lst.comeco->ano && m < lst.comeco->mes && d < lst.comeco->dia){
        novaData->eloP = lst.comeco;
        lst.comeco->eloA = novaData;
        lst.comeco = novaData;
        return true;
    }
    if( a > lst.comeco->ano && m > lst.comeco->mes && d > lst.comeco->dia ){
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

int main(){
    Data agenda;
    inicializarLista(agenda);
    inserirData(agenda, 24,2,2023);

    inserirData(agenda, 24,3,2023);
    inserirData(agenda, 24,3,2023);
    inserirData(agenda, 24,3,2020);

    inserirData(agenda, 23,3,2023);
    inserirData(agenda, 22,3,2023);

    mostrarAgenda(agenda, "Datas");

    return 0;
}
