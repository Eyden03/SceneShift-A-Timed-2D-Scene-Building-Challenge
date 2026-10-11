#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__ // apple == mac
#include <GLUT/glut.h> // for mac
#else
#include <GL/glut.h> // for windows/linux
#endif

#include <iostream>
#include <cmath>

#include <cstdlib>   // rand(), srand()
#include <ctime>     // time()
#include <cstdio>    // sprintf()

using namespace std;



// =====  Initialization and Variables ==========
// Forward declarations for functions defined later in this file
void evaluateObjective();
void updateObjectiveResult();


// === STATE VARIABLES ===

int timeOfDay = 0;
int ground = 0;
int building = 0;
int skyFeature = 0;
int weather = 0;

// === GAME STATE ===
// 0 = Title
// 1 = Instructions
// 2 = Playing
// 3 = Paused
// 4 = Objective Results
// 5 = Game Over
// 6 = Final Results

int gameState = 0;


// Current objective progress: 1 to 10
int currentObjective = 1;

// Time remaining for the current objective
int timeLeft = 30;

// Score for the current objective: 0 to 5
int objectiveScore = 0;

// Player lives
int lives = 3;

// Accumulated match score for all objectives
int totalScore = 0;





// Ordinary arrays for storing the X and Y positions of the stars
const int STAR_COUNT = 12;

// stars are stored in two separate arrays for X and Y positions
float starX[STAR_COUNT] =
{
    -0.85f, -0.68f, -0.48f, -0.25f,
     0.05f,  0.28f,  0.50f,  0.72f,
    -0.58f, -0.12f,  0.38f,  0.86f
};

float starY[STAR_COUNT] =
{
     0.78f,  0.55f,  0.82f,  0.62f,
     0.88f,  0.58f,  0.76f,  0.48f,
     0.32f,  0.40f,  0.36f,  0.68f
};

// === AURORA ARRAYS AND ANIMATION ===

const int AURORA_VERTEX_COUNT = 32;


// Module 4: Vertex array
// 8 narrow Aurora strips, with 4 vertices for each strip.
GLfloat auroraVertices[AURORA_VERTEX_COUNT * 2] =
{
    // Strip 1
    -0.90f, 0.68f,
    -0.90f, 0.34f,
    -0.68f, 0.74f,
    -0.68f, 0.24f,

    // Strip 2
    -0.70f, 0.74f,
    -0.70f, 0.22f,
    -0.48f, 0.80f,
    -0.48f, 0.10f,

    // Strip 3
    -0.50f, 0.80f,
    -0.50f, 0.18f,
    -0.28f, 0.75f,
    -0.28f, 0.30f,

    // Strip 4
    -0.30f, 0.75f,
    -0.30f, 0.28f,
    -0.08f, 0.70f,
    -0.08f, 0.08f,

    // Strip 5
    -0.10f, 0.70f,
    -0.10f, 0.12f,
     0.12f, 0.76f,
     0.12f, 0.26f,

     // Strip 6
      0.10f, 0.76f,
      0.10f, 0.24f,
      0.32f, 0.82f,
      0.32f, 0.11f,

      // Strip 7
       0.30f, 0.82f,
       0.30f, 0.16f,
       0.55f, 0.76f,
       0.55f, 0.29f,

       // Strip 8
        0.53f, 0.76f,
        0.53f, 0.27f,
        0.82f, 0.70f,
        0.82f, 0.18f
};


// Module 4: Color array
// Each Aurora vertex has a corresponding RGB color.
GLfloat auroraColors[AURORA_VERTEX_COUNT * 3] =
{
    // Strip 1
    0.68f, 1.00f, 0.76f,
    0.22f, 0.18f, 0.68f,
    0.48f, 0.96f, 0.88f,
    0.32f, 0.14f, 0.72f,

    // Strip 2
    0.68f, 1.00f, 0.76f,
    0.22f, 0.18f, 0.68f,
    0.48f, 0.96f, 0.88f,
    0.32f, 0.14f, 0.72f,

    // Strip 3
    0.68f, 1.00f, 0.76f,
    0.22f, 0.18f, 0.68f,
    0.48f, 0.96f, 0.88f,
    0.32f, 0.14f, 0.72f,

    // Strip 4
    0.68f, 1.00f, 0.76f,
    0.22f, 0.18f, 0.68f,
    0.48f, 0.96f, 0.88f,
    0.32f, 0.14f, 0.72f,

    // Strip 5
    0.68f, 1.00f, 0.76f,
    0.22f, 0.18f, 0.68f,
    0.48f, 0.96f, 0.88f,
    0.32f, 0.14f, 0.72f,

    // Strip 6
    0.68f, 1.00f, 0.76f,
    0.22f, 0.18f, 0.68f,
    0.48f, 0.96f, 0.88f,
    0.32f, 0.14f, 0.72f,

    // Strip 7
    0.68f, 1.00f, 0.76f,
    0.22f, 0.18f, 0.68f,
    0.48f, 0.96f, 0.88f,
    0.32f, 0.14f, 0.72f,

    // Strip 8
    0.68f, 1.00f, 0.76f,
    0.22f, 0.18f, 0.68f,
    0.48f, 0.96f, 0.88f,
    0.32f, 0.14f, 0.72f
};


// Simple Aurora animation values
float auroraMove = 0.0004f;
float auroraOffset = 0.0f;

const float AURORA_MOVE_LIMIT = 0.02f;
const int ANIMATION_INTERVAL = 25;

// Cloud animation
float cloudMove = 0.0f;
float cloudSpeed = 0.001f;
const float CLOUD_MOVE_LIMIT = 0.04f;

// Weather animation speeds
const float RAIN_SPEED = 0.015f;
const float SNOW_SPEED = 0.008f;

// === WEATHER VARIABLES ===

//  SNOW ARRAYS 

const int SNOW_COUNT = 18;

float snowX[SNOW_COUNT] =
{
    -0.90f, -0.78f, -0.65f, -0.52f, -0.40f, -0.28f,
    -0.15f, -0.03f,  0.10f,  0.22f,  0.35f,  0.47f,
     0.58f,  0.68f,  0.77f,  0.85f,  0.92f,  0.52f
};

float snowY[SNOW_COUNT] =
{
     0.88f,  0.60f,  0.32f,  0.76f,  0.48f,  0.92f,
     0.20f,  0.68f,  0.40f,  0.82f,  0.15f,  0.58f,
     0.90f,  0.35f,  0.70f,  0.05f,  0.50f, -0.18f
};



// Rain animation
const int RAIN_COUNT = 14;

float rainX[RAIN_COUNT] =
{
    -0.90f, -0.76f, -0.62f, -0.48f, -0.34f,
    -0.20f, -0.06f,  0.10f,  0.24f,  0.38f,
     0.52f,  0.66f,  0.80f,  0.92f
};

float rainY[RAIN_COUNT] =
{
     0.85f,  0.45f,  0.70f,  0.20f,  0.92f,
     0.55f,  0.05f,  0.78f,  0.35f,  0.65f,
     0.15f,  0.88f,  0.50f,  0.28f
};


// ==================== OBJECTIVE DATA ====================

const int OBJECTIVE_COUNT = 10;
const int TARGET_COUNT = 5;


// Each row represents one objective.
// Columns:
// 0 = Ground
// 1 = Building
// 2 = Time of Day
// 3 = Sky Feature
// 4 = Weather

int objectiveTargets[OBJECTIVE_COUNT][TARGET_COUNT] =
{
    { 0, 1, 0, 0, 0 },  // 1  Grass + House + Day + Clear + Clear
    { 1, 4, 1, 0, 0 },  // 2  Desert + Lighthouse + Sunset + Clear + Clear
    { 0, 2, 2, 2, 2 },  // 3  Grass + Cabin + Twilight + Moon + Rain
    { 3, 3, 3, 1, 0 },  // 4  Ice + Observatory + Night + Starry + Clear
    { 2, 2, 3, 3, 3 },  // 5  Snow + Cabin + Night + Aurora + Snowfall
    { 1, 0, 0, 0, 1 },  // 6  Desert + None + Day + Clear + Cloudy
    { 2, 1, 2, 2, 0 },  // 7  Snow + House + Twilight + Moon + Clear
    { 3, 4, 3, 3, 1 },  // 8  Ice + Lighthouse + Night + Aurora + Cloudy
    { 0, 3, 2, 1, 1 },  // 9  Grass + Observatory + Twilight + Starry + Cloudy
    { 1, 1, 2, 2, 2 }   // 10 Desert + House + Twilight + Moon + Rain
};



// Objective sentences shown to the player
const char* objectiveText[OBJECTIVE_COUNT] =
{
    "Create a clear sunny day with a house on a grassy field.",

    "Create a clear desert sunset with a lighthouse among the sands.",

    "Create a rainy twilight with a cabin on the grass beneath the moon.",

    "Create a clear icy night with an observatory under a starry sky.",

    "Create a snowy night with a cabin beneath the Aurora and falling snow.",

    "Create a cloudy daytime desert with no building and no special sky feature.",

    "Create a clear snowy twilight with a house beneath the moon.",

    "Create a cloudy frozen night with a lighthouse beneath the Aurora.",

    "Create a cloudy twilight with an observatory on the grass beneath a starry sky.",

    "Create a rainy desert twilight with a house beneath the moon."
};




// Stores the randomized order of the 10 objectives
int objectiveOrder[OBJECTIVE_COUNT];

void shuffleObjectives()
{
    // Fill the array with objective indexes 0 to 9
    for (int i = 0; i < OBJECTIVE_COUNT; i++)
    {
        objectiveOrder[i] = i;
    }

    // Shuffle the objective indexes
    for (int i = OBJECTIVE_COUNT - 1; i > 0; i--)
    {
        // Choose a random index from 0 to i
        int randomIndex = rand() % (i + 1);

        // Swap the two values
        int temp = objectiveOrder[i];
        objectiveOrder[i] = objectiveOrder[randomIndex];
        objectiveOrder[randomIndex] = temp;
    }
}




void startObjective()
{
    // Reset editable scene to default
    ground = 0;          // Grass
    building = 0;        // None
    timeOfDay = 0;       // Day
    skyFeature = 0;      // Clear
    weather = 0;         // Clear


    // Set starting time based on objective progress
    if (currentObjective <= 3)
    {
        timeLeft = 30;
    }
    else if (currentObjective <= 6)
    {
        timeLeft = 25;
    }
    else if (currentObjective <= 9)
    {
        timeLeft = 20;
    }
    else
    {
        timeLeft = 15;
    }


    glutPostRedisplay();
}

void submitObjective()
{
    // Submit only while actively Playing
    if (gameState != 2)
        return;

    // Evaluate the five scene categories
    evaluateObjective();

    // Update total score and lives
    updateObjectiveResult();

    // Show the Objective Result screen
    gameState = 4;

    glutPostRedisplay();
}



void restartGame()
{
    // Reset overall game progress
    currentObjective = 1;
    lives = 3;
    totalScore = 0;
    objectiveScore = 0;

    // Create a new randomized objective order
    shuffleObjectives();

    // Reset scene and assign Objective 1 starting time
    startObjective();

    // Return directly to gameplay
    gameState = 2; // Playing
}








// === GROUND OBJECTS ===

void drawTree(float x, float y)
{
    // Trunk
    glColor3f(0.35f, 0.18f, 0.08f);

    glBegin(GL_QUADS);
    glVertex2f(x - 0.025f, y);
    glVertex2f(x + 0.025f, y);
    glVertex2f(x + 0.025f, y + 0.18f);
    glVertex2f(x - 0.025f, y + 0.18f);
    glEnd();

    // Lower leaves
    glColor3f(0.10f, 0.45f, 0.15f);

    glBegin(GL_TRIANGLES);
    glVertex2f(x - 0.15f, y + 0.12f);
    glVertex2f(x + 0.15f, y + 0.12f);
    glVertex2f(x, y + 0.42f);
    glEnd();

    // Upper leaves
    glColor3f(0.12f, 0.55f, 0.18f);

    glBegin(GL_TRIANGLES);
    glVertex2f(x - 0.12f, y + 0.27f);
    glVertex2f(x + 0.12f, y + 0.27f);
    glVertex2f(x, y + 0.55f);
    glEnd();
}


void drawGrassTuft(float x, float y)
{
    glColor3f(0.08f, 0.38f, 0.10f);

    glBegin(GL_LINES);
    glVertex2f(x, y);
    glVertex2f(x - 0.03f, y + 0.10f);

    glVertex2f(x, y);
    glVertex2f(x, y + 0.12f);

    glVertex2f(x, y);
    glVertex2f(x + 0.03f, y + 0.10f);
    glEnd();
}


void drawFlower(float x, float y)
{
    // Stem
    glColor3f(0.10f, 0.45f, 0.10f);

    glBegin(GL_LINES);
    glVertex2f(x, y);
    glVertex2f(x, y + 0.08f);
    glEnd();

    // Flower head
    glPointSize(6.0f);
    glColor3f(1.0f, 0.45f, 0.65f);

    glBegin(GL_POINTS);
    glVertex2f(x, y + 0.09f);
    glEnd();
}


void drawRock(float x, float y)
{
    glColor3f(0.38f, 0.34f, 0.30f);

    glBegin(GL_POLYGON);
    glVertex2f(x - 0.08f, y);
    glVertex2f(x + 0.08f, y);
    glVertex2f(x + 0.06f, y + 0.07f);
    glVertex2f(x, y + 0.11f);
    glVertex2f(x - 0.06f, y + 0.07f);
    glEnd();
}


void drawCactus(float x, float y)
{
    glColor3f(0.10f, 0.48f, 0.22f);

    // Main stem
    glBegin(GL_QUADS);
    glVertex2f(x - 0.035f, y);
    glVertex2f(x + 0.035f, y);
    glVertex2f(x + 0.035f, y + 0.35f);
    glVertex2f(x - 0.035f, y + 0.35f);
    glEnd();

    // Left horizontal arm
    glBegin(GL_QUADS);
    glVertex2f(x - 0.12f, y + 0.17f);
    glVertex2f(x - 0.035f, y + 0.17f);
    glVertex2f(x - 0.035f, y + 0.23f);
    glVertex2f(x - 0.12f, y + 0.23f);
    glEnd();

    // Left raised arm
    glBegin(GL_QUADS);
    glVertex2f(x - 0.12f, y + 0.17f);
    glVertex2f(x - 0.07f, y + 0.17f);
    glVertex2f(x - 0.07f, y + 0.30f);
    glVertex2f(x - 0.12f, y + 0.30f);
    glEnd();

    // Right horizontal arm
    glBegin(GL_QUADS);
    glVertex2f(x + 0.035f, y + 0.22f);
    glVertex2f(x + 0.13f, y + 0.22f);
    glVertex2f(x + 0.13f, y + 0.28f);
    glVertex2f(x + 0.035f, y + 0.28f);
    glEnd();

    // Right raised arm
    glBegin(GL_QUADS);
    glVertex2f(x + 0.08f, y + 0.22f);
    glVertex2f(x + 0.13f, y + 0.22f);
    glVertex2f(x + 0.13f, y + 0.34f);
    glVertex2f(x + 0.08f, y + 0.34f);
    glEnd();
}


void drawSnowyPine(float x, float y)
{
    // Trunk
    glColor3f(0.30f, 0.17f, 0.08f);

    glBegin(GL_QUADS);
    glVertex2f(x - 0.025f, y);
    glVertex2f(x + 0.025f, y);
    glVertex2f(x + 0.025f, y + 0.16f);
    glVertex2f(x - 0.025f, y + 0.16f);
    glEnd();

    // Pine
    glColor3f(0.08f, 0.30f, 0.18f);

    glBegin(GL_TRIANGLES);
    glVertex2f(x - 0.15f, y + 0.10f);
    glVertex2f(x + 0.15f, y + 0.10f);
    glVertex2f(x, y + 0.45f);

    glVertex2f(x - 0.12f, y + 0.25f);
    glVertex2f(x + 0.12f, y + 0.25f);
    glVertex2f(x, y + 0.55f);
    glEnd();

    // Snow layers
    glColor3f(0.96f, 0.98f, 1.0f);

    glBegin(GL_TRIANGLES);
    glVertex2f(x - 0.12f, y + 0.18f);
    glVertex2f(x + 0.12f, y + 0.18f);
    glVertex2f(x, y + 0.28f);

    glVertex2f(x - 0.09f, y + 0.33f);
    glVertex2f(x + 0.09f, y + 0.33f);
    glVertex2f(x, y + 0.42f);
    glEnd();
}


void drawSnowman(float x, float y)
{
    // Lower body
    glColor3f(0.95f, 0.97f, 1.0f);

    glBegin(GL_POLYGON);
    glVertex2f(x - 0.09f, y);
    glVertex2f(x + 0.09f, y);
    glVertex2f(x + 0.11f, y + 0.08f);
    glVertex2f(x + 0.07f, y + 0.16f);
    glVertex2f(x, y + 0.19f);
    glVertex2f(x - 0.07f, y + 0.16f);
    glVertex2f(x - 0.11f, y + 0.08f);
    glEnd();

    // Gray shadow outline keeps the lower body visible against light backgrounds
    glColor3f(0.68f, 0.70f, 0.72f);
    glLineWidth(2.5f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x - 0.09f, y);
    glVertex2f(x + 0.09f, y);
    glVertex2f(x + 0.11f, y + 0.08f);
    glVertex2f(x + 0.07f, y + 0.16f);
    glVertex2f(x, y + 0.19f);
    glVertex2f(x - 0.07f, y + 0.16f);
    glVertex2f(x - 0.11f, y + 0.08f);
    glEnd();

    // Head
    glColor3f(0.95f, 0.97f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(x - 0.065f, y + 0.17f);
    glVertex2f(x + 0.065f, y + 0.17f);
    glVertex2f(x + 0.075f, y + 0.23f);
    glVertex2f(x + 0.04f, y + 0.29f);
    glVertex2f(x - 0.04f, y + 0.29f);
    glVertex2f(x - 0.075f, y + 0.23f);
    glEnd();

    // Gray shadow outline around the head
    glColor3f(0.48f, 0.53f, 0.58f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x - 0.065f, y + 0.17f);
    glVertex2f(x + 0.065f, y + 0.17f);
    glVertex2f(x + 0.075f, y + 0.23f);
    glVertex2f(x + 0.04f, y + 0.29f);
    glVertex2f(x - 0.04f, y + 0.29f);
    glVertex2f(x - 0.075f, y + 0.23f);
    glEnd();
    glLineWidth(1.0f);

    // Eyes
    glPointSize(3.0f);
    glColor3f(0.08f, 0.08f, 0.10f);

    glBegin(GL_POINTS);
    glVertex2f(x - 0.025f, y + 0.245f);
    glVertex2f(x + 0.025f, y + 0.245f);
    glEnd();

    // Carrot nose
    glColor3f(1.0f, 0.45f, 0.05f);

    glBegin(GL_TRIANGLES);
    glVertex2f(x, y + 0.225f);
    glVertex2f(x, y + 0.245f);
    glVertex2f(x + 0.07f, y + 0.232f);
    glEnd();
}


void drawIceFormation(float x, float y)
{
    glColor3f(0.75f, 0.95f, 1.0f);

    glBegin(GL_TRIANGLES);
    glVertex2f(x - 0.10f, y);
    glVertex2f(x - 0.02f, y);
    glVertex2f(x - 0.05f, y + 0.20f);

    glVertex2f(x - 0.02f, y);
    glVertex2f(x + 0.08f, y);
    glVertex2f(x + 0.03f, y + 0.28f);

    glVertex2f(x + 0.05f, y);
    glVertex2f(x + 0.14f, y);
    glVertex2f(x + 0.10f, y + 0.16f);
    glEnd();
}


void drawPenguin(float x, float y)
{
    // Black body
    glColor3f(0.08f, 0.10f, 0.13f);

    glBegin(GL_POLYGON);
    glVertex2f(x - 0.08f, y);
    glVertex2f(x + 0.08f, y);
    glVertex2f(x + 0.10f, y + 0.13f);
    glVertex2f(x + 0.07f, y + 0.25f);
    glVertex2f(x, y + 0.30f);
    glVertex2f(x - 0.07f, y + 0.25f);
    glVertex2f(x - 0.10f, y + 0.13f);
    glEnd();

    // White belly
    glColor3f(0.92f, 0.95f, 0.96f);

    glBegin(GL_POLYGON);
    glVertex2f(x - 0.045f, y + 0.04f);
    glVertex2f(x + 0.045f, y + 0.04f);
    glVertex2f(x + 0.055f, y + 0.14f);
    glVertex2f(x, y + 0.20f);
    glVertex2f(x - 0.055f, y + 0.14f);
    glEnd();

    // Eyes
    glPointSize(3.0f);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_POINTS);
    glVertex2f(x - 0.025f, y + 0.245f);
    glVertex2f(x + 0.025f, y + 0.245f);
    glEnd();

    // Beak
    glColor3f(1.0f, 0.55f, 0.10f);

    glBegin(GL_TRIANGLES);
    glVertex2f(x - 0.025f, y + 0.215f);
    glVertex2f(x + 0.025f, y + 0.215f);
    glVertex2f(x, y + 0.18f);
    glEnd();
}


// === GROUND ENVIRONMENTS ===

void drawGrassGround()
{
    // Base terrain
    glColor3f(0.25f, 0.65f, 0.25f);

    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, -0.4f);
    glVertex2f(-1.0f, -0.4f);
    glEnd();

    // Darker grassy mound
    glColor3f(0.18f, 0.55f, 0.18f);

    glBegin(GL_POLYGON);
    glVertex2f(-1.00f, -0.78f);
    glVertex2f(-0.82f, -0.68f);
    glVertex2f(-0.62f, -0.73f);
    glVertex2f(-0.48f, -0.80f);

    glVertex2f(-0.48f, -1.00f);
    glVertex2f(-1.00f, -1.00f);
    glEnd();

    // Decorations
    drawTree(-0.75f, -0.80f);
    drawTree(0.75f, -0.82f);

    drawGrassTuft(-0.48f, -0.92f);
    drawGrassTuft(0.48f, -0.90f);

    drawFlower(-0.58f, -0.90f);
    drawFlower(0.60f, -0.91f);

    drawRock(0.88f, -0.93f);
}


void drawDesertGround()
{
    // Base terrain
    glColor3f(0.88f, 0.72f, 0.42f);

    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, -0.4f);
    glVertex2f(-1.0f, -0.4f);
    glEnd();

    // Left dune
    glColor3f(0.95f, 0.80f, 0.50f);

    glBegin(GL_POLYGON);
    glVertex2f(-1.00f, -0.82f);
    glVertex2f(-0.82f, -0.70f);
    glVertex2f(-0.62f, -0.74f);
    glVertex2f(-0.45f, -0.82f);

    glVertex2f(-0.45f, -1.00f);
    glVertex2f(-1.00f, -1.00f);
    glEnd();

    // Right dune
    glColor3f(0.82f, 0.64f, 0.34f);

    glBegin(GL_POLYGON);
    glVertex2f(0.48f, -0.84f);
    glVertex2f(0.70f, -0.72f);
    glVertex2f(0.88f, -0.76f);
    glVertex2f(1.00f, -0.83f);

    glVertex2f(1.00f, -1.00f);
    glVertex2f(0.48f, -1.00f);
    glEnd();

    // Decorations
    drawCactus(-0.75f, -0.82f);
    drawCactus(0.78f, -0.86f);

    drawRock(-0.48f, -0.94f);
    drawRock(0.50f, -0.93f);
}


void drawSnowGround()
{
    // Base terrain
    glColor3f(0.92f, 0.95f, 1.0f);

    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, -0.4f);
    glVertex2f(-1.0f, -0.4f);
    glEnd();

    // Left snowbank
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_POLYGON);
    glVertex2f(-1.00f, -0.82f);
    glVertex2f(-0.85f, -0.72f);
    glVertex2f(-0.68f, -0.76f);
    glVertex2f(-0.50f, -0.84f);

    glVertex2f(-0.50f, -1.00f);
    glVertex2f(-1.00f, -1.00f);
    glEnd();

    // Right snowbank
    glBegin(GL_POLYGON);
    glVertex2f(0.52f, -0.85f);
    glVertex2f(0.70f, -0.75f);
    glVertex2f(0.87f, -0.73f);
    glVertex2f(1.00f, -0.82f);

    glVertex2f(1.00f, -1.00f);
    glVertex2f(0.52f, -1.00f);
    glEnd();

    // Decorations
    drawSnowyPine(-0.76f, -0.82f);
    drawSnowyPine(0.78f, -0.84f);

    drawSnowman(0.55f, -0.94f);
}


void drawIceGround()
{
    // Base terrain
    glColor3f(0.55f, 0.82f, 0.90f);

    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, -0.4f);
    glVertex2f(-1.0f, -0.4f);
    glEnd();

    // Ice cracks
    glColor3f(0.25f, 0.58f, 0.70f);

    glBegin(GL_LINES);
    // Left crack
    glVertex2f(-0.45f, -0.82f);
    glVertex2f(-0.32f, -0.90f);

    glVertex2f(-0.32f, -0.90f);
    glVertex2f(-0.20f, -0.86f);

    glVertex2f(-0.32f, -0.90f);
    glVertex2f(-0.26f, -0.98f);

    // Right crack
    glVertex2f(0.15f, -0.78f);
    glVertex2f(0.28f, -0.87f);

    glVertex2f(0.28f, -0.87f);
    glVertex2f(0.39f, -0.83f);

    glVertex2f(0.28f, -0.87f);
    glVertex2f(0.34f, -0.96f);
    glEnd();

    // Decorations
    drawIceFormation(-0.73f, -0.95f);
    drawIceFormation(0.72f, -0.95f);

    drawPenguin(0.55f, -0.94f);
}


// === GROUND SELECTION ===

void drawGround()
{
    if (ground == 0)
        drawGrassGround();

    else if (ground == 1)
        drawDesertGround();

    else if (ground == 2)
        drawSnowGround();

    else if (ground == 3)
        drawIceGround();
}


// === BUILDINGS ===

void drawHouse()
{
    // Main wall
    glColor3f(0.72f, 0.24f, 0.20f);
    glBegin(GL_QUADS);
    glVertex2f(-0.28f, -0.75f);
    glVertex2f(0.28f, -0.75f);
    glVertex2f(0.28f, -0.40f);
    glVertex2f(-0.28f, -0.40f);
    glEnd();

    // Roof
    glColor3f(0.18f, 0.20f, 0.28f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.36f, -0.40f);
    glVertex2f(0.36f, -0.40f);
    glVertex2f(0.00f, -0.12f);
    glEnd();

    // Chimney
    glColor3f(0.38f, 0.20f, 0.16f);
    glBegin(GL_QUADS);
    glVertex2f(0.17f, -0.29f);
    glVertex2f(0.23f, -0.29f);
    glVertex2f(0.23f, -0.13f);
    glVertex2f(0.17f, -0.13f);
    glEnd();

    // Window frames
    glColor3f(0.95f, 0.92f, 0.82f);
    glBegin(GL_QUADS);
    // Left frame
    glVertex2f(-0.225f, -0.585f);
    glVertex2f(-0.085f, -0.585f);
    glVertex2f(-0.085f, -0.445f);
    glVertex2f(-0.225f, -0.445f);

    // Right frame
    glVertex2f(0.085f, -0.585f);
    glVertex2f(0.225f, -0.585f);
    glVertex2f(0.225f, -0.445f);
    glVertex2f(0.085f, -0.445f);
    glEnd();

    // Windows
    glColor3f(1.0f, 0.78f, 0.28f);
    glBegin(GL_QUADS);
    glVertex2f(-0.21f, -0.57f);
    glVertex2f(-0.10f, -0.57f);
    glVertex2f(-0.10f, -0.46f);
    glVertex2f(-0.21f, -0.46f);

    glVertex2f(0.10f, -0.57f);
    glVertex2f(0.21f, -0.57f);
    glVertex2f(0.21f, -0.46f);
    glVertex2f(0.10f, -0.46f);
    glEnd();

    // Window dividers
    glColor3f(0.35f, 0.20f, 0.15f);
    glBegin(GL_LINES);
    glVertex2f(-0.155f, -0.57f);
    glVertex2f(-0.155f, -0.46f);
    glVertex2f(-0.21f, -0.515f);
    glVertex2f(-0.10f, -0.515f);

    glVertex2f(0.155f, -0.57f);
    glVertex2f(0.155f, -0.46f);
    glVertex2f(0.10f, -0.515f);
    glVertex2f(0.21f, -0.515f);
    glEnd();

    // Door frame
    glColor3f(0.92f, 0.88f, 0.76f);
    glBegin(GL_QUADS);
    glVertex2f(-0.08f, -0.75f);
    glVertex2f(0.08f, -0.75f);
    glVertex2f(0.08f, -0.525f);
    glVertex2f(-0.08f, -0.525f);
    glEnd();

    // Door
    glColor3f(0.30f, 0.15f, 0.08f);
    glBegin(GL_QUADS);
    glVertex2f(-0.065f, -0.75f);
    glVertex2f(0.065f, -0.75f);
    glVertex2f(0.065f, -0.54f);
    glVertex2f(-0.065f, -0.54f);
    glEnd();

    // Doorknob
    glPointSize(4.0f);
    glColor3f(0.95f, 0.75f, 0.20f);
    glBegin(GL_POINTS);
    glVertex2f(0.04f, -0.65f);
    glEnd();

    // Front step
    glColor3f(0.40f, 0.40f, 0.42f);
    glBegin(GL_QUADS);
    glVertex2f(-0.10f, -0.79f);
    glVertex2f(0.10f, -0.79f);
    glVertex2f(0.10f, -0.75f);
    glVertex2f(-0.10f, -0.75f);
    glEnd();
}


void drawCabin()
{
    // Main wooden body
    glColor3f(0.38f, 0.20f, 0.07f);
    glBegin(GL_QUADS);
    glVertex2f(-0.36f, -0.75f);
    glVertex2f(0.36f, -0.75f);
    glVertex2f(0.36f, -0.43f);
    glVertex2f(-0.36f, -0.43f);
    glEnd();

    // Large roof with overhang
    glColor3f(0.16f, 0.09f, 0.05f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.45f, -0.43f);
    glVertex2f(0.45f, -0.43f);
    glVertex2f(0.00f, -0.14f);
    glEnd();

    // Stone-style chimney
    glColor3f(0.32f, 0.30f, 0.28f);
    glBegin(GL_QUADS);
    glVertex2f(0.22f, -0.30f);
    glVertex2f(0.30f, -0.30f);
    glVertex2f(0.30f, -0.10f);
    glVertex2f(0.22f, -0.10f);
    glEnd();

    // Horizontal log lines
    glColor3f(0.20f, 0.10f, 0.03f);
    glBegin(GL_LINES);
    glVertex2f(-0.36f, -0.49f);
    glVertex2f(0.36f, -0.49f);

    glVertex2f(-0.36f, -0.56f);
    glVertex2f(0.36f, -0.56f);

    glVertex2f(-0.36f, -0.63f);
    glVertex2f(0.36f, -0.63f);

    glVertex2f(-0.36f, -0.70f);
    glVertex2f(0.36f, -0.70f);
    glEnd();

    // Window frames
    glColor3f(0.18f, 0.09f, 0.03f);
    glBegin(GL_QUADS);
    glVertex2f(-0.29f, -0.635f);
    glVertex2f(-0.135f, -0.635f);
    glVertex2f(-0.135f, -0.485f);
    glVertex2f(-0.29f, -0.485f);

    glVertex2f(0.135f, -0.635f);
    glVertex2f(0.29f, -0.635f);
    glVertex2f(0.29f, -0.485f);
    glVertex2f(0.135f, -0.485f);
    glEnd();

    // Warm windows
    glColor3f(1.0f, 0.68f, 0.20f);
    glBegin(GL_QUADS);
    glVertex2f(-0.27f, -0.615f);
    glVertex2f(-0.155f, -0.615f);
    glVertex2f(-0.155f, -0.505f);
    glVertex2f(-0.27f, -0.505f);

    glVertex2f(0.155f, -0.615f);
    glVertex2f(0.27f, -0.615f);
    glVertex2f(0.27f, -0.505f);
    glVertex2f(0.155f, -0.505f);
    glEnd();

    // Door
    glColor3f(0.16f, 0.08f, 0.03f);
    glBegin(GL_QUADS);
    glVertex2f(-0.075f, -0.75f);
    glVertex2f(0.075f, -0.75f);
    glVertex2f(0.075f, -0.53f);
    glVertex2f(-0.075f, -0.53f);
    glEnd();

    // Door knob
    glPointSize(4.0f);
    glColor3f(0.85f, 0.60f, 0.15f);
    glBegin(GL_POINTS);
    glVertex2f(0.045f, -0.65f);
    glEnd();
}


void drawObservatory()
{
    // Dark base / foundation
    glColor3f(0.42f, 0.46f, 0.50f);
    glBegin(GL_QUADS);
    glVertex2f(-0.36f, -0.78f);
    glVertex2f(0.36f, -0.78f);
    glVertex2f(0.36f, -0.72f);
    glVertex2f(-0.36f, -0.72f);
    glEnd();


    // Large pale observatory dome
    glColor3f(0.90f, 0.92f, 0.94f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.34f, -0.72f);
    glVertex2f(-0.33f, -0.58f);
    glVertex2f(-0.29f, -0.44f);
    glVertex2f(-0.22f, -0.32f);
    glVertex2f(-0.13f, -0.24f);
    glVertex2f(-0.05f, -0.20f);

    glVertex2f(0.05f, -0.20f);
    glVertex2f(0.13f, -0.24f);
    glVertex2f(0.22f, -0.32f);
    glVertex2f(0.29f, -0.44f);
    glVertex2f(0.33f, -0.58f);
    glVertex2f(0.34f, -0.72f);
    glEnd();


    // Geometric panel lines on the dome
    glColor3f(0.48f, 0.53f, 0.58f);
    glBegin(GL_LINES);

    // Horizontal panel divisions
    glVertex2f(-0.31f, -0.50f);
    glVertex2f(0.31f, -0.50f);

    glVertex2f(-0.25f, -0.37f);
    glVertex2f(0.25f, -0.37f);

    // Vertical / diagonal panel divisions
    glVertex2f(-0.20f, -0.72f);
    glVertex2f(-0.13f, -0.24f);

    glVertex2f(0.20f, -0.72f);
    glVertex2f(0.13f, -0.24f);

    glVertex2f(-0.31f, -0.50f);
    glVertex2f(-0.05f, -0.20f);

    glVertex2f(0.31f, -0.50f);
    glVertex2f(0.05f, -0.20f);

    // Cross diagonals to suggest geodesic panels
    glVertex2f(-0.31f, -0.50f);
    glVertex2f(-0.20f, -0.72f);

    glVertex2f(-0.25f, -0.37f);
    glVertex2f(0.00f, -0.50f);

    glVertex2f(0.25f, -0.37f);
    glVertex2f(0.00f, -0.50f);

    glVertex2f(0.31f, -0.50f);
    glVertex2f(0.20f, -0.72f);

    glEnd();


    // Narrow dome opening / slit
    glColor3f(0.20f, 0.25f, 0.32f);
    glBegin(GL_QUADS);
    glVertex2f(-0.035f, -0.50f);
    glVertex2f(0.035f, -0.50f);
    glVertex2f(0.025f, -0.20f);
    glVertex2f(-0.025f, -0.20f);
    glEnd();


    // Filled slanted telescope tube
    glColor3f(0.40f, 0.44f, 0.48f);
    glBegin(GL_QUADS);
    glVertex2f(-0.01f, -0.27f);
    glVertex2f(0.025f, -0.24f);
    glVertex2f(0.18f, -0.07f);
    glVertex2f(0.14f, -0.11f);
    glEnd();


    // Wider telescope end
    glColor3f(0.20f, 0.23f, 0.27f);
    glBegin(GL_QUADS);
    glVertex2f(0.14f, -0.11f);
    glVertex2f(0.18f, -0.07f);
    glVertex2f(0.22f, -0.09f);
    glVertex2f(0.17f, -0.14f);
    glEnd();


    // Small entrance at the base
    glColor3f(0.32f, 0.36f, 0.40f);
    glBegin(GL_QUADS);
    glVertex2f(-0.055f, -0.72f);
    glVertex2f(0.055f, -0.72f);
    glVertex2f(0.055f, -0.57f);
    glVertex2f(-0.055f, -0.57f);
    glEnd();


    // Small entrance highlight
    glColor3f(0.60f, 0.82f, 0.90f);
    glBegin(GL_QUADS);
    glVertex2f(-0.035f, -0.66f);
    glVertex2f(0.035f, -0.66f);
    glVertex2f(0.035f, -0.59f);
    glVertex2f(-0.035f, -0.59f);
    glEnd();

    // Dome outline, matching the geometric panel lines
    glColor3f(0.48f, 0.53f, 0.58f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.34f, -0.72f);
    glVertex2f(-0.33f, -0.58f);
    glVertex2f(-0.29f, -0.44f);
    glVertex2f(-0.22f, -0.32f);
    glVertex2f(-0.13f, -0.24f);
    glVertex2f(-0.05f, -0.20f);
    glVertex2f(0.05f, -0.20f);
    glVertex2f(0.13f, -0.24f);
    glVertex2f(0.22f, -0.32f);
    glVertex2f(0.29f, -0.44f);
    glVertex2f(0.33f, -0.58f);
    glVertex2f(0.34f, -0.72f);
    glEnd();
    glLineWidth(1.0f);
}

void drawLighthouse()
{
    glPushMatrix();
    glScalef(0.75f, 1.0f, 1.0f);

    // Main tapered tower
    glColor3f(0.92f, 0.90f, 0.82f);
    glBegin(GL_QUADS);
    glVertex2f(-0.18f, -0.75f);
    glVertex2f(0.18f, -0.75f);
    glVertex2f(0.11f, -0.22f);
    glVertex2f(-0.11f, -0.22f);
    glEnd();

    // Lower red stripe
    glColor3f(0.75f, 0.16f, 0.14f);
    glBegin(GL_QUADS);
    glVertex2f(-0.165f, -0.63f);
    glVertex2f(0.165f, -0.63f);
    glVertex2f(0.15f, -0.51f);
    glVertex2f(-0.15f, -0.51f);
    glEnd();

    // Upper red stripe
    glColor3f(0.75f, 0.16f, 0.14f);
    glBegin(GL_QUADS);
    glVertex2f(-0.135f, -0.39f);
    glVertex2f(0.135f, -0.39f);
    glVertex2f(0.12f, -0.29f);
    glVertex2f(-0.12f, -0.29f);
    glEnd();

    // Lantern platform
    glColor3f(0.18f, 0.20f, 0.22f);
    glBegin(GL_QUADS);
    glVertex2f(-0.16f, -0.24f);
    glVertex2f(0.16f, -0.24f);
    glVertex2f(0.16f, -0.20f);
    glVertex2f(-0.16f, -0.20f);
    glEnd();

    // Lantern room
    glColor3f(1.0f, 0.78f, 0.20f);
    glBegin(GL_QUADS);
    glVertex2f(-0.11f, -0.20f);
    glVertex2f(0.11f, -0.20f);
    glVertex2f(0.11f, -0.06f);
    glVertex2f(-0.11f, -0.06f);
    glEnd();

    // Lantern room frame lines
    glColor3f(0.16f, 0.18f, 0.20f);
    glBegin(GL_LINES);
    glVertex2f(-0.055f, -0.20f);
    glVertex2f(-0.055f, -0.06f);

    glVertex2f(0.0f, -0.20f);
    glVertex2f(0.0f, -0.06f);

    glVertex2f(0.055f, -0.20f);
    glVertex2f(0.055f, -0.06f);
    glEnd();

    // Lighthouse roof
    glColor3f(0.18f, 0.20f, 0.22f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.15f, -0.06f);
    glVertex2f(0.15f, -0.06f);
    glVertex2f(0.00f, 0.08f);
    glEnd();

    // Door frame
    glColor3f(0.55f, 0.55f, 0.52f);
    glBegin(GL_QUADS);
    glVertex2f(-0.075f, -0.75f);
    glVertex2f(0.075f, -0.75f);
    glVertex2f(0.075f, -0.59f);
    glVertex2f(-0.075f, -0.59f);
    glEnd();

    // Door
    glColor3f(0.28f, 0.18f, 0.13f);
    glBegin(GL_QUADS);
    glVertex2f(-0.055f, -0.75f);
    glVertex2f(0.055f, -0.75f);
    glVertex2f(0.055f, -0.61f);
    glVertex2f(-0.055f, -0.61f);
    glEnd();

    // Small tower windows
    glColor3f(0.35f, 0.65f, 0.82f);
    glBegin(GL_QUADS);
    glVertex2f(-0.04f, -0.48f);
    glVertex2f(0.04f, -0.48f);
    glVertex2f(0.04f, -0.41f);
    glVertex2f(-0.04f, -0.41f);
    glEnd();

    glPopMatrix();
}

void drawBuilding()
{
    if (building == 1)
        drawHouse();

    else if (building == 2)
        drawCabin();

    else if (building == 3)
        drawObservatory();

    else if (building == 4)
        drawLighthouse();
}



// === SKY ===

void drawSky()
{
    if (timeOfDay == 0) // Day
        glColor3f(0.5f, 0.8f, 1.0f);

    else if (timeOfDay == 1) // Sunset
        glColor3f(1.0f, 0.5f, 0.3f);

    else if (timeOfDay == 2) // Twilight
        glColor3f(0.35f, 0.3f, 0.6f);

    else if (timeOfDay == 3) // Night
        glColor3f(0.05f, 0.08f, 0.2f);

    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.4f);
    glVertex2f(1.0f, -0.4f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();
}

// === SKY FEATURES ===

void drawStar(float x, float y)
{
    glBegin(GL_TRIANGLE_FAN);

    // Center point to make the star fill correctly
    glVertex2f(x, y);

    // Top point
    glVertex2f(x, y + 0.04f);

    // Upper-right inner point
    glVertex2f(x + 0.01f, y + 0.012f);

    // Right point
    glVertex2f(x + 0.04f, y + 0.012f);

    // Lower-right inner point
    glVertex2f(x + 0.016f, y - 0.006f);

    // Bottom-right point
    glVertex2f(x + 0.025f, y - 0.035f);

    // Bottom inner point
    glVertex2f(x, y - 0.017f);

    // Bottom-left point
    glVertex2f(x - 0.025f, y - 0.035f);

    // Lower-left inner point
    glVertex2f(x - 0.016f, y - 0.006f);

    // Left point
    glVertex2f(x - 0.04f, y + 0.012f);

    // Upper-left inner point
    glVertex2f(x - 0.01f, y + 0.012f);

    // Close back to top point
    glVertex2f(x, y + 0.04f);

    glEnd();
}

void drawStars()
{
    glColor3f(1.0f, 1.0f, 0.85f);

    // Ordinary arrays + loop for repeated star positions
    for (int i = 0; i < STAR_COUNT; i++)
    {
        drawStar(starX[i], starY[i]);
    }
}
void drawMoon()
{
    float centerX = 0.60f;
    float centerY = 0.52f;
    float radius = 0.12f;

    const float PI = 3.1415926f;

    glColor3f(1.0f, 0.95f, 0.70f);

    glBegin(GL_POLYGON);

    for (int i = 0; i < 100; i++)
    {
        float angle = 2.0f * PI * i / 100;

        float x = centerX + radius * cosf(angle);
        float y = centerY + radius * sinf(angle);

        glVertex2f(x, y);
    }

    glEnd();
}

void drawAurora()
{
    // Module 1: smoothly interpolate the vertex colors
    glShadeModel(GL_SMOOTH);

    // Module 4: enable vertex array and color array
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    // Module 4: connect the arrays to OpenGL
    glVertexPointer(2, GL_FLOAT, 0, auroraVertices);
    glColorPointer(3, GL_FLOAT, 0, auroraColors);


    // Draw the 8 Aurora strips from the same arrays
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 24, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 28, 4);


    // Disable the arrays after drawing
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}




void drawSkyFeature()
{

    // skyFeature == 0 = Clear

    if (skyFeature == 1) // Starry
        drawStars();

    else if (skyFeature == 2) // Moon
        drawMoon();


    else if (skyFeature == 3) // Aurora
        drawAurora();


}

// === WEATHER ===

void drawClouds()
{
    glColor3f(0.85f, 0.87f, 0.90f);

    // Left cloud
    glBegin(GL_POLYGON);

    glVertex2f(-0.85f + cloudMove, 0.65f);
    glVertex2f(-0.80f + cloudMove, 0.75f);
    glVertex2f(-0.70f + cloudMove, 0.80f);
    glVertex2f(-0.60f + cloudMove, 0.75f);
    glVertex2f(-0.55f + cloudMove, 0.65f);
    glEnd();

    // Middle cloud
    glBegin(GL_POLYGON);
    glVertex2f(-0.20f + cloudMove, 0.55f);
    glVertex2f(-0.15f + cloudMove, 0.65f);
    glVertex2f(-0.05f + cloudMove, 0.70f);
    glVertex2f(0.05f + cloudMove, 0.65f);
    glVertex2f(0.10f + cloudMove, 0.55f);
    glEnd();

    // Right cloud
    glBegin(GL_POLYGON);
    glVertex2f(0.55f + cloudMove, 0.70f);
    glVertex2f(0.60f + cloudMove, 0.80f);
    glVertex2f(0.70f + cloudMove, 0.85f);
    glVertex2f(0.80f + cloudMove, 0.80f);
    glVertex2f(0.85f + cloudMove, 0.70f);
    glEnd();
}



void drawRain()
{
    glColor3f(0.45f, 0.70f, 0.95f);

    glLineWidth(4.0f);
    glBegin(GL_LINES);

    for (int i = 0; i < RAIN_COUNT; i++)
    {
        // Top of raindrop
        glVertex2f(rainX[i], rainY[i]);

        // Bottom of raindrop
        // Slight X difference makes the rain diagonal
        glVertex2f(rainX[i] - 0.025f,
            rainY[i] - 0.10f);
    }

    glEnd();

    // Reset line width for other GL_LINES
    glLineWidth(1.0f);
}



void drawSnowfall()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(5.0f);

    glBegin(GL_POINTS);

    for (int i = 0; i < SNOW_COUNT; i++)
    {
        glVertex2f(snowX[i], snowY[i]);
    }

    glEnd();

    glPointSize(1.0f);
}



void drawWeather()
{
    if (weather == 1)          // Cloudy
        drawClouds();

    else if (weather == 2)     // Rain
        drawRain();

    else if (weather == 3)     // Snowfall
        drawSnowfall();

    // weather == 0 means Clear
}






// === ANIMATION FUNCTIONS ===

void updateAurora()
{
    // Animate selected lower Y coordinates only.
    // X coordinates and the upper edge stay fixed.

    // Strip 1
    auroraVertices[3] += auroraMove;
    auroraVertices[7] += auroraMove;

    // Strip 2 - opposite direction
    auroraVertices[11] -= auroraMove;
    auroraVertices[15] -= auroraMove;

    // Strip 4
    auroraVertices[27] += auroraMove;
    auroraVertices[31] += auroraMove;

    // Strip 6 - opposite direction
    auroraVertices[43] -= auroraMove;
    auroraVertices[47] -= auroraMove;

    // Strip 8
    auroraVertices[59] += auroraMove;
    auroraVertices[63] += auroraMove;


    // Track how far the Aurora has moved
    auroraOffset += auroraMove;

    // Reverse the movement direction at the limit
    if (auroraOffset >= AURORA_MOVE_LIMIT ||
        auroraOffset <= -AURORA_MOVE_LIMIT)
    {
        auroraMove = -auroraMove;
    }
}

void updateSnowfall()
{
    for (int i = 0; i < SNOW_COUNT; i++)
    {
        // Move the snowflake downward
        snowY[i] -= SNOW_SPEED;

        // If it falls below the screen,
        // return it to the top
        if (snowY[i] < -1.0f)
        {
            snowY[i] = 1.0f;
        }
    }
}


void updateClouds()
{
    cloudMove += cloudSpeed;

    if (cloudMove >= CLOUD_MOVE_LIMIT ||
        cloudMove <= -CLOUD_MOVE_LIMIT)
    {
        cloudSpeed = -cloudSpeed;
    }
}



void updateRain()
{
    for (int i = 0; i < RAIN_COUNT; i++)
    {
        rainY[i] -= RAIN_SPEED;

        if (rainY[i] < -1.0f)
        {
            rainY[i] = 1.0f;
        }
    }
}


//shared timer function for all animations
void animationTimer(int value)
{
    bool animationUpdated = false;

    // Title and Instructions screen snowfall
    if (gameState == 0 || gameState == 1)
    {
        updateSnowfall();
        animationUpdated = true;
    }

    // Visual animations only update while Playing
    else if (gameState == 2)
    {
        // Aurora
        if (skyFeature == 3)
        {
            updateAurora();
            animationUpdated = true;
        }

        // Cloudy
        if (weather == 1)
        {
            updateClouds();
            animationUpdated = true;
        }

        // Rain
        if (weather == 2)
        {
            updateRain();
            animationUpdated = true;
        }

        // Snowfall
        if (weather == 3)
        {
            updateSnowfall();
            animationUpdated = true;
        }
    }

    if (animationUpdated)
    {
        glutPostRedisplay();
    }

    // Keep the timer alive for when Playing begins
    glutTimerFunc(ANIMATION_INTERVAL, animationTimer, 0);
}


void countdownTimer(int value)
{
    // Countdown only while Playing
    if (gameState == 2 && timeLeft > 0)
    {
        timeLeft--;

        // Automatically submit when time reaches 0
        if (timeLeft == 0)
        {
            submitObjective();
        }
        else
        {
            glutPostRedisplay();
        }
    }

    // Keep the countdown timer running
    glutTimerFunc(1000, countdownTimer, 0);
}

// ============= HUD =================

void drawText(float x, float y, const char* text, void* font)
{
    glRasterPos2f(x, y);

    for (int i = 0; text[i] != '\0'; i++)
    {
        glutBitmapCharacter(font, text[i]);
    }
}

void drawCenteredText(float centerX, float y, const char* text, void* font)
{
    int textWidth = glutBitmapLength(font,
        reinterpret_cast<const unsigned char*>(text));
    float textWidthInScene = (2.0f * textWidth) / 960.0f;

    drawText(centerX - (textWidthInScene / 2.0f), y, text, font);
}

const char* getGroundName()
{
    if (ground == 0)
        return "Grass";
    else if (ground == 1)
        return "Desert";
    else if (ground == 2)
        return "Snow";
    else
        return "Ice";
}


const char* getBuildingName()
{
    if (building == 0)
        return "None";
    else if (building == 1)
        return "House";
    else if (building == 2)
        return "Cabin";
    else if (building == 3)
        return "Observatory";
    else
        return "Lighthouse";
}

const char* getTimeOfDayName()
{
    if (timeOfDay == 0)
        return "Day";
    else if (timeOfDay == 1)
        return "Sunset";
    else if (timeOfDay == 2)
        return "Twilight";
    else
        return "Night";
}

const char* getSkyFeatureName()
{
    if (skyFeature == 0)
        return "Clear";
    else if (skyFeature == 1)
        return "Starry";
    else if (skyFeature == 2)
        return "Moon";
    else
        return "Aurora";
}

const char* getWeatherName()
{
    if (weather == 0)
        return "Clear";
    else if (weather == 1)
        return "Cloudy";
    else if (weather == 2)
        return "Rain";
    else
        return "Snowfall";
}





void drawHUD()
{

    // Find which predefined objective is active
    int activeObjective = objectiveOrder[currentObjective - 1];


    // Create the Objective number text
    char objectiveNumberText[30];

    sprintf(
        objectiveNumberText,
        "OBJECTIVE %d / %d",
        currentObjective,
        OBJECTIVE_COUNT
    );

    // Create the Time text
    char timeText[20];

    sprintf(
        timeText,
        "TIME: %d",
        timeLeft
    );


    // === TOP HEADER ===

    glColor3f(0.12f, 0.16f, 0.22f);

    glBegin(GL_QUADS);
    glVertex2f(-1.0f, 0.72f);
    glVertex2f(1.0f, 0.72f);
    glVertex2f(1.0f, 1.00f);
    glVertex2f(-1.0f, 1.00f);
    glEnd();


    // === HEADER TEXT ===

// Main information
    glColor3f(1.0f, 1.0f, 1.0f);

    drawText(-0.94f, 0.89f, objectiveNumberText, GLUT_BITMAP_HELVETICA_18);

    glColor3f(1.0f, 0.5f, 0.5f);
    drawText(0.72f, 0.89f, timeText, GLUT_BITMAP_HELVETICA_18);


    // Objective - smaller secondary text
    glColor3f(0.82f, 0.85f, 0.90f);

    // Objective label
    glColor3f(0.82f, 0.85f, 0.90f);

    drawText(
        -0.94f,
        0.78f,
        "Objective:",
        GLUT_BITMAP_HELVETICA_12
    );

    // Current shuffled objective
    drawText(
        -0.78f,
        0.78f,
        objectiveText[activeObjective],
        GLUT_BITMAP_HELVETICA_12
    );



    // Header separator
    glColor3f(0.65f, 0.70f, 0.75f);

    glBegin(GL_LINES);
    glVertex2f(-1.0f, 0.72f);
    glVertex2f(1.0f, 0.72f);
    glEnd();


    // === RIGHT SIDEBAR ===

    glColor3f(0.15f, 0.19f, 0.25f);

    glBegin(GL_QUADS);
    glVertex2f(0.68f, -0.68f);
    glVertex2f(1.00f, -0.68f);
    glVertex2f(1.00f, 0.72f);
    glVertex2f(0.68f, 0.72f);
    glEnd();


    // Sidebar separator
    glColor3f(0.65f, 0.70f, 0.75f);

    glBegin(GL_LINES);
    glVertex2f(0.68f, -0.68f);
    glVertex2f(0.68f, 0.72f);
    glEnd();


    // === CURRENT SCENE TEXT ===

    // Sidebar heading
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(0.72f, 0.61f,
        "CURRENT SCENE",
        GLUT_BITMAP_HELVETICA_12
    );


    // Ground
    glColor3f(0.65f, 0.70f, 0.76f);
    drawText(0.72f, 0.43f,
        "Ground",
        GLUT_BITMAP_HELVETICA_12
    );

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(0.72f, 0.35f,
        getGroundName(),
        GLUT_BITMAP_HELVETICA_12
    );


    // Building
    glColor3f(0.65f, 0.70f, 0.76f);
    drawText(0.72f, 0.20f,
        "Building",
        GLUT_BITMAP_HELVETICA_12
    );

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(
        0.72f,
        0.12f,
        getBuildingName(),
        GLUT_BITMAP_HELVETICA_12
    );


    // Time of Day
    glColor3f(0.65f, 0.70f, 0.76f);
    drawText(
        0.72f,
        -0.03f,
        "Time of Day",
        GLUT_BITMAP_HELVETICA_12
    );

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(
        0.72f,
        -0.11f,
        getTimeOfDayName(),
        GLUT_BITMAP_HELVETICA_12
    );


    // Sky Feature
    glColor3f(0.65f, 0.70f, 0.76f);
    drawText(
        0.72f,
        -0.26f,
        "Sky Feature",
        GLUT_BITMAP_HELVETICA_12
    );

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(
        0.72f,
        -0.34f,
        getSkyFeatureName(),
        GLUT_BITMAP_HELVETICA_12
    );


    // Weather
    glColor3f(0.65f, 0.70f, 0.76f);
    drawText(
        0.72f,
        -0.49f,
        "Weather",
        GLUT_BITMAP_HELVETICA_12
    );

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(
        0.72f,
        -0.57f,
        getWeatherName(),
        GLUT_BITMAP_HELVETICA_12
    );

    // === BOTTOM CONTROLS ===

    glColor3f(0.12f, 0.16f, 0.22f);

    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -1.00f);
    glVertex2f(1.0f, -1.00f);
    glVertex2f(1.0f, -0.68f);
    glVertex2f(-1.0f, -0.68f);
    glEnd();


    // Bottom separator
    glColor3f(0.65f, 0.70f, 0.75f);

    glBegin(GL_LINES);
    glVertex2f(-1.0f, -0.68f);
    glVertex2f(1.0f, -0.68f);
    glEnd();

    // === BOTTOM CONTROLS TEXT ===

    // Main heading
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(
        -0.94f,
        -0.76f,
        "CONTROLS",
        GLUT_BITMAP_HELVETICA_18
    );


    // === KEYBOARD ===

    glColor3f(0.65f, 0.70f, 0.76f);
    drawText(
        -0.94f,
        -0.84f,
        "KEYBOARD",
        GLUT_BITMAP_HELVETICA_12
    );


    // First row: Scene editing controls
    glColor3f(1.0f, 1.0f, 1.0f);

    drawText(
        -0.72f,
        -0.84f,
        "[A] Building",
        GLUT_BITMAP_HELVETICA_12
    );

    drawText(
        -0.50f,
        -0.84f,
        "[S] Time of Day",
        GLUT_BITMAP_HELVETICA_12
    );

    drawText(
        -0.32f,
        -0.84f,
        "[D] Weather",
        GLUT_BITMAP_HELVETICA_12
    );


    drawText(
        -0.72f,
        -0.91f,
        "[ENTER] Submit",
        GLUT_BITMAP_HELVETICA_12
    );


    drawText(
        -0.72f,
        -0.97f,
        "[ESC] Exit",
        GLUT_BITMAP_HELVETICA_12
    );


    // Second group: Pause / Restart
    drawText(
        -0.45f,
        -0.91f,
        "[P] Pause",
        GLUT_BITMAP_HELVETICA_12
    );



    // === MOUSE ===

    glColor3f(0.65f, 0.70f, 0.76f);
    drawText(
        0.20f,
        -0.84f,
        "MOUSE",
        GLUT_BITMAP_HELVETICA_12
    );

    glColor3f(1.0f, 1.0f, 1.0f);

    drawText(
        0.36f,
        -0.84f,
        "[Left Click] Ground",
        GLUT_BITMAP_HELVETICA_12
    );

    drawText(
        0.36f,
        -0.92f,
        "[Right Click] Sky Feature",
        GLUT_BITMAP_HELVETICA_12
    );

} // End of drawHUD()

void drawTitleScreen()
{
    // === BACKGROUND ===

    glColor3f(0.12f, 0.16f, 0.22f);

    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();


    // Reuse the existing animated snowfall
    drawSnowfall();


    // === MAIN TITLE ===
    // Scalable GLUT stroke text

    glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix();

    glTranslatef(-0.42f, 0.58f, 0.0f);
    glScalef(0.0015f, 0.0015f, 1.0f);

    const char* title = "SceneShift";

    for (int i = 0; title[i] != '\0'; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, title[i]);
    }

    glPopMatrix();


    // Subtitle
    glColor3f(0.75f, 0.82f, 0.88f);

    drawText(
        -0.17f,
        0.43f,
        "A Timed 2D Scene-Building Challenge",
        GLUT_BITMAP_HELVETICA_12
    );


    // === DECORATIVE MINI-SCENE ===

    // Simple ground/platform
    glColor3f(0.20f, 0.48f, 0.27f);

    glBegin(GL_QUADS);
    glVertex2f(-0.72f, -0.30f);
    glVertex2f(0.72f, -0.30f);
    glVertex2f(0.72f, -0.20f);
    glVertex2f(-0.72f, -0.20f);
    glEnd();


    // Reuse the existing House as a decorative object
    glPushMatrix();

    glTranslatef(0.0f, 0.24f, 0.0f);
    glScalef(0.55f, 0.55f, 1.0f);

    drawHouse();

    glPopMatrix();


    // === PLAY ===

    glColor3f(1.0f, 0.7f, 0.7f);

    drawText(
        -0.11f,
        -0.60f,
        "[ENTER] PLAY",
        GLUT_BITMAP_HELVETICA_18
    );
}

void drawInstructionsScreen()
{
    // ==================== BACKGROUND ====================

    glColor3f(0.12f, 0.16f, 0.22f);

    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();

    // Decorative snow on the instructions screen
    drawSnowfall();


    // ==================== TITLE ====================

    glColor3f(1.0f, 1.0f, 1.0f);

    drawCenteredText(
        0.0f,
        0.76f,
        "HOW TO PLAY",
        GLUT_BITMAP_HELVETICA_18
    );


    // ==================== INSTRUCTIONS ====================

    glColor3f(0.82f, 0.85f, 0.90f);

    drawCenteredText(
        0.0f,
        0.60f,
        "Recreate the target scene before time runs out.",
        GLUT_BITMAP_HELVETICA_12
    );

    drawCenteredText(
        0.0f,
        0.50f,
        "Each round gives you a scene objective. Change the five",
        GLUT_BITMAP_HELVETICA_12
    );

    drawCenteredText(
        0.0f,
        0.41f,
        "scene categories until your environment matches the target:",
        GLUT_BITMAP_HELVETICA_12
    );

    glColor3f(0.85f, 0.75f, 1.0f);

    drawCenteredText(
        0.0f,
        0.30f,
        "Ground | Building | Time of Day | Sky Feature | Weather",
        GLUT_BITMAP_HELVETICA_12
    );

    glColor3f(0.82f, 0.85f, 0.90f);

    drawCenteredText(
        0.0f,
        0.18f,
        "When the timer reaches 0, your current scene is submitted.",
        GLUT_BITMAP_HELVETICA_12
    );


    // ==================== TOP SEPARATOR ====================

    glColor3f(0.55f, 0.60f, 0.66f);

    glBegin(GL_LINES);
    glVertex2f(-0.75f, 0.08f);
    glVertex2f(0.75f, 0.08f);
    glEnd();


    // ==================== INPUT GUIDE ====================

 // Keyboard heading
    glColor3f(0.65f, 0.70f, 0.76f);

    drawCenteredText(
        -0.35f,
        -0.02f,
        "KEYBOARD",
        GLUT_BITMAP_HELVETICA_12
    );


    // Keyboard controls
    glColor3f(1.0f, 1.0f, 1.0f);

    // Left keyboard column
    drawCenteredText(
        -0.52f,
        -0.14f,
        "[A] Building",
        GLUT_BITMAP_HELVETICA_12
    );

    drawCenteredText(
        -0.52f,
        -0.24f,
        "[S] Time of Day",
        GLUT_BITMAP_HELVETICA_12
    );

    drawCenteredText(
        -0.52f,
        -0.34f,
        "[D] Weather",
        GLUT_BITMAP_HELVETICA_12
    );


    // Right keyboard column
    drawCenteredText(
        -0.18f,
        -0.14f,
        "[ENTER] Submit",
        GLUT_BITMAP_HELVETICA_12
    );

    drawCenteredText(
        -0.18f,
        -0.24f,
        "[P] Pause / Resume",
        GLUT_BITMAP_HELVETICA_12
    );

    drawCenteredText(
        -0.18f,
        -0.34f,
        "[ESC] Exit",
        GLUT_BITMAP_HELVETICA_12
    );


    // Mouse heading
    glColor3f(0.65f, 0.70f, 0.76f);

    drawCenteredText(
        0.40f,
        -0.02f,
        "MOUSE",
        GLUT_BITMAP_HELVETICA_12
    );


    // Mouse controls
    glColor3f(1.0f, 1.0f, 1.0f);

    drawCenteredText(
        0.40f,
        -0.14f,
        "[Left Click] Ground",
        GLUT_BITMAP_HELVETICA_12
    );

    drawCenteredText(
        0.40f,
        -0.24f,
        "[Right Click] Sky Feature",
        GLUT_BITMAP_HELVETICA_12
    );

    // ==================== BOTTOM SEPARATOR ====================

    glColor3f(0.55f, 0.60f, 0.66f);

    glBegin(GL_LINES);
    glVertex2f(-0.75f, -0.45f);
    glVertex2f(0.75f, -0.45f);
    glEnd();


    // ==================== START ====================

    glColor3f(1.0f, 0.7f, 0.7f);

    drawCenteredText(
        0.0f,
        -0.66f,
        "[ENTER] START",
        GLUT_BITMAP_HELVETICA_18
    );
}


void drawPauseOverlay()
{
    // Dark pause panel
    glColor3f(0.12f, 0.16f, 0.22f);

    glBegin(GL_QUADS);
    glVertex2f(-0.30f, -0.18f);
    glVertex2f(0.30f, -0.18f);
    glVertex2f(0.30f, 0.18f);
    glVertex2f(-0.30f, 0.18f);
    glEnd();


    // Pause title
    glColor3f(1.0f, 1.0f, 1.0f);

    drawText(
        -0.08f,
        0.05f,
        "PAUSED",
        GLUT_BITMAP_HELVETICA_18
    );


    // Resume instruction
    glColor3f(0.82f, 0.85f, 0.90f);

    drawText(
        -0.10f,
        -0.07f,
        "[P] Resume",
        GLUT_BITMAP_HELVETICA_12
    );
}


void evaluateObjective()
{
    // Find the predefined objective currently being played
    int activeObjective = objectiveOrder[currentObjective - 1];

    // Start the current objective score from zero
    objectiveScore = 0;

    // Ground
    if (ground == objectiveTargets[activeObjective][0])
        objectiveScore++;

    // Building
    if (building == objectiveTargets[activeObjective][1])
        objectiveScore++;

    // Time of Day
    if (timeOfDay == objectiveTargets[activeObjective][2])
        objectiveScore++;

    // Sky Feature
    if (skyFeature == objectiveTargets[activeObjective][3])
        objectiveScore++;

    // Weather
    if (weather == objectiveTargets[activeObjective][4])
        objectiveScore++;
}






void drawObjectiveResult()
{
    // ==================== BACKGROUND ====================

    glColor3f(0.12f, 0.16f, 0.22f);

    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();


    // ==================== TITLE ====================

    glColor3f(1.0f, 1.0f, 1.0f);

    drawText(
        -0.18f,
        0.55f,
        "OBJECTIVE RESULT",
        GLUT_BITMAP_HELVETICA_18
    );


    // ==================== OBJECTIVE NUMBER ====================

    char objectiveResultText[30];

    sprintf(
        objectiveResultText,
        "Objective %d / %d",
        currentObjective,
        OBJECTIVE_COUNT
    );

    glColor3f(0.82f, 0.85f, 0.90f);

    drawText(
        -0.12f,
        0.30f,
        objectiveResultText,
        GLUT_BITMAP_HELVETICA_12
    );


    // ==================== SCORE ====================

    // ==================== SCORES AND LIVES ====================

    char objectiveScoreText[30];
    char totalScoreText[30];
    char livesText[30];

    sprintf(
        objectiveScoreText,
        "Objective Score: %d / 5",
        objectiveScore
    );

    sprintf(
        totalScoreText,
        "Total Score: %d / 50",
        totalScore
    );

    sprintf(
        livesText,
        "Lives: %d / 3",
        lives
    );
    glColor3f(1.0f, 1.0f, 1.0f);

    drawText(
        -0.15f,
        0.12f,
        objectiveScoreText,
        GLUT_BITMAP_HELVETICA_12
    );

    drawText(
        -0.15f,
        0.02f,
        totalScoreText,
        GLUT_BITMAP_HELVETICA_12
    );

    drawText(
        -0.15f,
        -0.08f,
        livesText,
        GLUT_BITMAP_HELVETICA_12
    );


    // ==================== RESULT ====================

    if (objectiveScore == 5)
    {
        drawText(
            -0.08f,
            -0.30f,
            "PERFECT",
            GLUT_BITMAP_HELVETICA_18
        );
    }
    else if (objectiveScore == 4)
    {
        drawText(
            -0.05f,
            -0.30f,
            "PASS",
            GLUT_BITMAP_HELVETICA_18
        );
    }
    else
    {
        drawText(
            -0.04f,
            -0.30f,
            "FAIL",
            GLUT_BITMAP_HELVETICA_18
        );
    }

    // ==================== CONTINUE ====================

    glColor3f(0.82f, 0.85f, 0.90f);

    drawText(
        -0.08f,
        -0.52f,
        "[ENTER] Continue",
        GLUT_BITMAP_HELVETICA_18
    );

    drawText(
        -0.08f,
        -0.62f,
        "[ESC] Exit",
        GLUT_BITMAP_HELVETICA_18
    );

}


void updateObjectiveResult()
{
    // Add the current objective score to the total score
    totalScore += objectiveScore;

    // A score of 0 to 3 is a Fail, so lose one life
    if (objectiveScore <= 3)
    {
        lives--;
    }
}

void drawGameOver()
{
    // ==================== BACKGROUND ====================

    glColor3f(0.12f, 0.16f, 0.22f);

    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();


    // ==================== TITLE ====================

    glColor3f(1.0f, 1.0f, 1.0f);

    drawText(
        -0.10f,
        0.45f,
        "GAME OVER",
        GLUT_BITMAP_HELVETICA_18
    );


    // ==================== RESULTS ====================

    char completedText[40];
    char totalScoreText[40];

    sprintf(
        completedText,
        "Objectives Completed: %d / %d",
        currentObjective,
        OBJECTIVE_COUNT
    );

    sprintf(
        totalScoreText,
        "Total Match Score: %d / 50",
        totalScore
    );

    // Play Again
    glColor3f(1.0f, 1.0f, 1.0f);

    drawText(
        -0.12f,
        -0.30f,
        "[R] Play Again",
        GLUT_BITMAP_HELVETICA_18
    );


    drawText(
        -0.12f,
        -0.40f,
        "[ESC] Exit",
        GLUT_BITMAP_HELVETICA_18
    );



    glColor3f(0.82f, 0.85f, 0.90f);

    drawText(
        -0.20f,
        0.15f,
        completedText,
        GLUT_BITMAP_HELVETICA_12
    );

    drawText(
        -0.20f,
        0.02f,
        totalScoreText,
        GLUT_BITMAP_HELVETICA_12
    );
}




void drawFinalResults()
{
    // ==================== BACKGROUND ====================

    glColor3f(0.12f, 0.16f, 0.22f);

    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();


    // ==================== TITLE ====================

    glColor3f(1.0f, 1.0f, 1.0f);

    drawText(
        -0.13f,
        0.45f,
        "FINAL RESULTS",
        GLUT_BITMAP_HELVETICA_18
    );


    // ==================== RESULTS ====================

    char completedText[40];
    char totalScoreText[40];
    char livesText[40];

    sprintf(
        completedText,
        "Objectives Completed: %d / %d",
        OBJECTIVE_COUNT,
        OBJECTIVE_COUNT
    );

    sprintf(
        totalScoreText,
        "Total Match Score: %d / 50",
        totalScore
    );

    sprintf(
        livesText,
        "Lives Remaining: %d / 3",
        lives
    );

    // Play Again
    glColor3f(1.0f, 1.0f, 1.0f);

    drawText(
        -0.12f,
        -0.35f,
        "[R] Play Again",
        GLUT_BITMAP_HELVETICA_18
    );

    drawText(
        -0.12f,
        -0.45f,
        "[ESC] Exit",
        GLUT_BITMAP_HELVETICA_18
    );


    glColor3f(0.82f, 0.85f, 0.90f);

    drawText(
        -0.20f,
        0.15f,
        completedText,
        GLUT_BITMAP_HELVETICA_12
    );

    drawText(
        -0.20f,
        0.02f,
        totalScoreText,
        GLUT_BITMAP_HELVETICA_12
    );

    drawText(
        -0.20f,
        -0.11f,
        livesText,
        GLUT_BITMAP_HELVETICA_12
    );
}





// =========== DISPLAY ===========

void display()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    // === TITLE ===
    if (gameState == 0)
    {
        drawTitleScreen();
    }


    // === INSTRUCTIONS ===
    else if (gameState == 1)
    {
        drawInstructionsScreen();
    }


    // === PLAYING / PAUSED ===
    else if (gameState == 2 || gameState == 3)
    {
        // === SCENE ===

        glPushMatrix();

        // Fit the complete scene inside the HUD's visible scene area
        glTranslatef(-0.16f, 0.02f, 0.0f);
        glScalef(0.84f, 0.70f, 1.0f);

        drawSky();
        drawSkyFeature();
        drawGround();
        drawBuilding();
        drawWeather();

        glPopMatrix();


        // === HUD ===

        drawHUD();


        // === PAUSE OVERLAY ===

        if (gameState == 3)
        {
            drawPauseOverlay();
        }
    }


    // === OBJECTIVE RESULT ===
    else if (gameState == 4)
    {
        drawObjectiveResult();
    }

    // === GAME OVER ===
    else if (gameState == 5)
    {
        drawGameOver();
    }


    // === FINAL RESULTS ===
    else if (gameState == 6)
    {
        drawFinalResults();
    }


    glFlush();
}


// === KEYBOARD ===

void keyboard(unsigned char key, int x, int y)
{

    // ESC - Exit program
    if (key == 27)
    {
        exit(0);
    }

    // === TITLE ===
    if (gameState == 0)
    {
        if (key == 13) // ENTER
        {
            gameState = 1;
            glutPostRedisplay();
        }

        return;
    }



    // === INSTRUCTIONS ===
    if (gameState == 1)
    {
        if (key == 13) // ENTER
        {
            gameState = 2;

            // Prepare Objective 1
            startObjective();
        }

        return;
    }


    // === PAUSE WHILE PLAYING ===
    if (gameState == 2 && (key == 'p' || key == 'P'))
    {
        gameState = 3;
        glutPostRedisplay();
        return;
    }


    // === RESUME WHILE PAUSED ===
    if (gameState == 3 && (key == 'p' || key == 'P'))
    {
        gameState = 2;
        glutPostRedisplay();
        return;
    }


    // Ignore other keyboard controls while Paused
    if (gameState == 3)
    {
        return;
    }


    // === PLAYING CONTROLS ===
    if (gameState == 2)
    {
        // Time of Day
        if (key == 's' || key == 'S')
        {
            timeOfDay++;

            if (timeOfDay > 3)
                timeOfDay = 0;

            glutPostRedisplay();
        }

        // Building
        else if (key == 'a' || key == 'A')
        {
            building++;

            if (building > 4)
                building = 0;

            glutPostRedisplay();
        }

        // Weather
        else if (key == 'd' || key == 'D')
        {
            weather++;

            if (weather > 3)
                weather = 0;

            glutPostRedisplay();
        }

        // Submit Objective
        else if (key == 13) // ENTER
        {
            submitObjective();
        }

        return;
    }


    // === OBJECTIVE RESULT ===
    if (gameState == 4)
    {
        if (key == 13) // ENTER
        {
            // No lives remaining
            if (lives == 0)
            {
                gameState = 5; // Game Over
            }

            // All objectives completed
            else if (currentObjective == OBJECTIVE_COUNT)
            {
                gameState = 6; // Final Results
            }

            // Continue to next objective
            else
            {
                currentObjective++;

                // Reset scene and assign the correct starting time
                startObjective();

                gameState = 2; // Playing
            }

            glutPostRedisplay();
        }

        return;
    }


    // === GAME OVER / FINAL RESULTS ===
    if (gameState == 5 || gameState == 6)
    {
        if (key == 'r' || key == 'R')
        {
            restartGame();
        }

        return;
    }



}





// === MOUSE ===

// Left Click - change Ground
// Right Click - change Sky Feature
void mouse(int button, int state, int x, int y)
{
    // Mouse interaction only works while Playing
    if (gameState == 2 && state == GLUT_DOWN)
    {
        // Only allow interaction inside the visible scene area
        if (x <= 806 && y >= 101 && y <= 605)
        {
            // Left Click - Ground
            if (button == GLUT_LEFT_BUTTON)
            {
                ground++;

                if (ground > 3)
                    ground = 0;

                glutPostRedisplay();
            }

            // Right Click - Sky Feature
            else if (button == GLUT_RIGHT_BUTTON)
            {
                skyFeature++;

                if (skyFeature > 3)
                    skyFeature = 0;

                glutPostRedisplay();
            }
        }
    }
}

// === MAIN ===

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    srand(time(0)); // Seed the random number generator for snow and rain
    glutInitWindowSize(960, 720); // 4:3 aspect ratio helps prevent object stretching
    glutInitWindowPosition(100, 100);
    glutCreateWindow("SceneShift");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);



    // Start the shared visual animation timer
    glutTimerFunc(ANIMATION_INTERVAL, animationTimer, 0);

    // Game countdown timer
    glutTimerFunc(1000, countdownTimer, 0);


    // Randomize the objective order for this game
    shuffleObjectives();



    glutMainLoop();



    return 0;
}
