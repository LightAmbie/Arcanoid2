#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<time.h>
#include<string>
using namespace sf;
using namespace std;

double randValue() {
    int sign = rand() % 2;
    if (sign == 1)
    {
        return rand() % 5 + 3;
    }
    else
    {
        return -(rand() % 5 + 3);
    }
}


void fillBlocks(RectangleShape* block) {
    int n = 0;
    for (int i = 0; i < 11; i++) {
        Color color(0,0,0);
        for (int j = 0; j < 11; j++)
        {
            if (i == 0 || j == 0 || j == 10) { color.r = 128; color.b = 128; color.g = 128; }
            else { color.r = 23 * (i + 1); color.g = 0; color.b = 0;}
            block[n].setFillColor(color);
            block[n].setSize(Vector2f(50, 20));
            block[n].setPosition(225 + j * 51, 50 + i * 21);
            n++;
        }
    }
}

void fillLife(Sprite* life, int lifeCnt) {
    for (int i = 0; i < lifeCnt; i++)
    {
        life[i].setPosition(50, 100 + (i * 30 * 2.5));
    }
}

int main()
{

    srand(time(0));

    const int gameWidth = 1000;
    const int gameHeight = 800;
    int const lifeCnt = 3;
    const int blockCnt = 121;

    RenderWindow window(VideoMode(gameWidth, gameHeight), "Arcanoid!",Style::Default);
    window.setFramerateLimit(60);

    SoundBuffer blockCollBuff, collBuff, damageBuff;
    if (!blockCollBuff.loadFromFile("resources/classic.wav"))
        return EXIT_FAILURE;
    if (!collBuff.loadFromFile("resources/boom.wav"))
        return EXIT_FAILURE;
    if (!damageBuff.loadFromFile("resources/bruh.wav"))
        return EXIT_FAILURE;
    Sound blockCollSound(blockCollBuff);
    Sound collSound(collBuff);
    Sound damageSound(damageBuff);

    Font font;
    if (!font.loadFromFile("resources/sansation.ttf"))
        return EXIT_FAILURE;

    Text pauseMessage;
    pauseMessage.setFont(font);
    pauseMessage.setCharacterSize(40);
    pauseMessage.setPosition(gameWidth/2-200, gameHeight/2);
    pauseMessage.setFillColor(sf::Color::White);
    pauseMessage.setString("Welcome to Arcanoid game!\nPress spacebar to start");

    Texture t1, t2;
    t1.loadFromFile("resources/ball.png");
    t2.loadFromFile("resources/heart.png");
    Sprite sBall(t1);

    Sprite life[lifeCnt];
    for (int i = 0; i < lifeCnt; i++)
    {
        life[i].setTexture(t2);
    }
    fillLife(life, lifeCnt);

    RectangleShape inter1;
    RectangleShape inter2;

    inter1.setSize(Vector2f(150, gameHeight));
    inter1.setFillColor(Color::Blue);
    inter1.setPosition(0, 0);
    inter2.setSize(Vector2f(150, gameHeight));
    inter2.setFillColor(Color::Blue);
    inter2.setPosition(850, 0);

    Text lifeText, scoreText, scoreValue;

    lifeText.setFont(font);
    lifeText.setCharacterSize(30);
    lifeText.setPosition(40, 30);
    lifeText.setString("Lifes:");

    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setPosition(890, 30);
    scoreText.setString("Score:");

    scoreValue.setFont(font);
    scoreValue.setCharacterSize(30);
    scoreValue.setPosition(910, 80);
    scoreValue.setString(to_string(0));

    RectangleShape sPaddle;
    sPaddle.setSize(Vector2f(100, 10));
    sPaddle.setFillColor(Color::Red);
    sPaddle.setPosition(gameWidth/2, gameHeight - 10);

    

    RectangleShape block[1000];
    fillBlocks(block);
    

    double dx = randValue(), dy = randValue();
    double x = gameWidth/2, y = gameHeight/2;
    bool isPlaying = false;
    int currentLifeCnt = lifeCnt;
    int score = 0;
    while (window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
        }

        if (isPlaying)
        {
            if (!currentLifeCnt || Keyboard::isKeyPressed(Keyboard::L))
            {
                pauseMessage.setString("You lost!\nPress spacebar to try again");
                isPlaying = false;
            }

            if (score == 100 * 90 || Keyboard::isKeyPressed(Keyboard::W)) {
                isPlaying = false;
                pauseMessage.setString("You won!\nPress spacebar to try again");
            }
            if (Keyboard::isKeyPressed(Keyboard::G)) {
                x = gameWidth / 2; y = gameHeight / 2;
                dx = randValue(); dy = randValue();
            }

            x += dx;
            for (int i = 0; i < blockCnt; i++)
                if (FloatRect(x + 3, y + 3, 6, 6).intersects(block[i].getGlobalBounds()))
                {
                    if (!(block[i].getFillColor().b > 0))
                    {
                        blockCollSound.play();
                        block[i].setPosition(-100, 0);
                        score += 100;
                        scoreValue.setString(to_string(score));
                    }
                    dx = -dx;
                }

            y += dy;
            for (int i = 0; i < blockCnt; i++)
                if (FloatRect(x + 3, y + 3, 6, 6).intersects(block[i].getGlobalBounds()))
                {
                    if (!(block[i].getFillColor().b > 0))
                    {
                        blockCollSound.play();
                        block[i].setPosition(-100, 0);
                        score += 100;
                        scoreValue.setString(to_string(score));
                    }
                    dy = -dy;
                }

            if (Keyboard::isKeyPressed(Keyboard::Right) && sPaddle.getPosition().x < 750) sPaddle.move(7, 0);
            if (Keyboard::isKeyPressed(Keyboard::Left) && sPaddle.getPosition().x > 150) sPaddle.move(-7, 0);

            if (FloatRect(x, y, 12, 12).intersects(sPaddle.getGlobalBounds())) {dy = -(rand() % 5 + 2); collSound.play();};
            if (x < 150 || x>840) {dx = -dx; collSound.play();}
            if (y < 0 || y>gameHeight)  dy = -dy;
            if (y > gameHeight) {
                damageSound.play();
                currentLifeCnt--;
                x = gameWidth/2; y = gameHeight/2;
            }

            sBall.setPosition(x, y);
            
        }
        else
        {
            if (Keyboard::isKeyPressed(Keyboard::Space)) isPlaying = true;
            x = gameWidth/2; y = gameHeight/2;
            dx = randValue(); dy = randValue();
            score = 0;
            scoreValue.setString(to_string(0));
            fillBlocks(block);
            fillLife(life, lifeCnt);
            currentLifeCnt = lifeCnt;
        }

        window.clear();
        if (isPlaying)
        {
            window.draw(sBall);
            window.draw(sPaddle);

            for (int i = 0; i < blockCnt; i++)
                window.draw(block[i]);

            window.draw(inter1);
            window.draw(inter2);
            for (int i = 0; i < currentLifeCnt; i++)
            {
                window.draw(life[i]);
            }
            window.draw(scoreText);
            window.draw(scoreValue);
            window.draw(lifeText);  
        }
        else {
            window.draw(pauseMessage);
        }


        window.display();

    }

    return 0;
}