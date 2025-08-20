// Timber.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <array>
#include <sstream>      // std::stringstream
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;


const int NUM_BRANCHES = 6;
//Player Location
enum class side { LEFT, RIGHT, NONE };

vector<Sprite> branches;
vector<side> branchPositions(NUM_BRANCHES, side::NONE);

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

    // Make Three Clouds
    Texture textureCloud("graphics/cloud.png");//Load Texture from file path

    Sprite spriteCloud1(textureCloud); // Create Cloud Sprite using loaded texture
    spriteCloud1.setPosition({ 0,0 }); // Set position
    bool bCloud1Active{ false };
    float cloud1Speed{ 0.0f };

    Sprite spriteCloud2(textureCloud); // Create Cloud Sprite using loaded texture
    spriteCloud2.setPosition({ 0,150 }); // Set position
    bool bCloud2Active{ false };
    float cloud2Speed{ 0.0f };

    Sprite spriteCloud3(textureCloud); // Create Cloud Sprite using loaded texture
    spriteCloud3.setPosition({ 0,300 }); // Set position
    bool bCloud3Active{ false };
    float cloud3Speed{ 0.0f };
    
    // Game Parameters
    // Score
    Font font("fonts/KOMIKAP_.ttf");
    Text scoreText(font, "Score = 0", 75);
    scoreText.setFillColor(Color::White);
    int score = 0;

    Text messageText(font, "Press Enter To Start!", 100);
    messageText.setFillColor(Color::White);

    FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin({ textRect.getCenter()});
    messageText.setPosition({ 1920 / 2.0f, 1080 / 2.0f });

    scoreText.setPosition({ 20, 20 });

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

    bool bPaused{ true };

    updateBranches(1);
    updateBranches(2);
    updateBranches(3);
    updateBranches(4);
    updateBranches(5);
    updateBranches(6);

    while (window.isOpen())
    {
        if (!bPaused)
        {
            
            // Measure Time
            Time dt = clock.restart();

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
                    spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()),
                    spriteBee.getPosition().y
                    });
                if (spriteBee.getPosition().x < -100.f)
                {
                    bBeeActive = false;
                }

            }
            // Manage the clouds
            if (!bCloud1Active)
            {
                // How Fast is the cloud?
                srand((int)time(0) * 10);
                cloud1Speed = static_cast<float>((rand() % 200));

                // How high is the cloud?
                srand((int)time(0) * 10);
                float height = static_cast<float>((rand() % 150));
                spriteCloud1.setPosition({ -200.f, height });
                bCloud1Active = true;
            }
            else
            {
                // Move the cloud
                spriteCloud1.setPosition({
                    spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()),
                    spriteCloud1.getPosition().y
                    });
                if (spriteCloud1.getPosition().x > 1920.f)
                {
                    bCloud1Active = false;
                }
            }

            if (!bCloud2Active)
            {
                // How Fast is the cloud?
                srand((int)time(0) * 20);
                cloud2Speed = static_cast<float>((rand() % 200));

                // How high is the cloud?
                srand((int)time(0) * 20);
                float height = static_cast<float>((rand() % 300) - 150);
                spriteCloud2.setPosition({ -200.f, height });
                bCloud2Active = true;
            }
            else
            {
                // Move the cloud
                spriteCloud2.setPosition({
                    spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()),
                    spriteCloud2.getPosition().y
                    });
                if (spriteCloud2.getPosition().x > 1920.f)
                {
                    bCloud2Active = false;
                }
            }

            if (!bCloud3Active)
            {
                // How Fast is the cloud?
                srand((int)time(0) * 30);
                cloud3Speed = static_cast<float>((rand() % 200));

                // How high is the cloud?
                srand((int)time(0) * 30);
                float height = static_cast<float>((rand() % 450) - 150);
                spriteCloud3.setPosition({ -200.f, height });
                bCloud3Active = true;
            }
            else
            {
                // Move the cloud
                spriteCloud3.setPosition({
                    spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()),
                    spriteCloud3.getPosition().y
                    });
                if (spriteCloud3.getPosition().x > 1920.f)
                {
                    bCloud3Active = false;
                }
            }

            // Update the score text
            stringstream ss;
            ss << "Score = " << score;
            scoreText.setString(ss.str());

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
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::Enter))
        {
            bPaused = false;
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
        {
            window.close();
        }
        window.clear();

        window.draw(spriteBg); // draw Background

        window.draw(spriteCloud1); // draw cloud 1
        window.draw(spriteCloud2); // draw cloud 2
        window.draw(spriteCloud3); // draw cloud 3

        //draw branches
        for (auto& branch : branches)
            window.draw(branch);

        window.draw(spriteTree); // draw Tree
        window.draw(spriteBee); // draw Bee
        
        /** 
         UI
        */
        window.draw(scoreText); // Draw Score Text UI
        if (bPaused)
        {
            window.draw(messageText); // Draw Message Text UI
        }



        window.display();
    }
    return 0;
}
