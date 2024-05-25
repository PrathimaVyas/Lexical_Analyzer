#ifndef LEX_H
#define LEX_H


#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

enum
{
    e_failure,
    e_success,
};

static char *function[] = {"printf","scanf","fprintf","fscanf","fgets","fputs"};
static char *res_kwords_data[] = {"const","volatile","extern","auto","register","static","signed","unsigned","short","long","double","char","int","float","struct","union","enum","typedef","void",""};

static char *res_kwords_non_data[] = {"goto","return","continue","break","if","else","for","while","do","switch","case","default","sizeof",""};

static char operators[] = {'/','+','-','*','%','=','<','>','&','|','!','^'};

static char symbols[] = {'{','}',')','(',':','?'};

static char *pre_directives[] = {"#include","#define","#if","#else","elif","#endif","undef","#ifndef","#error","#pragma","#line","#warning"};

int get_line(FILE *fptr);
int check_word(char *str);
int ispreprocessor(char *word);
int isreserved_data(char *word);
int isreserved_non_data(char *word);
int isnum(char ch);
int isoper(char ch);
int issymbols(char ch);
int isfunction(char *word);
int isstring(char *word);
int ispointer(char ch);

#endif


