#include <GL/glut.h>
#include <iostream>
#include <math.h>

// Window dimensions
const int WIDTH = 800;
const int HEIGHT = 600;

// Paddle dimensions
const int PADDLE_WIDTH = 10;
const int PADDLE_HEIGHT = 60;
const float PADDLE_SPEED = 20.0f;

// Crab dimensions
const float CRAB_SIZE = 20.0f;
const float CRAB_SPEED_X = 2.0f;
const float CRAB_SPEED_Y = 2.0f;

// Paddle positions
float paddle1Y = HEIGHT / 2;
float paddle2Y = HEIGHT / 2;

// Crab position and velocity
float crabX = WIDTH / 2;
float crabY = HEIGHT / 2;
float crabVelX = CRAB_SPEED_X;
float crabVelY = CRAB_SPEED_Y;

// Score variables
int score1 = 0;
int score2 = 0;

// Function to draw a rectangle
void drawRect(float x, float y, float width, float height) {
    glRectf(x, y, x + width, y + height);
}

// Function to draw the paddles
void drawPaddles() {
    glColor3f(1.0f, 1.0f, 1.0f); // Set paddle color to white

    // Draw paddle 1
    drawRect(0, paddle1Y - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT);

    // Draw paddle 2
    drawRect(WIDTH - PADDLE_WIDTH, paddle2Y - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT);
}

// Function to draw the crab
void drawCrab() {
    glColor3f(0.0f, 0.0f, 0.0f); // Set crab color to white
    glTranslatef(crabX, crabY, 0.0f); // Translate to crab position

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f); // Center of the crab shape
    const int numSegments = 100; // Number of segments to approximate the crab shape
    const float angleIncrement = 2.0f * 3.14159f / numSegments;
    const float radius = CRAB_SIZE / 2.0f;
    for (int i = 0; i <= numSegments; ++i) {
        float angle = i * angleIncrement;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();

    glTranslatef(-crabX, -crabY, 0.0f); // Reset translation
}

// Function to update the game state
void updateGame() {
    // Update crab position
    crabX += crabVelX;
    crabY += crabVelY;

    // Check for collision with paddles
    if (crabX <= PADDLE_WIDTH && crabY >= paddle1Y - PADDLE_HEIGHT / 2 && crabY <= paddle1Y + PADDLE_HEIGHT / 2) {
        crabVelX = -crabVelX; // Reverse crab's x velocity
    } else if (crabX >= WIDTH - PADDLE_WIDTH && crabY >= paddle2Y - PADDLE_HEIGHT / 2 && crabY <= paddle2Y + PADDLE_HEIGHT / 2) {
        crabVelX = -crabVelX; // Reverse crab's x velocity
    }

    // Check for collision with top and bottom walls
    if (crabY >= HEIGHT - CRAB_SIZE / 2 || crabY <= CRAB_SIZE / 2) {
        crabVelY = -crabVelY; // Reverse crab's y velocity
    }

    // Check for scoring
    if (crabX <= 0) {
        score2++; // Player 2 scores
        crabX = WIDTH / 2;
        crabY = HEIGHT / 2;
    } else if (crabX >= WIDTH) {
        score1++; // Player 1 scores
        crabX = WIDTH / 2;
        crabY = HEIGHT / 2;
    }
}

// Function to handle keyboard input
void keyboard(unsigned char key, int x, int y) {
    // Move paddle 1
    if (key == 's' && paddle1Y < HEIGHT - PADDLE_HEIGHT / 2) {
        paddle1Y += PADDLE_SPEED;
    } else if (key == 'w' && paddle1Y > PADDLE_HEIGHT / 2) {
        paddle1Y -= PADDLE_SPEED;
    }

    // Move paddle 2
    if (key == 'k' && paddle2Y < HEIGHT - PADDLE_HEIGHT / 2) {
        paddle2Y += PADDLE_SPEED;
    } else if (key == 'i' && paddle2Y > PADDLE_HEIGHT / 2) {
        paddle2Y -= PADDLE_SPEED;
    }
}

// Function to handle window resizing
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

// Function to display the game
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawPaddles();
    drawCrab();

    glutSwapBuffers();
}

// Function to update the game and redraw the screen
void update(int value) {
    updateGame();
    glutPostRedisplay();
    glutTimerFunc(10, update, 0);
}

// Main function
int main(int argc, char** argv) {
    // Initialize OpenGL and GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Crab-Pong Game");

    // Register callback functions
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);

    // Set the timer for game updates
    glutTimerFunc(10, update, 0);

    // Enter the main loop
    glutMainLoop();
    return 0;
}
