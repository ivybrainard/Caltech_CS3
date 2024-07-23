# Game Design Document

## Section 0: Summary

Space Escape
Ivy Brainard Stavya Arora, Isha Goswami,
An astronaut (or multiple) navigates space in a straight path while avoiding obstacles in the form of rocket ships (which can be climbed on) and meteors on multiple tracks.

## Section 1: Gameplay

The game starts in a horizontal display, with a start screen with two buttons: single player or dual player. Single player consists of one user, while dual player consist of two players. With dual players if the players collide, the game is over. When the user(s) chooses which setting the screen will switch to the display of the game and start the game. The user can click the button with their mouse. The display of the game is three paths with either rocket ships or meteors. The user can jump on top of rocket ships and go over the meteors. Also, the rocket ships can be moving or stationary. To jump over the rocket ships, the user presses the right arrow. To go under the meteors, the user presses the left arrow. If the user collides with one of the obstacles, or for dual players the players collide, the player(s) lose. During the game, there will be coins to collect and the user(s) goal is try and collect as many coins as they can. Music will be played when the game is over. Also, there will be a timer to see how long the user(s) can last. As time goes by, the obstacles will come faster and faster. The rocket ships, meteors, and astronauts will be displayed with images, using the body-asset image rendering developed in project 6. 

## Section 2: Feature Set

Priority 1 features:
- Image rendering, different screens: Display a character (two in the dual player mode), obstacles, and different scenes, such as the start screen, character-choosing scene, and in-game scene with images. 
- Key and mouse handlers: For the mouse handler, there will be lots of different options for the game and the mouse handler allows for the user to decide what they want for the game. The mouse handler will be used for the user to choose if they want a single or dual player. Also when customizing the character the user will use the mouse to choose which character they want.
- Text: Text will be rendered in varying sized/fonts/displays on various screens in different implementations. It will be present on the start screen in the form of instruction, on the customization screen, in the actual game with money counter and health values displayed, and with the timer displayed in the corner of the screen.

Priority 2 features:
- Varying obstacles, different behaviors of each, Collision detection: There will be different sized obstacles coming at different speeds towards the player(s), that will also look different (different images being rendered). Also will keep track when players hit different obstacles or the other player. 
- Player Customization: Before the user plays the game the user will have the option to choose which character they will play in the single player version. They will be shown all the characters and they click the button for which character they want. Once they choose the character the game will begin with the player. 
- Changing background throughout the game: The concept of the game is that the player will start on Pluto, and as they progress through the game, they will travel through the solar system to eventually get back to Earth. So, as time continues in the game, the background image displayed will be a different planet of the solar system (Pluto first, then Neptune, Uranus, etc.). 

Priority 3 features: 
- Health: Each player will get a certain number of lives when they start the game, and they get that many chances to hit an obstacle and still stay in the game. 
- Money counter: Each time that a player collides with a coin in their path of avoiding obstacles, the coin counter will increase, and in the multiplayer mode each player will have their own count of coins.  After a certain amount of money has been collected the user will gain life and the money goes back down. This gives an incentive to gain back lives. 
- Game difficulty progression: As more time goes on the game that the player is able to successfully survive, the difficulty level of the game will increase through an increasing speed and frequency of obstacles. 

Priority 4 features:
- Single and dual player modes: There will be a mode of the game where two players can play the game at once and compete against each other for the highest score. Once one player loses, the screen will render to a new screen with text congratulating the player who won.
- Music: There will be sound effects for when the game is over.
- Power-ups: While the player can collect coins, there will also be power-ups instead of coins every now and then which can give the player some type of boost (adding extra points or going through all obstacles for a certain period of time after collecting the power up without losing a life). 
