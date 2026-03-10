#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H
#define T 30
typedef enum _retorno{
	OK, ERROR, NO_IMPLEMENTADO
}tipoRet;

typedef enum _acciones{
	CrearArchivo, BorrarArchivo, CrearVersion, BorrarVersion, MostrarVersiones, InsertarLinea, 
		BorrarLinea, MostrarTexto, MostrarCambios, Iguales, VersionIndependiente, Salir, Error, Ayuda, Limpiar
}acciones;



typedef char *cadena;

typedef struct _textoArchivo{
	cadena linea;
	unsigned int numLinea;
	_textoArchivo *sigLinea = NULL;
} *textoArchivo;

typedef struct _arbolV{
	_arbolV *hijo = NULL;
	cadena Sversion;
	_textoArchivo *texto = NULL; 
	_textoArchivo *historial = NULL;
	_arbolV *hermano = NULL;
}*arbol;

typedef struct _archivo{
	cadena nombre;
	_arbolV *version = NULL;
	struct _archivo* sigVersion;
	struct _archivo* antVersion;
} *archivo;


#endif
