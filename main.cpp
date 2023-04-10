// Rafael Fernando dos reis Mecabô e Matheus Armando Timm Barbieri

#include <iostream>

using namespace std;

struct NoCompromisso {
    //int h.comeco,m.comeco,h.fim,m.fim;
    int hInicio, hFinal;
    int mInicio, mFinal;
    string *texto;
    NoCompromisso *eloP, *eloA;
};

struct listCompromisso {
    NoCompromisso *comeco, *fim;
};

struct NoData {
    unsigned int dia, mes, ano;
    NoData *eloA, *eloP;
    listCompromisso compromisso;
};

struct listData {
    NoData *comeco, *fim;
};

bool anoBissexto(int ano) {
    return ((ano % 4 == 0) && ((!(ano % 100 == 0)) ||
                               (ano % 400 == 0)));
}

void mostrarAgenda(listData lst, char frase[]) {
    NoData *aux;

    cout << frase << ": ";
    aux = lst.comeco;
    while (aux != nullptr) {
        cout << aux->dia << "/" << aux->mes << "/" << aux->ano << "  ";
        aux = aux->eloP;
    }
    cout << endl;
}

NoData *buscarData(listData lst, int d, int m, int a) {
    NoData *aux = lst.comeco;
    while (aux != nullptr) {
        if (aux->dia == d && aux->mes == m && aux->ano == a) return aux;
        aux = aux->eloP;
    }
    return nullptr;
}

int compararData(NoData *data1, NoData *data2) {
    int D_anos = data1->ano - data2->ano;
    int D_meses = data1->mes - data2->mes;
    int D_dias = data1->dia - data2->dia;

    if (D_anos != 0) return D_anos;
    if (D_meses != 0) return D_meses;
    return D_dias;
}

void inicializarLista(listData &lstDat, listCompromisso &lstCompr) {
    lstDat.comeco = nullptr;
    lstDat.fim = nullptr;
    lstCompr.comeco = nullptr;
    lstCompr.fim = nullptr;
}

bool verificarAgenda(listData &lst, int d, int m, int a) {
    NoData *aux = lst.comeco;
    while (aux != nullptr) {
        if (aux->dia == d && aux->mes == m && aux->ano == a) {
            return true;
        }
        aux = aux->eloP;
    }
    return false;
}

bool inserirData(listData &lst, int d, int m, int a) {

    NoData *novaData = new NoData;
    if (novaData == nullptr) return false;

    novaData->dia = d;
    novaData->mes = m;
    novaData->ano = a;

    novaData->eloA = nullptr;
    novaData->eloP = nullptr;

    if (lst.comeco == nullptr) {
        lst.comeco = novaData;
        lst.fim = novaData;
        return true;
    }
    ///Verifica se a nova é menor que a data do começo
    if (a < lst.comeco->ano && m < lst.comeco->mes && d < lst.comeco->dia) {
        novaData->eloP = lst.comeco;
        lst.comeco->eloA = novaData;
        lst.comeco = novaData;
        return true;
    }
    ///Verifica se a nova é menor que a data do final
    if (a > lst.comeco->ano && m > lst.comeco->mes && d > lst.comeco->dia) {
        lst.fim->eloP = novaData;
        novaData->eloA = lst.fim;
        lst.fim = novaData;
        return true;
    }
    NoData *aux = lst.comeco;
    while (aux != nullptr) {
        if (aux->dia == d && aux->mes == m && aux->ano == a) {
            delete novaData; // a data já existe, então não precisamos adicioná-la novamente
            return false;
        }
        if (compararData(novaData, aux) <= 0) { // novaData é anterior ou igual a aux
            if (aux->eloA != nullptr) { // se aux não é o primeiro elemento da lista
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

bool retirarData(listData &lst, int d, int m, int a) {
    NoData *aux, *ant, *prox;

    aux = buscarData(lst, d, m, a);
    if (aux == nullptr) return false; // Valor não encontrado

    ant = aux->eloA;
    prox = aux->eloP;

    // Remover o primeiro ou unico
    if (aux == lst.comeco) {
        lst.comeco = prox;
        if (aux == lst.fim) lst.fim = prox;
        else prox->eloA = nullptr;
    } else {
        ant->eloP = aux->eloP;
        if (aux == lst.fim) lst.fim = ant;
        else prox->eloA = ant;
    }
    delete aux;
    return true;
}

bool removerCompromisso(listData &lst, int dia, int mes, int ano, int hInicio, int mInicio) {
    NoData *data = buscarData(lst, dia, mes, ano);
    if (data == nullptr) return false;

    NoCompromisso *comp = data->compromisso.comeco;
    while (comp != nullptr) {
        if (comp->hInicio == hInicio && comp->mInicio == mInicio) {
            if (comp->eloA != nullptr) {
                comp->eloA->eloP = comp->eloP;
            } else {
                data->compromisso.comeco = comp->eloP;
            }
            if (comp->eloP != nullptr) {
                comp->eloP->eloA = comp->eloA;
            } else {
                data->compromisso.fim = comp->eloA;
            }
            delete comp;
            return true;
        }
        comp = comp->eloP;
    }
}

void inserirCompromisso(listData &lstDat, listCompromisso &lstCompr, int dia, int mes, int ano, int horaInicial, int minutoInicial, int horaFinal, int minutoFinal, string texto) {
    // Insere a data na lista
    inserirData(lstDat, dia, mes, ano);

    // Busca o nó correspondente à data na lista
    NoData *data = buscarData(lstDat, dia, mes, ano);

    // Cria o nó de compromisso
    NoCompromisso *novoCompromisso = new NoCompromisso;
    novoCompromisso->hInicio = horaInicial;
    novoCompromisso->mInicio = minutoInicial;
    novoCompromisso->hFinal = horaFinal;
    novoCompromisso->mFinal = minutoFinal;
    novoCompromisso->texto = new string(texto);
    novoCompromisso->eloA = nullptr;
    novoCompromisso->eloP = nullptr;

    // Associa o nó de compromisso à lista de compromissos do nó de data
    if (data->compromisso.comeco == nullptr) {
        data->compromisso.comeco = novoCompromisso;
        data->compromisso.fim = novoCompromisso;
    } else {
        data->compromisso.fim->eloP = novoCompromisso;
        novoCompromisso->eloA = data->compromisso.fim;
        data->compromisso.fim = novoCompromisso;
    }
}

void mostrarCompromisso(listData lst) {
    NoData *auxData = lst.comeco;
    while (auxData != nullptr) {
        cout << auxData->dia << "/" << auxData->mes << "/" << auxData->ano << ":" << endl;
        NoCompromisso *auxCompromisso = auxData->compromisso.comeco;
        while (auxCompromisso != nullptr) {
            cout << "\t" << auxCompromisso->hInicio << ":" << auxCompromisso->mInicio << " - "
                 << auxCompromisso->hFinal << ":" << auxCompromisso->mFinal << " "
                 << *auxCompromisso->texto << endl;
            auxCompromisso = auxCompromisso->eloP;
        }
        auxData = auxData->eloP;
    }
}

bool verificaData(int d, int m, int a){
    int dias[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if( anoBissexto(a) ) dias[2] = 29;

    if( m >= 1 && m <= 12 && a > 1900 && d > 0 && d <= dias[m] ){
        return 1;
    }
    return 0;
}

bool verificaHora(int hora, int minuto){
    if (hora >= 0 && hora <= 23 && minuto >= 0 && minuto <= 59) {
       return 1;
    }
    return 0;
}

int menu(){
    system("cls");
    int escolha;
    cout<<"================================================================================="<<endl;
    cout<<"Porgrama bonzao\n\n"<<endl;
    cout<<"Escolha oque quer fazer:"<<endl;
    cout<<"1 - Inserir compromisso"<<endl;
    cout<<"2 - Mostrar agenda"<<endl;
    cout<<"3 - Remover data"<<endl;
    cout<<"4 - Remover compromisso"<<endl;
    cout<<"0 - Encerrar."<<endl;
    cout<<"\n\n"<<endl;
    cout<<"================================================================================="<<endl;
    cin>>escolha;
    return escolha;
}

int main() {
    listData lstDat;
    listCompromisso lstCompr;
    inicializarLista(lstDat, lstCompr);
    int res = 1;
    int dia, mes, ano, horaInicio, minInicio, horaFim, minFim;
    string texto;

    inserirCompromisso(lstDat, lstCompr, 2, 4, 2023, 14, 0, 15, 0, "Compromisso 1");
    inserirCompromisso(lstDat, lstCompr, 2, 4, 2023, 10, 30, 11, 30, "Compromisso 2");
    inserirCompromisso(lstDat, lstCompr, 1, 4, 2023, 10, 0, 12, 0, "Compromisso 3");
    while (res != 0) {
        res=menu();

        switch(res){
            case 1://inserir
                system("cls");
                cout<<"Escreva seu compromisso:[d/m/a/hInicial/mInicial/hFinal/mFinal/compromisso]"<<endl;
                cin>>dia >> mes >> ano >> horaInicio >> minInicio >> horaFim >> minFim >> texto;
                if(verificaData(dia,mes,ano)==1 && verificaHora(horaInicio,minInicio)==1 && verificaHora(horaFim,minFim)==1){

                    inserirCompromisso(lstDat, lstCompr, dia, mes, ano, horaInicio, minInicio, horaFim, minFim, texto);

                    cout<<"Compromisso inserido!"<<endl;

                }else{
                    cout<<"Data ou Hora invalida"<<endl;

                }
                break;
            case 2://mostrar

                mostrarCompromisso(lstDat);

                break;
            case 3://remover data

                mostrarAgenda(lstDat, "Datas Inseridas");
                cout<<"Que data quer remover?[d/m/a]"<<endl;
                cin>>dia>>mes>>ano;
                if(retirarData(lstDat,dia,mes,ano)==1){
                    cout<<"A data "<<dia<<"/"<<mes<<"/"<<ano<<" foi removida."<<endl;
                }else{
                    cout<<"A data não existe."<<endl;
                }

                break;
            case 4://remover compromisso

                mostrarCompromisso(lstDat);
                cout<<"Que compromisso quer remover?[d/m/a/hora inicial/minuto inicial]"<<endl;
                cin>>dia >> mes >> ano >> horaInicio >> minInicio;
                if(removerCompromisso(lstDat,dia, mes, ano, horaInicio, minInicio)==1){
                    cout<<"O compromisso "<<dia<<"/"<<mes<<"/"<<ano<<" as "<<horaInicio<<":"<<minInicio<<" foi removido."<<endl;
                }else{
                    cout<<"A data não existe."<<endl;
                }

                break;
            case 0:

                return 0;
                break;
        }
        system("pause");
    }
    return 0;
}
