//Programa creado originalmente el 9 de Agosto de 2024 por: Luis Eduardo Vadillo Rojas
//Sistema de Gestión de Trabajadores
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



//Modulo de Datos de Trabajadores  --------------------------------------------------------------------------

#define CAPACIDAD 50000 // Tamano de la Tabla Hash

// Funcion para limpiar el buffer de entrada
void limpiar_buffer() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

typedef struct {
	char curp[19];
	char rfc[14];
	char nss[12];
	char nombres[100];
	char primer_apellido[100];
	char segundo_apellido[100];
	char pais_origen[100];
	char ciudad_residencia[100];
	char num_cuenta[20];
	char genero;
	int proporciono_ine;
	int proporciono_comprobante_domicilio;
	int proporciono_comprobante_estudios;
} DatosTrabajador;  //Estructura de datos que contiene los datos del trabajador

typedef struct {
	DatosTrabajador* trabajador;
} ElementoHash; /*Estructura que define que cada trabajador debe tener cada
									/dato que se encuentra en la estructura DatosTrabajador.*/

typedef struct {
	ElementoHash** elementos;
	int tamano;
	int conteo;
} TablaHash;

// Funcion hash creada en el primer apellido
unsigned long funcion_hash(char* str) {
	unsigned long i = 0;
	for (int j = 0; str[j]; j++)
		i += str[j];
	return i % CAPACIDAD;
}

// Crear un nuevo elemento de la tabla hash
ElementoHash* crear_elemento(DatosTrabajador* trabajador) {
	ElementoHash* elemento = (ElementoHash*)malloc(sizeof(ElementoHash));
	elemento->trabajador = trabajador;
	return elemento;
}

// Crear una nueva tabla hash
TablaHash* crear_tabla(int tamano) {
	TablaHash* tabla = (TablaHash*)malloc(sizeof(TablaHash));
	tabla->tamano = tamano;
	tabla->conteo = 0;
	tabla->elementos = (ElementoHash**)calloc(tabla->tamano, sizeof(ElementoHash*));
	return tabla;
}

// Insertar un trabajador en la tabla hash
void insertar(TablaHash* tabla, DatosTrabajador* trabajador) {
	unsigned long indice = funcion_hash(trabajador->primer_apellido);
	ElementoHash* elemento = crear_elemento(trabajador);
	if (tabla->elementos[indice] == NULL) {
		tabla->elementos[indice] = elemento;
		tabla->conteo++;
	} else {
		// Manejar colision (sondeo lineal)
		while (tabla->elementos[indice] != NULL) {
			indice = (indice + 1) % tabla->tamano;
		}
		tabla->elementos[indice] = elemento;
		tabla->conteo++;
	}
}

// Buscar un trabajador por primer apellido en la tabla hash
DatosTrabajador* buscar(TablaHash* tabla, char* primer_apellido) {
	unsigned long indice = funcion_hash(primer_apellido);
	while (tabla->elementos[indice] != NULL) {
		if (strcmp(tabla->elementos[indice]->trabajador->primer_apellido, primer_apellido) == 0) {
			return tabla->elementos[indice]->trabajador;
		}
		indice = (indice + 1) % tabla->tamano;
	}
	return NULL;
}

// Mostrar la informacion de un trabajador
void mostrar_trabajador(DatosTrabajador* trabajador) {
	if (trabajador == NULL) {
		printf("Trabajador no encontrado.\n");
		return;
	}
	printf("\n\nNombres: %s\n", trabajador->nombres);
	printf("Primer Apellido: %s\n", trabajador->primer_apellido);
	printf("Segundo Apellido: %s\n", trabajador->segundo_apellido);
	printf("Genero: %c\n", trabajador->genero);  // Muestra el gC)nero
	printf("CURP: %s\n", trabajador->curp);
	printf("RFC: %s\n", trabajador->rfc);
	printf("NSS: %s\n", trabajador->nss); //Numero de Seguridad Social
	printf("Pais de Origen: %s\n", trabajador->pais_origen);
	printf("Ciudad de Residencia: %s\n", trabajador->ciudad_residencia);
	printf("Numero de Cuenta: %s\n", trabajador->num_cuenta);
	printf("Proporciono INE: %s\n", trabajador->proporciono_ine ? "Si" : "No");
	printf("Proporciono Comprobante de Domicilio: %s\n", trabajador->proporciono_comprobante_domicilio ? "Si" : "No");
	printf("Proporciono Comprobante de Estudios: %s\n", trabajador->proporciono_comprobante_estudios ? "Si" : "No");
}

// Mostrar todos los trabajadores registrados
void mostrar_todos_los_trabajadores(TablaHash* tabla) {
	int encontrados = 0;
	for (int i = 0; i < tabla->tamano; i++) {
		if (tabla->elementos[i] != NULL) {
			mostrar_trabajador(tabla->elementos[i]->trabajador);
			printf("\n");
			encontrados++;
		}
	}
	if (encontrados == 0) {
		printf("No hay trabajadores registrados.\n");
	}
}

// Editar los datos de un trabajador
void editar_trabajador(DatosTrabajador* trabajador) {
	if (trabajador == NULL) {
		printf("Trabajador no encontrado.\n");
		return;
	}
	printf("Editar datos del trabajador:\n");

	printf("Nombres actuales: %s. Ingrese nuevos nombres (o presione Enter para dejar sin cambios): ", trabajador->nombres);
	fgets(trabajador->nombres, 100, stdin);
	strtok(trabajador->nombres, "\n"); // Eliminar salto de lC-nea

	printf("Nueva Ciudad de Residencia (o presione Enter para dejar sin cambios): ");
	fgets(trabajador->ciudad_residencia, 100, stdin);
	strtok(trabajador->ciudad_residencia, "\n");

	// Editar gC)nero
	printf("Genero actual: %c. Ingrese nuevo genero (M/F/X, o presione Enter para dejar sin cambios): ", trabajador->genero);
	char nuevo_genero = getchar();
	limpiar_buffer();  // Limpiar el buffer de entrada
	if (nuevo_genero == 'M' || nuevo_genero == 'F' || nuevo_genero == 'X') {
		trabajador->genero = nuevo_genero;
	}
}

// Eliminar un trabajador por primer apellido
void eliminar_trabajador(TablaHash* tabla, char* primer_apellido) {
	unsigned long indice = funcion_hash(primer_apellido);
	while (tabla->elementos[indice] != NULL) {
		if (strcmp(tabla->elementos[indice]->trabajador->primer_apellido, primer_apellido) == 0) {
			free(tabla->elementos[indice]->trabajador);
			free(tabla->elementos[indice]);
			tabla->elementos[indice] = NULL;
			tabla->conteo--;
			printf("Trabajador eliminado exitosamente.\n");
			return;
		}
		indice = (indice + 1) % tabla->tamano;
	}
	printf("Trabajador no encontrado.\n");
}

// Liberar memoria asignada a la tabla hash
void liberar_tabla(TablaHash* tabla) {
	for (int i = 0; i < tabla->tamano; i++) {
		if (tabla->elementos[i] != NULL) {
			free(tabla->elementos[i]->trabajador);
			free(tabla->elementos[i]);
		}
	}
	free(tabla->elementos);
	free(tabla);
}

// Funcion principal
void sys_ges_tra() {
	TablaHash* tabla = crear_tabla(CAPACIDAD);
	int opcion;
	char primer_apellido[100];
	DatosTrabajador* trabajador;

	do {
		printf("\n--- Sistema de Gestion de Trabajadores ---\n");
		printf("1. Agregar Trabajador\n");
		printf("2. Editar Trabajador\n");
		printf("3. Buscar Trabajador\n");
		printf("4. Mostrar Todos los Trabajadores\n");
		printf("5. Eliminar Trabajador\n");
		printf("6. Salir\n");
		printf("Seleccione una opcion: ");
		scanf("%d", &opcion);
		limpiar_buffer(); // Limpia el salto de linea residual del buffer

		switch (opcion) {
		case 1:
			trabajador = (DatosTrabajador*)malloc(sizeof(DatosTrabajador));

			printf("Ingrese Nombres: ");
			fgets(trabajador->nombres, 100, stdin);
			strtok(trabajador->nombres, "\n");

			printf("Ingrese Primer Apellido: ");
			fgets(trabajador->primer_apellido, 100, stdin);
			strtok(trabajador->primer_apellido, "\n");

			printf("Ingrese Segundo Apellido: ");
			fgets(trabajador->segundo_apellido, 100, stdin);
			strtok(trabajador->segundo_apellido, "\n");

			// Agregar gC)nero
			printf("Ingrese Genero (M para Masculino, F para Femenino, X para No Binario): ");
			trabajador->genero = getchar();
			limpiar_buffer();  // Limpiamos el buffer despues de leer el gC)nero

			if (trabajador->genero != 'M' && trabajador->genero != 'F' && trabajador->genero != 'X') {
				printf("GC)nero no valido.\n");
				free(trabajador);
				break;
			}

			printf("Ingrese CURP (Deben de ser 18 caracteres alfanumericos): ");
			fgets(trabajador->curp, 19, stdin);
			strtok(trabajador->curp, "\n");
			limpiar_buffer();  // Limpiamos el buffer despues de leer el CURP

			if (strlen(trabajador->curp) != 18 || strlen(trabajador->curp) > 18) {
				printf("CURP invalida.\n");
				free(trabajador);
				return;
			}

			printf("Ingrese RFC (Deben ser 13 caracteres alfanumericos): ");
			fgets(trabajador->rfc, 14, stdin);
			strtok(trabajador->rfc, "\n");
			limpiar_buffer();  // Limpiamos el buffer despues de leer el RFC

			if (strlen(trabajador->rfc) != 13) {
				printf("RFC invalido.\n");
				free(trabajador);
				return;
			}

			printf("Ingrese NSS (Deben ser 11 numeros enteros del 0 al 9): ");
			fgets(trabajador->nss, 12, stdin);
			strtok(trabajador->nss, "\n");
			if (strlen(trabajador->nss) != 11) {
				printf("NSS invalido.\n");
				free(trabajador);
				break;
			}

			printf("Ingrese Pais de Origen: ");
			fgets(trabajador->pais_origen, 100, stdin);
			strtok(trabajador->pais_origen, "\n");
			limpiar_buffer();  // Limpiamos el buffer despues de leer el CURP

			printf("Ingrese Ciudad de Residencia: ");
			fgets(trabajador->ciudad_residencia, 100, stdin);
			strtok(trabajador->ciudad_residencia, "\n");

			printf("Ingrese NC:mero de Cuenta donde se va a depositar el salario (10 a 18 digitos): ");
			fgets(trabajador->num_cuenta, 20, stdin);
			strtok(trabajador->num_cuenta, "\n");

			// Validacion para Proporciono INE
			printf("Proporciono INE (1 para Si, 0 para No): ");
			while (scanf("%d", &trabajador->proporciono_ine) != 1 || (trabajador->proporciono_ine != 0 && trabajador->proporciono_ine != 1)) {
				printf("Valor no valido. Debe ser 0 o 1.\n");
				printf("Proporciono INE (1 para Si, 0 para No): ");
				limpiar_buffer(); // Limpia el buffer de entrada
			}

			// ValidaciC3n para ProporcionC3 Comprobante de Domicilio
			printf("Proporciono Comprobante de Domicilio (1 para Si, 0 para No): ");
			while (scanf("%d", &trabajador->proporciono_comprobante_domicilio) != 1 || (trabajador->proporciono_comprobante_domicilio != 0 && trabajador->proporciono_comprobante_domicilio != 1)) {
				printf("Valor no valido. Debe ser 0 o 1.\n");
				printf("ProporcionC3 Comprobante de Domicilio (1 para Si, 0 para No): ");
				limpiar_buffer(); // Limpia el buffer de entrada
			}

			// ValidaciC3n para Proporciono Comprobante de Estudios
			printf("Proporciono Comprobante de Estudios (1 para Si, 0 para No): ");
			while (scanf("%d", &trabajador->proporciono_comprobante_estudios) != 1 || (trabajador->proporciono_comprobante_estudios != 0 && trabajador->proporciono_comprobante_estudios != 1)) {
				printf("Valor no valido. Debe ser 0 o 1.\n");
				printf("Proporciono Comprobante de Estudios (1 para Si, 0 para No): ");
				limpiar_buffer(); // Limpia el buffer de entrada
			}

			// VerificaciC3n de documentos necesarios
			if (trabajador->proporciono_ine == 0 || trabajador->proporciono_comprobante_domicilio == 0 || trabajador->proporciono_comprobante_estudios == 0) {
				printf("\n\nEl trabajador debe proporcionar todos los documentos necesarios por ley.\n");
				free(trabajador);
				break;
			}

			insertar(tabla, trabajador);
			printf("\n\nTrabajador agregado exitosamente.\n\n");
			break;

		case 2:
			if (tabla->conteo == 0) {
				printf("No hay trabajadores registrados.\n");
			} else {
				printf("Ingrese el Primer Apellido del Trabajador a editar: ");
				fgets(primer_apellido, 100, stdin);
				strtok(primer_apellido, "\n");
				trabajador = buscar(tabla, primer_apellido);
				editar_trabajador(trabajador);
			}
			break;

		case 3:
			if (tabla->conteo == 0) {
				printf("No hay trabajadores registrados.\n");
			} else {
				printf("Ingrese el Primer Apellido del Trabajador a buscar: ");
				fgets(primer_apellido, 100, stdin);
				strtok(primer_apellido, "\n");
				trabajador = buscar(tabla, primer_apellido);
				mostrar_trabajador(trabajador);
			}
			break;

		case 4:
			if (tabla->conteo == 0) {
				printf("No hay trabajadores registrados.\n");
			} else {
				mostrar_todos_los_trabajadores(tabla);
			}
			break;

		case 5:
			if (tabla->conteo == 0) {
				printf("No hay trabajadores registrados.\n");
			} else {
				printf("Ingrese el Primer Apellido del Trabajador a eliminar: ");
				fgets(primer_apellido, 100, stdin);
				strtok(primer_apellido, "\n");
				eliminar_trabajador(tabla, primer_apellido);
			}
			break;

		case 6:
			printf("Saliendo...\n\n\n");
			break;

		default:
			printf("Opcion no valida. Intente de nuevo.\n");
		}
	} while (opcion != 6);

	liberar_tabla(tabla);
}

//FIN Modulo de Datos de Trabajadores----------------------------------------------------------------------------------------



//Modulo de Cumplimiento de Asistencia-------------------------------------------------------------------------------------------------
#define MAX_TRABAJADORES 100
#define JORNADA_DIURNA_MAX 8
#define JORNADA_NOCTURNA_MAX 7
#define JORNADA_MIXTA_MAX 7.5
#define HORAS_EXTRAS_MAX_DIA 3
#define HORAS_EXTRAS_MAX_SEMANA 9
#define DIAS_LABORALES_SEMANA 6

typedef enum {
	DIURNA,
	NOCTURNA,
	MIXTA
} TipoJornada;

typedef struct {
	int id;
	char nombre[50];
	TipoJornada tipo_jornada;
	float horas_trabajadas;
	float horas_extras;
	int dias_trabajados;
	bool descanso_semanal;
} Trabajador;

Trabajador trabajadores[MAX_TRABAJADORES];
int num_trabajadores = 0;

void registrar_horas(int id);

float obtener_jornada_maxima(TipoJornada tipo) {
	switch (tipo) {
	case DIURNA:
		return JORNADA_DIURNA_MAX;
	case NOCTURNA:
		return JORNADA_NOCTURNA_MAX;
	case MIXTA:
		return JORNADA_MIXTA_MAX;
	default:
		return 0;
	}
}

void registrar_trabajador() {
	if (num_trabajadores < MAX_TRABAJADORES) {
		Trabajador t;
		t.id = num_trabajadores + 1;

		printf("Ingrese el nombre del trabajador: ");
		scanf("%49s", t.nombre);

		int jornada;
		do {
			printf("Seleccione el tipo de jornada (0: Diurna, 1: Nocturna, 2: Mixta): ");
			scanf("%d", &jornada);
			if (jornada < 0 || jornada > 2) {
				printf("Valor invalido. Por favor, seleccione un valor entre 0 y 2.\n");
			}
		} while (jornada < 0 || jornada > 2);

		t.tipo_jornada = (TipoJornada)jornada;

		t.horas_trabajadas = 0;
		t.horas_extras = 0;
		t.dias_trabajados = 0;
		t.descanso_semanal = false;

		trabajadores[num_trabajadores++] = t;
		printf("Trabajador registrado: %s (ID: %d)\n", t.nombre, t.id);

		printf("Ahora, registre las horas trabajadas este dia para este trabajador.\n");
		registrar_horas(t.id);
	} else {
		printf("Error: Numero maximo de trabajadores alcanzado.\n");
	}
}

void analizar_cumplimiento(Trabajador* t) {
	float jornada_maxima = obtener_jornada_maxima(t->tipo_jornada);
	float horas_esperadas = jornada_maxima * t->dias_trabajados;
	float porcentaje_asistencia = (t->horas_trabajadas / horas_esperadas) * 100;

	printf("\nAnalisis de cumplimiento para %s:\n", t->nombre);
	printf("- Tipo de jornada: %s\n", t->tipo_jornada == DIURNA ? "Diurna" : (t->tipo_jornada == NOCTURNA ? "Nocturna" : "Mixta"));
	printf("- Jornada m%cxima diaria: %.2f horas\n", 160, jornada_maxima);
	printf("- Horas trabajados: %d\n", t->dias_trabajados);
	printf("- Horas trabajadas: %.2f\n", t->horas_trabajadas);
	printf("- Horas extras: %.2f\n", t->horas_extras);
	printf("- Porcentaje de asistencia: %.2f%%\n", porcentaje_asistencia);

	if (porcentaje_asistencia >= 100) {
		printf("  El trabajador cumple con el 100%% de asistencia.\n");
	} else if (porcentaje_asistencia >= 80) {
		printf("  El trabajador cumple con al menos el 80%% de asistencia.\n");
	} else {
		printf("  El trabajador no cumple con el minimo del 80%% de asistencia.\n");
	}

	if (t->horas_extras > 0) {
		if (t->horas_extras <= HORAS_EXTRAS_MAX_SEMANA) {
			printf("  Las horas extras estan dentro del limite semanal permitido.\n");
		} else {
			printf("  Se ha excedido el limite semanal de horas extras.\n");
		}
	}

	if (t->dias_trabajados >= DIAS_LABORALES_SEMANA && !t->descanso_semanal) {
		printf("  Advertencia: No se ha tomado el dia de descanso semanal.\n");
	}
}

void registrar_horas(int id) {
	float horas;
	Trabajador* t = NULL;

	for (int i = 0; i < num_trabajadores; i++) {
		if (trabajadores[i].id == id) {
			t = &trabajadores[i];
			break;
		}
	}

	if (t == NULL) {
		printf("Error: Trabajador no encontrado.\n");
		return;
	}

	printf("Ingrese las horas trabajadas este dia para %s: ", t->nombre);
	scanf("%f", &horas);

	float jornada_maxima = obtener_jornada_maxima(t->tipo_jornada);
	float horas_regulares = (horas > jornada_maxima) ? jornada_maxima : horas;
	float horas_extras = (horas > jornada_maxima) ? horas - jornada_maxima : 0;

	t->horas_trabajadas += horas_regulares;
	t->horas_extras += horas_extras;
	t->dias_trabajados++;

	if (t->dias_trabajados % DIAS_LABORALES_SEMANA == 0) {
		t->descanso_semanal = true;
	}

	printf("\nHoras registradas para %s:\n", t->nombre);
	printf("- Horas regulares: %.2f\n", horas_regulares);
	printf("- Horas extras: %.2f\n", horas_extras);

	if (horas_extras > HORAS_EXTRAS_MAX_DIA) {
		printf("Advertencia: Se excedio el limite diario de horas extras.\n");
	}

	analizar_cumplimiento(t);
}

void mostrar_menu() {
	printf("\n--- Modulo de Cumplimiento de Asistencia ---\n");
	printf("1. Registrar trabajador\n");
	printf("2. Registrar horas trabajadas este dia\n");
	printf("3. Salir\n");
	printf("Seleccione una opcion: ");
}

void mod_cump_asis() {
	int opcion;
	do {
		mostrar_menu();
		scanf("%d", &opcion);

		switch(opcion) {
		case 1:
			registrar_trabajador();
			break;
		case 2:
		{
			int id;
			printf("Ingrese el ID del trabajador: ");
			scanf("%d", &id);
			registrar_horas(id);
		}
		break;
		case 3:
			printf("Saliendo del programa...\n\n\n");
			break;
		default:
			printf("Opcion no vC!lida. Intente de nuevo.\n");
		}
	} while(opcion != 3);
}


//FIN Modulo de Cumplimiento de Asistencia-----------------------------------------------------------------------------------------------------

//Modulo de Calculo de Salario-----------------------------------------------

#define NUM_EMPLEADOS 5
#define LONGITUD_FECHA 20  // Tamano de la cadena para almacenar la fecha

// Declaracion de funciones
void Mostrar(char nombres[NUM_EMPLEADOS][50], float salariosBrutos[NUM_EMPLEADOS],
             float impuestos[NUM_EMPLEADOS], float seguridadSocial[NUM_EMPLEADOS],
             float aportacionInfonavit[NUM_EMPLEADOS], float aportacionAfore[NUM_EMPLEADOS],
             float ahorroAnual[NUM_EMPLEADOS], float salarioNeto[NUM_EMPLEADOS],
             int edades[NUM_EMPLEADOS], char fechaPago[NUM_EMPLEADOS][LONGITUD_FECHA],
             char lugaresPago[NUM_EMPLEADOS][50], int numEmpleados);
void AgregarEmpleado(char nombres[NUM_EMPLEADOS][50], float salariosBrutos[NUM_EMPLEADOS],
                     float impuestos[NUM_EMPLEADOS], float seguridadSocial[NUM_EMPLEADOS],
                     float aportacionInfonavit[NUM_EMPLEADOS], float aportacionAfore[NUM_EMPLEADOS],
                     float ahorroAnual[NUM_EMPLEADOS], float salarioNeto[NUM_EMPLEADOS],
                     int edades[NUM_EMPLEADOS], int *numEmpleados,
                     char fechaPago[NUM_EMPLEADOS][LONGITUD_FECHA],
                     char lugaresPago[NUM_EMPLEADOS][50]);
float calcularISR(float salarioBruto);
float calcularSeguridadSocial(float salarioBruto);
float calcularInfonavit(float salarioBruto);
float calcularAfore(float salarioBruto, int edad);
float calcularAhorroAnual(float salarioBruto, float porcentajeAhorro);

void modulo_calc_salario () {
	char nombres[NUM_EMPLEADOS][50];
	float salariosBrutos[NUM_EMPLEADOS];
	float impuestos[NUM_EMPLEADOS];
	float seguridadSocial[NUM_EMPLEADOS];
	float aportacionInfonavit[NUM_EMPLEADOS];
	float aportacionAfore[NUM_EMPLEADOS];
	float ahorroAnual[NUM_EMPLEADOS];
	float salarioNeto[NUM_EMPLEADOS];
	char fechaPago[NUM_EMPLEADOS][LONGITUD_FECHA]; // Cambiado a cadena
	char lugaresPago[NUM_EMPLEADOS][50];
	int edades[NUM_EMPLEADOS];
	int numEmpleados = 0;
	int opcion;

	do {
		printf("\n\t\tSistema para pagos netos\n\n");
		printf("Seleccione una opcion:\n");
		printf("1. Agregar Empleado\n");
		printf("2. Mostrar Empleados\n");
		printf("3. Salir\n");
		printf("Opcion: ");
		scanf("%d", &opcion);
		while (getchar() != '\n');

		switch(opcion) {
		case 1:
			AgregarEmpleado(nombres, salariosBrutos, impuestos, seguridadSocial,
			                aportacionInfonavit, aportacionAfore, ahorroAnual,
			                salarioNeto, edades, &numEmpleados,
			                fechaPago, lugaresPago);
			break;
		case 2:
			Mostrar(nombres, salariosBrutos, impuestos, seguridadSocial,
			        aportacionInfonavit, aportacionAfore, ahorroAnual,
			        salarioNeto, edades, fechaPago, lugaresPago, numEmpleados);
			break;
		case 3:
			printf("Saliendo...\n\n\n");
			break;
		default:
			printf("Opcion no valida. Intente de nuevo.\n");
		}
	} while (opcion != 3);
}

// Funcion para calcular el ISR basado en el salario bruto
float calcularISR(float salarioBruto) {
	float isr = 0.0;

	if (salarioBruto >= 0.01 && salarioBruto <= 368.10) {
		isr = 0.00 + (salarioBruto - 0.01) * 0.0192;
	} else if (salarioBruto >= 368.11 && salarioBruto <= 3124.35) {
		isr = 7.05 + (salarioBruto - 368.11) * 0.0640;
	} else if (salarioBruto >= 3124.36 && salarioBruto <= 5490.75) {
		isr = 183.45 + (salarioBruto - 3124.36) * 0.1088;
	} else if (salarioBruto >= 5490.76 && salarioBruto <= 6382.80) {
		isr = 441.00 + (salarioBruto - 5490.76) * 0.1600;
	} else if (salarioBruto >= 6382.81 && salarioBruto <= 7641.90) {
		isr = 583.65 + (salarioBruto - 6382.81) * 0.1792;
	} else if (salarioBruto >= 7641.91 && salarioBruto <= 15412.80) {
		isr = 809.25 + (salarioBruto - 7641.91) * 0.2136;
	} else if (salarioBruto >= 15412.81 && salarioBruto <= 24292.65) {
		isr = 2469.15 + (salarioBruto - 15412.81) * 0.2352;
	} else if (salarioBruto >= 24292.66 && salarioBruto <= 46378.50) {
		isr = 4557.75 + (salarioBruto - 24292.66) * 0.3000;
	} else if (salarioBruto >= 46378.51 && salarioBruto <= 61838.10) {
		isr = 11183.40 + (salarioBruto - 46378.51) * 0.3200;
	} else if (salarioBruto >= 61838.11 && salarioBruto <= 185514.30) {
		isr = 16130.55 + (salarioBruto - 61838.11) * 0.3400;
	} else if (salarioBruto >= 185514.31) {
		isr = 58180.35 + (salarioBruto - 185514.31) * 0.3500;
	}

	return isr;
}

// Funcion para calcular la seguridad social
float calcularSeguridadSocial(float salarioBruto) {
	float salarioDiarioBaseCotizacion = salarioBruto * 0.0696;
	int diasTrabajados = 15; // Asumimos quincenal
	float baseCotizacion = salarioDiarioBaseCotizacion * diasTrabajados;
	float seguridadSocial = baseCotizacion * 0.02375;
	return seguridadSocial;
}

// Funcion para calcular la aportacion patronal al INFONAVIT
float calcularInfonavit(float salarioBruto) {
	return salarioBruto * 0.05;
}

// Funcion para calcular la aportacion a la AFORE
float calcularAfore(float salarioBruto, int edad) {
	float minimo, maximo;
	if (edad <= 36) {
		minimo = 6.9;
		maximo = 15.59;
	} else if (edad >= 37 && edad <= 45) {
		minimo = 6.4;
		maximo = 15.0;
	} else if (edad >= 46 && edad <= 59) {
		minimo = 6.0;
		maximo = 13.0;
	} else {
		minimo = 5.5;
		maximo = 10.7;
	}

	printf("Ingrese el porcentaje de su salario quincenal que desea destinar a su AFORE (mC-nimo %.2f%%, mC!ximo %.2f%%): ", minimo, maximo);
	float porcentajeAfore;
	while (scanf("%f", &porcentajeAfore) != 1 || porcentajeAfore < minimo || porcentajeAfore > maximo) {
		printf("Porcentaje no vC!lido. Intente de nuevo (mC-nimo %.2f%%, mC!ximo %.2f%%): ", minimo, maximo);
		while (getchar() != '\n');
	}
	while (getchar() != '\n');

	return salarioBruto * (porcentajeAfore / 100);
}

// Funcion para calcular el ahorro anual
float calcularAhorroAnual(float salarioBruto, float porcentajeAhorro) {
	float ahorroQuincenal = salarioBruto * (porcentajeAhorro / 100);
	return ahorroQuincenal * 24 * 2; // Duplicado porque el empleador aporta lo mismo
}

// Funcion para calcular el salario neto quincenal
float calcularSalarioNeto(float salarioBruto, float isr, float seguridadSocial, float afore, float ahorro) {
	return salarioBruto - (isr + seguridadSocial + afore + ahorro);
}

void AgregarEmpleado(char nombres[NUM_EMPLEADOS][50], float salariosBrutos[NUM_EMPLEADOS],
                     float impuestos[NUM_EMPLEADOS], float seguridadSocial[NUM_EMPLEADOS],
                     float aportacionInfonavit[NUM_EMPLEADOS], float aportacionAfore[NUM_EMPLEADOS],
                     float ahorroAnual[NUM_EMPLEADOS], float salarioNeto[NUM_EMPLEADOS],
                     int edades[NUM_EMPLEADOS], int *numEmpleados,
                     char fechaPago[NUM_EMPLEADOS][LONGITUD_FECHA],
                     char lugaresPago[NUM_EMPLEADOS][50]) {
	if (*numEmpleados >= NUM_EMPLEADOS) {
		printf("No se pueden agregar mas empleados. Capacidad maxima alcanzada.\n");
		return;
	}

	int i = *numEmpleados;

	printf("Primer Apellido del Empleado %d: ", i + 1);
	fgets(nombres[i], sizeof(nombres[i]), stdin);
	nombres[i][strcspn(nombres[i], "\n")] = '\0';

	printf("Ingrese Edad del Empleado: ");
	while (scanf("%d", &edades[i]) != 1 || edades[i] < 18 || edades[i] > 100) {
		printf("Edad no valida. Intente de nuevo: ");
		while (getchar() != '\n'); //Elimina los caracteres no validos del buffer de entrada
	}
	while (getchar() != '\n'); /*Elimina del buffer el valor válido ingresado para que no
        						/existan conflictos con la siguiente entrada.*/

	printf("Ingrese Lugar de Pago: ");
	fgets(lugaresPago[i], sizeof(lugaresPago[i]), stdin);
	lugaresPago[i][strcspn(lugaresPago[i], "\n")] = '\0';

	printf("Ingrese Fecha de Pago (formato dd-Mes-yyyy): ");
	fgets(fechaPago[i], sizeof(fechaPago[i]), stdin);
	fechaPago[i][strcspn(fechaPago[i], "\n")] = '\0';

	printf("Ingrese el salario bruto: ");
	while (scanf("%f", &salariosBrutos[i]) != 1) {
		printf("Caracter no valido para salario bruto. Intente de nuevo: ");
		while (getchar() != '\n');
	}
	while (getchar() != '\n');

	// Calcular el ISR basado en el salario bruto
	impuestos[i] = calcularISR(salariosBrutos[i]);

	// Calcular la Seguridad Social
	seguridadSocial[i] = calcularSeguridadSocial(salariosBrutos[i]);

	// Calcular la Aportacion al INFONAVIT
	aportacionInfonavit[i] = calcularInfonavit(salariosBrutos[i]);

	// Calcular la Aportacion a la AFORE
	aportacionAfore[i] = calcularAfore(salariosBrutos[i], edades[i]);

	// Preguntar si la empresa ofrece una caja de ahorro
	char ofreceCajaAhorro;
	printf("La empresa ofrece una caja de ahorro? (S/N): ");
	scanf(" %c", &ofreceCajaAhorro);
	while (getchar() != '\n');

	if (ofreceCajaAhorro == 'S' || ofreceCajaAhorro == 's') {
		printf("Ingrese el porcentaje de su salario que desea aportar quincenalmente a la caja de ahorro (7%% - 13%%): ");
		float porcentajeAhorro;
		while (scanf("%f", &porcentajeAhorro) != 1 || porcentajeAhorro < 7 || porcentajeAhorro > 13) {
			printf("Porcentaje no valido. Intente de nuevo (7%% - 13%%): ");
			while (getchar() != '\n');
		}
		while (getchar() != '\n');
		ahorroAnual[i] = calcularAhorroAnual(salariosBrutos[i], porcentajeAhorro);
	} else {
		ahorroAnual[i] = 0.0;
	}

	// Preguntar si el trabajador pertenece a un sindicato
	char perteneceSindicato;
	printf("Pertenece a un sindicato? (S/N): ");
	scanf(" %c", &perteneceSindicato);
	while (getchar() != '\n');

	if (perteneceSindicato == 'S' || perteneceSindicato == 's') {
		printf("La cuota sindical minima es %.2f y la maxima es %.2f.\n", salariosBrutos[i] * 0.01, salariosBrutos[i] * 0.03);
	}

	// Calcular el Salario Neto Quincenal
	salarioNeto[i] = calcularSalarioNeto(salariosBrutos[i], impuestos[i], seguridadSocial[i], aportacionAfore[i], ahorroAnual[i] / 24);

	(*numEmpleados)++;
}

void Mostrar(char nombres[NUM_EMPLEADOS][50], float salariosBrutos[NUM_EMPLEADOS],
             float impuestos[NUM_EMPLEADOS], float seguridadSocial[NUM_EMPLEADOS],
             float aportacionInfonavit[NUM_EMPLEADOS], float aportacionAfore[NUM_EMPLEADOS],
             float ahorroAnual[NUM_EMPLEADOS], float salarioNeto[NUM_EMPLEADOS],
             int edades[NUM_EMPLEADOS], char fechaPago[NUM_EMPLEADOS][LONGITUD_FECHA],
             char lugaresPago[NUM_EMPLEADOS][50], int numEmpleados) {
	if (numEmpleados == 0) {
		printf("\nNo hay empleados registrados.\n");
	} else {
		printf("\nLos empleados registrados son:\n");
		for (int i = 0; i < numEmpleados; ++i) {
			printf("Empleado %d: %s\n", i + 1, nombres[i]);
			printf("Edad: %d anios\n", edades[i]);  // Corregir para incluir el codigo ascii del caracter
			printf("Lugar de Pago: %s\n", lugaresPago[i]);
			printf("Fecha de Pago: %s\n", fechaPago[i]);
			printf("Salario Bruto: %.2f\n", salariosBrutos[i]);
			printf("ISR Calculado: %.2f\n", impuestos[i]);
			printf("Seguridad Social: %.2f\n", seguridadSocial[i]);
			printf("Aportacion al INFONAVIT: %.2f\n", aportacionInfonavit[i]);
			printf("Aportacion a la AFORE: %.2f\n", aportacionAfore[i]);
			if (ahorroAnual[i] > 0) {
				printf("Ahorro Anual en Caja de Ahorro (duplicado por la empresa): %.2f\n", ahorroAnual[i]);
			}
			printf("Salario Neto Quincenal: %.2f\n", salarioNeto[i]);  // Mostrar Salario Neto Quincenal
			printf("\n");
		}
	}
}




void main ()
{
	int opcion;
	do {
		
		printf ("Bienvenido, a este programa: \n");
		printf ("Eliga cual modulo desea iniciar: \n");
		printf ("(1) Modulo de Datos de Trabajadores.\n"
		        "(2) Modulo de Cumplimiento de Asistencia.\n"
		        "(3) Modulo de Calculo de Salario.\n"
		        "(4) Salir.\n");
		scanf ("%d", &opcion);

		switch (opcion)
		{
		case 1:
			sys_ges_tra();
			break;
		case 2:
			mod_cump_asis();
			break;

		case 3:
			modulo_calc_salario();
			break;
		
		case 4:
		    exit;
		    break;

		default:
			printf ("Opcion invalida");
			exit(25);
		}
	} while (opcion!=4);

	}