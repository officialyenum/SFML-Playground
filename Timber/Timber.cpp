// Timber.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <array>
#include <sstream>      // std::stringstream
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <deque>

using namespace sf;
using namespace std;


const int NUM_BRANCHES = 6;
const int NUM_CLOUDS = 3;
//Player Location
enum class side { LEFT, RIGHT, NONE };

vector<Sprite> branches;
vector<side> branchPositions(NUM_BRANCHES, side::NONE);
bool showFPS{ false };


//Function Declaration
void updateBranches(int seed)
{
    cout << "-----Start-----" << "\n\n";
    
    // move all branches down one place 
    for (int j = NUM_BRANCHES - 1; j > 0; j--)
    {
        branchPositions[j] = branchPositions[j - 1];
    }


    srand((int)time(0) + seed);
    int r = (rand() % 5);
    cout << "Random number is" << r << "\n";
    switch (r)
    {
    case 0:
        branchPositions[0] = side::LEFT;
        break;
    case 1:
        branchPositions[0] = side::RIGHT;
        break;
    default:
        branchPositions[0] = side::NONE;
        break;
    }
    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        cout << static_cast<int>(branchPositions[i]) << "\n";
    }
    cout << "-----End-----" << "\n\n";
};


int main()
{
    VideoMode vm({ 1920, 1080 });
    RenderWindow window(vm, "Timber !!!", State::Fullscreen);

    //Make Background
    Texture textureBg("graphics/background.png");//Load Texture from file path
    Sprite spriteBg(textureBg); // Create Background Sprite using loaded texture
    spriteBg.setPosition({ 0,0 }); // Set position


    //Make Tree
    Texture textureTree("graphics/tree.png");//Load Texture from file path
    Sprite spriteTree(textureTree); // Create Tree Sprite using loaded texture
    spriteTree.setPosition({ 810,0 }); // Set position


    //Make Bee
    Texture textureBee("graphics/bee.png");//Load Texture from file path
    Sprite spriteBee(textureBee); // Create Bee Sprite using loaded texture
    spriteBee.setPosition({ 0,810 }); // Set position
    bool bBeeActive{ false };
    float beeSpeed{ 0.0f };

    //Prepare Three Clouds
    vector<Sprite> clouds;
    clouds.reserve(NUM_CLOUDS);
    vector<bool> cloudsActive;
    cloudsActive.resize(NUM_CLOUDS, false);
    vector<float> cloudSpeed;
    cloudSpeed.resize(NUM_CLOUDS, 0.0f);
    Texture textureCloud("graphics/cloud.png");

    for (int i = 0; i < NUM_CLOUDS; i++)
    {
        
        //Set the texture for each cloud sprite
        clouds.emplace_back(textureCloud);  // construct Sprite with texture
        clouds[i].setPosition({ 0, i * 150.0f });
    }
    
    // Game Parameters
    // Score
    Font font("fonts/KOMIKAP_.ttf");
    Text scoreText(font, "Score = 0", 75);
    scoreText.setFillColor(Color::White);
    int score = 0;

    FloatRect scoreTextBounds = scoreText.getLocalBounds();
    RectangleShape scoreBackground({ scoreTextBounds.size.x + 40, scoreTextBounds.size.y + 40 });
    scoreBackground.setFillColor(Color::Black);
    scoreBackground.setPosition({ 20, 20 });
    scoreText.setPosition({
        scoreBackground.getPosition().x + 10,
        scoreBackground.getPosition().y + 10 });

    // FPS 
    Text fpsText(font, "FPS = 120.0", 75);
    fpsText.setFillColor(Color::White);
    FloatRect fpsTextBounds = fpsText.getLocalBounds();
    RectangleShape fpsBackground({ fpsTextBounds.size.x + 40, fpsTextBounds.size.y + 40 });
    if (showFPS)
    {
        fpsBackground.setFillColor(Color::Black);
        fpsBackground.setPosition({
            1920.0f - fpsTextBounds.size.x - 20, 
            20});
        fpsText.setPosition({
            fpsBackground.getPosition().x + 10 - fpsTextBounds.position.x, 
            fpsBackground.getPosition().y + 10
            });
    }


    // Message
    Text messageText(font, "Press Enter To Start!", 100);
    messageText.setFillColor(Color::White);

    FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin({ textRect.getCenter()});
    messageText.setPosition({ 1920 / 2.0f, 1080 / 2.0f });


    //Prepare 5 branches
    vector<Sprite> branches;
    branches.reserve(NUM_BRANCHES);
    Texture textureBranch("graphics/branch.png");

    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        //Set the texture for each branch sprite
        branches.emplace_back(textureBranch);  // construct Sprite with texture
        branches[i].setPosition({ -2000, -2000 });
        branches[i].setOrigin({ 220, 20 });
    }
    
    // Time
    Clock clock; 
    const int SAMPLE_COUNT = 100;               // average over last 100 frames
    std::deque<float> frameTimes;               // store deltaTimes
    float fps = 0.0f;

    // Time Bar
    RectangleShape timeBar;
    float timeBarStartWidth{ 400 };
    float timeBarHeight{ 80 };
    
    timeBar.setSize({ timeBarStartWidth, timeBarHeight });
    timeBar.setFillColor(Color::Red);
    timeBar.setPosition({ (1920 / 2) - timeBarStartWidth / 2, 980 });
    Time gameTimeTotal;
    float timeRemaining{ 6.0f };
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    bool bPaused{ true };

    // Generate Random Branches
    updateBranches(1);
    updateBranches(2);
    updateBranches(3);
    updateBranches(4);
    updateBranches(5);
    updateBranches(6);

    // Prepare the Player
    Texture texturePlayer("graphics/player.png");
    Sprite spritePlayer(texturePlayer);
    spritePlayer.setPosition({ 580, 720 });

    //The Player should start on the left
    side playerSide = side::LEFT;

    // Prepare the gravestone
    Texture textureRIP("graphics/rip.png");
    Sprite spriteRIP(textureRIP);
    spriteRIP.setPosition({ 600, 860 });

    // Prepare the Axe
    Texture textureAxe("graphics/axe.png");
    Sprite spriteAxe(textureAxe);
    spriteAxe.setPosition({ 700, 830 });
    //line Axe with tree
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    //Prepare the flying log
    Texture textureLog("graphics/log.png");
    Sprite spriteLog(textureLog);
    spriteLog.setPosition({ 810, 720 });
    bool bLogActive{ false };
    float logSpeedX{ 1000 };
    float logSpeedY{ -1500 };

    //Player Control input
    bool bAcceptInput{ false };

    // Sound Declaration
    // Chop Sound
    SoundBuffer chopBuffer("sound/chop.wav"); 
    Sound chopSound(chopBuffer);
    // Death Sound
    SoundBuffer deathBuffer("sound/death.wav");
    Sound deathSound(deathBuffer);
    // Out of Time Sound
    SoundBuffer outOfTimeBuffer("sound/out_of_time.wav");
    Sound outOfTimeSound(outOfTimeBuffer);

    while (window.isOpen())
    {
        // --- Smoothed FPS ---
        float dt = clock.restart().asSeconds();
        if (!bPaused)
        {
            // Measure Time
            // Subtract from  time remaining
            timeRemaining -= dt;
            // size up time bar
            timeBar.setSize({ timeBarWidthPerSecond * timeRemaining, timeBarHeight });

            if (timeRemaining <= 0.f)
            {
                //Pause Game
                bPaused = true;
                // change message to out of time
                messageText.setString("Out Of Time!!");


                //Reposition the text based on its new size
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.getCenter());
                messageText.setPosition({ 1920 / 2, 1080 / 2 });
                //Play Out Of Time Sound
                outOfTimeSound.play();
            }
            if (!bBeeActive)
            {
                // How Fast is the bee?
                srand((int)time(0));
                beeSpeed = static_cast<float>((rand() % 200) + 200);

                // How high is the bee?
                srand((int)time(0));
                float height = static_cast<float>((rand() % 500) + 500);
                spriteBee.setPosition({ 2000.f, height });
                bBeeActive = true;
            }
            else
            {
                // Move the bee
                spriteBee.setPosition({
                    spriteBee.getPosition().x - (beeSpeed * dt),
                    spriteBee.getPosition().y
                    });
                if (spriteBee.getPosition().x < -100.f)
                {
                    bBeeActive = false;
                }

            }

            // Manage the clouds
            for (int i = 0; i < NUM_CLOUDS; i++)
            {
                if (!cloudsActive[i]) {
                    // How Fast is the cloud?
                    srand((int)time(0) * 10);
                    cloudSpeed[i] = static_cast<float>((rand() % 200));
                    // How high is the cloud?
                    srand((int)time(0) * 10);
                    float height = static_cast<float>((rand() % 150));
                    clouds[i].setPosition({-200.f, height});
                    cloudsActive[i] = true;
                }
                else
                {
                    // Move the cloud
                    clouds[i].setPosition({
                        clouds[i].getPosition().x + (cloudSpeed[i] * dt),
                        clouds[i].getPosition().y
                        });
                    if (clouds[i].getPosition().x > 1920.f)
                    {
                        cloudsActive[i] = false;
                    }
                }
            }

            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                float height = static_cast<float>(i * 150);
                if (branchPositions[i] == side::LEFT)
                {
                    branches[i].setPosition({ 610, height });
                    branches[i].setRotation(degrees(180.f));
                }
                else if (branchPositions[i] == side::RIGHT)
                {
                    branches[i].setPosition({ 1330, height });
                    branches[i].setRotation(degrees(0.f));
                }
                else
                {
                    branches[i].setPosition({ 3000, height });
                }
            }

            /*
            Handle Flying Log
            */
            if (bLogActive)
            {
                spriteLog.setPosition({ 
                    spriteLog.getPosition().x + logSpeedX * dt,  
                    spriteLog.getPosition().y + logSpeedY * dt });

                // Has Log reached the right hand edge?
                if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().y > 2000)
                {
                    bLogActive = false;
                    spriteLog.setPosition({ 810, 720 });
                }
            }

            /*
                Handle death
            */
            if (branchPositions[5] == playerSide) {
                bPaused = true;
                bAcceptInput = false;

                //Draw the Gravestone
                spriteRIP.setPosition({525, 760});
                //Hide the player
                spritePlayer.setPosition({ 2000, 660 });

                //Change Text of Message
                messageText.setString("SQUISHED!!!");


                //Center it on the Screen
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.getCenter());

                messageText.setPosition({ 1920 / 2, 1080 / 2 });
                //Play Death Sound
                deathSound.play();
            }
        }
        
        /* Handle Event Area */
        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
            {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<Event::KeyPressed>())
            {
                /*
                * Handle Keypressed Event Input Area
                */
                if (keyPressed->scancode == Keyboard::Scancode::Escape)
                    window.close();
            }
            else if (const auto* keyReleased = event->getIf<Event::KeyReleased>())
            {
                bAcceptInput = true;
                // Hide sprite
                spriteAxe.setPosition({ 2000, spriteAxe.getPosition().y });
            }
        }
        /*
        * Handle Input Area
        */
        //if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
        //{
        //    window.close();
        //}
        if (Keyboard::isKeyPressed(Keyboard::Key::Enter))
        {
            bPaused = false;
            score = 0;
            timeRemaining = 6;

            // Make all the branches disappear
            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                branchPositions[i] = side::NONE;
            }
            //Make sure gravestone is hidden
            spriteRIP.setPosition({ 675, 2000 });
            //Move Player into position
            spritePlayer.setPosition({ 580, 720 });
            bAcceptInput = true;
            //chopSound.play();
        }
        // Check if game can accept gameplay input
        if (bAcceptInput)
        {
            if (Keyboard::isKeyPressed(Keyboard::Key::Right))
            {
                //Play Chop Sound
                chopSound.play();
                playerSide = side::RIGHT;
                score++;
                timeRemaining += static_cast<float>((2 / score) + 0.15f);

                spriteAxe.setPosition({ AXE_POSITION_RIGHT, spriteAxe.getPosition().y });
                spritePlayer.setPosition({ 1200, 720 });
                // update branches
                updateBranches(score);

                // set log flying to the left
                spriteLog.setPosition({ 810, 720 });
                logSpeedX = -5000;
                bLogActive = true;

                bAcceptInput = false;
            }
            //Handle the left cursor key
            if (Keyboard::isKeyPressed(Keyboard::Key::Left))
            {
                //Play Chop Sound
                chopSound.play();
                playerSide = side::LEFT;
                score++;
                timeRemaining += static_cast<float>((2 / score) + 0.15f);

                spriteAxe.setPosition({ AXE_POSITION_LEFT, spriteAxe.getPosition().y });
                spritePlayer.setPosition({ 580, 720 });
                // update branches
                updateBranches(score);

                // set log flying to the left
                spriteLog.setPosition({ 810, 720 });
                logSpeedX =  5000;
                bLogActive = true;

                bAcceptInput = false;
            }
            // Update the score text
            stringstream ss;
            ss << "Score = " << score;
            scoreText.setString(ss.str());
        }
        window.clear();
        
        /*
            Measure FPS
        */
        if (showFPS)
        {
            frameTimes.push_back(dt);
            if (frameTimes.size() > SAMPLE_COUNT)
                frameTimes.pop_front();
            // Keep only last SAMPLE_COUNT values
            if (frameTimes.size() > SAMPLE_COUNT)
                frameTimes.pop_front();

            // Compute average deltaTime
            float avgDelta = 0.0f;
            for (float dt : frameTimes)
                avgDelta += dt;
            avgDelta /= frameTimes.size();

            // Compute FPS
            fps = (avgDelta > 0) ? (1.0f / avgDelta) : 0.0f;
            fpsText.setString("FPS: " + std::to_string(static_cast<int>(fps)));
        }
        /*
        * Draw Sprite Area
        */
        window.draw(spriteBg); // draw Background

        //draw clouds
        for (auto& cloud : clouds)
            window.draw(cloud);

        //draw branches
        for (auto& branch : branches)
            window.draw(branch);

        window.draw(spriteTree); // draw Tree
        window.draw(spritePlayer); // draw Player
        window.draw(spriteAxe); // draw Axe
        window.draw(spriteLog); // draw Log
        window.draw(spriteRIP); // draw Gravestone
        window.draw(spriteBee); // draw Bee
        
        /** 
         UI
        */
        window.draw(scoreBackground); // Draw Score Background UI
        window.draw(scoreText); // Draw Score Text UI
        if (showFPS)
        {
            window.draw(fpsBackground); // Draw FPS Background UI
            window.draw(fpsText); // Draw FPS Text UI
        }

        window.draw(timeBar); // Draw Time Bar UI
        if (bPaused)
        {
            window.draw(messageText); // Draw Message Text UI
        }



        window.display();
    }
    return 0;
}
