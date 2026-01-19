#ifndef GESTION_CITAS_H
#define GESTION_CITAS_H

#include <stdio.h>
#include <string.h> 

#define MAX_CITAS 100
#define ARCHIVO_TXT "citas.txt"
#define LONG_CODIGO 15
#define LONG_NOMBRE 50
#define LONG_FECHA 15

typedef struct {
    char codigo[LONG_CODIGO];
    char nombre[LONG_NOMBRE];
    char especialidad[LONG_NOMBRE];
    char fecha[LONG_FECHA];
    char hora[10];
    char medico[LONG_NOMBRE];
    int activo; 
} Cita;


void cargarDesdeTXT(Cita *lista, int *n);
void guardarEnTXT(Cita *lista, int n);


void registrarCita(Cita *lista, int *n);
void listarCitas(Cita *lista, int n);
void buscarCita(Cita *lista, int n);
void actualizarCita(Cita *lista, int n);
void eliminarCita(Cita *lista, int *n);

void limpiarBuffer();
int validarFecha(const char *fecha);
int existeCodigo(Cita *lista, int n, const char *codigo);
int existeHorario(Cita *lista, int n, const char *fecha, const char *hora);
void leerCadena(char *destino, int longitud);

#endif