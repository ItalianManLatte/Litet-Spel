# Litet-Spel

Note, to try.
Test //Filip

Naming
	Classes
	class PlayerController;
	class RenderComponent;

	Functions
	void Update(float someVariable);
	void Render();

	Variables
	float deltaTime;
	Vector3 playerPosition;
	bool isGrounded;

	Constants
	constexprfloat MaxPlayerSpeed = 1.0f;


Technical Architecture

Game
|
|_______ Gameplay
|       |    
|	|_ e.g gameState
|
|
|
|_______ Engine
|       |    
|	|_ e.g Scene
|
|
|
|_______ Renderer
|       |    
|	|_ e.g DX11
|
|
|
|_______ Tools
|       |    
|	|_ e.g Debug UI
