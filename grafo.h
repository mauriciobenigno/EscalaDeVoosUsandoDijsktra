#ifndef GRAFO_H
#define GRAFO_H
#include "qlist.h"
#include "qstring.h"
#include "qdebug.h"

class aresta;

class vertice{
public:
    int id;
    QString nome;
    QString hora;
    QList<aresta*> arestas;
};
class aresta{
public:
    int peso;
    vertice* origem;
    vertice* destino;
    
};

class grafo
{
public:
    QList <vertice*> vertices;
    QList <aresta*> arestas;
    grafo();
    void Imprimir()
    {
        for(int i=0;i<vertices.size();i++)
        {
            qDebug()<<"ID: "<<vertices.at(i)->id<<" nome: "<<vertices.at(i)->nome+" apontamentos: "+QString::number(vertices.at(i)->arestas.size());
            for(int k=0;k<vertices.at(i)->arestas.size();k++)
            {
                qDebug()<<"-- Aresta "+QString::number(k)+" Liga "+vertices.at(i)->arestas.at(k)->origem->nome+" a "+vertices.at(i)->arestas.at(k)->destino->nome+" com peso: "+QString::number(+vertices.at(i)->arestas.at(k)->peso);
            }
        }
    }

};
class cidade{
public:
    QString nome;
    vertice* origem;
    vertice* destino;
    QList <vertice*> partida;
    QList <vertice*> chegada;
};

#endif // GRAFO_H
