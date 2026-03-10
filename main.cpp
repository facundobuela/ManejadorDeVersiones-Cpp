#include<iostream>
#include <stdio_ext.h>
#include <thread>   // <-- para std::this_thread::sleep_for
#include <chrono>   // <-- para std::chrono::seconds
using namespace std;
#include "string.h"
#define T 30
#include "estructuras.h"
#include "prototipos.h"

int main (int argc, char *argv[]) {
	
	archivo arch = NULL;
	
	muestroRetorno(RetCrearArchivo(arch, (cadena)"a"));
	cout << "Se ha creado exitosamente el archivo a." << endl;
	
	if (arch == NULL){
		cout << "No se pudo crear el archivo. Finalizando..." << endl;
		return 1;
	}
	
	
	cadena comando = new char[100];
	comando[0] = '\0';
	cadena parametro[5]; 
	
	char delim[] = ",()"; //defino los limitadores en su respectivo orden
	acciones accion;
	
	while (true){
		cout << ">";
		cin.getline(comando, 100);
		
		
		int contador = 0;
		parametro[contador] = strtok(comando, delim);
		while (parametro[contador] != NULL && contador < 4){
			contador++;
			parametro[contador] = strtok(NULL, delim);
		}
		
		if (parametro[0] == NULL) continue;
		
		for(int i = 1; i < 5; i++) {
			if (parametro[i] == NULL) continue; // Si no hay parametro, saltar
			
			cadena str = parametro[i];
			int len = strlen(str);
			
			// Quitar caracteres extra al FINAL (espacios, comillas)
			while (len > 0 && (str[len-1] == ' ' || str[len-1] == '\"')) {
				str[len-1] = '\0'; 
				len--;
			}
			
			// Quitar caracteres extra al INICIO
			while (*str != '\0' && (*str == ' ' || *str == '\"')) {
				str++; // Mover el puntero de inicio
			}
			
			// Asignar el puntero limpio de vuelta al arreglo
			parametro[i] = str;
		}
		
		accion = Error;
		
		if (strcmp(parametro[0], "Salir") == 0) {
			muestroRetorno(RetBorrarArchivo(arch)); 
			// Esperar 2 segundos antes de finalizar
			this_thread::sleep_for(chrono::seconds(1));
			
			exit(0);
		}
		if (strcmp(parametro[0], "CrearArchivo") == 0) accion = CrearArchivo;
		else if (strcmp(parametro[0], "CrearVersion") == 0) accion = CrearVersion;
		else if (strcmp(parametro[0], "BorrarVersion") == 0) accion = BorrarVersion;
		else if (strcmp(parametro[0], "MostrarVersiones") == 0) accion = MostrarVersiones;
		else if (strcmp(parametro[0], "InsertarLinea") == 0) accion = InsertarLinea;
		else if (strcmp(parametro[0], "BorrarLinea") == 0) accion = BorrarLinea;
		else if (strcmp(parametro[0], "MostrarTexto") == 0) accion = MostrarTexto;
		else if (strcmp(parametro[0], "MostrarCambios") == 0) accion = MostrarCambios;
		else if (strcmp(parametro[0], "Iguales") == 0) accion = Iguales;
		else if (strcmp(parametro[0], "VersionIndependiente") == 0) accion = VersionIndependiente;
		else if (strcmp(parametro[0], "Limpiar") == 0) accion = Limpiar;
		else if (strcmp(parametro[0], "Ayuda") == 0) accion = Ayuda;
		
		
		switch (accion){
			
		case CrearArchivo:
			muestroRetorno(RetCrearArchivo(arch, parametro[1]));
			break;
			
		case CrearVersion:
			muestroRetorno(RetCrearVersion(arch, parametro[2]));
			break;
			
		case BorrarVersion:
			muestroRetorno(RetBorrarVersion(arch, parametro[2]));
			break;
			
		case MostrarVersiones:
			muestroRetorno(RetMostrarVersiones(arch));
			break;
			
		case InsertarLinea:
			muestroRetorno(RetInsertarLinea(arch, parametro[2], parametro[3], atoi(parametro[4])));
			break;
			
		case BorrarLinea:
			muestroRetorno(RetBorrarLinea(arch, parametro[2], atoi(parametro[3])));
			break;
			
		case MostrarTexto:
			muestroRetorno(RetMostrarTexto(arch, parametro[2]));
			break;
			
		case MostrarCambios:
			muestroRetorno(RetMostrarCambios(arch, parametro[2]));
			break;
			
		case Iguales:
			muestroRetorno(RetIguales(arch, parametro[2], parametro[3], parametro[4]));
			break;
			
		case VersionIndependiente:
			muestroRetorno(RetVersionIndependiente());
			break;
			
		case Limpiar:
			system("clear");
			break;
			
		case Ayuda:
			help();
			break;
			
		default:
			muestroRetorno(ERROR);
			break;
		}
		
		__fpurge(stdin);
	}
	
	return 0;
}
