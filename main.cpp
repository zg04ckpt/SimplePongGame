#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <cmath>
#include <iostream>
#include <vector>

sf::RenderWindow window;
sf::RectangleShape leftPaddle;
sf::RectangleShape rightPaddle;
sf::CircleShape ball;
sf::Music bgMusic;
std::vector<sf::Sound> sounds;
sf::Sound gameover;
sf::SoundBuffer sb1, sb2, sb3, sbGameover;

const double PI = 3.14159265;

const std::string TITLE = "PONG GAME";
const sf::Color BACKGROUND = sf::Color::Cyan;

const int SCREEN_HEIGHT = 600;
const int SCREEN_WIDTH = 800;
const int SCREEN_BPP = 32;

const int PADDLE_HEIGHT = 80;
const float PADDLE_WIDTH = 10.f;
const float PADDLE_SPEED = 250.f;
const sf::Color OUTLINE_PADDLE_COLOR = sf::Color::Black;
const sf::Color LEFT_PADDLE_COLOR = sf::Color::Red;
const sf::Color RIGHT_PADDLE_COLOR = sf::Color::Blue;
const sf::Color PADDLE_OUTLINE_COLOR = sf::Color::White;
const int PADDLE_OUTLINE_WIDTH = 2;

const float BALL_RADIOUS = 8.f;
const float BALL_SPEED = 300.f;
const sf::Color BALL_COLOR = sf::Color::Yellow;
const sf::Color BALL_OUTLINE_COLOR = sf::Color::Cyan;
const int BALL_OUTLINE_WIDTH = 2;

float ballSpeed = BALL_SPEED;
float paddleSpeed = PADDLE_SPEED;
float angle = 30.f;

sf::RectangleShape InitPaddle()
{
    sf::RectangleShape paddle(sf::Vector2f(PADDLE_WIDTH, PADDLE_HEIGHT));
    paddle.setOrigin(PADDLE_WIDTH/2.0, PADDLE_HEIGHT/2.0);
    paddle.setOutlineThickness(PADDLE_OUTLINE_WIDTH);
    paddle.setOutlineColor(PADDLE_OUTLINE_COLOR);
    return paddle;
}

sf::CircleShape InitBall()
{
    sf::CircleShape ball(BALL_RADIOUS);
    ball.setOrigin(BALL_RADIOUS, BALL_RADIOUS);
    ball.setFillColor(BALL_COLOR);
    ball.setOutlineThickness(BALL_OUTLINE_WIDTH);
    ball.setOutlineColor(BALL_OUTLINE_COLOR);
    return ball;
}

bool InitMusic()
{
    //"D:\Code\Game\C++\Data\Music\bg-music.wav"
    if (bgMusic.openFromFile("../Data/Music/bg-music.wav")) return true;
    return false;
}

bool InitSound()
{
    if (!sb1.loadFromFile("../Data/Music/sound1.wav")) return false;
    if (!sb2.loadFromFile("../Data/Music/sound2.wav")) return false;
    if (!sb3.loadFromFile("../Data/Music/sound3.wav")) return false;
    if (!sbGameover.loadFromFile("../Data/Music/gameover.wav")) return false;

    sf::Sound sound;
    sound.setBuffer(sb1);
    sounds.push_back(sound);
    sound.setBuffer(sb2);
    sounds.push_back(sound);
    sound.setBuffer(sb3);
    sounds.push_back(sound);
    gameover.setBuffer(sbGameover);
    return true;
}

void reset()
{
    ball.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);
    leftPaddle.setPosition(PADDLE_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);
    rightPaddle.setPosition(SCREEN_WIDTH - PADDLE_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);
    ballSpeed = BALL_SPEED;
    paddleSpeed = PADDLE_SPEED;
}



int main()
{
    window.create(
        sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP),
        TITLE,
        sf::Style::Default
    );

    leftPaddle = InitPaddle();
    leftPaddle.setFillColor(LEFT_PADDLE_COLOR);
    leftPaddle.setPosition(PADDLE_WIDTH/2.f, SCREEN_HEIGHT / 2.f);

    rightPaddle = InitPaddle();
    rightPaddle.setFillColor(RIGHT_PADDLE_COLOR);
    rightPaddle.setPosition(SCREEN_WIDTH- PADDLE_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);

    ball = InitBall();
    ball.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);

    if (!InitMusic()) window.close();
    if (!InitSound()) window.close();
    srand(time(0));

    sf::Font font;
    sf::Text beginText;
    if (!font.loadFromFile("../Data/Font/test.otf"))
    {
        window.close();
    }
    else
    {
        beginText.setString("Press space to play!");
        beginText.setPosition(SCREEN_WIDTH / 2.f-220, SCREEN_HEIGHT / 2.f-15);
        beginText.setFillColor(sf::Color::Yellow);
        beginText.setCharacterSize(30);
        beginText.setOrigin(0, 0);
        beginText.setFont(font);
    }

    //điểm
    int score_i = 0;
    sf::Text scoreText;
    scoreText.setCharacterSize(20);
    scoreText.setPosition(SCREEN_WIDTH / 2.f - 20, 0);
    scoreText.setString("Score:" + std::to_string(score_i));
    scoreText.setFont(font);

    /*sf::Text score_s;
    score_s.setCharacterSize(20);
    score_s.setPosition(SCREEN_WIDTH / 2.f + 20, 0);
    score_s.setString(score_i+"");
    score_s.setFont(font);*/

    bool isPlaying = false;
    
    angle = rand()%91 - 45;
    sf::Clock clock;
    float timeLoadLoop;
    bgMusic.setLoop(true);
    bgMusic.play();
    while (window.isOpen())
    {
        timeLoadLoop = clock.restart().asSeconds();
        //kiểm tra tất cả các sự kiện của cửa sổ đã được kích hoạt kể từ lần lặp cuối cùng của vòng lặp
        sf::Event event;
        //mỗi vòng lặp lấy 1 sự kiện trong hàng đợi và gán vào event
        while (window.pollEvent(event))
        {
            //sự kiện đóng
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear();

        if (!isPlaying && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) isPlaying = true;

        if (!isPlaying)
        {
            window.draw(beginText);
            window.display();
            continue;
        }

        float top_bound_left = leftPaddle.getPosition().y - PADDLE_HEIGHT / 2.f;
        float top_bound_right = rightPaddle.getPosition().y - PADDLE_HEIGHT / 2.f;

        //left event-------------------------------------------------------------------
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && top_bound_left > 0)
        {
            leftPaddle.move(0, -timeLoadLoop * paddleSpeed);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && (top_bound_left + PADDLE_HEIGHT) < SCREEN_HEIGHT)
        {
            leftPaddle.move(0, +timeLoadLoop * paddleSpeed);
        }

        //right event --------------------------------------------------------------------------
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && (top_bound_right + PADDLE_HEIGHT) < SCREEN_HEIGHT)
        {
            rightPaddle.move(0, +timeLoadLoop * paddleSpeed);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && top_bound_right > 0)
        {
            rightPaddle.move(0, -timeLoadLoop * paddleSpeed);
        }

        //ball event-----------------------------------------------------------------------
        if (
            ball.getPosition().x + BALL_RADIOUS > SCREEN_WIDTH ||
            ball.getPosition().x - BALL_RADIOUS < 0
            )
        {
            gameover.play();
            isPlaying = false;
            beginText.setString("Your score:" + std::to_string(score_i) + "\nPress space to replay!");
            beginText.setPosition(SCREEN_WIDTH / 2.f - 220, SCREEN_HEIGHT / 2.f);
            score_i = 0;
            angle = rand() % 91 - 45;
            reset();
            continue;
        }
        float realBallSpeed = ballSpeed * timeLoadLoop;
        if (
            //cần xử lí điều kiện va chạm
            ball.getPosition().x + BALL_RADIOUS >= SCREEN_WIDTH - PADDLE_WIDTH && abs(ball.getPosition().y - rightPaddle.getPosition().y) <= PADDLE_HEIGHT/2.f ||
            ball.getPosition().x - BALL_RADIOUS <= PADDLE_WIDTH && abs(ball.getPosition().y - leftPaddle.getPosition().y) <= PADDLE_HEIGHT/2.f
            )
        {
            angle = 180.f - angle;
            ballSpeed *= 1.1;
            paddleSpeed *= 1.1;
            score_i++;
            ball.move(realBallSpeed* cos(angle* PI / 180), -realBallSpeed * sin(angle * PI / 180));
            ball.move(realBallSpeed* cos(angle* PI / 180), -realBallSpeed * sin(angle * PI / 180));
            ball.move(realBallSpeed* cos(angle* PI / 180), -realBallSpeed * sin(angle * PI / 180));

            //kích hoạt sound ngẫu nhiên
            int idx = rand() % 3;
            sounds[idx].play();
        }

        if (
            ball.getPosition().y + BALL_RADIOUS >= SCREEN_HEIGHT ||
            ball.getPosition().y - BALL_RADIOUS <= 1
        ) 
        {
            angle = -angle;
        }
        
        ball.move(realBallSpeed * cos(angle * PI / 180), -realBallSpeed * sin(angle * PI / 180));
       
        scoreText.setString("Score:" + std::to_string(score_i));
        window.draw(scoreText);
        window.draw(leftPaddle);
        window.draw(ball);
        window.draw(rightPaddle);
        window.display();
    }
    return 0;
}