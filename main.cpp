//// 22 - MAYO - 2016
#include <iostream>
#include "fstream"

using namespace std;

fstream fp1("ArchivoClientes.txt", ios::in | ios::out|ios::binary /*| ios::trunc*/);
fstream fp2("ArchivoEncomiendas.txt", ios::in | ios::out|ios::binary /*| ios::trunc*/);

//////////////////////////////////////////////////////////////////////////////////////

/// ================= PRINCIPAL ================= ///

struct Cliente{
    int cedula;
    char nombre[10];
    char direccion[10];
    int puntero;
}regClientes;

struct Encomienda{
    int id;
    int peso;
    char descripcion[10];
    char destino[10];
    int sig;
}regEncomiendas;

struct Vertice{
    string nombre;
    struct Vertice *sig;
    struct subLVerticeArco * sublistaArcos;
    bool visitado;
    Vertice (string lug){
        nombre = lug;
        sig = NULL;
        sublistaArcos = NULL;
        visitado = false;
    }
}*grafo;

struct Arco{
    int codigo;
    int distancia;
    struct Vertice *origen;
    struct Vertice *destino;
    struct Arco *sig;
    Arco(int dist, int cod){
        distancia = dist;
        codigo = cod;
        origen = NULL;
        destino = NULL;
        sig = NULL;
    }
}*primeroArco;

struct subLVerticeArco{
    struct Arco *enlaceArco;
    struct subLVerticeArco *sig;
    subLVerticeArco(){
        enlaceArco = NULL;
        sig = NULL;
    }
};



/// ================= BUSQUEDA ================= ///

struct Vertice * buscarVertice(string ciudad){
    struct Vertice *temp = grafo;
    if(grafo==NULL)
        return NULL;
    else{
        while(temp!= NULL){
            if(temp->nombre == ciudad)
                return temp;
            else
                temp = temp->sig;
        }
        return NULL;
    }
}


struct Arco *buscarArco(int c){
    struct Arco *temp = primeroArco;
    if(primeroArco==NULL)
        return NULL;
    else{
        while(temp!=NULL){
            if(temp->codigo == c)
                return temp;
            else
                temp=temp->sig;
        }
        return NULL;
    }
};



/// ================= INSERTAR ================= ///

void insertarVertice(string nombre){
    struct Vertice *nVertice = new Vertice(nombre);

    if(grafo == NULL){
        grafo = nVertice;
        cout<<"Vertice insertado Correctamente"<<endl;
        return;
    }
    else if(buscarVertice(nombre) != NULL){
        cout<<"Datos Repetidos..";
        return;
    }
    nVertice->sig = grafo;
    grafo = nVertice;
    cout<<"Vertice insertado Correctamente"<<endl;
}



void insertarArco(string o, string d, int distancia, int cod){

    struct Vertice * origen = buscarVertice(o);
    struct Vertice * destino = buscarVertice(d);
    if((origen == NULL) or (destino == NULL)){
        cout<<"\n Datos incorrectos.";
        return;
    }

    struct subLVerticeArco *temp = origen->sublistaArcos;
    while(temp != NULL){
        if(temp->enlaceArco->destino == destino || temp->enlaceArco->origen == destino){
            cout<<"Ya existe un camino que conecta estas ciudades..."<<endl;
            return;
        }
        temp = temp->sig;
    }

    struct Arco *nArco = new Arco(distancia, cod);
    nArco->destino = destino;
    nArco->origen = origen;

    if(primeroArco == NULL){
        primeroArco = nArco;
    }
    else{
        nArco->sig = primeroArco;
        primeroArco = nArco;
    }


    //Enlaza origen con el destino
    struct subLVerticeArco *newS = new subLVerticeArco();
    newS->enlaceArco = nArco;
    temp = origen->sublistaArcos;
    if(temp == NULL){
        origen->sublistaArcos = newS;
    }
    else{
        while(temp->sig != NULL){
            temp = temp->sig;
        }
        temp->sig = newS;
    }


    temp = destino->sublistaArcos;
    if(temp == NULL){
        destino->sublistaArcos = newS;
    }
    else{
        while(temp->sig != NULL){
            temp = temp->sig;
        }
        temp->sig = newS;
    }
    cout<<"Arco insertado Correctamente"<<endl;

}

void insertarCliente(char *nomb, int ced, char *direccion, int puntero){
    if(!fp1)
        cout<<"Archivo inexistente!"<<endl;
    strcpy(regClientes.nombre, nomb);
    strcpy(regClientes.direccion, direccion);

    regClientes.cedula=ced;
    regClientes.puntero=puntero;

    fp1.seekg(0,ios::end);
    fp1.write(reinterpret_cast<char *>(&regClientes),sizeof(regClientes));
    fp1.flush();

}



/// ================= MODIFICAR ================= ///

void modificarCamino(string o, string d, int dist){
    //Busca si existe el arco y cambia su distancia
    struct Arco *arco = primeroArco;
    bool existe = false;
    do{
        if((arco->origen->nombre == o && arco->destino->nombre == d)||(arco->destino->nombre == o && arco->origen->nombre == d)){
            arco->distancia = dist;
            existe = true;
            break;
        }
        arco = arco->sig;
    }while(arco != NULL);
    if(existe == false){
        cout<<"El arco que desea modificar no existe"<<endl;
        return;
    }

    //Busca el una de las ciudades y si uno de sus destinos es la otra ciudad cambia su distancia
    struct Vertice *vertice = buscarVertice(o);
    struct subLVerticeArco *arcos = vertice->sublistaArcos;
    do{
        if((arcos->enlaceArco->origen->nombre == d)||(arcos->enlaceArco->destino->nombre == d)){
            arcos->enlaceArco->distancia = dist;
            break;
        }
        else{
            arcos = arcos->sig;
        }
    }while(arcos != NULL);

    //Busca la otra ciudad y si uno de sus destinos es la otra ciudad cambia su distancia
    vertice = buscarVertice(d);
    arcos = vertice->sublistaArcos;
    do{
        if((arcos->enlaceArco->origen->nombre == o)||(arcos->enlaceArco->destino->nombre == o)){
            arcos->enlaceArco->distancia = dist;
            break;
        }
        else{
            arcos = arcos->sig;
        }
    }while(arcos != NULL);
}


/// ================= BORRAR ================= ///

bool borrarCamino(string o, string d){
    struct Vertice *tempO = buscarVertice(o);
    struct Vertice *tempD = buscarVertice(d);
    if(tempO == NULL || tempD == NULL){
        return false;
    }

    if(primeroArco == NULL){
        return false;
    }
    struct Arco *arco = primeroArco;
    int x = 0;
    if(((arco->origen->nombre == tempO->nombre)&&(arco->destino->nombre == tempD->nombre))
       || ((arco->origen->nombre == tempD->nombre)&&(arco->destino->nombre == tempO->nombre))){
        primeroArco = arco->sig;
        x++;
    }
    else{
        while(arco->sig != NULL){
            if(((arco->sig->origen == tempO)&&(arco->sig->destino == tempD))||((arco->sig->origen == tempD)&&(arco->sig->destino == tempO))){
                arco->sig = arco->sig->sig;
                x++;
                break;
            }
            arco = arco->sig;
        }
    }
    if(x == 0){
        return false;
    }
    struct subLVerticeArco *listArcosO = tempO->sublistaArcos;
    if(listArcosO->enlaceArco->destino == tempD  || listArcosO->enlaceArco->origen == tempD){
        tempO->sublistaArcos = listArcosO->sig;
    }
    else{
        while(listArcosO != NULL){
            if(listArcosO->sig->enlaceArco->destino == tempD || listArcosO->sig->enlaceArco->origen == tempD){
                listArcosO->sig = listArcosO->sig->sig;
                break;
            }
            listArcosO = listArcosO->sig;
        }
    }

    struct subLVerticeArco *listArcosD = tempD->sublistaArcos;
    if(listArcosD->enlaceArco->origen == tempO || listArcosD->enlaceArco->destino == tempO){
        tempD->sublistaArcos = listArcosD->sig;
    }
    else{
        while(listArcosD->sig != NULL){
            if(listArcosD->sig->enlaceArco->destino == tempO || listArcosD->sig->enlaceArco->origen == tempO){
                listArcosD->sig = listArcosD->sig->sig;
                break;
            }
            listArcosD = listArcosD->sig;
        }
    }
    return true;
};

void borrarCiudad(string n){
    struct Vertice *v = buscarVertice(n);
    if(v == NULL){
        return;
    }
    struct Vertice *vertice = grafo;
    while(vertice != NULL){
        borrarCamino(n, vertice->nombre);
        vertice = vertice->sig;
    }

    vertice = grafo;
    if(vertice->nombre == n){
        grafo = grafo->sig;
    }
    else{
        while(vertice->sig != NULL){
            if(vertice->sig->nombre == n){
                vertice->sig = vertice->sig->sig;
                break;
            }
            vertice = vertice->sig;
        }
    }
}


/// ================= PLUS ================= ///

void desmarcar(){
    struct Vertice *temp = grafo;
    while(temp!= NULL){
        temp->visitado = false;
        temp = temp->sig;
    }
}

void conexo(Vertice *vertice){

    if(vertice == NULL){
        return;
    }
    else if(vertice->visitado == true){
        return;
    }

    vertice->visitado = true;
    struct Arco *arcos = primeroArco;
    while(arcos != NULL){
        if(arcos->origen == vertice){
            conexo(arcos->destino);
        }
        else if(arcos->destino == vertice){
            conexo(arcos->origen);
        }
        arcos = arcos->sig;
    }
}


/// ================= ARCHIVOS ================= ///


void escribirArchivoClientes(struct Cliente regClientes){
    fp1.write(reinterpret_cast<char *> (&regClientes), sizeof(regClientes));
}

void escribirArchivoEncomiendas(struct Encomienda regEncomiendas){
    fp2.write(reinterpret_cast<char *> (&regEncomiendas), sizeof(regEncomiendas));
}

//////////////////////////////////////////////////////////////////////////////////////


void leerArchivoClientes(){
    fstream temp("ArchivoClientes.txt", ios::in | ios::out |ios::binary);// |ios::trunc);

    temp.seekg(0);
    while(temp.eof() != true){//eof = end of file

        temp.read(reinterpret_cast<char *> (&regClientes), sizeof(regClientes));
        if(temp.eof() != true)
            cout<<"Cedula = "<<regClientes.cedula<<" Cliente = "<<regClientes.nombre<<" Direccion = "<<regClientes.direccion<<endl;
    }
    temp.close();
}

void leerArchivoEncomiendas(){
    fstream temp("ArchivoEncomiendas.txt", ios::in | ios::out |ios::binary);// |ios::trunc);

    temp.seekg(0);
    while(temp.eof() != true){//eof = end of file

        temp.read(reinterpret_cast<char *> (&regEncomiendas), sizeof(regEncomiendas));
        if(temp.eof() != true)
            cout<<"Codigo = "<<regEncomiendas.id<<" Descripcion = "<<regEncomiendas.descripcion<<" Peso = "<<regEncomiendas.peso<<" Destino = "<<regEncomiendas.destino<<endl;
    }
    temp.close();
}

//lee una registro en especifico
void leerArchivoClientes(int posicion){

    fp1.seekg(posicion *sizeof(regClientes));

    fp1.read(reinterpret_cast<char *> (&regClientes), sizeof(regClientes));
    cout<<"Cedula = "<<regClientes.cedula<<" Cliente = "<<regClientes.nombre<<" Direccion = "<<regClientes.direccion<<endl;
}

void leerArchivoEncomiendas(int posicion){

    fp2.seekg(posicion *sizeof(regEncomiendas));

    fp2.read(reinterpret_cast<char *> (&regEncomiendas), sizeof(regEncomiendas));
    cout<<" ID = "<<regEncomiendas.id<<" Descripcion = "<<regEncomiendas.descripcion<<" Peso = "<<regEncomiendas.peso<<" Destino = "<<regEncomiendas.destino<<endl;
}



/// ================== CONSULTAS =================== ///

//Determinar si el grafo de transporte es conexo: si existe un arco entre cualquier par de vértices.
bool Consulta1(){
    struct Vertice *vertice = grafo;
    desmarcar();
    conexo(vertice);
    while(vertice != NULL){
        if(vertice->visitado == false){
            return false;
        }
        vertice = vertice->sig;
    }
    desmarcar();
    return true;

}

/// ================= CARGAR DATOS ================= ///


void cargarDatos(){
}


int main() {
    return 0;
}
