#include<iostream>
using namespace std;
#include "estructuras.h"
#include "prototipos.h"
#include "string.h"
#include <iomanip>
#include <sstream>  

void help(){
	
	cout << "============================================"<< endl;
	cout << "- CrearVersion(archivo,version)"<< endl;
	cout << "- BorrarVersion(archivo,version)"<< endl;
	cout << "- MostrarVersiones(archivo)"<< endl;
	cout << endl;
	cout << "- InsertarLinea(archivo,version,texto,num linea)"<< endl;
	cout << "- BorrarLinea(archivo,version,num linea)"<< endl;
	cout << "- MostrarTexto(archivo,version)"<< endl;
	cout << endl;
	cout << "- MostrarCambios(archivo,version)"<< endl;
	cout << "- Iguales(archivo,version1,version2,iguales)"<< endl;
	cout << "- VersionIndependiente(archivo,version) //sin implementar"<< endl;
	
	cout << "- Limpiar " << endl;
	cout << "============================================"<< endl;
}

void mostrarCambios(textoArchivo a){
	while(a != NULL){
		if(a->linea != NULL){
			cout << "IL\t" << a->numLinea << "\t" << a->linea << endl;
		}else{
			cout << "BL\t" << a->numLinea << endl;
		}
		a = a->sigLinea;
	}
	
}
	
bool sonTextosIguales(textoArchivo t1, textoArchivo t2){
	while (true) {
		
		if (t1 == NULL && t2 == NULL) {
			return true;
		}
		
		if (t1 == NULL || t2 == NULL) {
			return false;
		}
		
		if (t1->numLinea != t2->numLinea) {
			return false;
		}
		
		if (t1->linea == NULL || t2->linea == NULL) {
			if (t1->linea != t2->linea) { // Uno es NULL y el otro no
				return false;
			}
			
		} else if (strcmp(t1->linea, t2->linea) != 0) {
			// Si ninguno es NULL, los comparamos con strcmp
			return false;
		}
		t1 = t1->sigLinea;
		t2 = t2->sigLinea;
	}
}
	

void mostrarVersionesR(arbol a, int nivel){
	if(a == NULL) return;
	if(a != NULL){
		cout << setfill('-') << setw(nivel) << a->Sversion << endl;
		nivel += 5;
		mostrarVersionesR(a->hijo, nivel);
		nivel = nivel - 5;
		mostrarVersionesR(a->hermano, nivel);
	}
	return;
}
		
cadena copiarCadena(cadena origen){
		
	if (origen == NULL) return NULL;
	cadena copia = new char[strlen(origen) + 1];
	strcpy(copia, origen);
	return copia;
}
	
archivo creoArchivo(cadena nombreA){
	archivo nuevo = new _archivo;
	nuevo->nombre = copiarCadena(nombreA);
	nuevo->version = NULL;
	nuevo->sigVersion = NULL;
	nuevo->antVersion = NULL;
	return nuevo;
}
		
textoArchivo crearTextoVacio(){
	
	textoArchivo txt = new _textoArchivo;
	txt->numLinea = 0;
	txt->linea = NULL;		
	return txt;
}
	
int cuentoLineas(textoArchivo textoAContar){
	
	int contador = 0;
	
	if(textoAContar == NULL)
		return 0;
	
	while(textoAContar != NULL){
		contador++;
		textoAContar = textoAContar->sigLinea;
	}
	
	return contador;
}
void colocarOrden(textoArchivo &txt){
	
	textoArchivo txtAux = txt;
	int i = 1;
	
	while (txtAux != NULL){
		txtAux->numLinea = i;
		i++;
		txtAux = txtAux->sigLinea;
	}
}

void mostrarTextoRec(textoArchivo txt){
	
	if (txt == NULL || txt->linea == NULL){
		return;
	}
	
	cout << txt->numLinea << "	";
	cout << txt->linea << endl;
	

	mostrarTextoRec(txt->sigLinea);
}

void borrarListaTexto(textoArchivo &lista) {
	while (lista != NULL) {
		textoArchivo aBorrar = lista;
		lista = lista->sigLinea;
		
		if (aBorrar->linea != NULL) {
			delete[] aBorrar->linea;
		}
		delete aBorrar;
	}
	
}

void borrarArbolVersiones(arbol &nodo) {
	if (nodo == NULL) {
		return; 
	}
	
	// Recorrer recursivamente hijos y hermanos (Post-orden)
	borrarArbolVersiones(nodo->hijo);
	borrarArbolVersiones(nodo->hermano);

	delete[] nodo->Sversion;
	borrarListaTexto(nodo->texto);
	borrarListaTexto(nodo->historial);

	delete nodo;
	nodo = NULL; 
}

archivo insertoLinea(archivo a,  cadena version, cadena lineaA, unsigned int nroLinea){
	arbol nodoVersion = buscarVersionRec(a->version, version);
	
	if (nodoVersion == NULL) {
		return a;
	}
	
	textoArchivo nueva = new _textoArchivo;
	nueva->linea = copiarCadena(lineaA);
	nueva->sigLinea = NULL;
	
	if (nodoVersion->texto == NULL) {
		nueva->numLinea = 1;
		nodoVersion->texto = nueva;
	}
	
	else if (nroLinea == 1) {
		nueva->sigLinea = nodoVersion->texto;
		nodoVersion->texto = nueva;
		colocarOrden(nodoVersion->texto);
	}
	
	// Insertar en el medio o al final
	else {
		textoArchivo actual = nodoVersion->texto;
		textoArchivo anterior = NULL;
		unsigned int contador = 1;
	
		while (actual != NULL && contador < nroLinea) {
			anterior = actual;
			actual = actual->sigLinea;
			contador++;
		}
		
		nueva->sigLinea = actual;
		if (anterior != NULL)
			anterior->sigLinea = nueva;
		
		colocarOrden(nodoVersion->texto);
	}
	
	// === Registro en el historial ===
	if (nodoVersion != NULL) {
		textoArchivo cambio = new _textoArchivo;
		cambio->numLinea = nroLinea;
		cambio->linea = copiarCadena(lineaA); 
		cambio->sigLinea = NULL;
		
		if (nodoVersion->historial == NULL) {
			nodoVersion->historial = cambio;
		} else {
			textoArchivo aux = nodoVersion->historial;
			while (aux->sigLinea != NULL)
				aux = aux->sigLinea;
			aux->sigLinea = cambio;
		}
	}
	
	return a;
}

archivo borroArchivo(archivo a){

	if (a == NULL) return NULL;

	while (a->antVersion != NULL)
		a = a->antVersion;

	while (a != NULL){
		archivo siguiente = a->sigVersion;

		if (a->nombre) delete[] a->nombre;
		if (a->version) delete[] a->version;
		
		delete a;

		a = siguiente;
	}
	
	return NULL;
}

archivo borroLinea(archivo arch, cadena version, unsigned int nroLinea){
	archivo v = arch;
	while (v != NULL && strcmp(v->version->Sversion, version) != 0) {
		v = v->sigVersion;
	}
	if (v == NULL) return arch;
	
	textoArchivo actual = v->version->texto;
	textoArchivo anterior = NULL;
	
	// Buscar la linea
	while (actual != NULL && actual->numLinea < nroLinea) {
		anterior = actual;
		actual = actual->sigLinea;
	}
	
	if (actual == NULL) return arch;
	
	// Borrar la linea
	if (anterior == NULL) {
		v->version->texto = actual->sigLinea;
	} else {
		anterior->sigLinea = actual->sigLinea;
	}
	
	delete[] actual->linea;
	delete actual;
	
	// Reajustar numeracion
	textoArchivo p = (anterior == NULL) ? v->version->texto : anterior->sigLinea;
	while (p != NULL) {
		p->numLinea--;
		p = p->sigLinea;
	}
	
	// === Registrar en el historial ===
	arbol nodoVersion = buscarVersionRec(arch->version, version);
	if (nodoVersion != NULL) {
		textoArchivo cambio = new _textoArchivo;
		cambio->numLinea = nroLinea;
		cambio->linea = NULL; 
		cambio->sigLinea = NULL;
		
		if (nodoVersion->historial == NULL) {
			nodoVersion->historial = cambio;
		} else {
			textoArchivo aux = nodoVersion->historial;
			while (aux->sigLinea != NULL)
				aux = aux->sigLinea;
			aux->sigLinea = cambio;
		}
	}
	
	return arch;
}

		
	
int obtenerUltimoSegmento(cadena version) {
	int num = 0;
	cadena punto = strrchr(version, '.');
	if (punto)
		num = atoi(punto + 1);
	else
		num = atoi(version);
	return num;
}
	
void reemplazarUltimoSegmento(cadena &version, int nuevoNum) {
	char nuevaVersion[50];
	const char* punto = strrchr(version, '.');
	if (punto) {
		int lenPrefijo = punto - version;
		strncpy(nuevaVersion, version, lenPrefijo);
		nuevaVersion[lenPrefijo] = '\0';
		sprintf(nuevaVersion + lenPrefijo, ".%d", nuevoNum);
	} else {
		sprintf(nuevaVersion, "%d", nuevoNum);
	}
	delete[] version;
	version = new char[strlen(nuevaVersion) + 1];
	strcpy(version, nuevaVersion);
}
	

	
	
//Copia una lista enlazada de texto (usado para heredar texto del padre).
textoArchivo copiarListaTexto(textoArchivo original) {
	if (original == NULL) {
		return NULL;
	}
	
	textoArchivo nuevaCabeza = new _textoArchivo;
	nuevaCabeza->linea = copiarCadena(original->linea);
	nuevaCabeza->numLinea = original->numLinea;
	nuevaCabeza->sigLinea = NULL;
	
	textoArchivo actOriginal = original->sigLinea;
	textoArchivo actNuevo = nuevaCabeza;
	
	while (actOriginal != NULL) {
		textoArchivo nuevoNodo = new _textoArchivo;
		nuevoNodo->linea = copiarCadena(actOriginal->linea);
		nuevoNodo->numLinea = actOriginal->numLinea;
		nuevoNodo->sigLinea = NULL;
		
		actNuevo->sigLinea = nuevoNodo;
		
		actNuevo = actNuevo->sigLinea;
		actOriginal = actOriginal->sigLinea;
	}
	
	return nuevaCabeza;
}

bool obtenerPadreString(cadena version, cadena outPadre) {

	strcpy(outPadre, version);
	cadena ultimoPunto = strrchr(outPadre, '.');
	if (ultimoPunto != NULL) {
		*ultimoPunto = '\0';
		return true; 
	}
	return false; 
}
		
void renombrarSubArbol(arbol nodo, cadena nuevoPrefijo) {
	arbol actual = nodo;
	while (actual != NULL) {
				
		int miSegmento = obtenerUltimoSegmento(actual->Sversion);
		char miNuevoNombre[100];
	
		sprintf(miNuevoNombre, "%s.%d", nuevoPrefijo, miSegmento);
		delete[] actual->Sversion;
	
		actual->Sversion = copiarCadena(miNuevoNombre);
		renombrarSubArbol(actual->hijo, miNuevoNombre);
	
		actual = actual->hermano;
	}
}
		
void desplazarVersionesHaciaAbajo(arbol nodo, const char* versionPadre) {
	arbol actual = nodo;
	
	while (actual != NULL) {
		
		int miSegmento = obtenerUltimoSegmento(actual->Sversion);
		int nuevoSegmento = miSegmento - 1;
		char miNuevoNombre[100];
	
		if (versionPadre == NULL) {
			sprintf(miNuevoNombre, "%d", nuevoSegmento);
		} else {
			sprintf(miNuevoNombre, "%s.%d", versionPadre, nuevoSegmento);
		}
		
		delete[] actual->Sversion;
		actual->Sversion = copiarCadena(miNuevoNombre);
		renombrarSubArbol(actual->hijo, miNuevoNombre);
		actual = actual->hermano;
	}
}

void desplazarVersiones(arbol nodo, cadena versionPadre, int desdeNum) {
	arbol actual = nodo;
	while (actual != NULL) {
		int miSegmento = obtenerUltimoSegmento(actual->Sversion);
		
		if (miSegmento >= desdeNum) {
			
			int nuevoSegmento = miSegmento + 1;
			char miNuevoNombre[100];
			
			if (versionPadre == NULL) { // Nivel raiz
				sprintf(miNuevoNombre, "%d", nuevoSegmento);
			} else {
				sprintf(miNuevoNombre, "%s.%d", versionPadre, nuevoSegmento);
			}
			
			delete[] actual->Sversion;
			actual->Sversion = copiarCadena(miNuevoNombre);
			
			renombrarSubArbol(actual->hijo, miNuevoNombre);
		}
		
		actual = actual->hermano;
	}
}


arbol buscarVersionRec(arbol a, cadena versionBuscada) {
	if (a == NULL) return NULL;
	
	if (strcmp(a->Sversion, versionBuscada) == 0)
		return a;
	
	arbol encontrado = buscarVersionRec(a->hijo, versionBuscada);
	if (encontrado != NULL) return encontrado;
	
	return buscarVersionRec(a->hermano, versionBuscada);
}


void liberarArbol(arbol nodo) {
	if (nodo == NULL) return;
	
	liberarArbol(nodo->hijo);
	
	
	// Libera texto
	textoArchivo t = nodo->texto;
	while (t != NULL) {
		textoArchivo siguiente = t->sigLinea;
		delete[] t->linea;
		delete t;
		t = siguiente;
	}
	
	// Libera historial
	textoArchivo h = nodo->historial;
	while (h != NULL) {
		textoArchivo siguiente = h->sigLinea;
		delete[] h->linea;
		delete h;
		h = siguiente;
	}
	
	delete[] nodo->Sversion;
	delete nodo;
}


arbol borrarVersion(arbol raiz, cadena version) {
	if (raiz == NULL) return NULL;
	
	if (strcmp(raiz->Sversion, version) == 0) {
		
		arbol proximoHermano = raiz->hermano; 
		raiz->hermano = NULL; 
		liberarArbol(raiz);   
		return proximoHermano;
	}
	
	// Si no es el nodo que buscamos, seguimos buscando recursivamente
	raiz->hijo = borrarVersion(raiz->hijo, version);
	raiz->hermano = borrarVersion(raiz->hermano, version);  
	
	return raiz;
}


tipoRet insertarVersion(archivo &arch, cadena nuevaVersion) {
	
	arbol nueva = new _arbolV;
	nueva->Sversion = copiarCadena(nuevaVersion);
	nueva->texto = NULL;
	nueva->historial = NULL;
	nueva->hijo = NULL;
	nueva->hermano = NULL;
	
	// Si es la primera version del archivo
	if (arch->version == NULL) {
		arch->version = nueva;
		return OK; 
	}
	
	// Buscar al padre
	cadena sPadre = new char[T];
	if (!obtenerPadreString(nuevaVersion, sPadre)) {
		
		arbol temp = arch->version;
		while (temp->hermano != NULL) {
			
			if(strcmp(temp->Sversion, nuevaVersion) == 0){
				cout << "ERROR: La version " << nuevaVersion << " ya existe." << endl;
				// Liberar la memoria de 'nueva' que no usamos
				delete[] nueva->Sversion;
				delete nueva;
				delete[] sPadre;
				return ERROR;
			}
			temp = temp->hermano;
		}
	
		// Chequeo por si es el ultimo
		if(strcmp(temp->Sversion, nuevaVersion) == 0){
			cout << "ERROR: La version " << nuevaVersion << " ya existe." << endl;
			delete[] nueva->Sversion;
			delete nueva;
			delete[] sPadre;
			return ERROR;
		}
		
		temp->hermano = nueva;
	} else {
		// Si llegamos aca es una subversion
		arbol padre = buscarVersionRec(arch->version, sPadre);
		
		
		if (padre == NULL) {
			cout << "Error: No existe la version previa " << sPadre << endl;
			
			// Liberar la memoria de 'nueva' que no usamos
			delete[] nueva->Sversion;
			delete nueva;
			delete[] sPadre;
			return ERROR;
		}
		
		// (Logica para agregar como hijo del padre)
		if (padre->hijo == NULL) {
			padre->hijo = nueva;
		} else {
			arbol temp = padre->hijo;
			while (temp->hermano != NULL) {
				if(strcmp(temp->Sversion, nuevaVersion) == 0){
					cout << "ERROR: La version " << nuevaVersion << " ya existe." << endl;
					delete[] nueva->Sversion;
					delete nueva;
					delete[] sPadre;
					return ERROR;
				}
				temp = temp->hermano;
			}
			if(strcmp(temp->Sversion, nuevaVersion) == 0){
				cout << "ERROR: La version " << nuevaVersion << " ya existe." << endl;
				delete[] nueva->Sversion;
				delete nueva;
				delete[] sPadre;
				return ERROR;
			}
			
			temp->hermano = nueva;
		}
	}
	
	delete[] sPadre;
	return OK; 
}

textoArchivo guardarHistorial(textoArchivo h, char IoB, unsigned int numLinea, cadena texto) {
	
	textoArchivo nuevoHistorial = new _textoArchivo;
	nuevoHistorial->numLinea = numLinea;
	nuevoHistorial->sigLinea = NULL;
	
	// Asignar la linea
	if (IoB == 'I' && texto != NULL) {
		nuevoHistorial->linea = new char[strlen(texto) + 1];
		strcpy(nuevoHistorial->linea, texto);
	} else {
		nuevoHistorial->linea = NULL;
	}
	
	// Agregar al final de la lista de historial h
	if (h == NULL) {
		
		return nuevoHistorial;
	} else {
		
		textoArchivo temp = h;
		while (temp->sigLinea != NULL) {
			temp = temp->sigLinea;
		}
		temp->sigLinea = nuevoHistorial;
		return h; // Devuelve el puntero al inicio de la lista
	}
}















