#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <climits>

using namespace std;

struct Alumno
{
string nombre;
string apellidos;
string carrera;
string mensualidad;

void showData(){
    cout<<"Nombre: "<<nombre<<", ";
    cout<<"Apellidos: "<<apellidos<<", ";
    cout<<"Carrera: "<<carrera<<", ";
    cout<<"Mensualidad: "<<mensualidad<<endl;
}

};


ostream & operator << (ostream & stream, Alumno & p)
{	
	stream<<p.nombre;
    stream<<"|";
	stream<<p.apellidos;
    stream<<"|";
    stream<<p.carrera;
    stream<<"|";    
    stream<<p.mensualidad;
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
        string datos="";
        int cont_atr=0;
        bool is_first_line=true;
        //while(file >> noskipws>>c){
        while(file.get(c)){
        
        if(is_first_line){
            if(c=='\n'){
                is_first_line=false;
            }            
        }
        else{
            if(c!='|'){
                if(c=='\n'){
                    cont_atr=0;
                    record.mensualidad=datos;
                    alumnos.push_back(record);
                    datos="";
                }
                else{
                datos=datos+c;       
                } 
            }
            else{
                cont_atr++;
                if(cont_atr==1){record.nombre=datos;}
                else if(cont_atr==2){record.apellidos=datos;}
                else if(cont_atr==3){record.carrera=datos;}
                datos="";            
            }
        }
        }
        record.mensualidad=datos;
        alumnos.push_back(record);
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
        ifstream file(this->file_name);

        if(!file.is_open()) throw "No se pudo abrir el archivo";
        char c;
        int cont_sn=1;
        int cont_atr=0;
        string datos="";
        bool find_sn=false;
        Alumno record=Alumno();        
        while(file.get(c)){
        
        if(cont_sn==pos+1){
            if(c!='|'){
                if(c=='\n'){
                    cont_atr=0;
                    record.mensualidad=datos;
                    datos="";
                    find_sn=true;
                }
                else{
                datos=datos+c;       
                }
            } 
            else{
                cont_atr++;
                if(cont_atr==1){record.nombre=datos;}
                else if(cont_atr==2){record.apellidos=datos;}
                else if(cont_atr==3){record.carrera=datos;}
                datos="";            
            }
            
        }
        if(c=='\n'){
            cont_sn++;
        }
        }
        if(!find_sn){
            cont_atr=0;
            record.mensualidad=datos;
            datos="";
        }

        return record;
    }

};

void Prueba1(){
    cout<<"PRUEBA 1 : load()"<<endl;    
    cout<<endl;
    FixedRecordFile prueba("datos.txt");

    auto a1=prueba.load();
    for(auto &s:a1){
        s.showData();
    }
    cout<<"###########################################################"<<endl;
}

void Prueba2(){
    cout<<"PRUEBA 2 : add(Alumno)"<<endl;
    cout<<endl;
    FixedRecordFile prueba("datos.txt");
    auto a1=prueba.load();
    FixedRecordFile prueba2("datos4.txt");
    prueba2.add(a1[2]);
    FixedRecordFile prueba3("datos4.txt");
    auto b=prueba3.load();
    for(auto &s:b){
        cout<<s.showData()<<endl;
    }
    cout<<"###########################################################"<<endl;
}

void Prueba3(){
    cout<<"PRUEBA 3 : readRecord(int pos)"<<endl;
    cout<<endl;
    FixedRecordFile prueba("datos.txt");
    auto a1=prueba.readRecord(3);
    a1.showData();
}

int main(){

    Prueba1();
    Prueba2();
    Prueba3();

    return 0;
}