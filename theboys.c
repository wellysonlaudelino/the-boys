// Programa principal do projeto "The Boys - 2024/2"
// Autor: Wellyson Matheus de Souza Laudelino, GRR 20231954

#include "theboys.h"
#include "entidades.h"
#include "eventos.h"
#include "lef.h"
#include "mundo.h"

int main () {

    // Definindo a seed 
    srand(time(NULL));

    // iniciando vars
    mundo mundo_virtual;
    lef* eventos_mundo;
    int tentativas = 0;
    int concluidas = 0;
    int mortalidade = 0;
    int eventos_tratados = 0;

    // Inicializar LEF
    if ((eventos_mundo = cria_lef()) == NULL) {
        printf("\033[0;31mERRO! [EVENTOS] Simulação abortada na inicialização :c\033[0;37m\n");
        return 1;
    }

    // Inicializa MUNDO
    if (inicializar_realidade_virtual(&mundo_virtual, eventos_mundo)) {
        printf("\033[0;31mERRO! [MUNDO] Simulação abortada na inicialização :c\033[0;37m\n");
        finalizar_realidade_virtual(&mundo_virtual, eventos_mundo);
        return 1;
    }

    // Clock do MUNDO
    // Vars de CLOCK
    int aux;
    double temp;
    evento* evento_atual;

    // Eventos do LEF
    while (!vazia_lef(eventos_mundo)) {

        if ((evento_atual = retirar_lef (eventos_mundo)) == NULL)
            return 1;
            
        eventos_tratados++;

        switch (evento_atual->tipo) {
        
        // CHEGA
        case 0:
            if(mundo_virtual.herois[evento_atual->dado1].morto) break;

            if (evento_chega (  evento_atual->tempo,
                                &mundo_virtual.herois[evento_atual->dado1],
                                mundo_virtual.bases[evento_atual->dado2],
                                eventos_mundo))
                printf ("%6d: CHEGA\t HEROI %2d BASE %2d (%2d/%2d) ESPERA\n",
                        evento_atual->tempo,
                        evento_atual->dado1, 
                        evento_atual->dado2,
                        mundo_virtual.bases[evento_atual->dado2].num_presentes,
                        mundo_virtual.bases[evento_atual->dado2].lotacao
                        );
            else
                printf ("%6d: CHEGA\t HEROI %2d BASE %2d (%2d/%2d) DESISTE\n",
                    evento_atual->tempo,
                    evento_atual->dado1, 
                    evento_atual->dado2,
                    mundo_virtual.bases[evento_atual->dado2].num_presentes,
                    mundo_virtual.bases[evento_atual->dado2].lotacao);
            break;

        // ESPERA
        case 1:
            if(mundo_virtual.herois[evento_atual->dado1].morto) break;

            printf ("%6d: ESPERA\t HEROI %2d BASE %2d (%2d)\n",
                        evento_atual->tempo,
                        evento_atual->dado1, 
                        evento_atual->dado2,
                        mundo_virtual.bases[evento_atual->dado2].espera->tamanho);
            evento_espera ( evento_atual->tempo,
                            mundo_virtual.herois[evento_atual->dado1],
                            &mundo_virtual.bases[evento_atual->dado2], 
                            eventos_mundo);
            break;

        // DESISTE
        case 2:
            if(mundo_virtual.herois[evento_atual->dado1].morto) break;

            printf ("%6d: DESISTE\t HEROI %2d BASE %2d\n",
                        evento_atual->tempo,
                        evento_atual->dado1, 
                        evento_atual->dado2);
            evento_desiste (evento_atual->tempo,
                            mundo_virtual.herois[evento_atual->dado1],
                            mundo_virtual.bases[evento_atual->dado2],
                            eventos_mundo,
                            mundo_virtual);
            break;

        // AVISA
        case 3:
            printf ("%6d: AVISA\t PORTEIRO BASE %2d (%2d/%2d) FILA ",
                        evento_atual->tempo,
                        evento_atual->dado1, 
                        mundo_virtual.bases[evento_atual->dado1].num_presentes,
                        mundo_virtual.bases[evento_atual->dado1].lotacao);
            imprimir_fila (mundo_virtual.bases[evento_atual->dado1].espera);
            printf("\n");
            evento_avisa (evento_atual->tempo, &mundo_virtual.bases[evento_atual->dado1], eventos_mundo, &mundo_virtual);
            break;

        // ENTRA
        case 4:
            if(mundo_virtual.herois[evento_atual->dado1].morto) break;

            aux = evento_entra (evento_atual->tempo,
                                mundo_virtual.herois[evento_atual->dado1],
                                mundo_virtual.bases[evento_atual->dado2],
                                eventos_mundo);
            printf ("%6d: ENTRA\t HEROI %2d BASE %2d (%2d/%2d) SAI %d\n",
                        evento_atual->tempo,
                        evento_atual->dado1,
                        evento_atual->dado2, 
                        mundo_virtual.bases[evento_atual->dado2].num_presentes,
                        mundo_virtual.bases[evento_atual->dado2].lotacao,
                        aux);
            break;

        // SAI
        case 5:
            if(mundo_virtual.herois[evento_atual->dado1].morto) break;

            evento_sai (evento_atual->tempo,
                        mundo_virtual.herois[evento_atual->dado1],
                        &mundo_virtual.bases[evento_atual->dado2],
                        eventos_mundo,
                        mundo_virtual);
            printf ("%6d: SAI\t HEROI %2d BASE %2d (%2d/%2d)\n",
                        evento_atual->tempo,
                        evento_atual->dado1,
                        evento_atual->dado2, 
                        mundo_virtual.bases[evento_atual->dado2].num_presentes,
                        mundo_virtual.bases[evento_atual->dado2].lotacao);
            break;

        // VIAJA
        case 6:
            if(mundo_virtual.herois[evento_atual->dado1].morto) break;

            temp = (evento_viaja(evento_atual->tempo,
                                    &mundo_virtual.herois[evento_atual->dado1],
                                    &mundo_virtual.bases[evento_atual->dado2],
                                    eventos_mundo,
                                    mundo_virtual));
            aux = ceil (temp);
            printf ("%6d: VIAJA\t HEROI %2d BASE %2d BASE %2d DIST %d VEL %d CHEGA %d\n",
                        evento_atual->tempo,
                        evento_atual->dado1,
                        mundo_virtual.herois[evento_atual->dado1].base_atual,
                        evento_atual->dado2,
                        aux,
                        mundo_virtual.herois[evento_atual->dado1].velocidade,
                        (((aux/mundo_virtual.herois[evento_atual->dado1].velocidade))+evento_atual->tempo+1));
            break;

        // MISSAO
        case 7:
            tentativas++;
            concluidas += evento_missao(evento_atual->tempo, 
                                        mundo_virtual.missoes[evento_atual->dado1], 
                                        eventos_mundo, 
                                        mundo_virtual,
                                        &mortalidade);
            break;

        // FINAL
        case 8:
            evento_fim (evento_atual->tempo, mundo_virtual, tentativas, concluidas, mortalidade, eventos_tratados);
            free (evento_atual);
            finalizar_realidade_virtual(&mundo_virtual, eventos_mundo);
            return 0;
            break;

        // MORTE
        case 9:
            evento_morte (evento_atual->tempo, 
                          mundo_virtual.missoes[evento_atual->dado2], 
                          &mundo_virtual.herois[evento_atual->dado1], 
                          &mundo_virtual.bases[mundo_virtual.herois[evento_atual->dado1].base_atual], 
                          eventos_mundo);
            break;
        
        default:
            break;
        }
        free (evento_atual);
    }
    return 0;
}
