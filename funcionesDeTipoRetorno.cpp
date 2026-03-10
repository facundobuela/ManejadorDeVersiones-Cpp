#include<iostream> //FACU
#include "estructuras.h"
#include "prototipos.h"

#include "string.h"
using namespace std;


void muestroRetorno(tipoRet retorno){
	
	switch (retorno){
	case OK:
		cout << "OK"<< endl;
		break;
	case ERROR:
		cout << "ERROR" << endl;
		break;
	case NO_IMPLEMENTADO:
		cout << "NO IMPLEMENTADA"<<endl;
	}
}
	
tipoRet RetCrearArchivo(archivo &arch, cadena nombre){
		
	if(nombre == NULL){
		cout << "Se debe especificar el nombre del archivo" << endl;
		return ERROR;
	}
	
	arch = creoArchivo(nombre);
	return OK;
}		
		
tipoRet RetBorrarArchivo(archivo &a){
	borroArchivo(a);
	cout << "Archivo borrado exitosamente" << endl;
	
	return OK;	
}					

	
tipoRet RetCrearVersion(archivo &arch, cadena version) {
	if (arch == NULL) {
		cout << "ERROR: No existe un archivo cargado." << endl;
		return ERROR;
	}
	
	if (version == NULL || strlen(version) == 0) {
		cout << "ERROR: Version invalida" << endl;
		return ERROR;
	}
	
	return insertarVersion(arch, version);	
}

tipoRet RetBorrarVersion(archivo &a, cadena version) {
		
	if (a == NULL) {
		cout << "ERROR: No hay archivo creado." << endl;
		return ERROR;
	}
	
	if (version == NULL || strcmp(version, "") == 0) {
		cout << "ERROR: Debe especificar una versión a borrar." << endl;
		return ERROR;
	}
	
	if (a->version == NULL) {
		cout << "ERROR: El archivo no contiene versiones." << endl;
		return ERROR;
	}
	

	arbol v = buscarVersionRec(a->version, version);
	if (v == NULL) {
		cout << "ERROR: La versión '" << version << "' no existe." << endl;
		return ERROR;
	}

	a->version = borrarVersion(a->version, version);
	
	cout << "Versión '" << version << "' y sus subversiones fueron borradas exitosamente." << endl;
	return OK;
}
	

tipoRet RetMostrarVersiones(archivo arch){
	cout << arch-> nombre << endl;
	
	if(arch->version == NULL){
		cout << "No hay versiones creadas" << endl;
		return OK;
	}
	cout << endl;
	mostrarVersionesR(arch->version, 0);
	return OK;
}

tipoRet RetInsertarLinea(archivo &arch, cadena version, cadena linea, unsigned int nroLinea){
	
	if (arch == NULL){
		cout << "ERROR: no hay archivo creado" << endl;
		return ERROR;
	}
	
	// Buscamos el nodo _arbolV correspondiente a 'version'
	arbol v = buscarVersionRec(arch->version, version);
	if (v == NULL){
		cout << "ERROR: la version no existe" << endl;
		return ERROR;
	}
	
	// Validar si el _arbolV tiene subversiones (usando el puntero al hijo)
	if (v->hijo != NULL) {
		cout << "ERROR: La version '" << version << "' tiene subversiones y no se puede modificar." << endl;
		return ERROR;
	}
	
	// Contar lineas del _textoArchivo de esta version
	unsigned int cantLineas = cuentoLineas(v->texto); 
	
	// Validar nroLinea contra la cantidad de lineas (n+1)
	if(nroLinea == 0 || nroLinea > cantLineas + 1){
		cout << "ERROR: La linea que desea insertar no es valida (Debe ser entre 1 y " << (cantLineas + 1) << ")" << endl;
		return ERROR;
	}
	
	// Insertamos la linea
	arch = insertoLinea(arch, version, linea, nroLinea);
	return OK;
}

tipoRet RetBorrarLinea(archivo &arch, cadena version, unsigned int nroLinea){
	
	if (arch == NULL) {
		cout << "ERROR: El archivo no ha sido creado." << endl;
		return ERROR;
	}
	if (version == NULL) {
		cout << "ERROR: Se debe especificar una version." << endl;
		return ERROR;
	}
	if (nroLinea <= 0) {
		cout << "ERROR: Numero de linea invalido." << endl;
		return ERROR;
	}
	
	arbol v = buscarVersionRec(arch->version, version);
	if (v == NULL) {
		cout << "ERROR: La version '" << version << "' no existe." << endl;
		return ERROR;
	}
	

	textoArchivo actual = v->texto;
	textoArchivo anterior = NULL;
	
	while (actual != NULL && actual->numLinea != nroLinea) {
		anterior = actual;
		actual = actual->sigLinea;
	}
	

	if (actual == NULL) {
		cout << "ERROR: La linea " << nroLinea << " no existe en la version " << version << "." << endl;
		return ERROR;
	}
	
	
	if (anterior == NULL) {
		v->texto = actual->sigLinea;
	} else {
		anterior->sigLinea = actual->sigLinea;
	}
	
	delete[] actual->linea; // Liberar la cadena
	delete actual;         // Liberar el nodo
	
	
	textoArchivo temp = (anterior == NULL) ? v->texto : anterior->sigLinea;
	while (temp != NULL) {
		temp->numLinea--;  
		temp = temp->sigLinea;
	}
	
	
	// Pasamos NULL como texto porque es un borrado.
	v->historial = guardarHistorial(v->historial, 'B', nroLinea, NULL); 
	
	return OK;
}

tipoRet RetMostrarTexto(archivo arch, cadena version){
	
	if (arch == NULL || version == NULL){
		cout << "ERROR: archivo o version no validos" << endl;
		return ERROR;
	}
		
	arbol v = buscarVersionRec(arch->version, version);
	if (v == NULL){
		cout << "ERROR: la version" << version << " no existe." << endl;
		return ERROR;
	}
	
	cout << arch->nombre << " - " << v->Sversion << endl;
	cout << endl;
	
	if (v->texto == NULL){
		cout << "No contiene lineas" << endl;
		return OK;
	}
	
	textoArchivo txt = v->texto;
	mostrarTextoRec(txt);
	
	return OK;
}

tipoRet RetMostrarCambios(archivo a, cadena version){

	if (a == NULL || version == NULL) return ERROR;
	
	arbol nodoVersion = buscarVersionRec(a->version, version);
	
	if (nodoVersion == NULL) return ERROR;
		
	cout << a->nombre << " - " << version << endl << endl;
	
	if (nodoVersion->historial == NULL) {
		cout << "No se realizaron modificaciones" << endl;
		return OK;
	}
	
	mostrarCambios(nodoVersion->historial);
	return OK;
}
	
tipoRet RetIguales(archivo a, cadena version1, cadena version2, cadena igual){
	if (a == NULL) {
		cout << "ERROR: El archivo no ha sido creado." << endl;
		return ERROR;
	}
	if (version1 == NULL || version2 == NULL) {
		cout << "ERROR: Se deben especificar dos versiones." << endl;
		return ERROR;
	}
	
	// 
	if (igual == NULL) {
		cout << "ERROR: Falta el cuarto parametro ('iguales')." << endl;
		return ERROR;
	}
	if(strcmp(igual, "iguales") != 0){
		cout << "ERROR: El cuarto parametro debe ser 'iguales'." << endl;
		return ERROR;
	}
	
	arbol v1 = buscarVersionRec(a->version, version1);
	if (v1 == NULL) {
		cout << "ERROR: La version '" << version1 << "' no existe." << endl;
		return ERROR;
	}
	
	arbol v2 = buscarVersionRec(a->version, version2);
	if (v2 == NULL) {
		cout << "ERROR: La version '" << version2 << "' no existe." << endl;
		return ERROR;
	}
	
	bool iguales = sonTextosIguales(v1->texto, v2->texto);
	
	if (iguales) {
		cout << "Las versiones '" << version1 << "' y '" << version2 << "' son IGUALES." << endl;
	} else {
		cout << "Las versiones '" << version1 << "' y '" << version2 << "' son DIFERENTES." << endl;
	}
	
	return OK;
}
	
tipoRet RetVersionIndependiente(){
	return NO_IMPLEMENTADO;
}
