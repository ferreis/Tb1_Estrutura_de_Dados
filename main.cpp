#include <iostream>
using namespace std;

//struct NoCompromisso{
//  int h.comeco,m.comeco,h.fim,m.fim;
//char *texto;
//};

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

void mostrarData( NoData *d ){
    cout << d->dia << "/" << d->mes << "/" << d->ano;
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

bool inserirData(Data &lst, int info){
    NoData *novaData = new NoData;
    if( novaData == NULL ) return false;

    novaData->info = valor;
    novaData->eloA = NULL;
    novaData->eloP = NULL;

}

int main(){
    NoData *d1, *d2;

    d1 = novaData(24, 3, 2023);
    d2 = novaData(29, 4, 2023);

    if( d1 == NULL || d2 == NULL ){
        cout << "Data invalida" << endl;
        return 0;
    }

    mostrarData(d1);
    int x = compararData(d1, d2 );
    if( x == 0 ) cout << " = ";
    else if( x < 0 ) cout << " < ";
    else cout << " > ";
    mostrarData(d2);
    cout << endl;

    return 0;
}
