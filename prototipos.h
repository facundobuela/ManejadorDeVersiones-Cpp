#ifndef PROTOTIPOS_H
#define PROTOTIPOS_H
#include <sstream> 
tipoRet RetCrearArchivo(archivo &arch, cadena nombre); 											// HECHO
tipoRet RetBorrarArchivo(archivo &a); 															// HECHO
tipoRet RetCrearVersion(archivo &arch, cadena nuevaVersion); 									// HECHO ;)
tipoRet RetBorrarVersion(archivo &a, cadena version); 											// HECHO
tipoRet RetMostrarVersiones(archivo arch);  													// HECHO
tipoRet RetInsertarLinea(archivo &arch, cadena version, cadena linea, unsigned int nroLinea); 	// HECHO 
tipoRet RetBorrarLinea(archivo &arch, cadena version, unsigned int nroLinea); 					// HECHO
tipoRet RetMostrarTexto(archivo arch, cadena version);   										// HECHO
tipoRet RetMostrarCambios(archivo a, cadena version);  											// HECHO
tipoRet RetIguales(archivo a, cadena version1, cadena version2, cadena igual);
tipoRet RetVersionIndependiente();

archivo creoArchivo(cadena nombre);
archivo borroArchivo(archivo a);
archivo insertoLinea(archivo a,  cadena version, cadena linea, unsigned int nroLinea);
archivo borroLinea(archivo arch, cadena version, unsigned int nroLinea);
tipoRet insertarVersion(archivo &arch, cadena nuevaVersion);

arbol buscarVersionRec(arbol a, cadena versionBuscada);
arbol borrarVersion(arbol raiz, cadena version);

bool sonTextosIguales(textoArchivo t1, textoArchivo t2);
bool obtenerPadreString(cadena version, cadena outPadre);

textoArchivo copiarListaTexto(textoArchivo original);
textoArchivo crearTextoVacio();
textoArchivo guardarHistorial(textoArchivo h, char IoB, unsigned int numLinea, cadena texto);

int cuentoLineas(textoArchivo textoAContar);
int obtenerUltimoSegmento(cadena version);

void mostrarVersionesR(arbol a, int nivel);
void mostrarCambios(textoArchivo a);
void muestroRetorno(tipoRet retorno);
void help();
void mostrarTextoRec(textoArchivo txt);
void colocarOrden(textoArchivo &txt);

void reemplazarUltimoSegmento(cadena &version, int nuevoNum);

void renombrarSubArbol(arbol nodo, cadena nuevoPrefijo);
void desplazarVersiones(arbol nodo, cadena versionPadre, int desdeNum);

void liberarArbol(arbol nodo);
void desplazarVersionesHaciaAbajo(arbol nodo, cadena versionPadre);
void borrarListaTexto(textoArchivo &lista);
void borrarArbolVersiones(arbol &nodo);

#endif

