#include "Nodo.h"


class QuadTree
{
private:
    /* data */
    Nodo* raiz;
    sf::RenderWindow* window;
    std::vector<sf::CircleShape*> objetos;
    sf::CircleShape* objeto;
public:
    QuadTree(/* args */);
    void Update();
    ~QuadTree();
};

QuadTree::QuadTree(/* args */)
{
    window=new sf::RenderWindow(sf::VideoMode(600,600),"QuadTree");

    this->raiz=new Nodo({0,0},{600,600});
}

void QuadTree::Update()
{
    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
            if (event.type == sf::Event::MouseButtonPressed)
            {
                this->objeto=new sf::CircleShape(3);
                sf::Vector2i pos=sf::Mouse::getPosition(*window);
                std::cout<<pos.x<<" - "<<pos.y<<std::endl;
                this->objeto->setOrigin(3,3);
                this->objeto->setPosition(pos.x,pos.y);
                this->objeto->setFillColor(sf::Color::Blue);
                this->objetos.push_back(objeto);                
                this->raiz->Insertar(objeto);
                //window->draw(*objeto);
            }
        }
        window->clear();
        raiz->Update(window);
        
    
        window->display();
    }
}

QuadTree::~QuadTree()
{
}
