//// 22 - MAYO - 2016
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;


/// =============== ARCHIVOS =============== ///

fstream fp1("ArchivoClientes.txt", ios::in | ios::out|ios::binary | ios::trunc);
fstream fp2("ArchivoEncomiendas.txt", ios::in | ios::out|ios::binary | ios::trunc);


/// =============== PRINCIPAL =============== ///

struct Vertice{
    string nombre;
    struct Vertice *sig;
    struct subLVerticeArco *sublistaArcos;
    bool visitado;
    Vertice (string n){
        nombre = n;
        sig = NULL;
        sublistaArcos = NULL;
        visitado = false;
    }
}*grafo;

//sublista
struct Arco{
    struct Vertice *destino;
    struct Vertice *origen;
    int distancia;
    struct Arco *sig;
    Arco(int d, int c){
        distancia = d;
        destino=NULL;
        origen=NULL;
        sig = NULL;
    }
}*primeroArco;

struct subLVerticeArco{
    struct Arco *enlaceArco;
    struct subLVerticeArco *sig;
    subLVerticeArco(){
        sig=NULL;
    }
};

struct Cliente{
    char nombre[10];
    int cedula;
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


//Arbol binario ordenado por ID lectura archivo encomienda.
struct arbolEncomienda{
    int id;
    struct arbolEncomienda *izq, *der;
    arbolEncomienda(int i){
        id = i;
        izq=NULL;
        der=NULL;
    }
};

/// =============== BUSQUEDA =============== ///

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



/// =============== INSERTAR PRINCIPAL =============== ///

void insertarVertice(string nombre){

    struct Vertice *nVertice = new Vertice(nombre);

    if(grafo == NULL){
        grafo = nVertice;
    }
    else{
        if(buscarVertice(nombre) != NULL){
            cout<<"Datos Repetidos..";
            return;
        }
        else{
            nVertice->sig = grafo;
            grafo = nVertice;
        }
    }
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
        if(temp->enlaceArco->destino == destino){
            cout<<"Ya existe un camino que conesta estas ciudades..."<<endl;
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
    struct subLVerticeArco *newS = new subLVerticeArco;
    temp = origen->sublistaArcos;
    if(temp == NULL){
        temp = newS;
        temp->enlaceArco = nArco;
    }
    else{
        while(temp->sig != NULL){
            temp = temp->sig;
        }
        temp->sig = newS;
        temp->sig->enlaceArco = nArco;
    }


    temp = destino->sublistaArcos;
    if(temp == NULL){
        temp = newS;
        temp->enlaceArco = nArco;
    }
    else{
        while(temp->sig != NULL){
            temp = temp->sig;
        }
        temp->sig = newS;
        temp->sig->enlaceArco = nArco;
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

/// =============== PLUS =============== ///

void desmarcar(){
    struct Vertice *temp = grafo;
    while(temp != NULL){
        temp->visitado = false;
        temp = temp->sig;
    }
}

///METODOS PARA ESCRIBIR EN LOS ARCHIVOS

void escribirArchivoClientes(struct Cliente regClientes){
    fp1.write(reinterpret_cast<char *> (&regClientes), sizeof(regClientes));
}

void escribirArchivoEncomiendas(struct Encomienda regEncomiendas){
    fp2.write(reinterpret_cast<char *> (&regEncomiendas), sizeof(regEncomiendas));
}

///METODOS PARA LEER LOS ARCHIVOS
void leerArchivoClientes(){
    fstream temp("ArchivoClientes.txt", ios::in | ios::out |ios::binary);// |ios::trunc);

    temp.seekg(0);
    while(temp.eof() != true){//eof = end of file

        temp.read(reinterpret_cast<char *> (&regClientes), sizeof(regClientes));
        if(temp.eof() != true)
            cout<<"Cedula: "<<regClientes.cedula<<"\tCliente: "<<regClientes.nombre<<"\t\tDireccion: "<<regClientes.direccion<<endl;
    }
    temp.close();
}

void leerArchivoEncomiendas(){
    fstream temp("ArchivoEncomiendas.txt", ios::in | ios::out |ios::binary);// |ios::trunc);

    temp.seekg(0);
    while(temp.eof() != true){//eof = end of file

        temp.read(reinterpret_cast<char *> (&regEncomiendas), sizeof(regEncomiendas));
        if(temp.eof() != true)
            cout<<"Codigo: "<<regEncomiendas.id<<"\tDescripcion: "<<regEncomiendas.descripcion<<"\tPeso: "<<regEncomiendas.peso<<"\tDestino: "<<regEncomiendas.destino<<endl;
    }
    temp.close();
}


/// =============== CARGAR =============== ///

void cargarDatos(){


}


int main() {
    insertarCliente("Stward",2014,"Talamanca", 2);
    leerArchivoClientes();
    cout<<endl;

    return 0;
}
