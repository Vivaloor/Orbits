#include <SFML/Graphics.hpp>
#include <string>
#include <math.h>

int main()
{
    // Create the main window
    sf::RenderWindow app(sf::VideoMode(1000, 1000), "Orbity");

    sf::CircleShape planeta;
    planeta.setRadius(40);
    planeta.setFillColor(sf::Color::Blue);
    planeta.setPosition(460, 460);

    sf::CircleShape satelita;
    satelita.setRadius(10);
    satelita.setFillColor(sf::Color::White);

    float x = 0, y = -250;
    float Vx = 0.06324555320336758663997787088865, Vy = 0;
    float r;

    const float C = 1;

    sf::Font font;
    font.loadFromFile("LucidaSansRegular.ttf");
    sf::Text T("hello", font);

    sf::VertexArray slady(sf::Points, 100000);
    int i = 0;


	// Start the game loop
    while (app.isOpen())
    {
        // Process events
        sf::Event event;
        while (app.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                app.close();
        }

        // Clear screen
        app.clear();

        x = x + Vx;
        y = y + Vy;

        r = sqrt((x)*(x)+(y)*(y));

        Vx = Vx + x/r * -1 * C / r / r;

        Vy = Vy + y/r * -1 * C / r / r;

        //if(y  > 225)
        {
            Vx = Vx + x/r * -1 * 0.001;
            Vy = Vy + y/r * -1 * 0.001;

            //Vx = Vx + x/r * -1 * 0.01 / r;
            //Vy = Vy + y/r * -1 * 0.01 / r;
        }

        slady[i].position = sf::Vector2f(x+520, y+520);
        slady[i].color = sf::Color::Red;
        i++;
        if(i==100000) i = 0;


        T.setString(std::to_string(r));

        satelita.setPosition(x + 510, y  + 510);

        app.draw(planeta);

        app.draw(slady);

        app.draw(satelita);
        app.draw(T);

        // Update the window
        app.display();
    }

    return EXIT_SUCCESS;
}
