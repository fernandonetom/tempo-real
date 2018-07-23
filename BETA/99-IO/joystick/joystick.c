/**
 * Universidade Federal da Paraíba - UFPB
 * Departamento de Engenharia Elétrica
 * Sistemas em Tempo Real
 * Isaac Maia Pessoa 
 *
 * API para controle de Joystick no Linux
 * Esse exemplo le e mostra os eventos de um joystick usb
 * https://www.kernel.org/doc/Documentation/input/joystick-api.txt
 * 
 * Compilação
 * gcc  -std=c99 -o joystick joystick.c 
 * 
 * Uso:
 * ./joystick
 */
 
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>
 
 
#include <linux/input.h>
#include <linux/joystick.h>
 
int main(int argc, char **argv) {
 
   /*************************************************************************************
    * Estrutura que representa um evento de joystick no linux
    * struct js_event {
    *    __u32 time;     o tempo em que o evento aconteceu
    *    __s16 value;    o eixo ou botao que gerou o evento
    *    __u8 type;      tipo do evento: botao pressionado/ movimento ou estado inicial
    *    __u8 number;    numero do eixo ou botão que gerou o eveto
    *};
    *************************************************************************************/       
    struct js_event ev;
 
 
    //descritor do dispositivo 
    int fd;   
  
   /****************************************
    * Abre o dispositivo para leitura
    * /dev/input/js0 eh o joystick 0
    ****************************************/    
    if ((fd = open("/dev/input/js0", O_RDONLY)) < 0) {
        perror("Erro ao abrir /dev/input/js0");
        return 1;
    }
 
 
    /****************************************
     * Leitura do evento
     ****************************************/
     while (1) {
        /**
         * A funcao read  vai bloquear o processo ate' um evento acontecer
         * Depois do evento, os valores da estrutura 'ev' serao atualizados
         * O numero de bytes retornado por read deve ser sempre igual ao tamanho de js_event
         */
         if (read(fd, &ev, sizeof(struct js_event)) != sizeof(struct js_event)) {
            perror("\nErro ao ler o evento!");
            return 1;
         }
 
         //mostra o estado do joystick
         printf("Evento: tipo %d,  numero %d, valor %d\n", ev.type, ev.number, ev.value);
 
         fflush(stdout); //faz o S.O esvaziar o buffer da saida padrao
 
         //combinacao do botao de baixo
         if (ev.type == 1 && ev.number == 2 && ev.value == 1) exit(0); 
     }
 
     return 0;    
}
