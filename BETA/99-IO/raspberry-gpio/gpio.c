/**
 * Universidade Federal da Paraíba - UFPB
 * Departamento de Engenharia Elétrica
 * Sistemas em Tempo Real
 * Isaac Maia Pessoa 
 *
 * Uso de GPIO através de sysfs no Linux
 * Esse exemplo faz um LED piscar 20x
 * O pino de saída deve ser configurado de acordo com a placa desejada
 *
 * Raspberry PI: GPIO 4
 * Beaglebone Black: GPIO 48
 * 
 * Compilação
 * gcc  -std=c99 -o gpio_demo gpio_demo.c 
 * 
 * Uso:
 * ./gpio_demo -t tempo -p pino
 *      -t indica o tempo (us) entre as escritas (100000us = padrão)
 *      -p indica o pindo que se dejesa usar (4 = padrão)
 */

#define _BSD_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv) {
    FILE *fd;
    unsigned long time = 0;
    char *gpio_pin = 0;
	
   /*********************************************************
    * Analisa os argumentos passados ao programa
    *********************************************************/
    for (int i=1 ; i<argc; i++) {
        if (strcmp(argv[i], "-t") == 0 && ((i+1) < argc) )		    
          time = atol(argv[i+1]);
        if (strcmp(argv[i], "-p") == 0 && ((i+1) < argc) )	
          gpio_pin = argv[i+1];
    }
    time = (time == 0) ? 100000 : time; //100000 = time padrão
    gpio_pin = (gpio_pin == 0) ? "4" : gpio_pin; // 4 = pino padrão na raspberry

	
   /*********************************************************
    * Exporta (habilita) um certo pino para uso	 
    *********************************************************/
    fd = fopen("/sys/class/gpio/export", "w");
    if ( fd == NULL ) {
	 fprintf(stderr, "ERRO: Não é possível abrir o arquivo 'export'\n");
	 exit(EXIT_FAILURE);
    }    
    //escreve o valor "gpio_pin" no arquivo, esse é o número do pino GPIO que se deseja usar
    fprintf(fd, "%s", gpio_pin ); 
    fclose(fd);

    sleep(1); //espera que o gpio seja habilidado pelo S.O
    
   /*********************************************************
    * Configura a direção do pino préviamente exportado
    * A direção pode ser de entrada (in) ou saída (out)
    *********************************************************/
    char *str = malloc(128*sizeof(char));	
    //forma a string com o caminho para o pino desejado
    sprintf(str, "/sys/class/gpio/gpio%s/direction" ,gpio_pin);
    fd = fopen(str, "w");
    if ( fd == NULL ) {
       fprintf(stderr, "%s%s\n","ERRO: Não é possível abrir o arquivo:", str);
       exit(EXIT_FAILURE);
    }
    //escreve o valor "out" no arquivo, essa é a direção desejada (saída)
    fprintf(fd, "%s", "out"); 
    fclose(fd);
    free(str); //libera a str da memoria
	 
    /*********************************************************
     * Escreve no pino de saída
     *********************************************************/
    str = malloc(128*sizeof(char));
    sprintf(str,"/sys/class/gpio/gpio%s/value", gpio_pin ); 
    fd = fopen(str, "w");
    if ( fd == NULL ) {
       fprintf(stderr, "%s%s\n","ERRO: Não é possível abrir o arquivo:", str);       
       exit(EXIT_FAILURE);
    }
    free(str); //libera a str da memoria
    
    //Realiza 20 escritas (pisca um led)
    for (int i=0; i < 20 ; i++)	{
		fprintf(fd, "%s", "1");
		fflush(fd); //esvazia o buffer do S.O, forçando a escrita
		usleep(time);
		fprintf(fd, "%s", "0");
		fflush(fd);
		usleep(time); //faz o processo dormir
    }
    fclose(fd);
		
    /*********************************************************
     * Desabilita o pino usado
     *********************************************************/
    fd = fopen("/sys/class/gpio/unexport", "w");
    if ( fd == NULL ) {
       fprintf(stderr, "ERRO: Não é possível abrir o arquivo 'unexport'\n");
       exit(EXIT_FAILURE);
    }    
    //escreve o valor de 'gpio_pin' no arquivo, esse é o número do pino GPIO que se quer desativar
    fprintf(fd, "%s", gpio_pin); 
    fclose(fd);	
	
    return 0;
}
