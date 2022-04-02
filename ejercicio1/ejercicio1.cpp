#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <climits>

using namespace std;

struct Alumno
{
char codigo [5];
char nombre [11];
char apellidos [20];
char carrera [15];


void showData(){
cout<<codigo<<" - "<<nombre<<" - "<<apellidos<<" - "<<carrera<<endl;
}

};

istream & operator >> (istream & stream, Alumno & p)
{	
	stream.get(p.codigo,5);		
	stream.get(p.nombre,11);		
	stream.get(p.apellidos,20);	
	stream.get(p.carrera,15);
    stream.get();
	return stream;
}

ostream & operator << (ostream & stream, Alumno & p)
{	
    stream<<p.codigo;
	stream<<p.nombre;
	stream<<p.apellidos;
    stream<<p.carrera;
	stream<<"\n";
	stream<<flush;
	return stream;
}


class FixedRecordFile
{
private:
    string file_name;
public:
    FixedRecordFile(string file_name){
        this->file_name = file_name;        
    } 
    // LOAD : LEYENDO DATOS FIJOS 
    vector<Alumno> load(){
        
        ifstream file(this->file_name);

        if(!file.is_open()) throw "No se pudo abrir el archivo";
        vector<Alumno> alumnos;
        Alumno record=Alumno();
        char c;
        while(!file.eof()){
            file >> record;
            alumnos.push_back(record);
        }
		file.close();
        return alumnos;
    }

    // AGREGAR DATOS FIJOS
    void add(Alumno a1){
        ofstream file(this->file_name, ios::app); // Se va al final
        if(!file.is_open()) throw "No se puede abrir el archivo";
        file<<a1;
        file.close();
    }
    // LEER EN LA POSICION
    Alumno readRecord(int pos){
        ifstream file(this->file_name, ios::app);
        if(!file.is_open()) throw "No se puede abrir el archivo";
        Alumno record=Alumno();
        //file.seekg(pos*sizeof(Alumno),ios::beg);
        char c;
        int cont=0;
        while(!file.eof()){
            file>>record;
            if(cont==pos-1){
                break;
            }
            cont++;
        }
        file.close();
        return record;
    }
};


void Prueba1(){
    cout<<"PRUEBA 1 : load()"<<endl;    
    Alumno a1;
    FixedRecordFile prueba("datos1.txt");
    // Imprimir datos:
    auto a=prueba.load();
    for (auto& s: a){
        cout << "Nombre: " << s.codigo <<" ";cout << "Nombre: " << s.nombre <<" ";
        cout << "Apellidos: " << s.apellidos <<" ";cout << "Carrera: " << s.carrera << endl;
    }
    cout<<"###########################################################"<<endl;
}

void Prueba2(){

    cout<<"PRUEBA 2 : add(Alumno)"<<endl;
    
    Alumno a1;
    FixedRecordFile prueba("datos1.txt");
    auto a=prueba.load();  
    FixedRecordFile prueba2("datos3.txt"); // Escribir en archivo nuevo
    prueba2.add(a[0]);
    prueba2.add(a[1]);
    FixedRecordFile prueba3("datos3.txt");
    auto b=prueba3.load();
    for (auto& s: b){
        cout << "Nombre: " << s.codigo <<" ";cout << "Nombre: " << s.nombre <<" ";
        cout << "Apellidos: " << s.apellidos <<" ";cout << "Carrera: " << s.carrera << endl;
    }
    cout<<"###########################################################"<<endl;
}

void Prueba3(){
    cout<<"PRUEBA 3 : readRecord(int pos)"<<endl;
    Alumno a1;
    FixedRecordFile prueba("datos1.txt");
    auto b=prueba.readRecord(1);
    cout << "Nombre: " << b.codigo <<" ";cout << "Nombre: " << b.nombre <<" ";
    cout << "Apellidos: " << b.apellidos <<" ";cout << "Carrera: " << b.carrera << endl;

}


int main(){


    //Alumno a=Alumno();
    Prueba1();
    Prueba2();
    Prueba3();
    /*
    ifstream prueba("ptmr.txt");
    char c;
    while(!prueba.eof()){
        prueba >> a;
    }

    cout<<a.codigo<<endl;
*/
    return 0;
}