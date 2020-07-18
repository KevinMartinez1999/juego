#include "jugador.h"
#include "mapa_gameplay.h"

extern Muro *muro;

Jugador::Jugador(QObject *parent) : QObject(parent)
{
    //Inicialzacion de las banderas de movimiento
    banLeft = false;
    banRight = false;
    banUp = false;
    banDown = false;
    banAttack = false;
    ultimoEstado = 1;
    posAnterior = QPoint(0,0);

    //Ancho y alto del sprite del jugador (inicialización de variables para el sprite)
    ancho = 84;
    alto  = 84;
    columnas = 0;
    fila = 0;

    // Se crea el timer que va a estar asociado al movimiento del jugador
    connect(&timer1, SIGNAL(timeout()), this, SLOT(moveLeft()));
    connect(&timer1, SIGNAL(timeout()), this, SLOT(moveRight()));
    connect(&timer1, SIGNAL(timeout()), this, SLOT(moveUp()));
    connect(&timer1, SIGNAL(timeout()), this, SLOT(moveDown()));
    connect(&timer1, SIGNAL(timeout()), this, SLOT(Attack()));
    connect(&timer1, SIGNAL(timeout()), this, SLOT(pos()));
    timer1.start(30);

    //Timer para las actualización y dibujo del sprite.
    /*Este timer nos permitira la constante actualizacion de la imagen de nuestro jugador*/
    connect(&timer,SIGNAL(timeout()),this,SLOT(Actualizacion()));
    timer.start(200);

    box.setRect(0,0,25,25);
    box.setPos(755,2167);
}

QRectF Jugador::boundingRect() const
{
    /*La funcion propia de qt bodingRect, crea y retorna el rectangulo
     que conforma la figura del jugador y en el cual pintaremos nuestra
     imagen. -ancho/2,-alto/2 permite que el punto de origen del rectangulo
     siempre sea le centro de la imagen y no la esquina superior.*/
    return QRectF(-ancho/2,-alto/2,ancho,alto);
}

void Jugador::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    /*Funcion propia de Qt que nos permite dibujar dentro del boundingRect la imagen
     que queramos, para hacer esto debemos introducirle al drawPixmap algunos datos como el
     punto en el que queremos que empiece a dibujar, el pixmap a dibujar, el ancho y alto de
     lo que se dibujara y tambien, se le pasara constantemente en la funcion actualizar la columna
     que representara el cuadro 84x84 que se dibujara en el momento. La variable fila representa
     el grupo de frames que se quiere realizar dependiendo a las acciones del
     jugador, esta variable cambia cuando el usuario activa un KeyEvent.*/
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawPixmap(-ancho/2,-alto/2,pixmap,columnas,fila,ancho,alto);
}

void Jugador::Actualizacion()
{
    /*La imagen sprite del jugador es una imagen que estaba dividida por filas y por columnas, cada fila
     determina un movimiento o accion diferente hecha por el jugador, y las columnas son frames que
     permiten que esa accion se vea con movimiento, entonces mediante un timer estaremos constantemente
     interactuando en las columnas de determinada fila para asi ir generando una animacion fluida y continua.*/
    if(columnas >= 336 or (fila >= 672 and columnas >= 168))
        //El archivo consta de 6 columnas de 84x84, cuando se llegue a la sexta columna se iniciara de nuevo
    {
        columnas = 84;
    }
    else{
        columnas += 84;
    }
    /*La funcion update constantemente actualiza el boundingRect del jugador para que su
     origen siempre sea la mitad de la imagen actual.*/
    this->update(-ancho/2,-alto/2,ancho,alto);
}

//Las siguientes son las señales de movimiento que funcionan con un timer;
//El jugador únicamente se va a mover cuando se respectiva bandera esté activada, por ejemplo,
//para moverse a la izquierda, banLeft debe ser true.

//una vez se verifica que la respectiva bandera esté activa se le da un valor a Fila que es donde
//debe ubicarse en la imagen SPRITEPLAYER.png para hacer las animaciones correctamente.

// Las definiciones de cada función son similares en el resto de funciones de movimiento;
// cambian en los limites de la escena donde el jugador puede moverse en la funcion setPos()
// que se le suma o se le resta dependiendo de hacia donde es el movimiento.

void Jugador::moveLeft()
{

    if (banLeft)
    {
        ultimoEstado = 2;
        fila = 420; //Actualiza el sprite
        if(x()>42){ //Condiciones del borde de las escena
            setPos(x()-5,y()); //Movimiento del jugador
            box.setPos(x()-20,y()+12); //Movimiento del hiteBox que colisiona
            if (box.collidesWithItem(muro)){ //Verifica la colision

                //En este punto con el fin de no intersectar al jugador con los objetos del mapa
                //lo que se hace es retroceder al jugador y a su vez a las caja que lo sigue una
                //vez esta colisiona con alguna part del mapa.

                setPos(x()+5,y());;
            }
        }
    }
}

//Las definiciones son similares para el resto de las funciones

void Jugador::moveRight()
{
    if (banRight)
    {
        ultimoEstado = 4;
        fila = 504;//Actualiza el sprite
        if(x()<2197){//Condiciones del borde de las escena
            setPos(x()+5,y());//Movimiento del jugador
            box.setPos(x()-10,y()+12);//Movimiento del hiteBox que colisiona
            if (box.collidesWithItem(muro)){//Verifica la colision
                //En este punto con el fin de no intersectar al jugador con los objetos del mapa
                //lo que se hace es retroceder al jugador y a su vez a las caja que lo sigue una
                //vez esta colisiona con alguna part del mapa.
                setPos(x()-5,y());
            }
        }
    }
}

void Jugador::moveUp()
{
    if (banUp)
    {
        ultimoEstado = 3;
        fila = 588;//Actualiza el sprite
        if(y() > 42){//Condiciones del borde de las escena
            setPos(x(),y()-5);//Movimiento del jugador
            box.setPos(x()-15,y()+7);//Movimiento del hiteBox que colisiona
            if (box.collidesWithItem(muro)){//Verifica la colision
                //En este punto con el fin de no intersectar al jugador con los objetos del mapa
                //lo que se hace es retroceder al jugador y a su vez a las caja que lo sigue una
                //vez esta colisiona con alguna part del mapa.
                setPos(x(),y()+5);
            }
        }
    }
}

void Jugador::moveDown()
{
    if (banDown)
    {
        ultimoEstado = 1;
        fila = 336;//Actualiza el sprite
        if(y()<2193){//Condiciones del borde de las escena
            setPos(x(),y()+5);//Movimiento del hiteBox que colisiona
            box.setPos(x()-15,y()+17);
            if (box.collidesWithItem(muro)){//Verifica la colision
                //En este punto con el fin de no intersectar al jugador con los objetos del mapa
                //lo que se hace es retroceder al jugador y a su vez a las caja que lo sigue una
                //vez esta colisiona con alguna part del mapa.
                setPos(x(),y()-5);
            }
        }
    }
}

//Dependiendo de la posicion anterior el jugador va a hacer una animacion de ataque
void Jugador::Attack()
{
    if (banAttack){
        if (fila != 672 and fila != 840 and fila != 756 and fila != 924)
            columnas = 0; /*Se reinicia columnas ya que puede estar en valor distinto de 0 y por ende la animacion
                            se va a mostrar mal. Esto solo pasa una vez antes de empezar la animacion de la espada.*/
        switch (ultimoEstado) {
        case 1: //abajo
            fila = 672;
            break;
        case 2: //izquierda
            fila = 840;
            break;
        case 3: //arriba
            fila = 756;
            break;
        case 4: //derecha
            fila = 924;
            break;
        default:
            break;
        }
    }
}

/*Esta función determina la posicion anterior del personaje para saber si el jugador está quieto*/
void Jugador::pos()
{
    if (banAttack) //Cuando este atacando no debe hacer la animacion de estar quieto
        return;
    if (posAnterior == QPoint(x(),y())){ //Si se cumple es porque el jugador está quieto
        switch (ultimoEstado) {
        case 1:
            fila=0;
            break;
        case 2:
            fila=168;
            break;
        case 3:
            fila=84;
            break;
        case 4:
            fila=252;
            break;
        default:
            break;
        }
    }
    posAnterior = QPoint(x(),y());
}
