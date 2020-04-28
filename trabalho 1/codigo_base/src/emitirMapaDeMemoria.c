#define _POSIX_C_SOURCE 200809L
#define MAX 2048

#include "montador.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

/*
 * Funcao para um inteiro entre 0 e 15 em hexadecimal
 */
char istr(int n){
    if (n > 9 && n < 16){
        char c = 55 +n;
        return c;
    }
    else{
        char c = n+48;
        return c;
    }
}

/*
 * Funcao para transformar qualquer decimal para hexadecimal
 */
char* dectohex(int n){
    char* h = malloc(3*sizeof(char));
    int v3 = n%16;
    n = n/16;
    int v2 = n%16;
    n = n/16;
    int v1 = n%16;
    h[0] = istr(v1);
    h[1] = istr(v2);
    h[2] = istr(v3);
    return h;
}

/*
 * Funcao para achar um valor decimal para uma string
 */
int strtodec(char *string){
    int n = 0;
    int pos = 0;
    for (int i = (strlen(string)-1)*8; i >= 0; i-=8){
        n+=string[pos]*pow(2,i);
        pos++;
    }
    return n;
}

/*
 * Hashing para instrucoes e rotulos/nomes
 */
int hash(char *string){
    int m = strtodec(string);
    double i =  m*(sqrt(5) - 1)/2;
    int n = i;
    i = i - n;
    i = 256*strlen(string)*i;
    n = abs(i);
    return n;
}

/*
 * Funcao para transformar hexadecimal em decimal
 */
int hextodec(char *h){
    int soma = 0;
    for (int i = 0; i < strlen(h); i++){
        char c = h[i];
        int n;

        if (c >= 65 && c <= 70)
            n = c - 55;
        else
            n = c - 48;
        soma+=n*pow(16, strlen(h)-1-i);
    }
    return soma;
}

/*
 * Funcao para corrigir um hexadecimal (deixar na forma XXX)
 */
char * hexcorrect(char *h){
    char* hex = malloc(3*sizeof(char));
    int j = 0;
    for (int i = 0; i < 3; i++){
        if (i < 3 - strlen(h))
            hex[i] = 48;
        else{
            hex[i] = h[j];
            j++;
        }
    }
    return hex;
}




int emitirMapaDeMemoria()
{
    /*
     * Criando um File para colocar os fprintf
     */
    char* buffer = NULL;
    size_t bufferSize = 0;
    FILE* mystream = open_memstream(&buffer, &bufferSize);

    /*
     * Atribuindo valores as instrucoes
     */
    char instrucoes[MAX][2];
    strcpy(instrucoes[hash("ld")], "01");
    strcpy(instrucoes[hash("ldinv")], "02");
    strcpy(instrucoes[hash("ldabs")], "03");
    strcpy(instrucoes[hash("ldmq")], "0A");
    strcpy(instrucoes[hash("ldmqmx")], "09");
    strcpy(instrucoes[hash("store")], "21");
    strcpy(instrucoes[hash("jump")], "0D");
    strcpy(instrucoes[hash("jge")], "0F");
    strcpy(instrucoes[hash("add")], "05");
    strcpy(instrucoes[hash("addabs")], "07");
    strcpy(instrucoes[hash("sub")], "06");
    strcpy(instrucoes[hash("subabs")], "08");
    strcpy(instrucoes[hash("mult")], "0B");
    strcpy(instrucoes[hash("div")], "0C");
    strcpy(instrucoes[hash("lsh")], "14");
    strcpy(instrucoes[hash("rsh")], "15");
    strcpy(instrucoes[hash("storend")], "12");
    
    char nomes[MAX][3];
    int pos = 0; //0 para esquerda e 1 para direita
    int n = getNumberOfTokens();
    int j = 0; //linha (endereco)

    /*
     * Percorrendo os tokens
     */
    for (int i = 0; i < n; i++){
        Token* t;
        t = recuperaToken(i);

        /*
         *  Caso seja uma Diretiva
         */
        if (t->tipo == 1001){

            /*
             * Atribuindo ao primeiro parametro o valor do
             * segundo parametro ja convertido para hexadecimal
             */
            if (strcmp(t->palavra, ".set") == 0){
                char hex[MAX];
                if (recuperaToken(i+2)->tipo == 1004)
                    strcpy(hex, dectohex(atoi(recuperaToken(i+2)->palavra)));
                else if (recuperaToken(i+2)->tipo == 1003)
                    strncpy(hex, recuperaToken(i+2)->palavra + 2, 3);
                strcpy(nomes[hash(recuperaToken(i+1)->palavra)], hexcorrect(hex));
                i+=2;
            }

            /*
             * Alterando o valor decimal da linha de acordo com o 
             * valor decimal do parametro
             */
            else if (strcmp(t->palavra, ".org") == 0){
                char hex[MAX];
                if (recuperaToken(i+1)->tipo == 1004)
                    strcpy(hex, dectohex(atoi(recuperaToken(i+1)->palavra)));
                else if (recuperaToken(i+1)->tipo == 1003)
                    strncpy(hex, recuperaToken(i+1)->palavra + 2, 3);
                j = hextodec(hexcorrect(hex));
                i++;
            }

            /*
             *  Alterando o valor decimal da linha de acordo com o parametro
             */
            else if (strcmp(t->palavra, ".align") == 0){
                if (pos%2 == 1)
                    j++;
                while (j%atoi(recuperaToken(i+1)->palavra) != 0)
                    j++;
                pos = 0;
                i++;
            }

            /*
             * Imprime em 'mystream' a impressao das N palavras de memoria
             * de acordo com os 2 parametros ja convertendo para hexadecimal
             */
            else if (strcmp(t->palavra, ".wfill") == 0){
                char hex[MAX];
                if (recuperaToken(i+2)->tipo == 1004)
                    strcpy(hex, dectohex(atoi(recuperaToken(i+2)->palavra)));
                else if (recuperaToken(i+2)->tipo == 1003)
                    strncpy(hex, recuperaToken(i+2)->palavra + 2, strlen(recuperaToken(i+2)->palavra) - 2);
                else if (recuperaToken(i+2)->tipo == 1005)
                    strcpy(hex, nomes[hash(recuperaToken(i+2)->palavra)]);
                for (int l = 0; l < strlen(hex); l++)
                    hex[l] = toupper(hex[l]);
                for (int k = 0; k < atoi(recuperaToken(i+1)->palavra); k++){
                    fprintf(mystream, "%s 00 000 00 %s\n", dectohex(j), hex);
                    j++;
                }
                i+=2;
            }

            /*
             *  Imprime em 'mystream' a palavra de memoria
             *  de acordo com o parametro, ja convertendo para hexadecimal
             */
            else if (strcmp(t->palavra, ".word") == 0){
                char hex[MAX];
                if (recuperaToken(i+1)->tipo == 1003)
                    strncpy(hex, recuperaToken(i+1)->palavra + 2, strlen(recuperaToken(i+1)->palavra));
                else if (recuperaToken(i+1)->tipo == 1004)
                    strcpy(hex, dectohex(atoi(recuperaToken(i+1)->palavra)));
                else if (recuperaToken(i+1)->tipo == 1005)
                    strcpy(hex, nomes[hash(recuperaToken(i+1)->palavra)]);
                strcpy(nomes[hash(dectohex(j))], hexcorrect(hex));
                fprintf(mystream, "%s 00 000 00 %s\n", dectohex(j), hexcorrect(hex));
                pos = 0;
                j++;
                i++;
            }
        }

        /*
         * Caso o token seja Instrucao ou Rotulo
         */
        else{

            /*
            *  Instrucao
            */
            if (t->tipo == 1000){

                /*
                 *  Caso a posicao atual seja a esquerda, imprime o endereco atual em 'mystream'
                 */
                if (pos%2 == 0)
                    fprintf(mystream, "%s ", dectohex(j));

                /*
                 *  Caso a instrucao nao tenha parametros, imprime o valor
                 *  da instrucao seguido de '000' em 'mystream'
                 */
                if (strcmp(t->palavra, "ldmq") == 0 || strcmp(t->palavra, "lsh") == 0 || strcmp(t->palavra, "rsh") == 0){
                    char hex[MAX];
                    strcpy(hex, instrucoes[hash(t->palavra)]);
                    fprintf(mystream, "%s 000", hex);
                }


                else {
                    char hex[MAX];
                    char inst[MAX];
                    strcpy(inst, instrucoes[hash(t->palavra)]); // Salva em 'inst' o valor hexadecimal da instrucao
                    if (recuperaToken(i+1)->tipo == 1003) // Se o parametro for um hexadecimal, apenas salva em 'hex' seu valor
                    strncpy(hex, recuperaToken(i+1)->palavra + 2, strlen(recuperaToken(i+1)->palavra));
                    else if (recuperaToken(i+1)->tipo == 1004) // Se o parametro for um decimal, apenas salva em 'hex' seu valor em hexadecimal
                        strcpy(hex, dectohex(atoi(recuperaToken(i+1)->palavra)));
                    else if (recuperaToken(i+1)->tipo == 1005){ // Se o parametro for um nome, verifica se ha algum valor atribuido a ele

                        /*
                         *  Caso nao tenha nenhum valor atribuido ao nome, 'mystream' eh liberado e imprime o erro apenas
                         */
                        if (strlen(nomes[hash(recuperaToken(i+1)->palavra)]) == 0 && strlen(nomes[hash(recuperaToken(i+1)->palavra)+1]) == 0){
                            fclose(mystream); 
                            free(buffer);
                            fprintf(stderr, "ERRO: Rótulo ou símbolo usado mas não definido: %s\n", recuperaToken(i+1)->palavra);
                            return 1;
                        }

                        /*
                         *  Caso contrario, o valor do nome esta na posicao n ou n+1 de nomes[]
                         */
                        else {

                            /*
                             *  Caso o valor nao esteja na posicao n
                             */
                            if (strlen(nomes[hash(recuperaToken(i+1)->palavra)]) == 0){
                                strcpy(hex, nomes[hash(recuperaToken(i+1)->palavra)+1]); // Eh salvo em 'hex' o valor em n+1

                                /*
                                 *  Caso a instrucao seja 'jump', 'jge' e 'storend', como o valor esta em n+1,
                                 *  significa que o rotulo esta atribuido a uma palavra a direita da memoria,
                                 *  logo o valor da instrucao eh o valor original + 1
                                 */
                                if (strcmp(t->palavra, "jump") == 0 || strcmp(t->palavra, "jge") == 0 || strcmp(t->palavra, "storend") == 0){
                                    int i =hextodec(instrucoes[hash(t->palavra)])+1;
                                    strncpy(inst, dectohex(i)+1, 2);
                                }
                            } 


                            else {
                                strcpy(hex, nomes[hash(recuperaToken(i+1)->palavra)]);
                            }
                        }
                    }


                    /*
                     *  Imprime em 'mystream' o valor da instrucao seguido do valor do parametro
                     */
                    fprintf(mystream, "%s %s", inst, hexcorrect(hex));
                }

                /*
                 *  Caso a posicao atual seja direita, ha uma quebra de linha, caso contrario imprime em 'mystream' um espaco ' '
                 */
                if (pos%2 == 1){
                    fprintf(mystream, "\n");
                    j++;
                }
                else
                    fprintf(mystream, " ");
                pos++;
                    
            }

            /*
             *  Caso o token seja um rotulo, eh atribuido o endereco atual ao nome do rotulo tirando o ':',
             *  salvando-o na posicao 'n' caso a posicao atual seja a esquerda ou 'n+1' caso a posicao
             *  atual seja a direita
             */
            else if (t->tipo == 1002){
                char *rot = malloc(MAX*sizeof(char));
                strncpy(rot, recuperaToken(i)->palavra, strlen(recuperaToken(i)->palavra)-1);
                strcpy(nomes[hash(rot) + pos%2], dectohex(j));
            }
        }
        
    }

    /*
     * Preenche com '00 000' caso a ultima instrucao a ser executada
     * seja a esquerda e imprime em 'mystream'
     */
    if (pos%2 == 1){
        fprintf(mystream, "00 000\n");
    }


    /*
     * Caso tudo de certo, imprime o que foi salvo em mystream e retorna 0
     */
    fclose(mystream);
    printf("%s", buffer);
    return 0;
}
