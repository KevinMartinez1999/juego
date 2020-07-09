#include "menu_partida.h"
#include "ui_menu_partida.h"
#include "widget.h"
#include "mapa_gameplay.h"

int num_jugadores;
extern QString user, pass;

Menu_partida::Menu_partida(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Menu_partida)
{
    ui->setupUi(this);
    /*Sistema de reproducción de gif en el menú:
    Para reproducir un gif primeramente se creara un nuevo QLabel al cual le asignaremos las dimensiones de la ventana, posterior a eso
    crearemos una variable QMovie con el gif a reproducir y también le asignaremos el tamaño de la pantalla, luego con
    la función setMovie le asignaremos al Label que contenga el gif y se reproduzca.*/
    QLabel *w = new QLabel(this);
    w->resize(1000,650);//Tamaño de la ventana.
    movie = new QMovie(":/Imagenes/GIF1.gif");
    movie->setScaledSize(QSize(1000,650));//Tamaño de la ventana.
    w->setMovie(movie);//Asignamos el gif al Label.
    movie->start();//Iniciamos la reproducción del gif.

    /*Ya que estamos creando el gif por código, siempre aparecerá en la ventana por encima de todos los elementos agregados en el .ui,
    para arreglar esto haremos uso de la función .raise(); que traerá al frente los elementos de la interfaz grafica
    (Labels, botones, lineEdits, etc). Primero traeremos al frente el label Interfaz que contiene la imagen diseñada para el menú,
    y luego se traerá al frente los demás elementos.*/
    ui->interfaz->raise();
    ui->un_jugador->raise();
    ui->dos_jugadores->raise();
    ui->cargar_partida->raise();
    ui->nueva_partida->raise();
    ui->volver->raise();

    /*Creacion del cursor del videojuego: con ayuda de QCursor podremos brindarle al cursor la imagen que deseamos.
    Primeramente crearemos un pixmap que contiene la imagen, luego se creara una variable QCursor que recibira la imagen
    y los puntos de eje del click, luego con setCursor establecemos ese cursor para toda la ventana.*/
    QPixmap Pixmap_Cursor = QPixmap(":/Imagenes/CURSOR.png");
    QCursor cursor = QCursor(Pixmap_Cursor,0,0);
    setCursor(cursor);

    ui->un_jugador->setChecked(true);/*Por defecto siempre estará señalada la opción de un jugador en caso de que el usuario no escoja.*/

    int cont = 0;

    ifstream file("../Videojuego_Final/Partidas/"+user.toUtf8()+".txt");
    if (!file.is_open())
        return;

    while(file.good())
        if(file.get()=='\n')
            cont++;

    if (cont <= 1)
        ui->cargar_partida->setEnabled(false);
    else
        ui->cargar_partida->setEnabled(true);
}

Menu_partida::~Menu_partida()
{
    delete ui;
}

void Menu_partida::on_volver_clicked()
{
    /*Si por alguna razon al entrar al menu de nueva-cargar partida deseamos cerrar sesion y volver al menu principal simplemente
    se cerrara la ventana y se creara un nuevo widget y se abrira.*/
    Widget *w = new Widget;
    w->show();
    movie->stop();//Se detendrá la reproducción de la variable QMovie que contiene al gif del menú para evitar problemas de memoria.
    close();
}

void Menu_partida::on_nueva_partida_clicked()
{
    if(ui->un_jugador->isChecked())
        num_jugadores = 1;
    else if(ui->dos_jugadores->isChecked())
        num_jugadores = 2;
    Mapa_GamePlay *mapa = new Mapa_GamePlay();
    mapa->show();
    movie->stop();//Se detendrá la reproducción de la variable QMovie que contiene al gif del menú para evitar problemas de memoria.
    close();
}

void Menu_partida::on_cargar_partida_clicked()
{
    /*Botón en el que se implementara la función que nos permitirá leer los datos guardados del
     jugador y así generar una partida exactamente igual a como se guardó.*/
}
