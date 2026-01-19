#include <stdio.h>
#include <stdlib.h>
#include "gestion_citas.h"

int main() {
    
    Cita agenda[MAX_CITAS]; 
    int totalRegistros = 0;
    int opcion;

    
    cargarDesdeTXT(agenda, &totalRegistros);

    do {
        printf("\n========================================\n");
        printf("   SISTEMA DE GESTION MEDICA v2.0\n");
        printf("========================================\n");
        printf("1. Registrar nueva cita\n");
        printf("2. Listar todas las citas\n");
        printf("3. Buscar paciente/cita\n");
        printf("4. Reprogramar (Actualizar) cita\n");
        printf("5. Cancelar (Eliminar) cita\n");
        printf("6. Guardar y Salir\n");
        printf("========================================\n");
        printf("Seleccione una opcion: ");
        
        if (scanf("%d", &opcion) != 1) {
            limpiarBuffer();
            opcion = 0; 
        } else {
            limpiarBuffer(); 
        }

        switch (opcion) {
            case 1:
                registrarCita(agenda, &totalRegistros);
                break;
            case 2:
                listarCitas(agenda, totalRegistros);
                break;
            case 3:
                buscarCita(agenda, totalRegistros);
                break;
            case 4:
                actualizarCita(agenda, totalRegistros);
                break;
            case 5:
                eliminarCita(agenda, &totalRegistros);
                break;
            case 6:
                guardarEnTXT(agenda, totalRegistros);
                printf("Saliendo del sistema...\n");
                break;
            default:
                printf("Opcion invalida. Intente nuevamente.\n");
        }

    } while (opcion != 6);

    return 0;
}