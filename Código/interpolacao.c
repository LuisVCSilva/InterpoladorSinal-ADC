#define FOSC 8000000UL// Clock Speed
#define BAUD 9600

#include <avr/io.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include <util/delay.h>
#include "processamento.h"
#define MYUBRR ((FOSC/ (BAUD * (long)16)))
#define  F_CPU 8000000
static int uart_putchar(char c, FILE *stream);
static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
double hex2Per (unsigned char x);
void USART_Init( unsigned intubrr);

#define atrasoEntreLeituras 10000
#define tamanhoAmostragem 5/*sizeof(entrada)/sizeof(unsigned char)*/

uint16_t ReadADC(uint8_t ch);
void InitADC();
void interpola (double *entradaOficial);

int main () {
	//unsigned char entrada[] = {0xFF,0xE6,0xCC,0xAF,0x6F};
	double entradaOficial[tamanhoAmostragem];
	/*for(int i=0;i<tamanhoAmostragem;i++)
	{
	entradaOficial[i] = hex2Per(entrada[i]);
	}*/
	InitADC();
	USART_Init(MYUBRR);
	unsigned char flagImpressao = 0x00;
	unsigned char leitorADC = 0x00;
	stdout = &mystdout;
	
	while(1)
	{
		if(flagImpressao==0x00)//Comeca o processamento
		{
			printf("Serao lidos %d valores\n",tamanhoAmostragem);
			_delay_ms(atrasoEntreLeituras);
			for(int i=0;i<tamanhoAmostragem;i++)
			{
			leitorADC = ReadADC(0);
			entradaOficial[i] = hex2Per(leitorADC);
			printf("\n T(%d) -> Valor do ADC = %f",i+1,entradaOficial[i]);
			_delay_ms(atrasoEntreLeituras);
			}
			interpola(entradaOficial);
			flagImpressao = 0x01;
		}
	}
	return 0;
}

void interpola (double *entradaOficial) {
printf("\n\nf(x) = ");
defineEntrada(entradaOficial,tamanhoAmostragem);
double coeficiente = 0;
int expoente = 0;
int expoenteAnterior = expoente;
convergencia(entradaOficial,&coeficiente,&expoente);

if(coeficiente==1)
{
	if(expoente==1)
	{
		printf("+(x)");
	}
	else
	{
		printf(expoente==0 ? "+(x^%d)" : "(x^%d)",expoente);
	}
}
else if((coeficiente!=0) || (expoente!=0))
{
	printf(expoente==0 ? "+(%f)\n" : expoente==1 ? "+(%f*x)" : "+(%f*x^%d)",coeficiente,expoente);
}
double *saida = (double *) malloc(sizeof(double)*tamanhoAmostragem);
divergencia(entradaOficial,saida,coeficiente,expoente);
convergencia(saida,&coeficiente,&expoente);
if(coeficiente==1)
{
	if(expoente==1)
	{
		printf("+(x)");
	}
	else
	{
		printf(expoente==0 ? "+(x^%d)" : "(x^%d)",expoente);
	}
}
else if((coeficiente!=0) || (expoente!=0))
{
	printf(expoente==0 ? "+(%f)\n" : expoente==1 ? "+(%f*x)" : "+(%f*x^%d)",coeficiente,expoente);
}
while((expoente!=0) && (expoenteAnterior!=expoente))
{
	expoenteAnterior = expoente;
	divergencia(saida,saida,coeficiente,expoente);
	convergencia(saida,&coeficiente,&expoente);
	if(coeficiente==1)
	{
		if(expoente==1)
		{
			printf("+(x)");
		}
		else
		{
			printf(expoente==0 ? "+(x^%d)" : "(x^%d)",expoente);
		}
	}
	else if((coeficiente!=0) || (expoente!=0))
	{
		printf(expoente==0 ? "+(%f)\n" : expoente==1 ? "+(%f*x)" : "+(%f*x^%d)",coeficiente,expoente);
	}
}
}

int uart_putchar(char c, FILE *stream) {
	if (c == '\n')
	uart_putchar('\r', stream);
	while(!(UCSRA & (1<<UDRE)));
	UDR = c;
	return 0;
}

void USART_Init(unsigned int ubrr)
{
UBRRH=(unsigned char)(ubrr>>8);
UBRRL=(unsigned char)ubrr;
UCSRB=(1<<RXEN)|(1<<TXEN);
UCSRC=(1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
}

double hex2Per (unsigned char x) {
return ((double) ((2*((float) (x & 0xFF)))/51));
}

void InitADC()
{
ADMUX=(1<<REFS0);            
ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}


uint16_t ReadADC(uint8_t ch)
{
ch=ch&0x07;
ADMUX|=ch;
ADCSRA|=(1<<ADSC);
while(!(ADCSRA & (1<<ADIF)));
ADCSRA|=(1<<ADIF);
return(ADC);
}