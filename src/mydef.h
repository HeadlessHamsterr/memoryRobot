#define X_ENABLE_PIN 13
#define X_STEP_PIN 12
#define X_DIR_PIN 11

#define Y_ENABLE_PIN 10
#define Y_STEP_PIN 9
#define Y_DIR_PIN 8

#define Z_ENABLE_PIN 7
#define Z_STEP_PIN 6
#define Z_DIR_PIN 5

#define X_ENDSTOP_PIN 2
#define Y_ENDSTOP_PIN 3
#define Z_ENDSTOP_PIN 4

#define SS_PIN 0
#define RST_PIN 0

#define STEPS_PER_MM_X 400
#define STEPS_PER_MM_Y 400
#define STEPS_PER_MM_Z 400

#define X_SIZE 200
#define Y_SIZE 200
#define Z_HEIGHT 200
#define BOARD_SIZE_X 4
#define BOARD_SIZE_Y 4
#define TOOL_OFFSET 0

#define MIN_X_POS 27
#define MAX_X_POS 222
#define MIN_Y_POS 43
#define MAX_Y_POS 334
#define X_POS_SCANNER 0
#define Y_POS_SCANNER 0
#define Z_POS_CARD 0
#define PARK_X 0
#define PARK_Y 0
#define PARK_Z 0
#define collectX 0
#define collectY 0

#define STEP_TIME 1000

volatile bool homingX = false;
volatile bool homingY = false;
volatile bool homingZ = false;

int currentX;
int currentY;
int currentZ;

bool XMoveFinished = false;
bool YMoveFinished = false;
bool ZMoveFinished = false;

const char* UID1 = "kaartType1";
const char* UID2 = "kaartType1";
const char* UID3 = "kaartType2";
const char* UID4 = "kaartType2";
const char* UID5 = "kaartType3";
const char* UID6 = "kaartType3";
const char* UID7 = "kaartType4";
const char* UID8 = "kaartType4";
const char* UID9 = "kaartType5";
const char* UID10 = "KaartType5";
const char* UID11 = "kaartType6";
const char* UID12 = "kaartType6";
const char* UID13 = "kaartType7";
const char* UID14 = "kaartType7";
const char* UID15 = "kaartType8";
const char* UID16 = "kaartType8";

const char* kaarten[4][4] = {
    {"empty1", "empty2", "empty3", "empty4"},
    {"empty5", "empty6", "empty7", "empty8"},
    {"empty9", "empty10", "empty11", "empty12"},
    {"empty13", "empty14", "empty15", "empty16"}
};

int kaartLocatiesX[] = {27, 92, 157, 222};
int kaartLocatiesY[] = {43, 140, 237, 334};