#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <math.h>


struct methodcir 
{
    char startchar[17];
    
    int *inputs;
    int *selector;
    int *outputs;


    int totalinp;
    int totalout;
};

int checkFile(FILE *file)
{
    if(!file){
        printf("ERROR");
        return 0;
    }
    return 1;
}

int powtwomethod(int expo, int bent)
{
    return pow(bent, expo);
}

int increment( int counter, int *arr)
{
    int vartwo =2; 
    for(int x = counter + 1; x >= vartwo; x--){
        arr[x] = !arr[x];
        if(arr[x] == 1){
            return 1;
        }
    }
    return 0;
}

void XOR(int *arr,  int inxnext, int inxstart, int fnlindex)
{
    arr[fnlindex] = arr[inxstart] ^ arr[inxnext];
}

void AND(int *arr,  int inxnext, int inxstart, int fnlindex)
{
    arr[fnlindex] = arr[inxnext] && arr[inxstart];
}

void OR(int *arr,  int inxnext, int inxstart, int fnlindex)
{
    arr[fnlindex] = arr[inxnext] || arr[inxstart];
}

void PASS(int *arr,  int fnlindex, int inxstart)
{
    arr[fnlindex] = arr[inxstart];
}

void NOT(int *arr,  int fnlindex, int index)
{
    arr[fnlindex] = !arr[index];
}

void NAND(int *arr,  int inxnext, int inxstart, int fnlindex)
{
    arr[fnlindex] = !(arr[inxstart] && arr[inxnext]);
}

void NOR(int *arr,  int inxnext, int inxstart, int fnlindex)
{
    arr[fnlindex] = !(arr[inxstart] || arr[inxnext]);
}

void MULTIPLEXER( int *outputarr, int *arr, int *inputarr,int inxout, int exponent) 
{
    int counter = 0;
    int newone =1;

    //extra counter
    for (int x = 0; x < exponent; x++) 
    {
        counter = (arr[outputarr[x]] * powtwomethod((exponent - x - newone ), 2)) + counter;
    }
    arr[inxout] = arr[inputarr[counter]];
}

void DECODER(int *arr, int exponent, int *inputarr, int *outputarr) 
{
    int counter = 0;
    int varone =1;
    for (int x = 0; x < powtwomethod(exponent, 2); x++) {
        arr[outputarr[x]] = 0;
    }

    for (int x = 0; x < exponent; x++) {
        counter = counter + arr[inputarr[x]] * powtwomethod(exponent - x - varone, 2);
    }
   
    arr[outputarr[counter]] = varone;
}

void listfreemethod(struct methodcir* listcir, int counter)
{
    for(int x = 0; x < counter; x++)
    {
        free(listcir[x].inputs);
        free(listcir[x].outputs);
        //free listx
        free(listcir[x].selector);
    }
    free(listcir);
}


int checkargmethod(int argc)
{

    if(2 != argc){
        printf("ERROR!");
        return 0;
    }
    return 1;
}

void truthtablemethod(int varsizing, char **ttvar)
{
    for(int x = 0; x < varsizing; x++)
    {
        free(ttvar[x]);
    }
    free(ttvar);
}

int index(char **arr, int varsizing, char *varchar)
{
    for(int x = 0; x < varsizing; x++)
    {
        if(0 == strcmp(arr[x], varchar)){
            return x;
        }
    }
    return -1;
}


int main(int argc, char ** argv)
{
    checkargmethod(argc);

    FILE *filenew;
    filenew = fopen(argv[1], "r");
    checkFile(filenew);

    int txtcount = 0;       
    int countout = 0;
    int countlist = 0;
    int introvar = 2; 
    int varsizing = 2;
    int startingvar =0;
    int numvar = 17; 
    int countinp = 0;
    

   

    char *chartypeone = (char*)malloc(sizeof(char) * numvar), **ttvar;
    struct methodcir* listcir;

    fscanf(filenew, "%16s", chartypeone);
    //another fscanf
    fscanf(filenew, " %d", &countinp);

    varsizing = countinp + varsizing;

    ttvar = (char**)malloc(sizeof(char*) * varsizing);

    for(int x = startingvar; x < varsizing; x++)
    {
        ttvar[x] = (char*) malloc(sizeof(char) * numvar);
    }

    strcpy(ttvar[0], "0\0");
    strcpy(ttvar[1], "1\0");

    for(int x = startingvar; x < countinp; x++)
    {
        fscanf(filenew, "%16s", chartypeone);

        strcpy(ttvar[introvar + x], chartypeone);
    }

    fscanf(filenew, "%16s", chartypeone);
    fscanf(filenew, "%d", &countout);

    varsizing = countout + varsizing;
    ttvar = realloc(ttvar, sizeof(char*) * varsizing);

    for(int x = startingvar; x < countout; x++)
    {
        ttvar[countinp + (x + introvar)] = (char*)malloc(sizeof(char) * numvar);
    }

    for(int x = startingvar; x < countout; x++)
    {
        fscanf(filenew, "%16s", chartypeone);

        strcpy(ttvar[introvar + countinp + x], chartypeone);
    }

    listcir = NULL;

    while(1 != feof(filenew))
    {
        struct methodcir keepercir;

        int countout = 1;
        int inputCounter = 2;
        

        keepercir.totalout = 0;
        keepercir.totalinp = 0;

        int onecheck=1;
        int zerocheck=0;

        countlist = onecheck + countlist;

        fscanf(filenew, " %s", chartypeone);
        strcpy(keepercir.startchar, chartypeone);


        if(zerocheck == strcmp(chartypeone, "NOT") )
        {
            inputCounter = onecheck;
        }
        else if(zerocheck == strcmp(chartypeone, "MULTIPLEXER"))
        {
            fscanf(filenew, "%d", &inputCounter);
            keepercir.totalout = inputCounter;
            inputCounter = powtwomethod( inputCounter, 2);
        }
        else if(zerocheck == strcmp(chartypeone, "DECODER"))
        {
            fscanf(filenew, "%d" , &inputCounter);
            keepercir.totalinp = inputCounter;
            countout = powtwomethod( inputCounter, 2);
        }
        
        else if (zerocheck == strcmp(chartypeone, "PASS"))
        {
            inputCounter = onecheck;
        }        

        keepercir.inputs = (int *)malloc(sizeof(int) * inputCounter);
        keepercir.selector = (int *)malloc(sizeof(int) * keepercir.totalout);
        keepercir.outputs = (int *)malloc(sizeof(int) * countout);
        
        

        for(int x = zerocheck; x < inputCounter; x++)
        {
            fscanf(filenew, "%16s", chartypeone);
            keepercir.inputs[x] = index(ttvar,varsizing, chartypeone);
        }

        for(int x = zerocheck; x < keepercir.totalout; x++)
        {
            fscanf(filenew, "%16s", chartypeone);
            keepercir.selector[x] = index(ttvar, varsizing, chartypeone);
        }

        for(int x = zerocheck; x < countout; x++)
        {
            fscanf(filenew, "%16s", chartypeone);
            int check = index(ttvar, varsizing, chartypeone);

            if(-1 != check)
            {
                keepercir.outputs[x] = check;
            }
            else if(-1 == check)
            {
                txtcount = onecheck + txtcount;
                varsizing = onecheck + varsizing;

                ttvar = realloc(ttvar, sizeof(char*) * varsizing);
                ttvar[varsizing - onecheck] = (char*)malloc(sizeof(char) * numvar);
                strcpy(ttvar[varsizing - onecheck], chartypeone);
                keepercir.outputs[x] = varsizing - onecheck;
            }
        }

        if(!listcir)
        {
            listcir = (struct methodcir*)malloc(sizeof(struct methodcir));
        }
        else 
        {
            listcir = realloc(listcir, countlist * sizeof(struct methodcir));
        }


        listcir[countlist - onecheck] = keepercir;
       
    }

    fclose(filenew);

    int *newdata;
    newdata = (int *)malloc(sizeof(int)* varsizing);


    for(int x = 0; x < varsizing; x++)
    {
        newdata[x] = 0;
    }
    // nextdata 
    newdata[1] = 1;

    while(1)
    {
        int extrraone=1;
        int extrazero =0; 

        for(int x = extrazero; x < countinp; x++){
            printf("%d ", newdata[2 + x]);
        }
        printf("|");

        for(int x = extrazero; x < countlist; x++)
        {
            struct methodcir keepercir = listcir[x];

            if(0 == strcmp(keepercir.startchar, "NOT"))
            {
                NOT(newdata,  keepercir.outputs[extrazero], keepercir.inputs[extrazero]);
            }
            else if(0 == strcmp(keepercir.startchar, "DECODER"))
            {
                DECODER(newdata, keepercir.totalinp, keepercir.inputs, keepercir.outputs);
            }
            else if(0 == strcmp(keepercir.startchar, "OR"))
            {
                OR(newdata,  keepercir.inputs[extrraone], keepercir.inputs[extrazero], keepercir.outputs[extrazero]);
            }
            else if(0 == strcmp(keepercir.startchar, "MULTIPLEXER"))
            {
                MULTIPLEXER(keepercir.selector, newdata,  keepercir.inputs,  keepercir.outputs[0], keepercir.totalout);
            }
            else if(0 == strcmp(keepercir.startchar, "NAND"))
            {
                NAND(newdata, keepercir.inputs[extrraone],  keepercir.inputs[extrazero], keepercir.outputs[extrazero]);
            }
            else if(0 == strcmp(keepercir.startchar, "NOR"))
            {
                NOR(newdata,  keepercir.inputs[extrraone], keepercir.inputs[extrazero], keepercir.outputs[extrazero]);
            }
            else if(0 == strcmp(keepercir.startchar, "XOR"))
            {
                XOR(newdata,  keepercir.inputs[extrraone], keepercir.inputs[extrazero], keepercir.outputs[extrazero]);
            }
            else if(0 == strcmp(keepercir.startchar, "AND"))
            {
                AND(newdata, keepercir.inputs[extrraone], keepercir.inputs[extrazero], keepercir.outputs[extrazero]);
            }
            else if(0 == strcmp(keepercir.startchar, "PASS"))
            {
                PASS(newdata, keepercir.outputs[extrazero], keepercir.inputs[extrazero]);
            }
           
        }

        for(int x = extrazero; x < countout; x++)
        {
            printf(" %d", newdata[countinp + 2 + x]);
        }
        printf("\n");

        if(increment(countinp, newdata) != extrraone)
        {
            break;
        }
    }



//final part
    listfreemethod(listcir, countlist);
    truthtablemethod(varsizing, ttvar);    
    free(newdata);
    free(chartypeone);    

    return 0;
}

