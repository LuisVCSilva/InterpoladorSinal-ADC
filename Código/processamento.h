#define margemDeErro 0.00001
#include <math.h>
#include <stdlib.h>

void divergencia (double *base,double *saida,double coeficiente,int expoente);
void convergencia (double *base,double *coeficiente,int *expoente);
double fatorial (double n);

unsigned int tamanhoAmostragem = 0;
double *input;

void defineEntrada (double *base,unsigned int tamanho) {
	input = base;
	tamanhoAmostragem = tamanho;
}

void divergencia (double *base,double *saida,double coeficiente,int expoente) {
	for(int i=0;i<tamanhoAmostragem;i++)
	{
	saida[i] = base[i]-(coeficiente*pow(i+1,expoente));
	}
}

unsigned char checaIgualdadeDaBase (double *base) {
	unsigned char flagIgualdade = 0x00;
	for(int i=1;i<tamanhoAmostragem;i++)
	{
		double x = base[i-1]-base[i];
		if(!((x<=margemDeErro) && (x>=-margemDeErro)))
		{
			flagIgualdade = 0x01;
			break;
		}
	}
	return flagIgualdade;
}

void convergencia (double *base,double *coeficiente,int *expoente) {
	double **tabelasIteradas;
	int j = 0;
	double valorFinal = 0;
	unsigned char flagIgualdade = checaIgualdadeDaBase(base);

	if(flagIgualdade==0x00)
	{
		*coeficiente = base[0];
		*expoente = 0;
	}

	else
	{
		flagIgualdade = 0x00;
		tabelasIteradas = (double **) malloc(sizeof(double)*1);
		/*if(tabelasIteradas==NULL)
		{
		}*/
		*(tabelasIteradas+0) = (double *) malloc(sizeof(double)*(tamanhoAmostragem-1));
		/*if(*(tabelasIteradas+0)==NULL)
		{
		}*/

		*(*(tabelasIteradas+(0))+0) = base[1]-base[0];

		for(int i=1;i<tamanhoAmostragem-1;i++)
		{
			*(*(tabelasIteradas+(0))+i) = base[i+1]-base[i];
			if(*(*(tabelasIteradas+(0))+i)!=*(*(tabelasIteradas+(0))+(i-1)))
			{
				flagIgualdade = 0x01;
			}
		}

		if(flagIgualdade==0x00)
		{
			valorFinal = *(*(tabelasIteradas+(0))+0);
			j = 1;
		}
		else
		{
			flagIgualdade = 0x00;
			for(j=0;;j++)
			{
				tabelasIteradas = (double **) realloc(tabelasIteradas,sizeof(double *)*2);
				/*if(tabelasIteradas==NULL)
				{
				}*/

				*(tabelasIteradas+((j+1)%2)) = (double *) malloc(sizeof(double)*(tamanhoAmostragem-j-1));
				/*if(*(tabelasIteradas+((j+1)%2))==NULL)
				{
				}*/
				
				flagIgualdade = 0x00;
				*(*(tabelasIteradas+((j+1)%2))+0) = *(*(tabelasIteradas+(j%2))+(0+1)) - *(*(tabelasIteradas+(j%2))+(0));
				if((*(*(tabelasIteradas+((j+1)%2))+0)<=margemDeErro) && ((*(*(tabelasIteradas+((j+1)%2))+0)>=-margemDeErro)))
				{
					flagIgualdade = 0x00;
					valorFinal = *(*(tabelasIteradas+j%2)+0);
					j++;
					break;
				}

				for(int i=1;i<tamanhoAmostragem-j-2;i++)
				{
					*(*(tabelasIteradas+((j+1)%2))+i) = *(*(tabelasIteradas+(j%2))+(i+1)) - *(*(tabelasIteradas+(j%2))+(i));
					if( (*(*(tabelasIteradas+((j+1)%2))+i)!=*(*(tabelasIteradas+((j+1)%2))+(i-1))) )
					{
						flagIgualdade = 0x01;
					}
				}
				if(flagIgualdade==0x00)
				{
					valorFinal = *(*(tabelasIteradas+((j+1)%2))+0);
					j += 2;
					break;
				}
			}
		}
		free(tabelasIteradas);
		*coeficiente = valorFinal/fatorial(j);
		*expoente = j;
	}
}

double fatorial (double n) { return n<=2 ? (n<=0 ? 1 : n) : n*fatorial(n-1);}