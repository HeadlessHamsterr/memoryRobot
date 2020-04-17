#define X_ENABLE_PIN 13
#define X_STEP_PIN 15
#define X_DIR_PIN 21

#define Y_ENABLE_PIN 10
#define Y_STEP_PIN 22
#define Y_DIR_PIN 23

#define Z_ENABLE_PIN 7
#define Z_STEP_PIN 3
#define Z_DIR_PIN 2

#define X_ENDSTOP_PIN 18
#define Y_ENDSTOP_PIN 19
#define Z_ENDSTOP_PIN 20

#define SS_PIN 0
#define RST_PIN 0
#define TEMP_0_PIN 7
#define TEMP_BED_PIN 6
#define HEATER_0_PIN 13
#define HEATER_BED_PIN 12
#define X_ENABLE_PIN 14
#define Y_ENABLE_PIN 14
#define Z_ENABLE_PIN 26
#define FAN_PIN 4
#define SDSS 31
#define LED_PIN 27

#define STEPS_PER_MM_X 50
#define STEPS_PER_MM_Y 37
#define STEPS_PER_MM_Z 400

#define X_SIZE 200
#define Y_SIZE 305
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

#define RADIUS_PULLEY_X 12.6
#define RADIUS_PULLEY_Y 12.6

int STEP_TIME_X = 1000;
int STEP_TIME_Y = 1000;
int STEP_TIME_Z = 1000;

volatile bool homingX = false;
volatile bool homingY = false;
volatile bool homingZ = false;

int currentX = 0;
int currentY = 0;
int currentZ = 0;

bool XMoveFinished = false;
bool YMoveFinished = false;
bool ZMoveFinished = false;

char kaarten[4][4] = {};

bool skip[4][4] = {
    {false, false, false, false},
    {false, false, false, false},
    {false, false, false, false},
    {false, false, false, false}
};

int kaartLocatiesX[] = {27, 92, 157, 222};
int kaartLocatiesY[] = {43, 140, 237, 334};

const char* cardUID1 = "cardUID1";
const char* cardUID2 = "cardUID2";
const char* cardUID3 = "cardUID3";
const char* cardUID4 = "cardUID4";
const char* cardUID5 = "cardUID5";
const char* cardUID6 = "cardUID6";
const char* cardUID7 = "cardUID7";
const char* cardUID8 = "cardUID8";
const char* cardUID9 = "cardUID9";
const char* cardUID10 = "cardUID10";
const char* cardUID11 = "cardUID11";
const char* cardUID12 = "cardUID12";
const char* cardUID13 = "cardUID13";
const char* cardUID14 = "cardUID14";
const char* cardUID15 = "cardUID15";
const char* cardUID16 = "cardUID16";

char cardType1 = 1;
char cardType2 = 2;
char cardType3 = 3;
char cardType4 = 4;
char cardType5 = 5;
char cardType6 = 6;
char cardType7 = 7;
char cardType8 = 8;