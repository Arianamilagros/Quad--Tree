#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <stdlib.h>  
class Nodo
{
private:
    /* data */
    sf::RectangleShape* cuadrado;
    Nodo* hijos[4];
    float ancho;
    float altura;
public:
    std::vector<float> p1;
    std::vector<float> p2;
    std::vector<sf::CircleShape*> objetos;
    Nodo(/* args */);
    Nodo(std::vector<float>, std::vector<float> );
    void Update(sf::RenderWindow* &);
    void Insertar(sf::CircleShape*& );
    Nodo* Encontrar(sf::CircleShape*& );
    void Dividir();
    void impHijos();
    bool tieneHijos();
    ~Nodo();
};

Nodo::Nodo(/* args */)
{
    for (auto& i: this->hijos)
        i=0;
}



Nodo::Nodo(std::vector<float> p1, std::vector<float> p2)
{
    for (auto& i: this->hijos)
        i=0;
    this->p1=p1;
    this->p2=p2;
    this->ancho=abs(p2[0]-p1[0]);
    this->altura=abs(p2[1]-p1[1]);
    this->cuadrado=new sf::RectangleShape(sf::Vector2f(ancho-2,altura-2));
    this->cuadrado->setPosition(p1[0]+1,p1[1]+1);
    this->cuadrado->setFillColor(sf::Color::Transparent);
    this->cuadrado->setOutlineColor(sf::Color::Red);
    this->cuadrado->setOutlineThickness(1);
    std::cout<<ancho<<" -creando- "<<altura<<std::endl;

}



Nodo::~Nodo()
{
}


void Nodo::Update(sf::RenderWindow * & window)
{
    if (!this->tieneHijos())
    {
        //std::cout<<"hoja"<<std::endl;
        window->draw(*(this->cuadrado));
        for (int i=0;i<this->objetos.size();i++)
            window->draw(*(this->objetos[i]));
    }
    else
    {
        hijos[0]->Update(window);
        hijos[1]->Update(window);
        hijos[2]->Update(window);
        hijos[3]->Update(window);

    }
}

void Nodo::impHijos()
{
    std::cout<<hijos[0]<<std::endl;
    std::cout<<hijos[1]<<std::endl;
    std::cout<<hijos[2]<<std::endl;
    std::cout<<hijos[3]<<std::endl;
}

bool Nodo::tieneHijos()
{
    //std::cout<<"tiene? "<<hijos[0]<<std::endl;
    if (hijos[0])
        return true;
    return false;
}

Nodo* Nodo::Encontrar(sf::CircleShape*& objeto)
{
    if (!this->tieneHijos())
    {
        std::cout<<"encontro "<<this<<std::endl;
        if (objeto->getPosition().x > this->p1[0] && 
            objeto->getPosition().x < this->p2[0] &&
            objeto->getPosition().y > this->p1[1] && 
            objeto->getPosition().y < this->p2[1])
        {
            return this;
        }
        return 0;
    }
    else
    {
        for (int i=0;i<=3;i++)
        {
            Nodo* res= hijos[i]->Encontrar(objeto);
            if (res)
                return res;
        }
    }

}

void Nodo::Dividir()
{
    if (this->tieneHijos())
        return;
    //std::cout<<"dividio "<<hijos[0]<<std::endl;
    std::vector<float> centro=   {this->p1[0]+ancho/2,this->p1[1]+altura/2};
    std::vector<float> derecha=  {this->p2[0],this->p1[1]+altura/2};
    std::vector<float> izquierda={this->p1[0],this->p1[1]+altura/2};
    std::vector<float> abajo=    {this->p1[0]+ancho/2,this->p2[1]};
    std::vector<float> arriba=   {this->p1[0]+ancho/2,this->p1[1]};
    this->hijos[0]=new Nodo(this->p1,centro);
    this->hijos[1]=new Nodo(arriba,derecha);
    this->hijos[2]=new Nodo(izquierda,abajo);
    this->hijos[3]=new Nodo(centro,this->p2);
    for (int i=0;i<this->objetos.size();i++)
    {
        this->Insertar(objetos[i]);
    }

}

void Nodo::Insertar(sf::CircleShape*& objeto)
{
    // if (!this->tieneHijos())
    //     this->objetos.push_back(objeto);

    Nodo* cuadrante=this->Encontrar(objeto);
    this->impHijos();
    //std::cout<<"encontro "<<cuadrante<<" actual: "<<this<<std::endl;
    if (cuadrante){
        cuadrante->objetos.push_back(objeto);
        if (cuadrante->objetos.size()==3)
        {
            cuadrante->Dividir();
        }
    }
    else
        std::cout<<"que fue?"<<std::endl;
}