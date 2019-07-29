#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "grafo.h"
#include "qlist.h"
#include "qstringlist.h"
#include "qfiledialog.h"
#include "qfile.h"
#include "qstack.h"
#include "qtextstream.h"
#include "qmessagebox.h"
#include "iostream"
#include "qvector.h"
#include "climits"
#include "utility"
using namespace std;
int num=0;
QString carregado;
QVector<int> dist;
QVector<vertice*> hist;
QList <cidade>Cidade;
bool loading=false;
grafo G;
void AjustarGrafo();
#define INFINITO (INT_MAX-1000000)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
bool ExisteCidade(QString nome)
{
    bool cidExis=false;
    for(int i=0;i<Cidade.size();i++)
    {
        if(Cidade.at(i).nome==nome)
        {
            return true;
        }
    }
    return cidExis;
}

void CriarCidade(QString nome)
{
    //qDebug()<<"Cidade "<<nome<<" criada!";
    vertice* ini=new vertice;
    ini->id=num;
    ini->nome="Partida - "+nome;
    G.vertices.push_back(ini);
    num++;
    vertice* fim=new vertice;
    fim->id=num;
    fim->nome="Chegada - "+nome;
    G.vertices.push_back(fim);
    num++;
    //aponta o vertice de partida para o vertice de fim com peso 0, para tratar casos como ex: Goiânia -> Goiânia
    aresta *mesmacid=new aresta;
    mesmacid->origem=ini;
    mesmacid->destino=fim;
    mesmacid->peso=0;
    ini->arestas.push_back(mesmacid);

    aresta *cortaescala=new aresta;
    cortaescala->origem=fim;
    cortaescala->destino=ini;
    cortaescala->peso=0;
    ini->arestas.push_back(cortaescala);

    cidade novacid;
    novacid.nome=nome;
    novacid.origem=ini;
    novacid.destino=fim;
    Cidade.push_back(novacid);
}
int CidadePOS(QString cidade)
{
    for(int i=0;i<Cidade.size();i++)
    {
        if(Cidade.at(i).nome==cidade)
        {
            return i;
        }
    }
    return 0;
}

void MontarGrafo(QString cidade1,QString cidade2, int peso,QString voo,QString hora1,QString hora2)
{
    if(!ExisteCidade(cidade1))
        CriarCidade(cidade1);
    if(!ExisteCidade(cidade2))
        CriarCidade(cidade2);

    vertice* saidaaeroporto=new vertice;//cria vertice de saida
    saidaaeroporto->id=num;
    saidaaeroporto->hora=hora1;
    saidaaeroporto->nome="Saida Voo: "+voo+" - "+cidade1+" horario: "+hora1;
    num++;
    G.vertices.push_back(saidaaeroporto);
    aresta* conexao1=new aresta; //cria aresta
    conexao1->destino=saidaaeroporto; // liga a aresta ao vertice de saida
    conexao1->peso=0;
    Cidade[CidadePOS(cidade1)].partida.push_back(saidaaeroporto);// vincula o vertice a lista de saida
    Cidade[CidadePOS(cidade1)].origem->arestas.push_back(conexao1);// vincula a aresta de saida a origem

    vertice* chegadaaeroporto=new vertice; // cria o vertice de chegada
    chegadaaeroporto->hora=hora2;
    chegadaaeroporto->id=num;
    chegadaaeroporto->nome="Chegada Voo: "+voo+" - "+cidade2+" horario: "+hora2;
    num++;
    G.vertices.push_back(chegadaaeroporto);
    aresta* conexao2=new aresta;// cria aresta
    conexao2->destino=Cidade[CidadePOS(cidade2)].destino;// aposta a aresta de chegada da cidade para o destino
    conexao2->peso=0;//30;
    chegadaaeroporto->arestas.push_back(conexao2);// guarda no vertice de chegada a aresta que aponta pro destino
    Cidade[CidadePOS(cidade2)].chegada.push_back(chegadaaeroporto);//guarda na lista de chegada o vertice de chegada

    aresta* ligacao=new aresta;// cria nova aresta
    ligacao->destino=chegadaaeroporto;// aponta aresta pro vertice de chegada da cidade2
    ligacao->peso=peso;// define o peso
    saidaaeroporto->arestas.push_back(ligacao);// cidade1 guarda endereço da aresta que aponta para cidade 2
}

void MontarGrafoA(QString cidade1,QString cidade2,QString cidade3, int peso,int peso2,int peso3,QString voo,QString hora1,QString hora2,QString hora3,QString hora4)
{
    if(!ExisteCidade(cidade1))
        CriarCidade(cidade1);
    if(!ExisteCidade(cidade2))
        CriarCidade(cidade2);
    if(!ExisteCidade(cidade3))
        CriarCidade(cidade3);

// INTERNO CIDADE 1
    vertice* saidaaeroporto=new vertice;//cria vertice de saida
    saidaaeroporto->hora=hora1;
    saidaaeroporto->id=num;
    saidaaeroporto->nome="Saida Voo: "+voo+" - "+cidade1+" horario: "+hora1;
    num++;
    G.vertices.push_back(saidaaeroporto);
    aresta* conexao1=new aresta; //cria aresta
    conexao1->destino=saidaaeroporto; // liga a aresta ao vertice de saida
    conexao1->peso=0;
    Cidade[CidadePOS(cidade1)].partida.push_back(saidaaeroporto);// vincula o vertice a lista de saida
    Cidade[CidadePOS(cidade1)].origem->arestas.push_back(conexao1);// vincula a aresta de saida a origem
// CIDADE DE ESCALA
    vertice* chegadaescala=new vertice; // cria o vertice de chegada
    chegadaescala->hora=hora2;
    chegadaescala->id=num;
    chegadaescala->nome="Chegada Escala Voo: "+voo+" - "+cidade2+" horario: "+hora2;
    num++;
    G.vertices.push_back(chegadaescala);
    vertice* saidaescala=new vertice; // cria o vertice de saida do segundo aeroporto
    saidaescala->hora=hora3;
    saidaescala->id=num;
    saidaescala->nome="Saida Escala Voo: "+voo+" - "+cidade2+" horario: "+hora3;
    num++;
    G.vertices.push_back(saidaescala);
    //aresta de ligação entre chegada e destino
    aresta* escaladestino=new aresta;
    escaladestino->destino=Cidade[CidadePOS(cidade2)].destino;
    escaladestino->peso=0;//30;
    chegadaescala->arestas.push_back(escaladestino);
    //aresta de ligação entre inicio e saida da escala
    aresta* escalaorigem=new aresta;
    escalaorigem->destino=saidaescala;
    escalaorigem->peso=0;
    Cidade[CidadePOS(cidade2)].origem->arestas.push_back(escalaorigem);
    //aresta de ligação entre voos
    aresta* conexaoescala=new aresta;// cria aresta
    conexaoescala->destino=saidaescala;// aponta a aresta de chegada da cidade para o saidaescala
    conexaoescala->peso=peso3;
    chegadaescala->arestas.push_back(conexaoescala);// guarda no vertice de chegada a aresta que aponta prasaida escala
    Cidade[CidadePOS(cidade2)].chegada.push_back(chegadaescala);//guarda na lista de chegada o vertice de chegada da escala
    Cidade[CidadePOS(cidade2)].partida.push_back(saidaescala);// guarda  na lista de saida o vertice de escala
//  LIGAÇÃO ENTRE CIDADE 1 E A CIDADE 2(DE ESCALA)
    aresta* ligacao=new aresta;// cria nova aresta
    ligacao->destino=chegadaescala;// aponta aresta pro vertice de chegada da cidade2
    ligacao->peso=peso;// define o peso
    saidaaeroporto->arestas.push_back(ligacao);// cidade1 guarda endereço da aresta que aponta para cidade 2
// CIDADE DESTINO
    vertice* chegadaaeroporto=new vertice;// cria o vertice de chegada do ultimo aeroporto
    chegadaaeroporto->id=num;
    chegadaaeroporto->hora=hora4;
    chegadaaeroporto->nome="Chegada Voo: "+voo+" - "+cidade3+" horario: "+hora4;
    num++;
    G.vertices.push_back(chegadaaeroporto);
    aresta* conexao2=new aresta;//cria a aresta que ira ligar o vertice de chegada ao destino
    conexao2->destino=Cidade[CidadePOS(cidade3)].destino;// liga a aresta com o destino
    conexao2->peso=0;//30;// peso e o tempo de desembarque/embarque
    chegadaaeroporto->arestas.push_back(conexao2);// vincula a aresta criada ao vertice de chegada
    Cidade[CidadePOS(cidade3)].chegada.push_back(chegadaaeroporto);//vincula o vertice de chegada a cidade destino
// LIGAÇÃO ENTRE A CIDADE DE ESCALA E A CIDADE DESTINO
    aresta* ligacao2= new aresta;
    ligacao2->destino=chegadaaeroporto;
    ligacao2->peso=peso2;
    saidaescala->arestas.push_back(ligacao2);
}

void MainWindow::on_actionCarregar_triggered()
{
    try{
    if(loading) throw QString(" Já carregado!");
    QString arquivo = QFileDialog::getOpenFileName(
                    this,tr("Selecionar Arquivo"),
                    "C:/Users/Mauricio Benigno/Desktop",
                    "Text File (*.txt);;"
                    );
    //QFile leitor("Arquivo/grafo-01.txt");// caminho completa o local do arquivo
    QFile leitor(arquivo);// caminho completa o local do arquivo
    if(!leitor.open(QFile::ReadOnly |QFile::Text)) throw QString("Arquivo inexistente, não pode ser lido");

    QTextStream in(&leitor);
   // QStack<int>Tempos;

    int numc=0,numh=0,nump=0,numv=0;
    while(!in.atEnd())
    {
        QStack<int>Tempos;
        QList<QString>Ncidades;
        QString saida = in.readLine();
        QString palavra;
        QString voo;
        QVector<QString> time;
        int ini=0,iant=0,num=0;
        for(int i=0;i<=saida.size();i++) // navega pelo vetor onde está contido a frase captada do arquivo
        {

            if(saida[i].isSpace()||saida[i].isNull())//||frase[i].isPunct())//identifica espaço e fim de linha, a fim de identificar as palavras
            {
                int cont=0;
                for(int k=ini;k<i;k++) // copia a palavra encontrada para o vetor palavra (Caracter por caracter)
                {
                    palavra[cont]=saida[i-iant];
                    iant--;
                    cont++;
                }
                num++;
                nump++;
                ini=i+1;
                if(num==1)
                {
                   numv++;
                   voo=palavra;
                }
                else if((num%2==0&&!palavra[0].isNumber())||(num%2!=0&&!palavra[0].isNumber()))
                {
                    numc++;
                    Ncidades.push_back(palavra);
                }
                else if(num%2!=0||(palavra[0].isNumber()&&num%2==0))
                {
                    numh++;
                    QString aux;
                    aux[0]=palavra[0];
                    aux[1]=palavra[1];
                    int hr=aux.toInt();
                    aux[0]=palavra[3];
                    aux[1]=palavra[4];
                    int min=aux.toInt();
                    min=(hr*60)+min;
                    Tempos.push(min);
                    time.push_back(palavra);
                }
                palavra.clear();

            }
            else
            {
                iant++;
            }
        }
        if(Ncidades.size()==2)
        {
            int tempo1 = Tempos.top();
            Tempos.pop();
            int tempo2 = Tempos.top();
            Tempos.pop();

            if(tempo1<tempo2)
            {
                tempo1=tempo1-tempo2;
                tempo1=1440+tempo1;
            }
            else
            {
                tempo1=tempo1-tempo2;
            }
            MontarGrafo(Ncidades.at(0),Ncidades.at(1),tempo1,voo,time[0],time[1]);
            //Armazenar(Ncidades.at(0),Ncidades.at(1),tempo1,voo);
        }
        else if(Ncidades.size()==3)
        {
            int peso1= Tempos.top();
            Tempos.pop();
            int peso2=Tempos.top();
            Tempos.pop();
            //qDebug()<<" peso1: "<<peso1<<" peso2: "<<peso2;
            if(peso1<peso2)
            {
                peso1=peso1-peso2;
                peso1=1440+peso1;
            }
            else
            {
                peso1=peso1-peso2;
            }


            int peso3= Tempos.top();
            Tempos.pop();
            int peso4 = Tempos.top();
            Tempos.pop();
            if(peso3<peso4)
            {
                peso4=peso3-peso4;
                peso4=1440+peso4;
            }
            else
            {
                peso4=peso3-peso4;
            }
            //qDebug()<<" peso3: "<<peso3<<" peso4: "<<peso4;
            //peso4=peso3-peso4;
            //qDebug()<<"Calculado p1: "<<peso1<<" p2: "<<peso4;
            if(peso2<peso4)
            {
                peso2=peso2-peso3;
                peso2=1440+peso2;
            }
            else
            {
                peso2=peso2-peso3;
            }
            //peso2=peso2-peso3;
            //qDebug()<<"Peso entre escala: "<<peso2;
            MontarGrafoA(Ncidades.at(0),Ncidades.at(1),Ncidades.at(2),peso4,peso1,peso2,voo,time[0],time[1],time[2],time[3]);
            //ArmazenarAvancado(Ncidades.at(0),Ncidades.at(1),Ncidades.at(2),peso4,peso1,peso2,voo);
            //peso4- 1ª e 2ª cidades; peso1 - 2ª e 3ª cidade; peso2 - tempo escala cidade2;
        }
    }
    QStringList origens;
    for(int i=0;i<Cidade.size();i++)
    {
        origens<<Cidade.at(i).nome;
    }
    ui->ListaOrigem->addItems(origens);
    ui->ListaDestino->addItems(origens);
    //G.Imprimir();
    AjustarGrafo();
    }
    catch(QString erro)
    {
        QMessageBox::information(this,"Não foi possível montar grafo!",erro);
    }
}
int CalculaPeso(QString a,QString b)
{
    QString aux;
    aux[0]=a[0];
    aux[1]=a[1];
    int hr=aux.toInt();
    aux[0]=a[3];
    aux[1]=a[4];
    int min=aux.toInt();
    min=(hr*60)+min;
    QString aux2;
    aux2[0]=b[0];
    aux2[1]=b[1];
    int hr2=aux2.toInt();
    aux2[0]=b[3];
    aux2[1]=b[4];
    int min2=aux2.toInt();
    min2=(hr2*60)+min2;
    if(min2<min)
    {
        min2=min2-min;
        min2=1440+min2;
    }
    else
    {
        min2=min2-min;
    }
    min2=min2-30;
    if(min2<0)
    {
        //min2=min2-min;
        min2=1440+min2;
    }
    else
    {
        //min2=min2-min;
    }
   //qDebug()<<"Peso: "<<min2;
    return min2+30;
}

void AjustarGrafo()
{
    for(int i=0;i<Cidade.size();i++)
    {
        for(int j=0;j<Cidade.at(i).chegada.size();j++)
        {
            for(int k=0;k<Cidade.at(i).partida.size();k++)
            {
                aresta* aux=new aresta;
                aux->origem=Cidade.at(i).chegada.at(j);
                aux->destino=Cidade.at(i).partida.at(k);
                aux->peso=CalculaPeso(Cidade[i].chegada[j]->hora,Cidade[i].partida[k]->hora);//-30;
                Cidade[i].chegada[j]->arestas.push_back(aux);
            }
        }
    }
}


bool ExisteVertice(QList<pair<vertice*,int> > *lista,vertice* ponto)
{
    for(int i=0;i<lista->size();i++)
    {
        if(lista->at(i).first==ponto)
        {
            return true;
        }
    }
    return false;
}
void TrocaPonto(QList<pair<vertice*,int> > *lista,vertice* ponto,int peso)
{
    for(int i=0;i<lista->size();i++)
    {
        if(lista->at(i).first==ponto)
        {
            pair<vertice*,int> md = lista->at(i);
            md.second=peso;
            lista->replace(i,md);
            //lista->at(i).second=peso;
            return;
        }
    }
}
void OrdenaLista(QList<pair<vertice *, int> > *lista)
{
    int m=lista->size();
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (lista->at(i).second<lista->at(j).second)
            {
                pair <vertice*,int> aux = lista->at(i);
                //lista->at(i).second = lista->at(j).second;
                lista->replace(i,lista->at(j));
                //lista->at(j).second = aux;
                lista->replace(j,aux);
            }
        }
    }
}

//QVector<int> Dijsktra(grafo G, int inicio)
pair<QVector<int>,QVector<vertice*> > Dijsktra(grafo G, int inicio)
{
    qDebug()<<" Aplicando Dijsktra.";
    int ver=0;
    vertice *navegando=G.vertices.at(inicio);
    QVector<int> distancias(G.vertices.size());
    for(int i=0;i<distancias.size();i++)
    {
        distancias[i]=INFINITO;
    }
    distancias[inicio]=0;
    QList<pair<vertice*,int> >pred;
    QVector<vertice*> historico(G.vertices.size());
    historico[inicio]=NULL;
    //pred.push(make_pair(navegando,distancias[inicio]));
    pred.push_back(make_pair(navegando,distancias[inicio]));
    //qDebug()<<" Chegou aqui";
    //int idant;
    while(pred.size()!=0)
    {
        ver++;
        pred.pop_front();
        for(int i=0;i<navegando->arestas.size();i++)
        {
             // fim do historico de navegação
             int peso=distancias[navegando->id]+navegando->arestas.at(i)->peso;
             int pesoant=distancias[navegando->arestas.at(i)->destino->id];
             if(peso<pesoant)
             {
                 //guardando historico de navegação
                  historico[navegando->arestas.at(i)->destino->id]=navegando;
                  //historico[navegando->arestas.at(i)->destino->id]navegando->arestas.at(i)->destino);

                 distancias[navegando->arestas.at(i)->destino->id]=peso;
                 if(ExisteVertice(&pred,navegando->arestas.at(i)->destino))
                    TrocaPonto(&pred,navegando->arestas.at(i)->destino,peso);
                 else
                    pred.push_back(make_pair(navegando->arestas.at(i)->destino,peso));
             }

        }
        OrdenaLista(&pred);
        if(!pred.isEmpty())
        {
            navegando=pred.first().first;
        }
       //idant=navegando->id;
    }
    qDebug()<<"Cidades varridas: "<<ver;
    return make_pair(distancias,historico);
}

void MainWindow::on_pushButton_clicked()
{
    QString origem = ui->ListaOrigem->currentText();
    QString destino = ui->ListaDestino->currentText();
    int id=INFINITO;
    int id2=INFINITO;

    bool acc=true,acc2=true;
    vertice* aux;
    for(int i=0;i<Cidade.size();i++)
    {
        if(acc==false&&acc2==false)
            break;
        if(Cidade.at(i).nome==origem)
        {
            vertice *temp;
            temp=Cidade.at(i).origem;
            id=temp->id;
            acc=false;
        }
        if(Cidade.at(i).nome==destino)
        {
            vertice *temp;
            temp=Cidade.at(i).destino;
            id2=temp->id;
            aux=temp;
            //qDebug()<<"Cidade "<<destino<<" ID "<<temp->id;
            acc2=false;
        }
    }
    if(carregado!=origem)
    {
        carregado=origem;
        pair<QVector<int>,QVector<vertice*> > result;
        result = Dijsktra(G,id);
        dist = result.first;
        hist = result.second;
        QString saida;
        saida+="-------------------------> ITNERARIO DE VOO <-------------------------"
               "\nEntre: "+origem+" e "+destino+"\n  Tempo de viagem: "+QString::number(dist[id2])+" minutos.\n";

        int ids = id2;
        QStack<QString> sd;
        sd.push("|-> "+aux->nome+"\n Fim de trajeto!");
        while(ids!=id)
        {
            sd.push("|-> "+hist[ids]->nome+"\n");
            ids=hist[ids]->id;
        }
        ids=sd.size();
        for(int i=0;i<ids;i++)
            saida+=sd.top(),sd.pop();

        ui->textEdit->setText(saida);
    }
    else
    {
        QString saida;
        saida+=" Tempo de viagem: "+QString::number(dist[id2])+" minutos.\n";
        int ids = id2;
        QStack<QString> sd;
        sd.push("|-> "+aux->nome+" Fim de trajeto!");
        while(ids!=id)
        {
            sd.push("|-> "+hist[ids]->nome+"\n");
            ids=hist[ids]->id;
        }
        ids=sd.size();
        for(int i=0;i<ids;i++)
            saida+=sd.top(),sd.pop();
        ui->textEdit->setText(saida);
    }
}
