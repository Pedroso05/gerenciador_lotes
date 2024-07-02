#include <stdio.h>
#include<string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_PESSOAS 10
struct dados {
    char nome_arq[20];
    int dia,mes,ano;
    double dado;
};

int validar_data(char *data) {
    for (int i = 0; i < strlen(data); i++) {
        if (!isdigit(data[i]) && data[i] != '/') {
            return 0;
        }
    }
    return 1;
}

int validar_litragem(char *litragem) {
    int pontos = 0;
    for (int i = 0; i < strlen(litragem); i++) {
        if (!isdigit(litragem[i])) {
            if (litragem[i] == '.' && pontos == 0) {
                pontos++;
            } else {
                return 0;
            }
        }
    }
    return 1;
}


void abri_txt(char nome[], struct dados arq[], int *quantidade) {
    FILE *file = fopen(nome, "r");

    if (file) {
        char linha[100];
        while (*quantidade < MAX_PESSOAS && fgets(linha, sizeof(linha), file)) {
            int dia, mes, ano;
            double dado;
            char data[11];
            char litragem[20];

            int ret = sscanf(linha, "%10s %19s", data, litragem);

            if (ret == 2 && validar_data(data) && validar_litragem(litragem)) {
                sscanf(data, "%d/%d/%d", &dia, &mes, &ano);
                dado = atof(litragem);
                arq[*quantidade].dia = dia;
                arq[*quantidade].mes = mes;
                arq[*quantidade].ano = ano;
                arq[*quantidade].dado = dado;
                strcpy(arq[*quantidade].nome_arq, nome);
                (*quantidade)++;
            } else {
                printf("Erro ao ler linha: %s. Dados inválidos no arquivo %s\n", linha, nome);
            }
        }
        fclose(file);
    } else {
        printf("Erro ao abrir arquivo %s\n", nome);
    }
}


void escreve_bin(struct dados arq[], int quantidade) {
    FILE *file_bin = fopen("binario.dat", "ab");

    if (file_bin) {
        fwrite(arq, sizeof(struct dados), quantidade, file_bin);
        fclose(file_bin);
    } else {
        printf("Erro ao abrir arquivo binário para escrita\n");
    }
}


void ler_bin(){
    struct dados arq[MAX_PESSOAS];
    FILE *file_bin = fopen("binario.dat", "rb");

    if(file_bin){

        int quantidade = 0;
        while (fread(&arq[quantidade], sizeof(struct dados), 1, file_bin)) {
            printf("%02d/%02d/%04d %.2lf\n", arq[quantidade].dia, arq[quantidade].mes, arq[quantidade].ano, arq[quantidade].dado);
            quantidade++;
        }
        fclose(file_bin);

    }else {
        printf("Erro ao abrir arquivo para leitura\n");
    }
}

void escreve_csv() {
    struct dados arq[MAX_PESSOAS];
    FILE *arq_bin = fopen("binario.dat", "rb");

    if (arq_bin) {
        FILE *file_csv = fopen("listagem.csv", "w");
        if (file_csv) {
            fputs("data, litragem, arquivo\r\n", file_csv);

            int quantidade = 0;
            while (fread(&arq[quantidade], sizeof(struct dados), 1, arq_bin)) {
                fprintf(file_csv, "%02d/%02d/%04d, %.2lf, %s\r\n",
                        arq[quantidade].dia, arq[quantidade].mes, arq[quantidade].ano,
                        arq[quantidade].dado, arq[quantidade].nome_arq);
                quantidade++;

                if (quantidade >= MAX_PESSOAS) {
                    break;
                }
            }

            fclose(file_csv);
        } else {
            printf("Erro ao abrir arquivo CSV para escrita.\n");
        }

        fclose(arq_bin);
    } else {
        printf("Erro ao abrir arquivo binário para leitura.\n");
    }
}

void somatorio_mensal() {
    struct dados arq[MAX_PESSOAS];
    FILE *arq_bin = fopen("binario.dat", "rb");

    if (arq_bin) {
        FILE *file_csv = fopen("somatorio_mensal.csv", "w");

        fread(arq, sizeof(struct dados), MAX_PESSOAS, arq_bin);
        fclose(arq_bin);

        if (file_csv) {
            fputs("Mes, Total de litros\r\n", file_csv);

            double somas[12] = {0};


            for (int i = 0; i < MAX_PESSOAS; ++i) {
                int mes = arq[i].mes;
                if (mes >= 1 && mes <= 12) {
                    somas[mes - 1] += arq[i].dado;
                }
            }


            for (int j = 0; j < 12; ++j) {
                fprintf(file_csv, "%d, %.2lf\r\n", j + 1, somas[j]);
            }

            fclose(file_csv);
            printf("Arquivo somatorio_mensal.csv gerado com sucesso.\n");
        } else {
            printf("Erro ao abrir arquivo CSV para escrita.\n");
        }
    } else {
        printf("Erro ao abrir arquivo binário para leitura.\n");
    }
}

void eliminar(int *quantidade) {
    struct dados arq[MAX_PESSOAS];
    FILE *file_bin = fopen("binario.dat", "rb");

    if (!file_bin) {
        printf("Erro ao abrir arquivo binário para leitura\n");
        return;
    }


    FILE *file_backup = fopen("backup_binario.dat", "wb");
    if (!file_backup) {
        printf("Erro ao criar arquivo de backup\n");
        fclose(file_bin);
        return;
    }

    int total = fread(arq, sizeof(struct dados), MAX_PESSOAS, file_bin);
    fwrite(arq, sizeof(struct dados), total, file_backup);
    fclose(file_backup);
    fclose(file_bin);

    if (total == 0) {
        printf("Nenhum dado encontrado no arquivo binário\n");
        return;
    }

    char obj[20];
    printf("Qual lote deseja eliminar? ");
    scanf("%19s", obj);

    int novo_total = 0;
    for (int i = 0; i < total; ++i) {
        if (strcmp(arq[i].nome_arq, obj) != 0) {
            arq[novo_total++] = arq[i];
        }
    }

    
    file_bin = fopen("binario.dat", "wb");
    if (!file_bin) {
        printf("Erro ao abrir arquivo binário para escrita\n");
        return;
    }

    fwrite(arq, sizeof(struct dados), novo_total, file_bin);
    fclose(file_bin);

    *quantidade = novo_total;
    printf("Lote %s eliminado com sucesso\n", obj);
}


    int main() {

    int quantidade;
    struct dados arq[MAX_PESSOAS];
    char nome[50];
    int opcao;

    for (int i = 0; i < MAX_PESSOAS; i++) {

    }
    do {
        printf("1-Inserir lote \n");
        printf("2-Eliminar lote \n");
        printf("3-Somatorio Mensal \n");
        printf("4-Listagem csv \n");
        printf("5-Sair \n");
        printf("Digite uma opcao: ");
        scanf("%d",&opcao);

        switch (opcao) {

            case 1:
                quantidade = 0;
                printf("Inserir lote \n");
                printf("Qual o lote ?");
                scanf("%s",nome);

                abri_txt(nome,arq,&quantidade);

                escreve_bin(arq,quantidade);

                ler_bin();
                break;
            case 2:
                printf("eliminar lote \n");

                eliminar(&quantidade);

                break;
            case 3:
                printf("Somatorio mensal\n");

                somatorio_mensal();

                break;
            case 4:
                printf("listagem csv\n");

                escreve_csv();

                break;
            case 5:
                break;
            default:
                printf("Numero invalido\n");
        }

    } while (opcao != 5);

    return 0;
}
