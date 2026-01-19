#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "gestion_citas.h"


void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


void leerCadena(char *destino, int longitud) {
    fgets(destino, longitud, stdin);
    size_t len = strlen(destino);
    if (len > 0 && destino[len - 1] == '\n') {
        destino[len - 1] = '\0';
    }
}


int validarFecha(const char *fecha) {
    if (strlen(fecha) != 10) return 0;
    if (fecha[2] != '/' || fecha[5] != '/') return 0;
    
    return 1;
}



void cargarDesdeTXT(Cita *lista, int *n) {
    FILE *f = fopen(ARCHIVO_TXT, "r");
    *n = 0;
    
    if (f == NULL) {
        printf("[INFO] No se encontro archivo previo. Se creara uno nuevo al guardar.\n");
        return;
    }

    char linea[256];
    
    while (fgets(linea, sizeof(linea), f)) {
        if (*n >= MAX_CITAS) break;
        
        
        linea[strcspn(linea, "\n")] = 0;

        Cita temp;
        char *token = strtok(linea, ";");
        if (!token) continue; strcpy(temp.codigo, token);

        token = strtok(NULL, ";"); if (!token) continue; strcpy(temp.nombre, token);
        token = strtok(NULL, ";"); if (!token) continue; strcpy(temp.especialidad, token);
        token = strtok(NULL, ";"); if (!token) continue; strcpy(temp.fecha, token);
        token = strtok(NULL, ";"); if (!token) continue; strcpy(temp.hora, token);
        token = strtok(NULL, ";"); if (!token) continue; strcpy(temp.medico, token);
        
        token = strtok(NULL, ";"); 
        temp.activo = (token) ? atoi(token) : 1;

        if (temp.activo == 1) { 
            lista[*n] = temp;
            (*n)++;
        }
    }
    fclose(f);
    printf(">> Datos cargados exitosamente: %d registros.\n", *n);
}

void guardarEnTXT(Cita *lista, int n) {
    FILE *f = fopen(ARCHIVO_TXT, "w");
    if (f == NULL) {
        printf("Error critico: No se puede escribir en el disco.\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        if (lista[i].activo) {
            fprintf(f, "%s;%s;%s;%s;%s;%s;%d\n",
                    lista[i].codigo, lista[i].nombre, lista[i].especialidad,
                    lista[i].fecha, lista[i].hora, lista[i].medico, lista[i].activo);
        }
    }
    fclose(f);
    printf(">> Datos guardados en '%s'.\n", ARCHIVO_TXT);
}



void registrarCita(Cita *lista, int *n) {
    if (*n >= MAX_CITAS) {
        printf("Error: Base de datos llena.\n");
        return;
    }

    Cita nueva;
    nueva.activo = 1;

    printf("\n--- NUEVA CITA ---\n");
    
    do {
        printf("Codigo unico: ");
        leerCadena(nueva.codigo, LONG_CODIGO);
        if (existeCodigo(lista, *n, nueva.codigo)) {
            printf("Error: Ese codigo ya existe. Intente otro.\n");
        } else {
            break;
        }
    } while (1);

    printf("Nombre Paciente: ");
    leerCadena(nueva.nombre, LONG_NOMBRE);

    printf("Especialidad: ");
    leerCadena(nueva.especialidad, LONG_NOMBRE);

    do {
        printf("Fecha (DD/MM/AAAA): ");
        leerCadena(nueva.fecha, LONG_FECHA);
        if (!validarFecha(nueva.fecha)) {
            printf("Formato incorrecto. Use DD/MM/AAAA.\n");
        }
    } while (!validarFecha(nueva.fecha));

    printf("Hora (HH:MM): ");
    leerCadena(nueva.hora, 10);

    
    if (existeHorario(lista, *n, nueva.fecha, nueva.hora)) {
        printf("ALERTA: Ya existe una cita en ese horario. Registre con precaucion.\n");
    }

    printf("Medico tratante: ");
    leerCadena(nueva.medico, LONG_NOMBRE);

    lista[*n] = nueva; 
    (*n)++; 
    printf(">> Cita registrada con exito.\n");
}

void listarCitas(Cita *lista, int n) {
    if (n == 0) {
        printf("\nNo hay citas registradas.\n");
        return;
    }
    printf("\n%-10s %-20s %-15s %-12s %-8s %-15s\n", "COD", "PACIENTE", "ESPEC.", "FECHA", "HORA", "MEDICO");
    printf("----------------------------------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%-10s %-20s %-15s %-12s %-8s %-15s\n",
               lista[i].codigo, lista[i].nombre, lista[i].especialidad,
               lista[i].fecha, lista[i].hora, lista[i].medico);
    }
    printf("----------------------------------------------------------------------------------------\n");
}

void buscarCita(Cita *lista, int n) {
    char busqueda[LONG_NOMBRE];
    int encontrados = 0;
    
    printf("\nIngrese codigo o nombre a buscar: ");
    leerCadena(busqueda, LONG_NOMBRE);

    printf("\n--- RESULTADOS DE BUSQUEDA ---\n");
    for (int i = 0; i < n; i++) {
        
        if (strcmp(lista[i].codigo, busqueda) == 0 || strstr(lista[i].nombre, busqueda) != NULL) {
            printf(">> [%s] %s | %s %s con Dr/a. %s\n", 
                   lista[i].codigo, lista[i].nombre, lista[i].fecha, lista[i].hora, lista[i].medico);
            encontrados++;
        }
    }
    if (encontrados == 0) printf("No se encontraron coincidencias.\n");
}

void actualizarCita(Cita *lista, int n) {
    char cod[LONG_CODIGO];
    printf("\nIngrese codigo de la cita a modificar: ");
    leerCadena(cod, LONG_CODIGO);

    for (int i = 0; i < n; i++) {
        if (strcmp(lista[i].codigo, cod) == 0) {
            printf("Modificando cita de: %s\n", lista[i].nombre);
            printf("Nueva Fecha (Actual: %s): ", lista[i].fecha);
            leerCadena(lista[i].fecha, LONG_FECHA);
            
            printf("Nueva Hora (Actual: %s): ", lista[i].hora);
            leerCadena(lista[i].hora, 10);
            
            printf(">> Cita actualizada correctamente.\n");
            return;
        }
    }
    printf("Error: Cita no encontrada.\n");
}

void eliminarCita(Cita *lista, int *n) {
    char cod[LONG_CODIGO];
    printf("\nIngrese codigo de la cita a eliminar: ");
    leerCadena(cod, LONG_CODIGO);

    for (int i = 0; i < *n; i++) {
        if (strcmp(lista[i].codigo, cod) == 0) {
            
            printf("¿Seguro que desea eliminar la cita de %s? (s/n): ", lista[i].nombre);
            char confirm;
            scanf("%c", &confirm);
            limpiarBuffer();

            if (confirm == 's' || confirm == 'S') {
                
                for (int j = i; j < *n - 1; j++) {
                    lista[j] = lista[j + 1];
                }
                (*n)--;
                printf(">> Registro eliminado permanentemente.\n");
            } else {
                printf(">> Operacion cancelada.\n");
            }
            return;
        }
    }
    printf("Error: Codigo no encontrado.\n");
}


int existeCodigo(Cita *lista, int n, const char *codigo) {
    for (int i = 0; i < n; i++) {
        if (strcmp(lista[i].codigo, codigo) == 0) return 1;
    }
    return 0;
}

int existeHorario(Cita *lista, int n, const char *fecha, const char *hora) {
    for (int i = 0; i < n; i++) {
        if (strcmp(lista[i].fecha, fecha) == 0 && strcmp(lista[i].hora, hora) == 0) return 1;
    }
    return 0;
}