#include"lex.h"

int main(int argc,char **argv)
{
    int sum;
    if(argc != 2)
    {
        printf("ERROR : INVALID COMMAND LINE ARGUMENTS\n");
        return 0;
    }
    char *file=malloc(sizeof(argv[1])+1);
 
    /* check the file is .c file or not */
    if(strstr(argv[1],".c") == NULL)
    {
        printf("INFO : GIVE THE .c FILE IN THE COMMAND LINE ARGUMENT\n");
        return 0;
    }

    /* copy the command line argument into the file */
    strcpy(file,argv[1]);
   
    /* open the file*/
    FILE *fp = fopen(file,"r");
    if(fp == NULL)
    {
        printf("FILE CANN'T BE OPENED\n");
        return 0;
    }

    char ch;
   /* read character by character */
    while((ch = fgetc(fp)) != EOF)
    {
        char buffer[100];
        int i=0;
	/* if the character is space or next line character means skip that character */
        if(ch == '\n' || ch == ' ')
        {
            continue;
        }
	/* if the character is # then check it is preprocessor or not */
        else if(ch == '#')
        {
            buffer[i++] = ch;
            while(((ch = fgetc(fp)) != EOF) && (ch != '\n'))
            {
                buffer[i++] = ch;
            }
            buffer[i] = '\0';
            if(ispreprocessor(buffer) == e_success)
                printf("%s -> preprocessor directive \n\n",buffer);
        }
	/* check the ch is number or not */
        else if(isnum(ch))
        {
            i=0;
            buffer[i++]=ch;
            while( ((ch = fgetc(fp)) != EOF) && (isnum(ch) || ch == '.') )
            {
                buffer[i++] = ch;
            }
            buffer[i] = '\0';
            fseek(fp,-1,SEEK_CUR);
            printf("%s -> constant\n\n",buffer);
        }
	/* check the character is ' or not , if ch is ' then move the cursor till next ' and store it in a buffer print it as character literal */
        else if(ch == '\'')
        {
            i=0;
            buffer[i++] = ch;
            while( ((ch = fgetc(fp)) != '\n') && ch != '\'')
                buffer[i++] = ch;
            buffer[i++] = ch;
            buffer[i] = '\0';
            printf("%s -> character literal\n\n",buffer);
        }
	/* if the ch is " then move the cursor till it reach next " then store it in a buffer and it is a string literal */
        else if(ch == '"')
        {
            i=0;
            buffer[i++] = ch;
            while( (ch = fgetc(fp)) != EOF && ch != '"')
                buffer[i++] = ch;
            buffer[i++] = ch;
            buffer[i] = '\0';
            printf("%s -> string literal\n\n",buffer);
        }
        else if(ch == ',')
            printf("%c -> comma\n\n",ch);
        else if(ch == ';')
            printf("%c -> end of the statement\n\n",ch);
	/* if the character is alphabet or underscore move till it reach space or special symbols and store it in a buffer then check it is reserved data or reserved non data or function or identifer */
        else if(isalpha(ch) || ch == '_')
        {
            i=0;
            buffer[i++] = ch;
            while( (ch = fgetc(fp)) != EOF && (isalnum(ch) || ch =='_') )
                buffer[i++] = ch;
            buffer[i] = '\0';
            fseek(fp,-1,SEEK_CUR);
            if(isreserved_data(buffer) == e_success)
                printf("%s -> reserved data\n\n",buffer);
            else if(isreserved_non_data(buffer) == e_success)
                printf("%s -> reserved non data\n\n",buffer);
            else if(isfunction(buffer) == e_success)
                printf("%s -> function\n\n",buffer);
            else
                printf("%s -> identifier, variable\n\n",buffer);
        }
        /* check for the pointer variable */
	else if(ispointer(ch))
        {
            i=0;
            buffer[i++] = ch;
            while( (ch = fgetc(fp)) != EOF && ch != ' ')
                buffer[i++] = ch;
            buffer[i] = '\0';
            fseek(fp,-1,SEEK_CUR);
            printf("%s -> pointer variable\n\n",buffer);
        }
	/* check the condition for single and multiline comments */
        else if(ch == '/')
        {
            i=0;
            buffer[i++] = ch;
            if((ch = fgetc(fp)) == '/')
            {
                buffer[i++] = ch;
                while( (ch = fgetc(fp)) != '\n')
                    buffer[i++] = ch;
                buffer[i] = '\0';
                printf("%s -> single line comment\n\n",buffer);
            }
            else  if( ch == '*')
            {
                buffer[i++] = ch;
                int flag=1;
                while(flag)
                {
                    while( (ch=fgetc(fp)) != '*')
                        buffer[i++] = ch;
                    buffer[i++] = ch;
                    if((ch=fgetc(fp)) == '/')
                    {
                        flag=0;
                        buffer[i++] = ch;
                        buffer[i] = '\0';
                        printf("%s -> multiline comment\n\n",buffer);
                    }
                    else
                        buffer[i++]=ch;

                }
            }
	    /* if the second char is neither \ nor * then it is an operator so move the cursor to one position back and print it as a operator */
            else
            {
                fseek(fp,-1,SEEK_CUR);
                printf("%c -> operator\n\n",buffer[0]);

            }

        }
        else if(isoper(ch) == e_success)
            printf("%c -> operator\n\n",ch);
        else if(issymbols(ch) == e_success)
            printf("%c -> symbol\n\n",ch);
    }
}


/* to check it is pointer variable or not */
int ispointer(char ch)
{
    if(ch == '*')
        return e_success;
    return e_failure;
}

/* to check it is function or not */
int isfunction(char *word)
{
    int i=0;
    while(function[i] != NULL)
    {
        if(strstr(word,function[i])!=NULL)
            return e_success;
        i++;
    }
    return e_failure;
}
  

/* to check it is preprocessor or not */
int ispreprocessor(char *word)
{
    int i=0;
    while(pre_directives[i] != NULL)
    {
        if(strcmp(word,pre_directives[i]))
            return e_success;
        i++;
    }
    return e_failure;
}


/* to check it is symbols or not */
int issymbols(char ch)
{
    int i=0;
    while(symbols[i] != '\0')
    {
        if(ch == symbols[i])
            return e_success;
        i++;
    }
    return e_failure;
}

/* to check it is symbols or not */
int isoper(char ch)
{
    int i=0;
    while(operators[i] != '\0')
    {
        if(ch == operators[i])
            return e_success;
        i++;
    }
    return e_failure;
}

/* to check it is reserved data or not */
int isreserved_data(char *word)
{
    int i=0;
    while(res_kwords_data[i] != NULL)
    {
        if(strcmp(word,res_kwords_data[i])==0)
            return e_success;
        i++;
    }
    return e_failure;
}


/* to check it is non reserved data or not */
int isreserved_non_data(char *word)
{
    int i=0;
    while(res_kwords_non_data[i] != NULL)
    {
        if(strcmp(word,res_kwords_non_data[i])==0)
            return e_success;
        i++;
    }
    return e_failure;
}


/* to check it is numbers or not */
int isnum(char ch)
{
    if(ch >= '0' && ch <= '9')
        return e_success;
    return e_failure;
}

