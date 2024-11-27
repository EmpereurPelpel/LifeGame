#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <algorithm>


/// <summary>
/// Largeur de la fenetre
/// </summary>
const int WIDTH = 800;

/// <summary>
/// Hauteur de la fenetre
/// </summary>
const int HEIGHT = 600;

/// <summary>
/// Dimension d'une cellule du jeu de la vie
/// </summary>
const int CELL_SIZE = 10;

/// <summary>
/// Nombre de cellules dans la fenetre sur la longueur
/// </summary>
const int GRID_WIDTH = WIDTH / CELL_SIZE;

/// <summary>
/// Nombre de cellules dans la fenetre sur la hauteur
/// </summary>
const int GRID_HEIGHT = HEIGHT / CELL_SIZE;

/// <summary>
/// Grille de jeu
/// </summary>
std::vector<std::vector<bool>> grid(GRID_HEIGHT, std::vector<bool>(GRID_WIDTH, false));
std::vector<std::vector<bool>> nextGrid(GRID_HEIGHT, std::vector<bool>(GRID_WIDTH, false));

/// <summary>
/// Etat du jeu de la vie (actif ou eteint)
/// </summary>
bool running = false;

/// <summary>
/// Vitesse du jeu de la vie
/// </summary>
double simulationSpeed = 1.0;

/// <summary>
/// Dessine le jeu de la vie dans la fenetre
/// </summary>
void drawGrid() {

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	//Affichage de la grille de jeu cellule par cellule
	glBegin(GL_QUADS);
	for (int i = 0; i < GRID_HEIGHT; ++i) {
		for (int j = 0; j < GRID_WIDTH; ++j) {
			if (grid[i][j]) {
				glColor3f(1.0f, 1.0f, 1.0f);
			}
			else {
				glColor3f(0.0f, 0.0f, 0.0f);
			}
			glVertex2f(j * CELL_SIZE, i * CELL_SIZE);
			glVertex2f(j * CELL_SIZE, (i + 1) * CELL_SIZE);
			glVertex2f((j + 1) * CELL_SIZE, (i + 1) * CELL_SIZE);
			glVertex2f((j + 1) * CELL_SIZE, i * CELL_SIZE);
		}
	}
	glEnd();

	//Affichage du bouton pause
	glBegin(GL_QUADS);
	glColor3f(0., 0.5, 0.);
	glVertex2f(0 * CELL_SIZE, 0 * CELL_SIZE);
	glVertex2f(0 * CELL_SIZE, 10 * CELL_SIZE);
	glVertex2f(10 * CELL_SIZE, 10 * CELL_SIZE);
	glVertex2f(10 * CELL_SIZE, 0 * CELL_SIZE);
	glEnd();

	//Si le jeu est actif
	if (running)
	{
		glBegin(GL_QUADS);
		glColor3f(1, 1, 1.);
		glVertex2f(2 * CELL_SIZE, 1 * CELL_SIZE);
		glVertex2f(2 * CELL_SIZE, 9 * CELL_SIZE);
		glVertex2f(4 * CELL_SIZE, 9 * CELL_SIZE);
		glVertex2f(4 * CELL_SIZE, 1 * CELL_SIZE);

		glVertex2f(6 * CELL_SIZE, 1 * CELL_SIZE);
		glVertex2f(6 * CELL_SIZE, 9 * CELL_SIZE);
		glVertex2f(8 * CELL_SIZE, 9 * CELL_SIZE);
		glVertex2f(8 * CELL_SIZE, 1 * CELL_SIZE);
		glEnd();


	}
	//Si le jeu est en pause
	else
	{
		glBegin(GL_TRIANGLES);
		glColor3f(1, 1, 1.);
		glVertex2f(3 * CELL_SIZE, 2 * CELL_SIZE);
		glVertex2f(3 * CELL_SIZE, 8 * CELL_SIZE);
		glVertex2f(8 * CELL_SIZE, 5 * CELL_SIZE);
		glEnd();

	}

	//Affichage du bouton accelerer

	glBegin(GL_QUADS);
	glColor3f(0.8, 0.5, 0.);
	glVertex2f((GRID_WIDTH - 10) * CELL_SIZE, 0 * CELL_SIZE);
	glVertex2f((GRID_WIDTH - 10) * CELL_SIZE, 10 * CELL_SIZE);
	glVertex2f(GRID_WIDTH * CELL_SIZE, 10 * CELL_SIZE);
	glVertex2f(GRID_WIDTH * CELL_SIZE, 0 * CELL_SIZE);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(1, 1, 1.);
	glVertex2f((GRID_WIDTH - 9) * CELL_SIZE, 2 * CELL_SIZE);
	glVertex2f((GRID_WIDTH - 9) * CELL_SIZE, 8 * CELL_SIZE);
	glVertex2f((GRID_WIDTH - 4) * CELL_SIZE, 5 * CELL_SIZE);

	glVertex2f((GRID_WIDTH - 6) * CELL_SIZE, 2 * CELL_SIZE);
	glVertex2f((GRID_WIDTH - 6) * CELL_SIZE, 8 * CELL_SIZE);
	glVertex2f((GRID_WIDTH - 1) * CELL_SIZE, 5 * CELL_SIZE);
	glEnd();

	//Affichage du bouton ralentir

	glBegin(GL_QUADS);
	glColor3f(0.8, 0.5, 0.);
	glVertex2f((GRID_WIDTH - 10) * CELL_SIZE, (GRID_HEIGHT - 10) * CELL_SIZE);
	glVertex2f((GRID_WIDTH - 10) * CELL_SIZE, GRID_HEIGHT * CELL_SIZE);
	glVertex2f(GRID_WIDTH * CELL_SIZE, GRID_HEIGHT * CELL_SIZE);
	glVertex2f(GRID_WIDTH * CELL_SIZE, (GRID_HEIGHT - 10) * CELL_SIZE);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(1, 1, 1.);
	glVertex2f((GRID_WIDTH - 1) * CELL_SIZE, (GRID_HEIGHT - 8) * CELL_SIZE);
	glVertex2f((GRID_WIDTH - 1) * CELL_SIZE, (GRID_HEIGHT - 2) * CELL_SIZE);
	glVertex2f((GRID_WIDTH - 6) * CELL_SIZE, (GRID_HEIGHT - 5) * CELL_SIZE);

	glVertex2f((GRID_WIDTH - 4) * CELL_SIZE, (GRID_HEIGHT - 8) * CELL_SIZE);
	glVertex2f((GRID_WIDTH - 4) * CELL_SIZE, (GRID_HEIGHT - 2) * CELL_SIZE);
	glVertex2f((GRID_WIDTH - 9) * CELL_SIZE, (GRID_HEIGHT - 5) * CELL_SIZE);
	glEnd();
}

/// <summary>
/// Compte les cellules vivantes voisines d'une cellule
/// </summary>
/// <param name="row">Ligne de la cellule</param>
/// <param name="col"> colonne de la cellule</param>
/// <returns>Un entier representant le nombre de voisins vivants de la cellule</returns>
int countNeighbors(int row, int col) {
	int count = 0;
	for (int i = -1; i <= 1; ++i) {
		for (int j = -1; j <= 1; ++j) {
			if (i == 0 && j == 0) continue;
			int newRow = row + i;
			int newCol = col + j;
			if (newRow >= 0 && newRow < GRID_HEIGHT && newCol >= 0 && newCol < GRID_WIDTH) {
				count += grid[newRow][newCol];
			}
		}
	}
	return count;
}

/// <summary>
/// Met a jour l'etat des differentes cellules de la grille
/// </summary>
void updateGrid() {
	for (int i = 0; i < GRID_HEIGHT; ++i) {
		for (int j = 0; j < GRID_WIDTH; ++j) {
			int neighbors = countNeighbors(i, j);
			if (grid[i][j]) {
				nextGrid[i][j] = (neighbors == 2 || neighbors == 3);
			}
			else {
				nextGrid[i][j] = (neighbors == 3);
			}
		}
	}
	std::swap(grid, nextGrid);
}

/// <summary>
/// Definit l'action a realiser en fonction de l'emplacement de la souris lors d'un clic
/// </summary>
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {

		//coordonnees de la souris
		double mouseX, mouseY;

		glfwGetCursorPos(window, &mouseX, &mouseY);
		int row = static_cast<int>(mouseY) / CELL_SIZE;
		int col = static_cast<int>(mouseX) / CELL_SIZE;
		//Actualisation de la grille de jeu lorsqu'on clique sur la fen�tre
		if (row >= 0 && row < GRID_HEIGHT && col >= 0 && col < GRID_WIDTH) {
			grid[row][col] = !grid[row][col];
		}


		//Action realisee lorsqu'on clique sur le bouton pause
		if (mouseX > 0 and mouseX <= 100 and mouseY > 0 and mouseY <= 100)
		{
			running = !running;
		}

		//Action realisee lorsqu'on clique sur le bouton accelerer
		if (mouseX > (WIDTH - 100) and mouseX <= WIDTH and mouseY > 0 and mouseY <= 100)
		{
			// Double la vitesse de simulation
			simulationSpeed *= 2.0;
		}

		//Action realisee lorsqu'on clique sur le bouton ralentir
		if (mouseX > (WIDTH - 100) and mouseX <= WIDTH and mouseY > (HEIGHT - 100) and mouseY <= HEIGHT)
		{
			// Divise la vitesse de simulation par deux
			simulationSpeed /= 2.0;
		}

	}
}
/// <summary>
/// Definit les actions realisees par la pression des touches du clavier
/// </summary>
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		running = !running;
	}
	else if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		simulationSpeed *= 2.0;
	}
	else if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		simulationSpeed /= 2.0; 
	}
}

int main() {
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW\n";
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Game of Life", nullptr, nullptr);
	if (!window) {
		std::cerr << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetKeyCallback(window, keyCallback);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, WIDTH, HEIGHT, 0.0f, 0.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		if (running) {
			// Ralentit ou accelere la simulation
			double deltaTime = 1.0 / simulationSpeed; 
			double timeElapsed = glfwGetTime();
			static double lastTime = 0.0;

			//Appelle la fonction UpdateGrid tous les deltaTime
			if (timeElapsed - lastTime > deltaTime) {
				updateGrid();
				lastTime = timeElapsed;
			}
		}

		drawGrid();

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}