#include <iostream>
#include <time.h>
#include <math.h>

#include <stdio.h>
#include <stdlib.h>
#define filas 3
#define colum 3
#define CERO 0
#define UNO 1
#include <vector>
using namespace std;


int ma[filas][colum];
vector<int>ve_fi;
vector<int>ve_co;

float cambiar=0;

void llenar()
{

    srand(time(NULL));
    for(int i=0; i<filas; ++i)
    {
        for(int j=0; j<colum; ++j)
        {
            ma[i][j] = rand() % 2;
        }
    }
}

void mostrar()
{

     for(int i=0; i<filas; ++i)
    {
        for(int j=0; j<colum; ++j)
            cout<<ma[i][j]<<" ";
        cout<<endl;
    }
}

int contadores()
{
    float contador=0;
    float contador2=0;
    
    for( int i=0;i<filas; ++i)
    {
        int j=0;
        for(; j<colum; ++j)
        {
           if(ma[i][j]==1)
            contador++;
        }
         cout<<"numeros de: "<<"fila: "<<i<<"-> "<<contador<<endl;
         ve_fi.push_back(contador);
         cambiar+=contador;
         contador=0;
    }
    cout<<endl;
    for( int i=0;i<filas; ++i)
    {
        int j=0;
        for(; j<colum; ++j)
        {
           if(ma[j][i]==1)
            contador2++;
        }
         cout<<"numeros de: "<<"columna: "<<i<<"-> "<<contador2<<endl;
         cambiar+=contador2;
         contador2=0;
         ve_co.push_back(contador2);
    }
    float tmp = filas + colum;
    cout<<"numero de 1 para cada fila y columna; "<< ceil(cambiar / tmp)<<endl;
    return 0;
}

bool ponerFila(int i)
{   
    int cont = 0;
    for(int j=0;j<filas;++j)
    {
        if(ma[i][j] == 1)
            cont++;
    }
    //cout<<" asa "<<cont<<" asas "<<cambiar<<endl;
    if(cont < cambiar / (filas + colum))
        return true;
    else
        return false;
}

bool ponerColum(int i)
{   
    int cont = 0;
    for(int j=0;j<colum;++j)
    {
        if(ma[j][i] == 1)
            cont++;
    }
    if(cont < cambiar / (filas + colum))
        return true;
    else
        return false;
}

void solucion()
{
    
    if (cambiar == filas)
    {
        for(int i=0; i<filas; ++i)
        {
            for(int j=0; j<colum; ++j)
                ma[i][j] = 1;
        }
        mostrar();
        return;
    }


     if(filas!=colum)
    {
        int cont=0;
     for(int i=0; i<filas; ++i)
        {
            for(int j=0; j<colum; ++j)
            {
                if(ma[i][j] == 0)
                {
                    ma[i][j] = 1;
                    cont++;
                }
            }
        }
            mostrar();
            return;
    }

    if(cambiar < 1)
    {
        for(int i=0; i<filas; ++i)
        {
            for(int j=0; j<colum; ++j)
            {
                if(ma[i][j] == 0)
                {
                    ma[i][j] = 0;
                    cont++;
                }
            }
        }
        mostrar();
        return;
    }

    contadores();
    for( int i=0;i<filas; ++i)
    {
        if(ponerFila(i)==true)
        {
            cout<<"se puede poner 1 en la fila; "<<i<<endl; 
        }else
            cout<<"fila' "<<i<<" llena"<<endl; 
    }

    for( int i=0;i<colum; ++i)
    {
        if(ponerColum(i)==true)
        {
            cout<<"se puede poner 1 en la columna; "<<i<<endl; 
        }else
            cout<<"columna' "<<i<<" llena"<<endl; 
    }

       

}

int main()

{
    llenar();
    mostrar();
    cout<<endl;
    solucion();
    return 0;
}
