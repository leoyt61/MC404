#include "montador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


/* Retorna:
 *  1 se houver erro lexico
 *  1000 se for instrucao 1
 *  2000 se for instrucao 2
 *  1001 se for diretiva .set
 *  2002 se for diretiva .org
 *  3003 se for diretiva .align
 *  4004 se for diretiva .wfill
 *  5005 se for diretiva .word
 *  1002 se for rotulo
 *  1003 se for hexadecimal
 *  1004 se for decimal
 *  1005 se for nome
 */
int retorna_tipo(char *token){
    char *instrucao1[] = {"ld", "ldinv", "ldabs", "ldmqmx", "store", "jump", "jge", "add", "addabs", "sub", "subabs", "mult", "div", "storend"};
    char *instrucao2[] = {"ldmq", "rsh", "lsh"};
    char *diretiva[] = {".set", ".org", ".align", ".wfill", ".word"};
    int rot = 0;
    int hex = 0;
    int dec = 0;
    int nome = 0;


    /*
     *  Instrucao 1: instrucoes que acompanham 1 parametro
     */
    for (int i = 0; i < 14; i++){
        if (strcmp(token, instrucao1[i]) == 0){
            return 1000;
        }
    }

    /*
     *  Instrucao 2 : instrucoes que nao acompanha parametros
     */
    for (int i = 0; i < 3; i++){
        if (strcmp(token, instrucao2[i]) == 0){
            return 2000;
        }
    }

    /*
     *  Diretivas .set, .org, .align, .wfill ou .word
     */
    for (int i = 0; i < 5; i++){
        if (strcmp(token, diretiva[i]) == 0){
            return ((i+1)*1001);
        }
    }

    /*
     *  Rotulo: primeiro caractere deve ser alfabetico ou '_',
     *          o ultimo caractere deve ser ':' e o resto deve
     *          ser alfanumerico ou '_'
     */
    if ((token[0] == '_' || isalpha(token[0])) && token[strlen(token)-1] == ':' ){
        rot++;
        for (int i = 1; i < strlen(token)-1; i++){
            if (isalpha(token[i]) || isdigit(token[i]) || token[i] == '_'){
                rot++;
            }
            else{
                rot = -1;
                break;
            }
        }
    }

    /*
     *  Hexadecimal: primeiro e segundo caractere sao '0' e 'x' 
     *               respectivamente, o resto eh composto apenas
     *               por caracteres numericos
     */
    if (token[0] == '0' && token[1] == 'x'){
        hex++;
        for (int i = 2; i < strlen(token); i++){
            if (isxdigit(token[i])){
                hex++;
            }
            else{
                hex = -1;
                break;
            }
        }
    }

    /*
     *  Decimal: apenas caracteres numericos
     */
    for (int i = 0; i < strlen(token); i++){
        if (isdigit(token[i])){
            dec++;
        }
        else{
            dec = -1;
            break;
        }
    }

    /* 
     *  Nome: comeca com caractere alfabetico ou '_' e se n for uma instrucao
     */
    if (isalpha(token[0]) || token[0] == '_'){
        nome++;
        for (int i = 1; i < strlen(token); i++){
            if (isalpha(token[i]) || isdigit(token[i]) || token[i] == '_'){
                nome++;
            }
            else{
                nome = -1;
                break;
            }
        }
    }
    if (rot > 0)
        return 1002;
    if (hex > 0)
        return 1003;
    if (dec > 0)
        return 1004;
    if (nome > 0)
        return 1005;
    return 1;
}



/*
 * Retorna o respectivo codigo do token (1000 para qualquer instrucao, 1001 para qualquer diretiva etc)
 */
int tipo(int cod){
    if (cod%1000 == 0)
        return 1000;
    else if (cod%1001 == 0)
        return 1001;
    else if (cod%1002 == 0)
        return 1002;
    else if (cod==1003)
        return 1003;
    else if (cod==1004)
        return 1004;
    else if (cod == 1005)
        return 1005;
    return 0;
}





/*
 *  Funcao para eliminar caracteres da string
 */
void str_cut(char *str, int begin, int tam){
    int l = strlen(str);
    if (tam < 0) 
        tam = l - begin;
    if (begin + tam > l) 
        tam = l - begin;
    memmove(str + begin, str + begin + tam, l - tam + 1);
}





int processarEntrada(char* entrada, unsigned tamanho)
{
    /*
     *  Bloco para eliminar comentarios
     */ 
    int end = 0;
    int begin = 0;
    while (entrada[end]){
        begin = end;
        if (entrada[end] == '#'){
            while (entrada[end] != '\0' && entrada[end] != '\n')
                end++;
        }
        if (end > begin){
            str_cut(entrada, begin, end-begin);
            end = begin;
        }
        end++;
    }


    /*
     * Para saber em que linha comeca a ter token, pois
     * as primeiras linhas podem ter apenas quebra de linha
     */
    int linha = 1;
    end = 0;
    while (entrada[end] == '\n'){
        linha++;
        end++;
    }




    int types[7] = {0, 0, 0, 0, 0, 0, 0}; // Uma linha deve ter no maximo 4 tokens, sendo no maximo 2 decimais
    int j = 0;
    int cont = 5;
    while (entrada[end]){
        begin = end;
        while (entrada[end] != ' ' && entrada[end] != '\0' && entrada[end] != '\n'){
            entrada[end] = tolower(entrada[end]); // Deixando os caracteres minusculos
            end++;
        }
        if (end > begin){
            char *palavra = malloc((end-begin)*sizeof(char));
            strncpy(palavra, entrada+begin, end-begin);
            types[j] = retorna_tipo(palavra); // Salvando o tipo do token da linha em sua respectiva posicao
            if (types[j] == 1){
                printf("ERRO LEXICO: palavra inválida na linha %d!\n", linha);
                return 1;
            }

            /*
             * Caso o token seja um numero decimal, ele eh em inteiro
             * e salvo em uma das duas ultimas posicoes do vetor
             * Primeiramente na posicao 5, pois caso haja outro token
             * decimal, este seja salvo na ultima posicao
             */
            if (types[j] == 1004){
                types[cont] = atoi(palavra);
                cont++;
            }
            adicionarToken(tipo(types[j]), palavra, linha);
            j++;
        }
        if (entrada[end] == '\n'){
            
            /*
             * Caso haja algum token nessa linha:
             */
            if (types[0] != 0){
                int k = 0;

                /*
                 * Caso o primeiro token seja um rotulo
                 * comparamos os proximos tokens em relacao
                 * ao primeiro (Ex: o segundo token eh o primeiro depois do rotulo etc)
                 */
                if (types[0] == 1002)
                    k = 1;

                /*
                 * Caso o primeiro token seja uma instrucao 1
                 */
                if (types[k] == 1000){

                    /*
                     * Caso essa instrucao receba mais de 1 parametro, existe um erro gramatical
                     */
                    if (types[k+2] != 0)
                        goto erroGramatical;

                    /*
                     * Caso o parametro dessa instrucao nao seja um hexadecimal, um nome nem um decimal entre 0 e 1023, tambem existe um erro gramatical
                     */
                    if (!((types[k+1] == 1004 && types[5] >= 0 && types[5] <= 1023) || types[k+1] == 1003 || types[k+1] == 1005))
                        goto erroGramatical;
                }

                /*
                 * Caso o primeiro token seja uma instrucao 2
                 */
                else if (types[k] == 2000){

                    /*
                     * Caso a instrucao nao receba nenhum parametro, tudo certo
                     */
                    if (types[k+1] != 0)
                        goto erroGramatical;
                }

                /*
                 * Caso o primeiro token seja uma diretiva .set
                 */
                else if (types[k] == 1001){

                    /*
                     * Caso a diretiva receba mais de 2 parametros, existe um erro gramatical
                     */
                    if (types[k+3] != 0)
                        goto erroGramatical;

                    /*
                     * Caso o primeiro parametro nao seja um nome ou o segundo nao seja nem um hexadecimal nem decimal entre 0 e 2^31 - 1, 
                     * tambem existe um erro gramatical
                     */
                    if (!(types[k+1] == 1005 && (types[k+2] == 1003 || (types[k+2] == 1004 && types[5] >= 0 && types[5] <= pow(2,31)-1))))
                        goto erroGramatical;
                }

                /*
                 * Caso o primeiro token seja uma diretiva .org
                 */
                else if (types[k] == 2002){

                    /*
                     * Caso a diretiva receba mais de 1 parametro, existe um erro gramatical
                     */
                    if (types[k+2] != 0)
                        goto erroGramatical;

                    /*
                     * Caso o parametro seja nao um hexadecimal nem decimal entre 0 e 1023, tambem existe um erro gramatical
                     */
                    if (!(types[k+1] == 1003 || (types[k+1] == 1004 && types[5] >= 0 && types[k+1] <= 1023)))
                        goto erroGramatical;
                }

                /*
                 * Caso o primeiro token seja uma diretiva .align
                 */
                else if (types[k] == 3003){

                    /*
                     * Caso a diretiva receba mais de 1 parametro, existe um erro gramatical
                     */
                    if (types[k+2] != 0)
                        goto erroGramatical;

                    /*
                     * Caso o parametro nao seja um decimal entre 1 e 1023, tambem existe um erro gramatical
                     */
                    if (!(types[k+1] == 1004 && types[5] >= 1 && types[5] <= 1023))
                        goto erroGramatical;
                }

                /*
                 * Caso o primeiro token seja uma diretiva .wfill
                 */
                else if (types[k] == 4004){

                    /*
                     * Caso a diretiva receba mais de 2 parametros, existe um erro gramatical
                     */
                    if (types[k+3] != 0)
                        goto erroGramatical;

                    /*
                     * Caso o primeiro parametro nao seja um decimal entre 1 e 1023 ou o segundo parametro
                     *  nao seja um nome, hexadecimal nem um decimal entre -2^31 e 2^31 - 1, tambem existe um erro gramatical
                     */
                    if (!(types[k+1] == 1004 && types[5] >= 1 && types[5] <= 1023 && (types[k+2] == 1005 || types[k+2] == 1003 || (types[k+2] == 1004 && types[6] >= -pow(2,31) && types[6] <= pow(2,31)-1))))
                        goto erroGramatical;
                }

                /*
                 * Caso o primeiro token seja uma diretiva .word
                 */
                else if (types[k] == 5005){

                    /*
                     * Caso a diretiva receba mais de 1 parametro, existe um erro gramatical
                     */
                    if (types[k+2] != 0)
                        goto erroGramatical;

                    /*
                     * Caso o parametro nao seja um nome, hexadecimal nem um decimal entre -2^31 e 2^31 - 1, tambem existe um erro gramatical
                     */
                    if (!((types[k+1] == 1004 && types[5] >= -pow(2,31) && types[5] <= pow(2,31)-1) || types[k+1] == 1003 || types[k+1] == 1005))
                        goto erroGramatical;
                }
                
                /*
                 * Zera o vetor que salva os tipos para poder fazer as comparacoes da proxima linha
                 */
                for (int k = 0; k < 7; k++)
                    types[k] = 0;
            }
            linha++;
            j = 0;
            cont = 5;
        }
        end++;
    }

    return 0; // Se tudo der certo, retorna 0


    erroGramatical:
    printf("ERRO GRAMATICAL: palavra na linha %d!\n", linha);
    return 1;
}
