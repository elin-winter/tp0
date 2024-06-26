#include "client.h"
#include <readline/readline.h>

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();
	
	if (logger == NULL) {
		// No se creó el log, terminar el programa
		printf ("No se ha podido crear el logger. \n");
		log_error(logger, "Could not create logger");
		exit(1);
	}

	log_info(logger, "Hola! Soy un Log. \n");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	if (config == NULL) {
		// No se creó la config, terminar el programa
		printf ("No se ha podido crear el config. \n");
		log_error(logger, "Could not create config");
		exit(2);
	}

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");

	// Loggeamos el valor de config
	log_info (logger, "Clave: %s, IP: %s, Puerto: %s \n", valor, ip, puerto);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("tp0.log", "LOG", true, LOG_LEVEL_INFO);

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("cliente.config");

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido = readline("> ");
	
	// Se leen y loguean todas las líneas ingresadas a consola hasta llegar a una vacía, allí termina el programa
	
	while (strcmp(leido, "") != 0) {
		log_info("%s", logger, leido);
		leido = readline("> ");
	}

	free(leido);
}

void paquete(int conexion)
{
	char* leido = readline("> ");
	t_paquete* paquete = crear_paquete();

	// Paso los comandos de consola al paquete
	while (strcmp(leido, "") != 0) {
		agregar_a_paquete(paquete, leido, strlen(leido)+1);
		leido = readline("> ");
	}

	free(leido);

	// Envio del paquete
	enviar_paquete(paquete, conexion);
	eliminar_paquete (paquete);
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	log_destroy(logger);
	config_remove_key(config, "CLAVE");
	config_destroy(config);
	liberar_conexion(conexion);
}
