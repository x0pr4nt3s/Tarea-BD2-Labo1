#include <iostream>
#include <fstream>
#include <cstring>
#include <climits>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

class Alumno//Record
{	
public:    
	char codigo[5];    
    char nombre[11];    
	char apellidos[20];    
	char carrera[15];    
    int ciclo;
    float mensualidad;
    int next_del=0;

public:

    void setData(){
        string mensualidad_temporal;
        cout<<"Codigo:";
        cin.getline(codigo,sizeof(codigo));
        cout<<"Nombre:";
        cin.getline(nombre,sizeof(nombre));
        cout<<"Apellidos:";
        cin.getline(apellidos,sizeof(apellidos));
        cout<<"Carrera";
        cin.getline(carrera,sizeof(carrera));
        cout<<"Ciclo";
        cin>>ciclo;
        cout<<"Mensualidad";
        cin>>mensualidad_temporal;
        mensualidad=stof(mensualidad_temporal);
        cout<<mensualidad<<endl;
    }

   
    Alumno(){};
    void showData(){
        cout<<codigo<<setw(10)<<nombre<<setw(25)<<apellidos<<setw(20)<<carrera<<setw(16)<<ciclo<<setw(18)<<mensualidad<<setw(16)<<next_del<<endl;
    }

};





class FixedRecordFile
{
private:
    string file_name;
public:

    FixedRecordFile(string file_name){
        this->file_name = file_name;        
    } 



    void writeRecord(Alumno record){
        fstream file(this->file_name, ios::in | ios::out | ios::app | ios::binary);
        if(!file.is_open()) throw "No se pudo abrir el archivo";
        file.seekp(0, ios::end);
        int header;
        if(file.tellp()==0){
            header=-1;
            file.write(reinterpret_cast<const char *>(&header),sizeof(header));
            file.seekp(0, ios::end);
            file.write((char*) &record, sizeof(Alumno));//guardar en formato binario
            file.close();
            return;
        }    
        file.seekp(0, ios::beg);
        file.read((char*)&header,sizeof(header));
        if(header!=-1){
            int temporal;
            file.seekg(sizeof(header)+(header)*sizeof(Alumno)-sizeof(header),ios::beg);
            file.read((char*)&temporal,sizeof(temporal));
            file.seekg(0,ios::beg);
            file.write(reinterpret_cast<const char *>(&temporal),sizeof(temporal));
            file.seekg(sizeof(header)+(header-1)*sizeof(Alumno),ios::beg);
            file.write((char*) &record, sizeof(Alumno));//guardar en formato binario
        }else{
            file.seekp(0, ios::end);
            file.write((char*) &record, sizeof(Alumno));//guardar en formato binario
        }

        file.close();
    }  



    void writeRecord(Alumno record, int pos){
        ofstream file(this->file_name, ios::app | ios::binary);
        if(!file.is_open()) throw "No se pudo abrir el archivo";
        file.seekp(pos * sizeof(Alumno)+sizeof(int), ios::beg);//fixed length record
        file.write((char*) &record, sizeof(Alumno));
        file.close();
    } 



    vector<Alumno> scanAll(){
        ifstream file(this->file_name, ios::in | ios::binary);
        if(!file.is_open()) throw "No se pudo abrir el archivo";
     
        vector<Alumno> alumnos;
        Alumno record; 
        int header;
        file.read((char*)&header,sizeof(header));
        file.seekg(sizeof(header),ios::beg);        
        cout<<header<<endl;
        while(!file.eof()){
            record = Alumno();               
            file.read((char*) &record, sizeof(Alumno));
           //cout<<file.tellg()<<endl;
            if(file.tellg()==-1){
                break;
            }
            alumnos.push_back(record);    
        }
        file.close();
        return alumnos;
    } 



    Alumno readRecord(int pos){
        ifstream file(this->file_name, ios::binary);
        if(!file.is_open()) throw "No se pudo abrir el archivo";
        Alumno record;
        file.seekg((pos-1) * sizeof(Alumno)+sizeof(int), ios::beg);//fixed length record
        file.read((char*) &record, sizeof(Alumno));
        file.close();
        return record;
    }



    int num_records(){
        ifstream file(this->file_name, ios::binary);
        if(!file.is_open()) throw "No se pudo abrir el archivo";   
        file.seekg(0, ios::end);//ubicar cursos al final del archivo
        long size = file.tellg();//cantidad de bytes del archivo        
        file.close();
        return size / sizeof(Alumno);

    }





    bool delete_record(int pos){
        if(pos<0 or pos>num_records()) throw "Posicion fuera de los limites";
        fstream file(this->file_name, ios::in | ios::out |ios::binary );
        if(!file.is_open()) throw "No se pudo abrir el archivo";
        int header;
        file.read((char*)&header,sizeof(header));
        file.seekp(0, ios::beg);
        file.write(reinterpret_cast<const char *>(&pos),sizeof(pos));
        file.seekg(sizeof(header)+(pos)*sizeof(Alumno)-sizeof(int),ios::beg);
        file.write(reinterpret_cast<const char *>(&header),sizeof(header));

        file.close();

        return true;

    } 



};





void Prueba1(FixedRecordFile &prueba){
    cout<<"\n********PRUEBA 1********"<<endl;
    auto valores2 = prueba.scanAll();
    for (auto val : valores2){
        val.showData();
    }
    cout<<"\n********PRUEBA 1********\n\n"<<endl;

}





void Prueba2(FixedRecordFile &prueba){
    cout<<"\n********PRUEBA 2********"<<endl;
    cout<<"ESCRIBIENDO NUEVA DATA: "<<endl;
    Alumno nuevo = Alumno();
    nuevo.setData();
    prueba.writeRecord(nuevo);
    cout<<"\n********PRUEBA 2********\n\n"<<endl; 

}



void Prueba3(FixedRecordFile &prueba){
    cout<<"\n********PRUEBA 3********"<<endl;
    cout<<"DATA EN LA POSICION 3"<<endl;
    prueba.readRecord(3).showData();
    cout<<"DATA EN LA POSICION 5"<<endl;
    prueba.readRecord(5).showData();
    cout<<"DATA EN LA POSICION 1"<<endl;
    prueba.readRecord(1).showData();
    cout<<"********PRUEBA 3********\n\n"<<endl;
}



void Prueba4(FixedRecordFile &prueba){
    cout<<"\n********PRUEBA 4********"<<endl;
    cout<<"ELIMINANDO REGISTRO 6"<<endl;
    prueba.delete_record(6);
    cout<<"ELIMINANDO REGISTRO 2"<<endl;
    prueba.delete_record(2);
    cout<<"ELIMINANDO REGISTRO 4\n"<<endl;
    prueba.delete_record(4);
    cout<<"ESTADO DE LOS REGISTROS LUEGO DE LA ELIMINACION"<<endl;
    auto valores2 = prueba.scanAll();
    for (auto val : valores2){
        val.showData();
    }
    cout<<"********PRUEBA 4********\n\n"<<endl;

}





int main()
{

    FixedRecordFile prueba("datosFinales.txt");

    Prueba1(prueba);
    Prueba2(prueba);
    Prueba3(prueba);
    Prueba4(prueba);


  

    return 0;

}