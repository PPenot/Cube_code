//#define JLL
//#define DEBUG
#define KY_040
//#define DOUBLE_CORE

#define VERSION 1
#define REVISION 0

#ifdef JLL
// définition E/S Arduino
// right
#define ledPin1 23 // IO23
#define SCL 22 // IO22
#define Reserv1 1 // IO1 !!! Attention utilisation restreinte
#define CLK_HC165 3 // IO3
#define SDA 21 //IO21
#define PL_HC165 19  // IO19 
#define Reserv2 18 // IO18
#define DATA_HC165 5 // IO5
#define DATA_LED 17 // IO17
#define Reserv4 16  // IO16
#define Reserv5 4 // IO4
#define IO2 2 // IO2 // !!! Attention probleme ADC ch2
#define Reserv6 15 // IO15 

// left
#define EN 0 // pour info
#define Slider1 36 // IO36 entree analogique slider fil gris valeur plage 0-1890 max 12 bits 4095
#define IO39 39 // IO39 entree reserve fil bleu
#define IO34 34 // IO34
#define IO35 35 //IO35
#define IO32 32  // IO32
#define IO33 33 // IO33
#define IO25 25 // IO25
#define LED_SYMBOLE 26 // IO26
#define LED_PUSH_BUTTON 27  // IO27
#define IO14 14 // IO14
#define IO12 12 // IO12
#define IO13 13 // IO13
#else

// définition E/S Arduino florian
// right
#define ledPin1 23 // IO23
//#define SCL 22 // IO22
#define Reserv1 1 // IO1 !!! Attention utilisation restreinte
#define CLK_HC165 3 // IO3
//#define SDA 21 //IO21
#define PL_HC165 19  // IO19 
#define Reserv2 18 // IO18
#define DATA_HC165 5 // IO5
#define Reserv6 17 // IO17
#define Reserv4 16  // IO16
#define Reserv5 4 // IO4
#define IO2 2 // IO2 // !!! Attention probleme ADC ch2
#define DATA_LED 15 // IO15 

// left
#define EN 0 // pour info
#define IO36 36 // IO36 entree analogique slider fil gris valeur plage 0-1890 max 12 bits 4095
#define IO39 39 // IO39 entree reserve fil bleu
#define IO34 34 // IO34
#define Slider1 35 //IO35
#define Slider2 32  // IO32
#define Slider3 33 // IO33
#define IO25 25 // IO25
#define LED_SYMBOLE 26 // IO26
#define LED_PUSH_BUTTON 27  // IO27
#define IO14 14 // IO14
#define IO12 12 // IO12
#define IO13 13 // IO13
#endif

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

//LiquidCrystal
#include <LiquidCrystal_I2C.h>

#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif
LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 20 chars and 4 line display

// Adafruit
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//ruban led
//#include <Adafruit_NeoPixel.h>
//#include <NeoPixelBus.h>
#include <NeoPixelBrightnessBus.h>

/***************************************/
/**************************************/
#ifdef JLL
const uint16_t PixelCount = 10; // this example assumes 4 pixels, making it smaller will cause a failure
#else
const uint16_t PixelCount = 200; // this example assumes 4 pixels, making it smaller will cause a failure
#endif
/***************************************/
/**************************************/

//const uint8_t PixelPin = 17;  // make sure to set this to the correct pin, ignored for Esp8266

#define colorSaturation 128

// three element pixels, in different order and speeds
//NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, DATA_LED);
//NeoPixelBrightnessBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, DATA_LED);
NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp32I2s1Sk6812Method> strip(PixelCount, DATA_LED);

//NeoPixelBus<NeoRgbFeature
//RgbColor red(colorSaturation, 0, 0);
//RgbColor green(0, colorSaturation, 0);
//RgbColor blue(0, 0, colorSaturation);
//RgbColor white(colorSaturation);
//RgbColor black(0);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 display1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 display3(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 display4(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//#define TAILLE_LED 10
//Declaration pour ruban led
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(TAILLE_LED, DATA_LED, NEO_GRB + NEO_KHZ800);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

// logo adafruit pour afficher sur init ecran
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
        {B00000000, B11000000,
         B00000001, B11000000,
         B00000001, B11000000,
         B00000011, B11100000,
         B11110011, B11100000,
         B11111110, B11111000,
         B01111110, B11111111,
         B00110011, B10011111,
         B00011111, B11111100,
         B00001101, B01110000,
         B00011011, B10100000,
         B00111111, B11100000,
         B00111111, B11110000,
         B01111100, B11110000,
         B01110000, B01110000,
         B00000000, B00110000
        };

// I2C adresse TCA9548A
#define TCAADDR 0x70

// EEPROM
#define EEPROM_SIZE 100 // 4-4096
#define EEP_ID_BOITE 0
#define EEP_PLAYER 1
#define EEP_BRIGHTNESS 2
#define EEP_ECRAN_LCD 3

// valeurs points étalonnage slider
#define EEP_SLIDER1_ETAL 10
//#define EEP_SLIDER2_ETAL 30 // défini par calcul
//#define EEP_SLIDER3_ETAL 50 // défini par calcul

/***************************************/
/**************************************/
#ifdef JLL
const char* ssid = "Lampaert";
const char* password = "marie123456";
const char* mqtt_server = "192.168.1.19";
#else
//const char* ssid = "Rocinante";
//const char* password = "floriandelphine";
//const char* mqtt_server = "192.168.1.71";
//const char* ssid = "boxscores_network";
//const char* password = "boxscores";
//const char* mqtt_server = "192.168.4.1";

const char *ssid = "N_PetC_24G";
const char *password = "camille&paul";
const char *mqtt_server = "51.83.185.134";
#endif
/**************************************/
/*************************************/

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[200];
int value = 0;

int blue_led = 13;
int Led1;

// Slider
//const int slider1 = 36; // entree analogique slider fil gris valeur plage 0-1890 max 12 bits 4095
//const int reserve = 39; // entree reserve fil bleu

/***************************************/
/**************************************/
#ifdef JLL
// bouton rotatif
const int BtnSw = 12;  // Connected to SW on KY-040
const int BtnClk = 11; // Connected to CLK on KY-040
const int BtnDt = 10; // Connected to DT on KY-040
#else
const int BtnSw = 8;  // Connected to SW on KY-040
const int BtnClk = 2; // Connected to CLK on KY-040
const int BtnDt = 3; // Connected to DT on KY-040
#endif
/**************************************/
/*************************************/

boolean SensHoraire;
boolean SensHoraireMemorise = false;
int SensRotationMemorise = 0;
int CompteurRotation = 0;
int Increment = 0;
int LastBtnClk;
int LastBtnDt;
boolean ChangeClk;
boolean ChangeDt;

// jeux bouton rotatif
int ConsigneRotation[] = {10, -10, 15, 0, 0, 0};
int RotationResult[] = {0, 0, 0, 0, 0, 0};
int IndexRotatif = 0;
int TimeJeuxRotatif;
int LevelJeuxRotatif = 3;
int IdAnswerRotatif;
int StartJeuxRotatif;
String CodeModuleRotatif;
String NomModuleRotatif;
int RotatifPourCent = 0;
boolean RotatifOk = false;
boolean FinJeuxRotatif = true;

/***************************************/
/**************************************/
#ifdef JLL
int NB_LED_ROTATIF = 10;
int MatrLedRotatif[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
#else
int NB_LED_ROTATIF = 40;
int MatrLedRotatif[] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
                        34, 35, 36, 37, 38, 39, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
#endif
/***************************************/
/**************************************/

/***************************************/
/**************************************/
#ifdef JLL
// jeux bouton position
// position du bouton dans la matrice
const int BtnPosition1 = 6;  // Numero Matrice bouton (vert1)
const int BtnPosition2 = 5;  // Numero Matrice bouton (vert2)
const int BtnPosition3 = 7;  // Numero Matrice bouton (vert3)
const int BtnPosition4 = 1;  // Numero Matrice bouton (jaune1)
const int BtnPosition5 = 2;  // Numero Matrice bouton (jaune2)
const int BtnPosition6 = 3;  // Numero Matrice bouton (jaune3)
#else
const int BtnPosition1 = 6;  // Numero Matrice bouton (vert1)
const int BtnPosition2 = 5;  // Numero Matrice bouton (vert2)
const int BtnPosition3 = 4;  // Numero Matrice bouton (vert3)
const int BtnPosition4 = 1;  // Numero Matrice bouton (jaune1)
const int BtnPosition5 = 0;  // Numero Matrice bouton (jaune2)
const int BtnPosition6 = 7;  // Numero Matrice bouton (jaune3)
#endif
/***************************************/
/**************************************/

/***************************************/
/**************************************/
#ifdef JLL
int NB_LED_POSITION = 5;
int MatrLedPosition[] = {0, 1, 2, 3, 4};
#else
int NB_LED_POSITION = 40;
int MatrLedPosition[] = {90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
                         111, 112, 113, 114, 115, 116, 117, 118, 119, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89};
#endif
/***************************************/
/**************************************/

int ConsignePosition[] = {0, 1, 0, 0, 0, 0};
int PositionResult[] = {0, 0, 0, 0, 0, 0};
int IndexPosition = 0; //ne sert pas compatibilité bloc jeux
int TimeJeuxPosition;
int LevelJeuxPosition = 3;
int IdAnswerPosition;
int StartJeuxPosition;
String CodeModulePosition;
String NomModulePosition;
int PositionPourCent = 0;
boolean PositionOk = false;
boolean FinJeuxPosition = true;

// jeux bouton ordre
// position du bouton dans la matrice
/***************************************/
/**************************************/
#ifdef JLL
const int BtnOrdre1 = 6;  // Numero Matrice bouton (vert1)
const int BtnOrdre2 = 5;  // Numero Matrice bouton (vert2)
const int BtnOrdre3 = 7;  // Numero Matrice bouton (vert3)
const int BtnOrdre4 = 1;  // Numero Matrice bouton (jaune1)
const int BtnOrdre5 = 2;  // Numero Matrice bouton (jaune2)
const int BtnOrdre6 = 3;  // Numero Matrice bouton (jaune3)
#else
const int BtnOrdre1 = 15;  // Numero Matrice bouton (vert1)
const int BtnOrdre2 = 10;  // Numero Matrice bouton (vert2)
const int BtnOrdre3 = 11;  // Numero Matrice bouton (vert3)
const int BtnOrdre4 = 12;  // Numero Matrice bouton (jaune1)
const int BtnOrdre5 = 13;  // Numero Matrice bouton (jaune2)
const int BtnOrdre6 = 14;  // Numero Matrice bouton (jaune3)
#endif
/***************************************/
/**************************************/

/***************************************/
/**************************************/
#ifdef JLL
int NB_LED_ORDRE = 5;
int MatrLedOrdre[] = {5, 6, 7, 8, 9};
#else
int NB_LED_ORDRE = 40;
int MatrLedOrdre[] = {130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148,
                      149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 120, 121, 122, 123, 124, 125, 126, 127,
                      128, 129};
#endif
/***************************************/
/**************************************/

int ConsigneOrdre[] = {2, 3, 5, 0, 0, 0};
int OrdreResult[] = {0, 0, 0, 0, 0, 0};
int IndexOrdre = 0;
int TimeJeuxOrdre;
int LevelJeuxOrdre = 3;
int IdAnswerOrdre;
int StartJeuxOrdre;
String CodeModuleOrdre;
String NomModuleOrdre;
int OrdrePourCent = 0;
boolean OrdreOk = false;
boolean FinJeuxOrdre = true;

// jeux bouton slider
//const int Slider1 = 36; // entree analogique slider fil gris valeur plage 0-1890 max 12 bits 4095
#ifdef JLL
const int Slider2 = 36; // utilisation slider 1 pour test
const int Slider3 = 36; // utilisation slider 1 pour test
#endif

/***************************************/
/**************************************/
#ifdef JLL
int NB_LED_SLIDER = 10;
int MatrLedSlider[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
#else
int NB_LED_SLIDER = 40;
int MatrLedSlider[] = {69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46,
                       45, 44, 43, 42, 41, 40, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70};
#endif
/***************************************/
/**************************************/

int ConsigneSlider[] = {6, 3, 2, 0, 0, 0};
int SliderResult[] = {0, 0, 0, 0, 0, 0};
int IndexSlider = 0; // ne sert pas compatibilite bloc jeux
int ValAnalogique; // variable globale pour valeur
int TimeJeuxSlider;
int LevelJeuxSlider = 3;
int IdAnswerSlider;
int StartJeuxSlider;
String CodeModuleSlider;
String NomModuleSlider;
int SliderPourCent;
boolean SliderOk = false;
boolean FinJeuxSlider = true;
int DerniereLectureSlider;
int INTERVAL_LECTURE_SLIDER = 50;

/***************************************/
/**************************************/
#ifdef JLL
const int BtnPushButton = 6; //vert 1
int NB_LED_PUSH_BUTTON = 10;
int MatrLedPushButton[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
#else
const int BtnPushButton = 9;
int NB_LED_PUSH_BUTTON = 40;
int MatrLedPushButton[] = {170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187,
                           188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 160, 161, 162, 163, 164, 165,
                           166, 167, 168, 169};
#endif
/***************************************/
/**************************************/

//int ConsignePushButton[] = {6, 3, 2, 0, 0, 0};
//int PushButtonResult[] = {0, 0, 0, 0, 0, 0};
int IndexPushButton = 0; // ne sert pas compatibilite bloc jeux
int TimeJeuxPushButton;
int LevelJeuxPushButton = 3;
int IdAnswerPushButton;
int StartJeuxPushButton;
String CodeModulePushButton;
String NomModulePushButton;
int PushButtonPourCent;
boolean ButtonOk = false;
boolean FinJeuxPushButton = true;
//boolean FinLedPushButton = true;
//int TimeFinLedPushButton;
int DerniereLecturePushButton;
int INTERVAL_LECTURE_PUSH_BUTTON = 50;

/***************************************/
/**************************************/
#ifdef JLL
// Boutons de service
const int BtnPlus = 6; //vert1
const int BtnMoins = 5; //vert2
const int BtnValid = 7; //vert3
const int BtnExit = 1; //jaune1

#else
// Boutons de service florian
//const int BtnPlus = 9;
//const int BtnMoins = 10;
//const int BtnValid = 11;
//const int BtnExit = 14;
const int BtnPlus = 15;
const int BtnMoins = 10;
const int BtnValid = 12;
const int BtnExit = 13;
#endif
/***************************************/
/**************************************/

/***************************************/
/**************************************/
#ifdef JLL
// Boutons Test
const int BtnVert1 = 6;
const int BtnVert2 = 5;
const int BtnVert3 = 7;
const int BtnJaune1 = 1;
const int BtnJaune2 = 2;
const int BtnJaune3 = 3;

#else
// Boutons Test
const int BtnVert1 = 4;
const int BtnVert2 = 5;
const int BtnVert3 = 6;
const int BtnVert4 = 7;
const int BtnJaune1 = 12;
const int BtnJaune2 = 13;
const int BtnJaune3 = 14;
const int BtnJaune4 = 15;
#endif
/***************************************/
/**************************************/

// Test
boolean TestActif = false;
int NumTest = 0;
int Aux1 = 0;
int Aux2 = 0;
int DerniereLectureTest;
int INTERVAL_LECTURE_TEST = 100;

// HC165
/***************************************/
/**************************************/
#ifdef JLL
const int TAILLE_BOUTON  = 16; // #define TAILLE_BOUTON 16 ne marche pas ASCII et pas entier
#else
const int TAILLE_BOUTON = 16; // #define TAILLE_BOUTON 16 ne marche pas ASCII et pas entier
#endif
/***************************************/
/**************************************/
int MatrBoutonEtat[TAILLE_BOUTON];
int MatrBoutonMemorise[TAILLE_BOUTON];
int MatrBoutonTraitement[TAILLE_BOUTON];
int MatrBoutonTime[TAILLE_BOUTON];
int MatrBoutonChange[TAILLE_BOUTON]; // pour test changement état

const int RebondTime = 30; // constante numérique
const int INTERVAL_LECTURE_BOUTON = 10;
int DerniereLectureBouton;

// Dac
const int TAILLE_ENRG_DAC = 4;
int MatrCircDac1[TAILLE_ENRG_DAC];
int IdxMatrDac1;
int MatrCircDac2[TAILLE_ENRG_DAC];
int IdxMatrDac2;
int MatrCircDac3[TAILLE_ENRG_DAC];
int IdxMatrDac3;

// LEDS
int ValRouge = 0;
int ValBleu = 0;
int ValVert = 0;

const int clRien = 0;
const int clRed = 1;
const int clGreen = 2;
const int clBlue = 3;
const int clYellow = 4;
const int clOrange = 5;
const int clPurple = 6;
const int clCyan = 7;
const int clPink = 8;
const int clMoneyGreen = 9;
const int clSkyBlue = 10;
const int clWhite = 11;

//const int TAILLE_LED = 10;
int MatrLed[PixelCount];
int MatrLedAff[PixelCount];
int DernierAffichageLed;
int DernierRefreshAffichageLed;
//int DernierRefreshEtalonnageSlider;
int DernierEtalonnageSlider;
const int INTERVAL_AFFICHAGE_LED = 10;
const int INTERVAL_REFRESH_AFFICHAGE_LED = 100;
//const int INTERVAL_REFRESH_ETALONNAGE_SLIDER = 100;
const int INTERVAL_ETALONNAGE_SLIDER = 200;

// Trace
int TraceNiveauMini = 0;
int TraceNiveauMaxi = 100;
boolean TraceAffichageTemps = true;
boolean TraceAffichageTag = true;
boolean TraceAffichageLevel = true;
boolean TraceActiveSerie = true;
boolean TraceActiveMqtt = true;
String NomTagFiltre = "";

// Trace tag
String TAG_INIT = "init";
String TAG_ROTATIF = "rotatif";
String TAG_POSITION = "postion";
String TAG_ORDRE = "ordre";
String TAG_SLIDER = "slider";
String TAG_WIFI = "wifi";
String TAG_JSON = "json";
String TAG_TEST = "test";

// EEPROM
int IdBoite = 0;
int Player = 0;
int Brightness = 50;
int EcranLcd = 0; // 0 = local, 1 = deporte, 2 = deporte affichage

// LCD
#define LCD_NB_COLONNE 20
#define LCD_NB_LIGNE  4
String Lcd[] = {"", "", "", ""};
boolean LcdChange = false;

// Etalonnage slider
int PositionSlider = 0;

/***************************************/
/**************************************/
#ifdef JLL
// OLED
#define NO_ECRAN -1 // pas d'écran
#define ECRAN_ROTATIF 0 // Tca9548 voie 0
#define ECRAN_ORDRE 5 // Tca9548 voie 5
#define ECRAN_POSITION 7 // Tca9548 voie 7
#define ECRAN_SLIDER 7 // utilisation ecran position pour test
#else
// OLED
#define NO_ECRAN -1 // pas d'écran
#define ECRAN_ROTATIF 2 // Tca9548 voie 2
#define ECRAN_ORDRE 5 // Tca9548 voie 5
#define ECRAN_POSITION 4 // Tca9548 voie 4
#define ECRAN_SLIDER 3 // Tca9548 voie 3
#endif
/***************************************/
/**************************************/

// MQTT
String MQTT_MAC_STATUS = "mac/status";
String MQTT_STATUS = "status";
String MQTT_SYST = "syst";
String MQTT_SERVER = "server";
String MQTT_DEBUG = "debug";
String MQTT_ACTIVATION_MODULE = "activateModule";
String MQTT_GOOD_MODULE = "goodModule";
String MQTT_BAD_MODULE = "badModule";
String MQTT_PUSH_BUTTON = "pushButton"; // Envoyer quand click sur le gros bouton (15)
String MQTT_GOOD_PUSH = "goodPush";// Channel a ecouter si qque chose eteindre le module pushbutton (led)
String MQTT_CONFIG = "config";
String MQTT_LOG = "log";
String MQTT_SEND_ANSWER = "sendAnswer";
String MQTT_DELETE_ANSWER = "deleteAnswer";
String MQTT_ERREUR = "erreur";
String MQTT_TEST = "test";
String MQTT_ECRAN = "screen";
String MQTT_END = "end";
String MQTT_CO = "connected";
String MQTT_DCO = "disconnected";

// MODULE
String MODULE_ROTATIF = "rotatif";
String MODULE_ORDRE = "buttons";
String MODULE_POSITION = "switchs";
String MODULE_SLIDER = "sliders";
String MODULE_PUSH_BUTTON = "pushButton";

// JSON
String JSON_NOM = "moduleName";
String JSON_CODE = "answerCode";
String JSON_REPONSE = "answer";
String JSON_TIME = "diffuseTime";
String JSON_ID_PLAYER_ANSWER = "idPlayerAnswer";
String JSON_LEVEL = "level";
String JSON_TXT_1 = "txt1";
String JSON_TXT_2 = "txt2";
String JSON_TXT_3 = "txt3";
String JSON_TXT_4 = "txt4";

// ANIMATION OLED
int DureeAnim1 = 0;
int StartAnim1 = 0;
int StartIntervalAnim1 = 0;
int IntervalAnim1 = 0;
int NivAnim1 = 0;
int ScreenAnim1 = 0;
int SensAnim1 = 0;
String TexteAnim1 = "";
boolean JeuxOkAnim1 = false;
int CoulLedAnim1 = 0;
boolean StopAnim1 = true;

int DureeAnim2 = 0;
int StartAnim2 = 0;
int StartIntervalAnim2 = 0;
int IntervalAnim2 = 0;
int NivAnim2 = 0;
int ScreenAnim2 = 0;
int SensAnim2 = 0;
String TexteAnim2 = "";
boolean JeuxOkAnim2 = false;
int CoulLedAnim2 = 0;
boolean StopAnim2 = true;

int DureeAnim3 = 0;
int StartAnim3 = 0;
int StartIntervalAnim3 = 0;
int IntervalAnim3 = 0;
int NivAnim3 = 0;
int ScreenAnim3 = 0;
int SensAnim3 = 0;
String TexteAnim3 = "";
boolean JeuxOkAnim3 = false;
int CoulLedAnim3 = 0;
boolean StopAnim3 = true;

int DureeAnim4 = 0;
int StartAnim4 = 0;
int StartIntervalAnim4 = 0;
int IntervalAnim4 = 0;
int NivAnim4 = 0;
int ScreenAnim4 = 0;
int SensAnim4 = 0;
String TexteAnim4 = "";
boolean JeuxOkAnim4 = false;
int CoulLedAnim4 = 0;
boolean StopAnim4 = true;

int DureeAnim5 = 0;
int StartAnim5 = 0;
int StartIntervalAnim5 = 0;
int IntervalAnim5 = 0;
int NivAnim5 = 0;
int ScreenAnim5 = 0;
int SensAnim5 = 0;
String TexteAnim5 = "";
boolean JeuxOkAnim5 = false;
int CoulLedAnim5 = 0;
boolean StopAnim5 = true;

//Systeme
String AdresseMac = "";
int CurrentTime;
int NbLoop;

//JsonParser
StaticJsonDocument<512> doc; // https://arduinojson.org/v6/assistant/

// Symboles pour afficheur LCD
uint8_t bell[8] = {0x4, 0xe, 0xe, 0xe, 0x1f, 0x0, 0x4};
uint8_t note[8] = {0x2, 0x3, 0x2, 0xe, 0x1e, 0xc, 0x0};
uint8_t clock1[8] = {0x0, 0xe, 0x15, 0x17, 0x11, 0xe, 0x0};
uint8_t heart[8] = {0x0, 0xa, 0x1f, 0x1f, 0xe, 0x4, 0x0};
uint8_t duck[8] = {0x0, 0xc, 0x1d, 0xf, 0xf, 0x6, 0x0};
uint8_t check[8] = {0x0, 0x1, 0x3, 0x16, 0x1c, 0x8, 0x0};
uint8_t cross[8] = {0x0, 0x1b, 0xe, 0x4, 0xe, 0x1b, 0x0};
uint8_t retarrow[8] = {0x1, 0x1, 0x5, 0x9, 0x1f, 0x8, 0x4};
uint8_t omega[8] = {0x0, 0x4, 0xa, 0x11, 0x11, 0xa, 0x1b};


/***************************************/
/*Declaration Tasks*/
/**************************************/
#ifdef DOUBLE_CORE
TaskHandle_t Taskloop;
TaskHandle_t TaskBouton;
#endif

/* byte smiley[8] = {
  B00000,
  B10001,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
  B00000
  };*/


/*
  //________________________________________________________________________

         setup                       ::::::::::::::::::::::::::::::::::::::
  //_______________________________________________________________________
*/

//******************************************************************
// functions
//******************************************************************
void setup_wifi();

//******************************************************************
// reconnect
//******************************************************************
void reconnect();


//*********************************************************************
// loopTestDebug
//***********************************************************************
void loopTestDebug();

//*********************************************************************
// loopTest vert3
//***********************************************************************
void loopTestVert3();

//***********************************************************************
// loopTest vert2
//***********************************************************************
void loopTestVert2();

//***********************************************************************
// loopTest vert1
//***********************************************************************
void loopTestVert1();

//***********************************************************************
// RazGene Raz générale
//***********************************************************************
void RazGene();


//*********************************************************************
// InitBouton
//***********************************************************************
void InitBouton();

//*********************************************************************
// loopBouton
//***********************************************************************
void loopBouton();

//*********************************************************************
// EtatBouton lecture de l'état des boutons
//***********************************************************************
void EtatBouton();

//*********************************************************************
// ActualisationBouton actualisation des boutons
//***********************************************************************
void ActualisationBouton();

//*********************************************************************
// ClearLed
//***********************************************************************
void ClearLed();

//***********************************************************************
// CouleurLedMatrice Coloration des leds définies par la matrice (raz = clRien)
//***********************************************************************
void CouleurLedMatrice(int pNbLed, int *pMatrPosLed, int pCoul);

//*********************************************************************
// RefreshLed
//***********************************************************************
void RefreshLed();

//*********************************************************************
// CouleurLed
//***********************************************************************
RgbColor CouleurLed(int pNum);

//*********************************************************************
// ControleLedBouton
//***********************************************************************
void ControleLedBouton();

//*********************************************************************
// AffichageLed
//***********************************************************************
void AffichageLed();

//***********************************************************************
// ProgressionLed Calcul et affichage de la progression
//***********************************************************************
void ProgressionLed(int pNbLed, int *pMatrPosLed, int *pPourCent);

//********************************************************************
// RazCompteurRotatif
//********************************************************************
void RazCompteurRotatif();

//********************************************************************
// ControleRazCompteur
//********************************************************************
void ControleRazCompteur();

//*****************************************************************
// controle enregistrement rotatif
//*****************************************************************
void ControleEnregistrementRotatif(int pIncrement);

//*****************************************************************
// controle consigne rotatif
//*****************************************************************
void ControleConsigneRotatif();

//***********************************************************************
// loopJeuxRotatif
//***********************************************************************
void loopJeuxRotatif();

//*****************************************************************
// controle consigne position
//*****************************************************************
void ControleConsignePosition();

//***********************************************************************
// loopJeuxPosition
//***********************************************************************
void loopJeuxPosition();

//*********************************************************************
// InitBufferCirculaire
//***********************************************************************
void InitBufferCirculaire();

//*****************************************************************
// Moyenne Analogique
//*****************************************************************
int MoyenneAnalogique(int pVal, int pNum);

//*****************************************************************
// lecture slider
//*****************************************************************
int LectureSlider(int pVoie, int pNum, int pValTest);

//*****************************************************************
// controle consigne slider
//*****************************************************************
void ControleConsigneSlider();

//***********************************************************************
// loopJeuxSlider
//***********************************************************************
void loopJeuxSlider();


//*****************************************************************
//  BoutonOrdreSelect  renvoie le numéro du bouton appuyé (0 = rien)
//*****************************************************************
int BoutonOrdreSelect();

//*****************************************************************
// controle consigne ordre
//*****************************************************************
void ControleConsigneOrdre();

//***********************************************************************
// loopJeuxOrdre
//***********************************************************************
void loopJeuxOrdre();

//***********************************************************************
// PushButtonOk
//***********************************************************************
void PushButtonOk();

//*****************************************************************
// controle consigne push button
//*****************************************************************
void ControleConsignePushButton();

//***********************************************************************
// loopJeuxPushButton
//***********************************************************************
void loopJeuxPushButton();


//*****************************************************************
// TimeOut controle du temps de jeux
//*****************************************************************
boolean TimeOut(int pActuel, int pStart, int pTime, boolean *pFin, int pEcran);


//********************************************************************
// Initialisation jeux
//********************************************************************
void InitJeux(String pTopic, String pMes);

//********************************************************************
// ProgConsigne
//********************************************************************
void ProgConsigne(int *pMatrSource, int *pMatrDest, int *pMatrResult, String pNom);

//********************************************************************
// CodeOled
//********************************************************************
void CodeOled(String jsCode, int pEcran);

//********************************************************************
// ProgJeux
//********************************************************************
void
ProgJeux(String jsNom, String *pNom, String jsCode, String *pCode, int jsTime, int *pTime, int jsLevel, int *pLevel,
         int jsIdAnswer, int *pIdAnswer, int *pStart, int *pIndex, boolean *pOk, boolean *pFinJeux);


//********************************************************************
// EffacementConsigneJeux
//********************************************************************
void EffacementConsigneJeux(String pTopic, String pMes);

//***********************************************************************
// ConsigneInverse inversion 0 1
//***********************************************************************
int ConsigneInverse(int pConsigne);

//********************************************************************
// AffichageConsigneJeux
//********************************************************************
void AffichageConsigneJeux(String pTopic, String pMes);


//******************************************************************
// callback
//******************************************************************
void callback(char *topic, byte *message, unsigned int length);

//********************************************************************
// SendModuleResult réponse fin de module
//********************************************************************
void SendModuleResult(String pNom, String pCode, int pTemps, int pIdAnswer, boolean pResult);

//********************************************************************
// SendMqtt envoi Mqtt
//********************************************************************
void SendMqtt(String pTopic, String pData, boolean pSecure);

//***********************************************************************
//    ScanI2cTotal Scan des periphériques I2C
//***********************************************************************
void ScanI2cTotal();

//***********************************************************************
//    ScanI2c Scan des periphériques I2C
//***********************************************************************
void ScanI2c(int pCanal);


//***********************************************************************
//    TcaOff deselection sortie
//***********************************************************************
void TcaOff();

//***********************************************************************
//    TcaVoieActive voie TCA9548A selectionnée
//***********************************************************************
byte TcaVoieActive();

//***********************************************************************
//    TcaSelect Selection voie TCA9548A
//***********************************************************************
void TcaSelect(uint8_t i);


//***********************************************************************
// Progression Calcul et affichage de la progression
//***********************************************************************
boolean Progression(int pActuel, int pStart, int pTime, int *pPourCent, int pEcran);

//***********************************************************************
// CalculPourcentage calcule le poucentage du temps ecoule
//***********************************************************************
int CalculPourcentage(int pActuel, int pStart, int pTime);

//***********************************************************************
// OledProgress   Barre de progression
//***********************************************************************
void OledProgress(int pPourCent, int pNumEcran);


//***********************************************************************
// ClearOled Clear afficheur
//***********************************************************************
void ClearOled(int pCanal, int pReset);

//***********************************************************************
// OledTexte ecriture texte sur oled
//***********************************************************************
void OledTexte(int X, int Y, int Size, String pTexte, int pEcran, boolean pNew, boolean pDisplay);

//*********************************************************************
// RazAnimation
//***********************************************************************
void RazAnimation();

//*********************************************************************
// ArretAnimation
//***********************************************************************
void ArretAnimation(int pEcran);

//*********************************************************************
// ProgAnimation
//***********************************************************************
void ProgAnimation(int pEcran, int pDuree, int pInterval, String pTexte, int pSens, boolean pJeuxOk);

//*********************************************************************
// ControleAnimation
//***********************************************************************
void ControleAnimation();

//*********************************************************************
// TraitementAnimation
//***********************************************************************
boolean TraitementAnimation(int *pDuree, int *pStart, int *pStartInterval, int *pInterval, int *pNiv, int *pScreen,
                            String *pTexte, int *pSens, int *pCoulLed, boolean *pJeuxOk, boolean *pStop);

//***********************************************************************
// InitLcd Initialisation LCD
//***********************************************************************
void InitLcd();

//***********************************************************************
// ClearLcd raz afficheur
//***********************************************************************
void ClearLcd();

//***********************************************************************
// EcritureLcd
//***********************************************************************
void EcritureLcd(String pTopic, String pMes);

//***********************************************************************
// AffichageLcd envoi texte memoire sur Lcd
//***********************************************************************
void AffichageLcd();

//***********************************************************************
// LcdToMqtt envoi texte memoire sur mqtt
//***********************************************************************
void LcdToMqtt();

//***********************************************************************
// AjoutLcdSpecial ajout d'un caractere special sur la chaine
//***********************************************************************
String AjoutLcdSpecial(String pChaine, int pNum);

//***********************************************************************
// AddLcd ajout d'un chaine sur l'afficheur
//***********************************************************************
void AddLcd(String pChaine);

//***********************************************************************
// SupLcdTxt suppression d'un chaine sur l'afficheur
//***********************************************************************
void SupLcdTxt(String pChaine);

//***********************************************************************
// SupLcdLine suppression d'une ligne sur l'afficheur
//***********************************************************************
void SupLcdLine(int pNum);

//***********************************************************************
// CompleteChaine avec espaces
//***********************************************************************
String CompleteChaine(String pChaine, int pNombre);

//***********************************************************************
// ReclasseAfficheur suppression des lignes vides sur afficheur
//***********************************************************************
void ReclasseAfficheur();


//********************************************************************
// Programmation configuration
//********************************************************************
void ProgConfig(String pTopic, String pMes);

//********************************************************************
// Renvoie le status EEPROM
//********************************************************************
void AffichageStatus(String pTopic, String pMes);

//*****************************************************************
// EcritureEeprom écriture d'une valeur EEPROM
//*****************************************************************
void EcritureEeprom(int pAdr, int pVal);

//*****************************************************************
//  LectureEeprom  lecture d'une valeur EEPROM
//*****************************************************************
int LectureEeprom(int pAdr);

//*****************************************************************
// EcritureEepromDouble écriture d'une valeur EEPROM
//*****************************************************************
void EcritureEepromDouble(int pAdr, int pVal);

//*****************************************************************
//  LectureEepromDouble  lecture d'une valeur EEPROM
//*****************************************************************
int LectureEepromDouble(int pAdr);


//********************************************************************
// Programmation trace
//********************************************************************
void ProgTrace(String pTopic, String pMes);

//*****************************************************************
//  TraceSelect  la trace est active si le niveau est supérieur au mini inférieur au maxi et la trace autorisée
//*****************************************************************
boolean TraceSelect(int pLevel);

//*****************************************************************
// Trace envoi info sur port série / Mqtt
//*****************************************************************
void Trace(int pNiv, String pTag, String pInfo);


//********************************************************************
// Programmation test
//********************************************************************
void ProgTest(String pTopic, String pMes);

//*********************************************************************
// loopTest
//***********************************************************************
void loopTest();

//*********************************************************************
// TestBoutonRotatif
//***********************************************************************
void TestBoutonRotatif();

//*********************************************************************
// Test DAC
//***********************************************************************
void TestDac();

//***********************************************************************
//    Vitesse commutation port
//***********************************************************************
void VitesseCommutationPort();

//*********************************************************************
// Test changement etat bouton
//***********************************************************************
void ChangementEtatBouton();

//*********************************************************************
// Test led brigthness
//***********************************************************************
void TestLedBrigthness();

//*********************************************************************
// Etalonnage des sliders
//***********************************************************************
void EtalonnageSlider();

//*********************************************************************
// Reset Oled
//***********************************************************************
void ResetOled();

//*********************************************************************
// Lecture eeprom slider
//***********************************************************************
void LectureEepromSlider();

//*********************************************************************
// Lecture conversion slider
//***********************************************************************
void LectureConversionSlider();


//******************************************************************
// setup
//******************************************************************
void setup() {
    String LigneLcd;

    // initialisation taille EEPROM
    EEPROM.begin(EEPROM_SIZE);
    IdBoite = LectureEeprom(EEP_ID_BOITE);
    Player = LectureEeprom(EEP_PLAYER);
    Brightness = LectureEeprom(EEP_BRIGHTNESS);
    EcranLcd = LectureEeprom(EEP_ECRAN_LCD);

    Serial.begin(115200);

    // Initialisation Lcd
    InitLcd();

    // Initialisation Led
    strip.Begin();
    strip.Show();

    // affichage Lcd
    ClearLcd();
    AddLcd("Waiting : WiFi");
    AffichageLcd();

    //wire.begin(SDA, SCL);
    //wire.begin();

    setup_wifi();

    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);

    // initialize pin as an input:
    pinMode(DATA_HC165, INPUT);
    //pinMode(Slider1, INPUT);

    // initialize pin as an output:
    pinMode(ledPin1, OUTPUT);

    // HC165
    pinMode(PL_HC165, OUTPUT);
    digitalWrite(PL_HC165, HIGH);
    pinMode(CLK_HC165, OUTPUT);
    digitalWrite(CLK_HC165, LOW);

    // LED
    pinMode(LED_PUSH_BUTTON, OUTPUT);
    digitalWrite(LED_PUSH_BUTTON, LOW);
    pinMode(LED_SYMBOLE, OUTPUT);
    digitalWrite(LED_SYMBOLE, LOW);

    CurrentTime = millis();
    //Serial.println(String(CurrentTime) + "  start");
    NbLoop = 0;

    /***************************************/
    /**************************************/
#ifdef JLL
    // prog tracage JLL
    TraceNiveauMini = 4;
    TraceAffichageTemps = true;
    TraceAffichageTag = false;
    TraceAffichageLevel = false;
    TraceActiveSerie = true;
    TraceActiveMqtt = true;
    NomTagFiltre = "";
#else
    // prog tracage autre
    TraceNiveauMini = 4;
    TraceAffichageTemps = true;
    TraceAffichageTag = false;
    TraceAffichageLevel = false;
    TraceActiveSerie = true;
    TraceActiveMqtt = true;
    NomTagFiltre = "";
#endif
    /***************************************/
    /**************************************/

    // initialisation boutons
    InitBouton();

    TcaSelect(ECRAN_ORDRE);
    // Initialisation Oled
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display1.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
        Serial.println(F("SSD1306 allocation ordre failed"));
        for (;;); // Don't proceed, loop forever
    }
    TcaSelect(ECRAN_POSITION);
    // Initialisation Oled
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display2.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
        Serial.println(F("SSD1306 allocation position failed"));
        for (;;); // Don't proceed, loop forever
    }

    TcaSelect(ECRAN_ROTATIF);
    // Initialisation Oled
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display3.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
        Serial.println(F("SSD1306 allocation rotatif failed"));
        for (;;); // Don't proceed, loop forever
    }

    TcaSelect(ECRAN_SLIDER);
    // Initialisation Oled
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display4.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
        Serial.println(F("SSD1306 allocation slider failed"));
        for (;;); // Don't proceed, loop forever
    }

    // affichage omega Lcd et version
    ClearLcd();
    LigneLcd = "      ";
    LigneLcd = AjoutLcdSpecial(LigneLcd, 8);
    LigneLcd = LigneLcd + "mega";
    AddLcd(LigneLcd);
    LigneLcd = "Version : " + String(VERSION) + "." + String(REVISION);
    AddLcd(LigneLcd);
    AffichageLcd();


    TcaSelect(ECRAN_SLIDER);
    // Show initial display buffer contents on the screen --
    // the library initializes this with an Adafruit splash screen.
    display4.display();
    //TcaOff(); // no channel select
    delay(500);

    TcaSelect(ECRAN_ROTATIF);
    // Show initial display buffer contents on the screen --
    // the library initializes this with an Adafruit splash screen.
    display3.display();
    //TcaOff(); // no channel select
    delay(500);

    TcaSelect(ECRAN_POSITION);
    display2.display();
    //TcaOff(); // no channel select
    delay(500);

    TcaSelect(ECRAN_ORDRE);
    display1.display();
    //TcaOff(); // no channel select
    delay(2000); // Pause for 1 seconds vérification oled start adafruit

    // Clear écrans
    ClearLcd();
    //LcdChange = true;
    ClearOled(ECRAN_ROTATIF, false);
    ClearOled(ECRAN_POSITION, false);
    ClearOled(ECRAN_ORDRE, false);
    ClearOled(ECRAN_SLIDER, false);

    // Clear leds
    ClearLed();

#ifdef DOUBLE_CORE
    setupTasks();
#endif
}

/*
  //________________________________________________________________________

         WiFi                        ::::::::::::::::::::::::::::::::::::::
  //_______________________________________________________________________
  setup_wifi
  reconnect
*/

//******************************************************************
// setup_wifi
//******************************************************************
void setup_wifi() {
    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    AdresseMac = WiFi.macAddress();

    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

//******************************************************************
// reconnect
//******************************************************************
void reconnect() {
    char bufSubscribe[50];
    char bufMqttClient[50];
    char bufMAC[50];
    char bufWillTopic[50];
    char bufWillMessage[500];
    String TopicEntete = "";
    String WillTopic = "";
    String WillMessage = "";

    // Loop until we're reconnected
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
//        AdresseMac.toCharArray(bufMqttClient, AdresseMac.length() + 1);
        AdresseMac.toCharArray(bufMAC, AdresseMac.length() + 1);
        WillTopic = AdresseMac+"/status";
        WillTopic.toCharArray(bufWillTopic, WillTopic.length() + 1);

        doc.clear();
        doc["MAC"] = AdresseMac;
        doc["idBox"] = IdBoite;
        doc["player"] = Player;
        doc["activity"] = "diffuse";
        doc["status"] = MQTT_DCO;
        serializeJson(doc, WillMessage);
        WillMessage.toCharArray(bufWillMessage, WillMessage.length() + 1);

        if (client.connect(bufMAC, bufWillTopic, 1, true, bufWillMessage)) {
//        if (client.connect(bufMqttClient, "team1/memory/answerbox/status", 1, true, "DISCONNECTED")) {
            Serial.println("connected");
//            client.publish("team1/memory/answerbox/status", "CONNECTED");
            WillMessage = "";
            doc.clear();
            doc["MAC"] = AdresseMac;
            doc["idBox"] = IdBoite;
            doc["player"] = Player;
            doc["activity"] = "diffuse";
            doc["status"] = MQTT_CO;
            serializeJson(doc, WillMessage);
            WillMessage.toCharArray(bufWillMessage, WillMessage.length() + 1);
            client.publish(bufWillTopic, bufWillMessage);

            // adressage système (adresse mac wifi esp32)
            TopicEntete = "server/" + AdresseMac + "/#";
            TopicEntete.toCharArray(bufSubscribe, TopicEntete.length() + 1);
            client.subscribe(bufSubscribe);

            // Broadcast pour info système (EEPROM)
            TopicEntete = MQTT_MAC_STATUS;
            TopicEntete.toCharArray(bufSubscribe, TopicEntete.length() + 1);
            client.subscribe(bufSubscribe);

            // Adressage système
            TopicEntete = "server/" + String(IdBoite) + "/" + MQTT_SYST + "/#";
            TopicEntete.toCharArray(bufSubscribe, TopicEntete.length() + 1);
            client.subscribe(bufSubscribe);

            // Adressage ecran
            //TopicEntete = String(IdBoite) + "/screen" + "/#";
            TopicEntete = String(IdBoite) + "/diffuse/" + String(Player) + "/#";
            TopicEntete.toCharArray(bufSubscribe, TopicEntete.length() + 1);
            client.subscribe(bufSubscribe);

            // adressage normal
            TopicEntete = "server/" + String(IdBoite) + "/diffuse/" + String(Player) + "/#";
            TopicEntete.toCharArray(bufSubscribe, TopicEntete.length() + 1);
            client.subscribe(bufSubscribe);

            // raz boite
            TopicEntete = "server/" + String(IdBoite) + "/diffuse/end";
            TopicEntete.toCharArray(bufSubscribe, TopicEntete.length() + 1);
            client.subscribe(bufSubscribe);

        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

/*
  //________________________________________________________________________

         loop                        ::::::::::::::::::::::::::::::::::::::
  //_______________________________________________________________________
*/

//***********************************************************************
// loop
//***********************************************************************

#ifdef DOUBLE_CORE
void setupTasks() {

    xTaskCreatePinnedToCore(
            f_Taskloop, /* Function to implement the task */
            "Taskloop", /* Name of the task */
            10000, /* Stack size in words */
            NULL, /* Task input parameter */
            10, /* Priority of the task */
            &Taskloop, /* Task handle. */
            0); /* Core where the task should run */
            delay(500);

            xTaskCreatePinnedToCore(
                    f_TaskBouton, /* Function to implement the task */
                    "TaskBouton", /* Name of the task */
                    10000, /* Stack size in words */
                    NULL, /* Task input parameter */
                    10, /* Priority of the task */
                    &TaskBouton, /* Task handle. */
                    1); /* Core where the task should run */
                    delay(500);

}

void f_Taskloop(void *parameter) {
    for (;;) {
        if (!client.connected()) {
            reconnect();
        }
        client.loop();

        /*
          #ifdef DEBUG
            NbLoop ++;
            loopTestVert3();
            loopTestVert2();
            loopTestVert1();
          #endif
        */

        CurrentTime = millis();

        loopBouton();

        if (FinJeuxRotatif == false) {
            loopJeuxRotatif();
        }

        if (FinJeuxPosition == false) {
            loopJeuxPosition();
        }

        if (FinJeuxOrdre == false) {
            loopJeuxOrdre();
        }

        if (FinJeuxSlider == false) {
            loopJeuxSlider();
        }

        if (FinJeuxPushButton == false) {
            loopJeuxPushButton();
        }

        //if (FinLedPushButton == false) {
        //  loopFinLedPushButton();
        // }

        if (LcdChange == true) {
            AffichageLcd();
        }

        if ((StopAnim1 == false) || (StopAnim2 == false) || (StopAnim3 == false)  || (StopAnim4 == false)  || (StopAnim5 == false)  ) {
            ControleAnimation();
        }

        if (TestActif == true) {
            loopTest();
        }
        // recopier loop sans affichage led
        delay(1);
    }
}

void f_TaskBouton(void *parameter) {
    for (;;) {
        // Trace(20, TAG_INIT, "loopBouton");
        AffichageLed();
        //loopBouton();
    }
}
#endif

void loop() {
#ifndef DOUBLE_CORE

    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    loopTestDebug();
    /*
      #ifdef DEBUG
      NbLoop ++;
      loopTestVert3();
      loopTestVert2();
      loopTestVert1();
      #endif
    */

    CurrentTime = millis();

    // pas de loop sur ecran déporté
    if (EcranLcd < 2) {
        loopBouton();
    }

    if (FinJeuxRotatif == false) {
        loopJeuxRotatif();
    }

    if (FinJeuxPosition == false) {
        loopJeuxPosition();
    }

    if (FinJeuxOrdre == false) {
        loopJeuxOrdre();
    }

    if (FinJeuxSlider == false) {
        loopJeuxSlider();
    }

    if (FinJeuxPushButton == false) {
        loopJeuxPushButton();
    }

    //if (FinLedPushButton == false) {
    //  loopFinLedPushButton();
    //}

    if (LcdChange == true) {
        AffichageLcd();
    }

    AffichageLed();

    if ((StopAnim1 == false) || (StopAnim2 == false) || (StopAnim3 == false) || (StopAnim4 == false) ||
        (StopAnim5 == false)) {
        ControleAnimation();
    }

    if (TestActif == true) {
        loopTest();
    }
#endif
}

//***********************************************************************
//    DEBUG
//***********************************************************************
/*void TestHc165() {
  digitalWrite(PL_HC165, LOW);
  Trace(20, TAG_INIT, "parralle load 0");
  delay(5000);
  digitalWrite(PL_HC165, HIGH);
  Trace(20, TAG_INIT, "parralle load 1");
  delay(5000);
  digitalWrite(PL_HC165, LOW);
  Trace(20, TAG_INIT, "parralle load 0");

  digitalWrite(CLK_HC165, LOW);
  Trace(20, TAG_INIT, "clock 0");
  delay(5000);
  digitalWrite(CLK_HC165, HIGH);
  Trace(20, TAG_INIT, "clock 1");
  delay(5000);
  digitalWrite(CLK_HC165, LOW);
  Trace(20, TAG_INIT, "clock 0");

  }*/

//*********************************************************************
// loopTestDebug
//***********************************************************************
void loopTestDebug() {
#ifdef DEBUG
    NbLoop ++;
    loopTestVert3();
    loopTestVert2();
    loopTestVert1();
#endif

}

//*********************************************************************
// loopTest vert3
//***********************************************************************
void loopTestVert3() {
    CurrentTime = millis();

    if (MatrBoutonTraitement[BtnVert3] == 1) {
        MatrBoutonTraitement[BtnVert3] = 0;
        Trace(20, TAG_INIT, "Appui vert 3");
        //Couleur = red;
        //strip.SetPixelColor(0, chooseColor(1));
        //strip.SetPixelColor(1, chooseColor(2));
        //Couleur = blue;
        //strip.SetPixelColor(2, chooseColor(3));
        //Couleur = white;
        //strip.SetPixelColor(3, white);
        strip.SetBrightness(10);
        strip.SetPixelColor(0, CouleurLed(1));
        strip.SetBrightness(30);
        strip.SetPixelColor(1, CouleurLed(2));
        strip.SetBrightness(50);
        strip.SetPixelColor(2, CouleurLed(3));
        strip.SetBrightness(70);
        strip.SetPixelColor(3, CouleurLed(4));
        strip.SetBrightness(90);
        strip.SetPixelColor(4, CouleurLed(5));
        strip.SetBrightness(110);
        strip.SetPixelColor(5, CouleurLed(6));
        strip.SetBrightness(130);
        strip.SetPixelColor(6, CouleurLed(7));
        strip.SetBrightness(150);
        strip.SetPixelColor(7, CouleurLed(8));
        strip.SetBrightness(200);
        strip.SetPixelColor(8, CouleurLed(9));

        // the following line demonstrates rgbw color support
        // if the NeoPixels are rgbw types the following line will compile
        // if the NeoPixels are anything else, the following line will give an error
        //strip.SetPixelColor(3, RgbwColor(colorSaturation));
        strip.Show();

        //VitesseCommutationPort();
        //ScanI2cTotal();
    }
}

//***********************************************************************
// loopTest vert2
//***********************************************************************
void loopTestVert2() {
    CurrentTime = millis();

    if (MatrBoutonTraitement[BtnVert2] == 1) {
        MatrBoutonTraitement[BtnVert2] = 0;
        //AddLcd("Bonjour");
        SupLcdTxt("S07");
        LcdChange = true;
        /*ProgAnimation(ECRAN_ORDRE, 2000 , 100, "Bouh", -1);
          ProgAnimation(ECRAN_POSITION, 2000 , 100, "WIN", 1);
          ProgAnimation(ECRAN_ROTATIF, 2000 , 100, "Goal", 1);
          #ifndef JLL
          ProgAnimation(ECRAN_SLIDER, 2000 , 100, "OUT", 1);
          #endif
          Trace(20, TAG_INIT, "Appui vert 2");*/
    }
}

//***********************************************************************
// loopTest vert1
//***********************************************************************
void loopTestVert1() {
    CurrentTime = millis();

    if (MatrBoutonTraitement[BtnVert1] == 1) {
        MatrBoutonTraitement[BtnVert1] = 0;
        ClearLcd();
        //LcdChange = true;

        //ClearLed();
        Trace(20, TAG_INIT, "Appui vert 1");
    }
}

//***********************************************************************
// RazGene Raz générale
//***********************************************************************
void RazGene() {

    // jeux
    FinJeuxRotatif = true;
    FinJeuxPosition = true;
    FinJeuxOrdre = true;
    FinJeuxSlider = true;
    FinJeuxPushButton = true;

    // animation
    StopAnim1 = true;
    StopAnim2 = true;
    StopAnim3 = true;
    StopAnim4 = true;
    StopAnim5 = true;

    // test
    TestActif = false;

    // écran
    ClearLcd();
    //LcdChange = true;
    ResetOled();

    //led
    ClearLed();

}

/*
  //_______________________________________________________________________
  //    Gestion boutons              ::::::::::::::::::::::::::::::::::::::
  //_______________________________________________________________________
  InitBouton
  loopBouton
  EtatBouton
  ActualisationBouton
*/

//*********************************************************************
// InitBouton
//***********************************************************************
void InitBouton() {
    CurrentTime = millis();

    for (int i = 0; i < TAILLE_BOUTON; i++) {
        MatrBoutonEtat[i] = 1;
        MatrBoutonMemorise[i] = 0;
        MatrBoutonTraitement[i] = 0;
        MatrBoutonTime[i] = CurrentTime;
    }
    DerniereLectureBouton = CurrentTime;
}

//*********************************************************************
// loopBouton
//***********************************************************************
void loopBouton() {
    CurrentTime = millis();

    if (CurrentTime > (DerniereLectureBouton + INTERVAL_LECTURE_BOUTON)) {
        DerniereLectureBouton = CurrentTime;
        EtatBouton();
        ActualisationBouton();
    }
}

//*********************************************************************
// EtatBouton lecture de l'état des boutons
//***********************************************************************
void EtatBouton() {
    String ChaineTrav = "status : ";

    CurrentTime = millis();

    // init clock
    digitalWrite(CLK_HC165, LOW);

    // on met la pin 1 du HC165 à 0 V pour enregistrer l'état des entrées
    digitalWrite(PL_HC165, LOW);
    delayMicroseconds(1);
    digitalWrite(PL_HC165, HIGH);
    delayMicroseconds(1);

    // lecture des registres
    for (int i = 0; i < TAILLE_BOUTON; i++) {
        // on note l'état de la sortie (pin 9) du HC165
        MatrBoutonEtat[i] = digitalRead(DATA_HC165);  //mémorisation valeur
        // et on envoie un front montant sur la pin 2 pour décaler les valeurs
        // cycle HIGH, LOW 250 ns
        digitalWrite(CLK_HC165, HIGH);
        delayMicroseconds(10);
        digitalWrite(CLK_HC165, LOW);
        delayMicroseconds(10);
    }

    // affichage des résultats
    /*for (int i = 0; i <= 15; i++) {
      ChaineTrav = ChaineTrav + String(MatrBoutonEtat[i]) + " ";
      }
      Trace(20, TAG_ROTATIF, ChaineTrav);*/

}

//*********************************************************************
// ActualisationBouton actualisation des boutons
//***********************************************************************
void ActualisationBouton() {
    //String ChaineTrav = "";

    for (int i = 0; i < TAILLE_BOUTON; i++) {

        // enregistrement anti rebond
        if ((MatrBoutonEtat[i] == 0) && (MatrBoutonMemorise[i] == 1)) {
            MatrBoutonTime[i] = CurrentTime;
        }

        // detection appui
        if ((MatrBoutonEtat[i] == 0) && (MatrBoutonMemorise[i] == 0)) {
            MatrBoutonMemorise[i] = 1;
            MatrBoutonTraitement[i] = 1;
            MatrBoutonTime[i] = CurrentTime;
            //ChaineTrav = "memo traitement : " + String(i); // debug affichage du numero dans la matrice
            //Trace(20, TAG_ROTATIF, ChaineTrav);
            //Serial.println ("memo traitement : " + String(i)); // debug
        }

        // retour à zéro
        if ((MatrBoutonEtat[i] == 1) && (MatrBoutonMemorise[i] == 1) && (MatrBoutonTraitement[i] == 0) &&
            (CurrentTime > (MatrBoutonTime[i] + RebondTime))) {
            MatrBoutonMemorise[i] = 0;
            //ChaineTrav = "effacement memo  : " + String(i); // debug
            //Trace(20, TAG_ROTATIF, ChaineTrav);
            //Serial.println ("effacement memo : " + String(i)); // debug
            // MatrBoutonTraitement[i] = 0;
        }
    }
}

/*
  //_______________________________________________________________________
  //    Gestion leds                 ::::::::::::::::::::::::::::::::::::::
  //_______________________________________________________________________
  ClearLed
  ClearLedMatrice(int pNbLed, int* pMatrPosLed)
  RefreshLed
  RgbColor CouleurLed(int pNum)
  ControleLedBouton
  AffichageLed
  ProgressionLed(int pNbLed, int* pMatrPosLed, int* pPourCent)
*/

//*********************************************************************
// ClearLed
//***********************************************************************
void ClearLed() {
    CurrentTime = millis();
    DernierAffichageLed = CurrentTime;
    DernierRefreshAffichageLed = CurrentTime;

    for (int i = 0; i < PixelCount; i++) {
        MatrLed[i] = 0;
        MatrLedAff[i] = -1;
    }
}

//***********************************************************************
// CouleurLedMatrice Coloration des leds définies par la matrice (raz = clRien)
//***********************************************************************
void CouleurLedMatrice(int pNbLed, int *pMatrPosLed, int pCoul) {
    int NumLed;

    for (int i = 0; i < pNbLed; i++) {
        NumLed = *pMatrPosLed;
        if (i < PixelCount) {
            MatrLed[NumLed] = pCoul;
            *pMatrPosLed++;
        }
    }
}

//*********************************************************************
// RefreshLed
//***********************************************************************
void RefreshLed() {

    for (int i = 0; i < PixelCount; i++) {
        MatrLedAff[i] = -1;
    }
}

//*********************************************************************
// CouleurLed
//***********************************************************************
RgbColor CouleurLed(int pNum) {
    int Rouge = 0;
    int Vert = 0;
    int Bleu = 0;

    switch (pNum) {

        case clRien :
            Rouge = 0;
            Vert = 0;
            Bleu = 0;
            break;

        case clRed :
            Rouge = colorSaturation;
            Vert = 0;
            Bleu = 0;
            break;

        case clGreen :
            Rouge = 0;
            Vert = colorSaturation;
            Bleu = 0;
            break;

        case clBlue :
            Rouge = 0;
            Vert = 0;
            Bleu = colorSaturation;
            break;

        case clYellow :
            Rouge = colorSaturation;
            Vert = colorSaturation;
            Bleu = 0;
            break;

        case clOrange :
            Rouge = colorSaturation;
            Vert = 40;
            Bleu = 0;
            break;

        case clPurple :
            Rouge = 80;
            Vert = 0;
            Bleu = colorSaturation;
            break;

        case clCyan :
            Rouge = 0;
            Vert = colorSaturation;
            Bleu = colorSaturation;
            break;

        case clPink :
            Rouge = colorSaturation;
            Vert = 0;
            Bleu = 80;
            break;

        case clMoneyGreen :
            Rouge = 0;
            Vert = 30;
            Bleu = 0;
            break;

        case clSkyBlue :
            Rouge = 0;
            Vert = 0;
            Bleu = 30;
            break;

        case clWhite :
            Rouge = colorSaturation;
            Vert = colorSaturation;
            Bleu = colorSaturation;
            break;

        default:
            Trace(20, TAG_ROTATIF, "une couleur inconnue est demandé");
            break;
    }

    RgbColor Couleur(Rouge, Vert, Bleu);
    return Couleur;

}

//*********************************************************************
// ControleLedBouton
//***********************************************************************
void ControleLedBouton() {

    if (FinJeuxPushButton == true) {
        digitalWrite(LED_PUSH_BUTTON, LOW);
    } else {
        digitalWrite(LED_PUSH_BUTTON, HIGH);
    }

    if (FinJeuxOrdre == true) {
        digitalWrite(LED_SYMBOLE, LOW);
    } else {
        digitalWrite(LED_SYMBOLE, HIGH);
    }
}

//*********************************************************************
// AffichageLed
//***********************************************************************
void AffichageLed() {
    boolean Changement;

    CurrentTime = millis();

    // controle refresh
    if (CurrentTime > (DernierRefreshAffichageLed + INTERVAL_REFRESH_AFFICHAGE_LED)) {
        DernierRefreshAffichageLed = CurrentTime;
        RefreshLed();
    }

    // controle du rafraichissement
    if (CurrentTime > (DernierAffichageLed + INTERVAL_AFFICHAGE_LED)) {
        DernierAffichageLed = CurrentTime;
        DernierRefreshAffichageLed = CurrentTime;
        ControleLedBouton(); // controle des leds boutons
        Changement = false;
        // Luminosité
        if (Brightness < 10) {
            Brightness = 10;
        }
        if (Brightness > 250) {
            Brightness = 250;
        }
        strip.SetBrightness(Brightness); // réglage luminosité

        //strip.setBrightness(50); // adafruit

        for (int i = 0; i < PixelCount; i++) {
            if (MatrLed[i] != MatrLedAff[i]) {
                Changement = true;
                MatrLedAff[i] = MatrLed[i];
                //Trace(20, TAG_TEST, "Changement led : " + String(i) + "  coul : " + String(MatrLed[i]));
                strip.SetPixelColor(i, CouleurLed(MatrLed[i]));
            }
        }

        if (Changement = true) {
            strip.Show();
        }
    }
}

//***********************************************************************
// ProgressionLed Calcul et affichage de la progression
//***********************************************************************
void ProgressionLed(int pNbLed, int *pMatrPosLed, int *pPourCent) {
    int Level;
    int NumLed;
    int CoulLed;
    int Progress;

    // Détermination couleur led
    Progress = *pPourCent;

    CoulLed = clGreen;

    if (Progress > 70) {
        CoulLed = clYellow;
    }
    if (Progress > 90) {
        CoulLed = clOrange;
    }

    //Trace(20, TAG_TEST, "Progress led : " + String(Progress) + "  coul : " + String(CoulLed));

    Level = pNbLed * (100 - *pPourCent) / 100;
    Level++;

    for (int i = 0; i < pNbLed; i++) {
        NumLed = *pMatrPosLed;
        if (i < Level) {
            MatrLed[NumLed] = CoulLed;
        } else {
            MatrLed[NumLed] = clRien;
        }
        *pMatrPosLed++;
    }
    RefreshLed();
}

/*
  //_______________________________________________________________________
  //    Jeux rotatif                 ::::::::::::::::::::::::::::::::::::::
  //_______________________________________________________________________
  RazCompteurRotatif
  ControleRazCompteur
  ControleEnregistrementRotatif
  ControleConsigneRotatif
  loopJeuxRotatif
*/
//********************************************************************
// RazCompteurRotatif
//********************************************************************
void RazCompteurRotatif() {
    CompteurRotation = 0;
    LastBtnDt = MatrBoutonEtat[BtnDt];
    LastBtnClk = MatrBoutonEtat[BtnClk];

    for (int i = 0; i < LevelJeuxRotatif; i++) {
        RotationResult[i] = 0;
    }
    Trace(3, TAG_ROTATIF, "Raz resultat");
}

//********************************************************************
// ControleRazCompteur
//********************************************************************
void ControleRazCompteur() {
    if (MatrBoutonTraitement[BtnSw] == 1) {
        MatrBoutonTraitement[BtnSw] = 0;
        RazCompteurRotatif();
    }
}

//*****************************************************************
// controle enregistrement rotatif
//*****************************************************************
void ControleEnregistrementRotatif(int pIncrement) {

    if (SensRotationMemorise == pIncrement) {
        CompteurRotation = CompteurRotation + pIncrement;

    } else {
        SensRotationMemorise = pIncrement;
        //RotationResult[IndexRotatif] = CompteurRotation;
        //ControleConsigneRotatif();
        IndexRotatif++;
        IndexRotatif = IndexRotatif % LevelJeuxRotatif;
        CompteurRotation = pIncrement;
    }

    RotationResult[IndexRotatif] = CompteurRotation;
    ControleConsigneRotatif();
}

//*****************************************************************
// controle consigne rotatif
//*****************************************************************
void ControleConsigneRotatif() {
    const int Ecart = 0; // écart maxi autorisé en pas (1 tour = 20 pas)
    int j;
    boolean ControleOk;


    Trace(2, TAG_ROTATIF, "Controle consigne " + String(CompteurRotation));
    // controle
    ControleOk = true;
    for (int i = 0; i < LevelJeuxRotatif; i++) {
        j = (i + IndexRotatif + 1) % LevelJeuxRotatif;
        Trace(2, TAG_ROTATIF,
              "i : " + String(i) + " j : " + String(j) + "  consigne " + String(ConsigneRotation[i]) + "  result " +
              String(RotationResult[j]));
        if (abs(ConsigneRotation[i] - RotationResult[j]) > Ecart) {
            ControleOk = false;
        }
    }

    // affichage pour test
    if (ControleOk == true) {
        RotatifOk = true;
        FinJeuxRotatif = true;
        ClearOled(ECRAN_ROTATIF, false);
        SendModuleResult(NomModuleRotatif, CodeModuleRotatif, (CurrentTime - StartJeuxRotatif), IdAnswerRotatif, true);
        ProgAnimation(ECRAN_ROTATIF, 2000, 100, "WIN", 1, true);
        //CouleurLedMatrice(NB_LED_ROTATIF, &MatrLedRotatif[0], clGreen);
        //SendMqtt(MQTT_GOOD_MODULE, NomModuleRotatif + "  temps : " + String( CurrentTime - StartJeuxRotatif), false);
    }
}

//***********************************************************************
// loopJeuxRotatif
//***********************************************************************
void loopJeuxRotatif() {
    CurrentTime = millis();

    ControleRazCompteur();

#ifdef KY_040
    // changement Clk
    if (LastBtnClk != MatrBoutonEtat[BtnClk]) {
        LastBtnClk = MatrBoutonEtat[BtnClk];
        if (ChangeDt == true) {
            ControleEnregistrementRotatif(-1);
            //CompteurRotation--;
            Trace(20, TAG_ROTATIF, "Compteur rotatif decrement : " + String(CompteurRotation));
        } else {
            ChangeClk = true;
        }
    }

    // changement dt
    if (LastBtnDt != MatrBoutonEtat[BtnDt]) {
        LastBtnDt = MatrBoutonEtat[BtnDt];
        if (ChangeClk == true) {
            ControleEnregistrementRotatif(1);
            //CompteurRotation++;
            Trace(20, TAG_ROTATIF, "Compteur rotatif increment : " + String(CompteurRotation));
        } else {
            ChangeDt = true;
        }
    }

    // Etat repos
    if (MatrBoutonEtat[BtnDt] == MatrBoutonEtat[BtnClk]) {
        ChangeClk = false;
        ChangeDt = false;
    }

#else
    // début enregitrement
    if (MatrBoutonTraitement[BtnClk] == 1) {
        MatrBoutonTraitement[BtnClk] = 0;
        if (MatrBoutonEtat[BtnDt] == 0) {
            ControleEnregistrementRotatif(-1);
            //SensHoraire = false;
        } else {
            ControleEnregistrementRotatif(1);
            //SensHoraire = true;
        }


        // Controle inversion
        /*
          if (abs(CompteurRotation) < 1) {
          SensHoraireMemorise = SensHoraire;
          } else {
          if (SensHoraire != SensHoraireMemorise) {
            SensHoraireMemorise = SensHoraire;
            Trace(2, TAG_ROTATIF, "Compteur change sens : " + String(CompteurRotation));
            RotationResult[IndexRotatif] = CompteurRotation;
            ControleConsigneRotatif();
            IndexRotatif ++;
            IndexRotatif = IndexRotatif % LevelJeuxRotatif;
            CompteurRotation = 0;
          }
          }*/


        // incrementation compteur
        /*
          if (SensHoraire == true) {
          CompteurRotation ++;
          } else {
          CompteurRotation --;
          }
          Trace(5, TAG_ROTATIF, "Compteur : " + String(CompteurRotation));
          if (abs(CompteurRotation) > 1) {
          RotationResult[IndexRotatif] = CompteurRotation;
          ControleConsigneRotatif();
          }*/
    }
#endif

    //Trace(5, TAG_ROTATIF, "Compteur : " + String(CompteurRotation));
    if (FinJeuxRotatif == false) {
        // calcul et affichage avancement
        if (Progression(CurrentTime, StartJeuxRotatif, TimeJeuxRotatif, &RotatifPourCent, ECRAN_ROTATIF) == true) {
            ProgressionLed(NB_LED_ROTATIF, &MatrLedRotatif[0], &RotatifPourCent);
        }

        // test TimeOut
        if (TimeOut(CurrentTime, StartJeuxRotatif, TimeJeuxRotatif, &FinJeuxRotatif, ECRAN_ROTATIF)) {
            RotatifOk = false;
            SendModuleResult(NomModuleRotatif, CodeModuleRotatif, (CurrentTime - StartJeuxRotatif), IdAnswerRotatif,
                             false);
            ProgAnimation(ECRAN_ROTATIF, 2000, 100, "Beuh", -1, false);
            //CouleurLedMatrice(NB_LED_ROTATIF, &MatrLedRotatif[0], clRed);
        }
    }

}

/*
  //_______________________________________________________________________
  //    Jeux position                ::::::::::::::::::::::::::::::::::::::
  //_______________________________________________________________________
  ControleConsignePosition
  loopJeuxPosition
*/

//*****************************************************************
// controle consigne position
//*****************************************************************
void ControleConsignePosition() {
    boolean ControleOk;

    PositionResult[0] = MatrBoutonEtat[BtnPosition1];
    PositionResult[1] = MatrBoutonEtat[BtnPosition2];
    PositionResult[2] = MatrBoutonEtat[BtnPosition3];
    PositionResult[3] = MatrBoutonEtat[BtnPosition4];
    PositionResult[4] = MatrBoutonEtat[BtnPosition5];
    PositionResult[5] = MatrBoutonEtat[BtnPosition6];

    // controle
    ControleOk = true;
    for (int i = 0; i < LevelJeuxPosition; i++) {
        if (ConsignePosition[i] == 1) {
            ConsignePosition[i] = 0;
        } else {
            ConsignePosition[i] = 1;
        }
        if (ConsignePosition[i] != PositionResult[i]) {
            ControleOk = false;
        }
    }

    // affichage pour test
    if (ControleOk == true) {
        PositionOk = true;
        FinJeuxPosition = true;
        ClearOled(ECRAN_POSITION, false);
        SendModuleResult(NomModulePosition, CodeModulePosition, (CurrentTime - StartJeuxPosition), IdAnswerPosition,
                         true);
        ProgAnimation(ECRAN_POSITION, 2000, 100, "WIN", 1, true);
        //CouleurLedMatrice(NB_LED_POSITION, &MatrLedPosition[0], clGreen);
    }
}

//***********************************************************************
// loopJeuxPosition
//***********************************************************************
void loopJeuxPosition() {
    CurrentTime = millis();

    ControleConsignePosition();

    if (FinJeuxPosition == false) {
        // calcul et affichage avancement
        if (Progression(CurrentTime, StartJeuxPosition, TimeJeuxPosition, &PositionPourCent, ECRAN_POSITION) == true) {
            ProgressionLed(NB_LED_POSITION, &MatrLedPosition[0], &PositionPourCent);
        }

        // test TimeOut
        if (TimeOut(CurrentTime, StartJeuxPosition, TimeJeuxPosition, &FinJeuxPosition, ECRAN_POSITION)) {
            PositionOk = false;
            SendModuleResult(NomModulePosition, CodeModulePosition, (CurrentTime - StartJeuxPosition), IdAnswerPosition,
                             false);
            ProgAnimation(ECRAN_POSITION, 2000, 100, "Bouh", -1, false);
            //CouleurLedMatrice(NB_LED_POSITION, &MatrLedPosition[0], clRed);
        }
    }
}

/*
  //_______________________________________________________________________
  //    Jeux slider                  ::::::::::::::::::::::::::::::::::::::
  //_______________________________________________________________________
  InitBufferCirculaire
  int MoyenneAnalogique(int pVal, int pNum)
  int LectureSlider(int pVoie, int pNum, int pValTest)
  ControleConsigneSlider
  loopJeuxSlider
*/

//*********************************************************************
// InitBufferCirculaire
//***********************************************************************
void InitBufferCirculaire() {

    for (int i = 0; i < TAILLE_ENRG_DAC; i++) {
        MatrCircDac1[i] = 0;
        MatrCircDac2[i] = 0;
        MatrCircDac3[i] = 0;
    }
}

//*****************************************************************
// Moyenne Analogique
//*****************************************************************
int MoyenneAnalogique(int pVal, int pNum) {
    int Somme = 0;
    int Moyenne = 0;

    if (pNum == 1) {
        IdxMatrDac1++;
        IdxMatrDac1 = IdxMatrDac1 % (TAILLE_ENRG_DAC);
        MatrCircDac1[IdxMatrDac1] = pVal;
        Somme = 0;
        for (int i = 0; i < TAILLE_ENRG_DAC; i++) {
            Somme = Somme + MatrCircDac1[i];
            //Trace(20, TAG_SLIDER, "somme i " + String(i) + " : " + String(Somme));
        }
    }

    if (pNum == 2) {
        IdxMatrDac2++;
        IdxMatrDac2 = IdxMatrDac2 % (TAILLE_ENRG_DAC);
        MatrCircDac2[IdxMatrDac2] = pVal;
        Somme = 0;
        for (int i = 0; i < TAILLE_ENRG_DAC; i++) {
            Somme = Somme + MatrCircDac2[i];
        }
    }

    if (pNum == 3) {
        IdxMatrDac3++;
        IdxMatrDac3 = IdxMatrDac3 % (TAILLE_ENRG_DAC);
        MatrCircDac3[IdxMatrDac3] = pVal;
        Somme = 0;
        for (int i = 0; i < TAILLE_ENRG_DAC; i++) {
            Somme = Somme + MatrCircDac3[i];
        }
    }

    Moyenne = Somme / TAILLE_ENRG_DAC;
    //Trace(20, TAG_SLIDER, "moyenne : " + String(Moyenne));
    return (Moyenne);
}

//*****************************************************************
// lecture slider
//*****************************************************************
int LectureSlider(int pVoie, int pNum, int pValTest) {
    int SliderEtal[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int IdxEep;
    int BorneInf;
    int ValSlider;
    int Ecart;
    int Delta;
    float SupBorne;

    // récupération matrice étalonnage
    //IdxEep = pNum * 10;
    IdxEep = EEP_SLIDER1_ETAL + ((pNum - 1) * 20);
    for (int i = 0; i < 10; i++) {
        SliderEtal[i] = LectureEepromDouble(IdxEep);
        IdxEep++;
        IdxEep++;
    }

    // lecture valeur
    if (pValTest > 0) {
        ValAnalogique = pValTest;
    } else {
        ValAnalogique = analogRead(pVoie);
        //Trace(20, TAG_SLIDER, "val analogique reelle " + String(pNum) + " : " + String(ValAnalogique));
        // correction valeur buffer circulaire
        ValAnalogique = MoyenneAnalogique(ValAnalogique, pNum);
        //Trace(20, TAG_SLIDER, "val analogique moyenne " + String(pNum) + " : " + String(ValAnalogique));
    }

    // recherche borne inférieure
    for (int i = 0; i < 9; i++) {
        BorneInf = i;
        if ((ValAnalogique >= SliderEtal[i]) && (ValAnalogique < SliderEtal[i + 1])) {
            break;
        }
    }
    // conversion
    // Init
    ValSlider = 0;

    if (ValAnalogique >= SliderEtal[9]) {
        ValSlider = 9;
    }

    if ((ValAnalogique >= SliderEtal[0]) && (ValAnalogique < SliderEtal[9])) {
        ValSlider = BorneInf;
        Ecart = SliderEtal[BorneInf + 1] - SliderEtal[BorneInf];
        //Trace(20, TAG_TEST, "val slider " + String(pNum) + " : " + String(ValAnalogique) + "  arrondie : " + String(ValSlider));
        Delta = ValAnalogique - SliderEtal[BorneInf];
        //Trace(20, TAG_TEST, "Ecart " + String(Ecart) + "  delta : " + String(Delta));
        SupBorne = float(Delta) / float(Ecart);
        if (SupBorne > 0.5) {
            ValSlider = BorneInf + 1;
            //Trace(20, TAG_TEST, "Depassement moitie " + String(ValSlider));
        }

    }

    // transformation valeur analogique lue (200..2600) en (0..10)
    //int ValAnalogique = analogRead(pVoie);
    //int ValNormalisee = round(float((ValAnalogique - 200) / 240));
    //Trace(20, TAG_SLIDER, "val slider final " + String(pNum) + " : " + String(ValAnalogique) + "  arrondie : " + String(ValSlider));
    return (ValSlider);
}

//*****************************************************************
// controle consigne slider
//*****************************************************************
void ControleConsigneSlider() {
    boolean ControleOk;
    //boolean ChangementEtat;

    //int ValAnalog1 = analogRead(Slider1);
    //int ValAnalog2 = analogRead(Slider2);
    //int ValAnalog3 = analogRead(Slider3);

    //SliderResult[0] = round(float((ValAnalog1 - 200) / 240));
    //SliderResult[1] = round(float((ValAnalog2 - 200) / 240));
    //SliderResult[2] = round(float((ValAnalog3 - 200) / 240));

    SliderResult[0] = LectureSlider(Slider1, 1, 0);
    SliderResult[1] = LectureSlider(Slider2, 2, 0);
    SliderResult[2] = LectureSlider(Slider3, 3, 0);

    // controle
    //Trace(20, TAG_SLIDER, "val slider 1 : " + String(ValAnalog1) + "  arrondie : " + String(SliderResult[0]));
    //Trace(20, TAG_SLIDER, "val slider 2 : " + String(ValAnalog2) + "  arrondie : " + String(SliderResult[1]));
    //Trace(20, TAG_SLIDER, "val slider 3 : " + String(ValAnalog3) + "  arrondie : " + String(SliderResult[2]));
    ControleOk = true;
    for (int i = 0; i < LevelJeuxSlider; i++) {
        /* if (ChangementEtat == true) {
           Trace(2, TAG_SLIDER, "i : " + String(i) + "  consigne " + String(ConsigneSlider[i]) + "  result " + String(SliderResult[i]));
          }*/

        if (ConsigneSlider[i] != SliderResult[i]) {
            ControleOk = false;
        }
    }

    // affichage pour test
    if (ControleOk == true) {
        ClearOled(ECRAN_SLIDER, false);
        SliderOk = true;
        FinJeuxSlider = true;
        SendModuleResult(NomModuleSlider, CodeModuleSlider, (CurrentTime - StartJeuxSlider), IdAnswerSlider, true);
        ProgAnimation(ECRAN_SLIDER, 2000, 100, "WIN", 1, true);
        //CouleurLedMatrice(NB_LED_SLIDER, &MatrLedSlider[0], clGreen);
    }
}

//***********************************************************************
// loopJeuxSlider
//***********************************************************************
void loopJeuxSlider() {

    CurrentTime = millis();

    if (CurrentTime > (DerniereLectureSlider + INTERVAL_LECTURE_SLIDER)) {
        DerniereLectureSlider = CurrentTime;
        ControleConsigneSlider();
    }

    if (FinJeuxSlider == false) {
        if (Progression(CurrentTime, StartJeuxSlider, TimeJeuxSlider, &SliderPourCent, ECRAN_SLIDER) == true) {
            ProgressionLed(NB_LED_SLIDER, &MatrLedSlider[0], &SliderPourCent);
        }

        // test TimeOut
        if (TimeOut(CurrentTime, StartJeuxSlider, TimeJeuxSlider, &FinJeuxSlider, ECRAN_SLIDER)) {
            SliderOk = false;
            SendModuleResult(NomModuleSlider, CodeModuleSlider, (CurrentTime - StartJeuxSlider), IdAnswerSlider, false);
            ProgAnimation(ECRAN_SLIDER, 2000, 100, "Bouh", -1, false);
            //CouleurLedMatrice(NB_LED_SLIDER, &MatrLedSlider[0], clRed);
        }
    }
}

/*
  //_______________________________________________________________________
  //    Jeux ordre                   ::::::::::::::::::::::::::::::::::::::
  //_______________________________________________________________________
  int BoutonOrdreSelect
  ControleConsigneOrdre
  loopJeuxOrdre
*/

//*****************************************************************
//  BoutonOrdreSelect  renvoie le numéro du bouton appuyé (0 = rien)
//*****************************************************************
int BoutonOrdreSelect() {

    int NumeroBouton = 0;

    if (MatrBoutonTraitement[BtnOrdre1] == 1) {
        MatrBoutonTraitement[BtnOrdre1] = 0;
        NumeroBouton = 1;
    }

    if (MatrBoutonTraitement[BtnOrdre2] == 1) {
        MatrBoutonTraitement[BtnOrdre2] = 0;
        if (NumeroBouton == 0) {
            NumeroBouton = 2;
        }
    }

    if (MatrBoutonTraitement[BtnOrdre3] == 1) {
        MatrBoutonTraitement[BtnOrdre3] = 0;
        if (NumeroBouton == 0) {
            NumeroBouton = 3;
        }
    }
    if (MatrBoutonTraitement[BtnOrdre4] == 1) {
        MatrBoutonTraitement[BtnOrdre4] = 0;
        if (NumeroBouton == 0) {
            NumeroBouton = 4;
        }
    }

    if (MatrBoutonTraitement[BtnOrdre5] == 1) {
        MatrBoutonTraitement[BtnOrdre5] = 0;
        if (NumeroBouton == 0) {
            NumeroBouton = 5;
        }
    }
    if (MatrBoutonTraitement[BtnOrdre6] == 1) {
        MatrBoutonTraitement[BtnOrdre6] = 0;
        if (NumeroBouton == 0) {
            NumeroBouton = 6;
        }
    }

    return (NumeroBouton);
}

//*****************************************************************
// controle consigne ordre
//*****************************************************************
void ControleConsigneOrdre() {
    int j;
    boolean ControleOk;

    // controle
    ControleOk = true;
    for (int i = 0; i < LevelJeuxOrdre; i++) {
        j = (i + IndexOrdre + 1) % LevelJeuxOrdre;
        Trace(5, TAG_ORDRE,
              "i : " + String(i) + " j : " + String(j) + "  consigne " + String(ConsigneOrdre[i]) + "  result " +
              String(OrdreResult[j]));
        if ((ConsigneOrdre[i] - OrdreResult[j]) != 0) {
            ControleOk = false;
        }
    }

    // affichage pour test
    if (ControleOk == true) {
        OrdreOk = true;
        FinJeuxOrdre = true;
        ClearOled(ECRAN_ORDRE, false);
        SendModuleResult(NomModuleOrdre, CodeModuleOrdre, (CurrentTime - StartJeuxOrdre), IdAnswerOrdre, true);
        ProgAnimation(ECRAN_ORDRE, 2000, 100, "WIN", 1, true);
        //CouleurLedMatrice(NB_LED_ORDRE, &MatrLedOrdre[0], clGreen);
    }
}

//***********************************************************************
// loopJeuxOrdre
//***********************************************************************
void loopJeuxOrdre() {
    CurrentTime = millis();
    int PourCent = 0;
    int NumeroBoutonSelectionne;

    NumeroBoutonSelectionne = BoutonOrdreSelect();

    if (NumeroBoutonSelectionne > 0) {
        Trace(20, TAG_ORDRE, "Bouton sélectionné : " + String(NumeroBoutonSelectionne));
        OrdreResult[IndexOrdre] = NumeroBoutonSelectionne;
        ControleConsigneOrdre();
        IndexOrdre++;
        IndexOrdre = IndexOrdre % LevelJeuxOrdre;
    }

    if (FinJeuxOrdre == false) {
        // calcul et affichage progression
        if (Progression(CurrentTime, StartJeuxOrdre, TimeJeuxOrdre, &OrdrePourCent, ECRAN_ORDRE) == true) {
            ProgressionLed(NB_LED_ORDRE, &MatrLedOrdre[0], &OrdrePourCent);
        }

        // test TimeOut
        if (TimeOut(CurrentTime, StartJeuxOrdre, TimeJeuxOrdre, &FinJeuxOrdre, ECRAN_ORDRE)) {
            OrdreOk = false;
            SendModuleResult(NomModuleOrdre, CodeModuleOrdre, (CurrentTime - StartJeuxOrdre), IdAnswerOrdre, false);
            ProgAnimation(ECRAN_ORDRE, 2000, 100, "Bouh", -1, false);
            //CouleurLedMatrice(NB_LED_ORDRE, &MatrLedOrdre[0], clRed);
        }
    }
}
/*
  //_______________________________________________________________________
  //    Jeux push button             ::::::::::::::::::::::::::::::::::::::
  //_______________________________________________________________________
  PushButtonOk
  loopFinLedPushButton
  ControleConsignePushButton
  loopJeuxPushButton
*/
//***********************************************************************
// PushButtonOk
//***********************************************************************
void PushButtonOk() {
    FinJeuxPushButton = true;
    ProgAnimation(NO_ECRAN, 2000, 100, "Rien", 1, true);
    //CouleurLedMatrice(NB_LED_PUSH_BUTTON, &MatrLedPushButton[0], clGreen);
    //FinLedPushButton = false;
    //TimeFinLedPushButton = CurrentTime + 2000;
}

//***********************************************************************
// loopFinLedPushButton
//***********************************************************************
/*void loopFinLedPushButton() {
  CurrentTime = millis();

  if (CurrentTime > TimeFinLedPushButton) {
    FinLedPushButton = true;
    CouleurLedMatrice(NB_LED_PUSH_BUTTON, &MatrLedPushButton[0], clRien);
  }
  }*/

//*****************************************************************
// controle consigne push button
//*****************************************************************
void ControleConsignePushButton() {

    if (MatrBoutonTraitement[BtnPushButton] == 1) {
        MatrBoutonTraitement[BtnPushButton] = 0;
        //FinJeuxPushButton = true;
        //Trace(20, TAG_ORDRE, "Appui push button : " + String(BtnPushButton));
        ButtonOk = true;
        SendModuleResult(NomModulePushButton, CodeModulePushButton, (CurrentTime - StartJeuxPushButton),
                         IdAnswerPushButton, true);
        //CouleurLedMatrice(NB_LED_PUSH_BUTTON, &MatrLedPushButton[0], clGreen);
    }

}

//***********************************************************************
// loopJeuxPushButton
//***********************************************************************
void loopJeuxPushButton() {

    CurrentTime = millis();

    if (CurrentTime > (DerniereLecturePushButton + INTERVAL_LECTURE_PUSH_BUTTON)) {
        DerniereLecturePushButton = CurrentTime;
        ControleConsignePushButton();
    }

    if (FinJeuxPushButton == false) {
        if (Progression(CurrentTime, StartJeuxPushButton, TimeJeuxPushButton, &PushButtonPourCent, NO_ECRAN) == true) {
            ProgressionLed(NB_LED_PUSH_BUTTON, &MatrLedPushButton[0], &PushButtonPourCent);
        }

        // test TimeOut
        if (TimeOut(CurrentTime, StartJeuxPushButton, TimeJeuxPushButton, &FinJeuxPushButton, NO_ECRAN)) {
            ButtonOk = false;
            FinJeuxPushButton = true;
            SendModuleResult(NomModulePushButton, CodeModulePushButton, (CurrentTime - StartJeuxPushButton),
                             IdAnswerPushButton, false);
            ProgAnimation(NO_ECRAN, 2000, 100, "Bouh", -1, false);
            //CouleurLedMatrice(NB_LED_PUSH_BUTTON, &MatrLedPushButton[0], clRed);
            //FinLedPushButton = false;
            //TimeFinLedPushButton = CurrentTime + 2000;
            //Trace(20, TAG_ORDRE, "Time out push button : " + String(BtnPushButton));
        }
    }
}

/*
  //_______________________________________________________________________
  //    TimeOut                      ::::::::::::::::::::::::::::::::::::::
  //_______________________________________________________________________
  TimeOut (int pActuel, int pStart, int pTime, boolean* pFin, int pEcran )
*/

//*****************************************************************
// TimeOut controle du temps de jeux
//*****************************************************************
boolean TimeOut(int pActuel, int pStart, int pTime, boolean *pFin, int pEcran) {
    if ((pActuel > (pStart + (pTime * 1000))) && (*pFin == false)) {
        *pFin = true;
        // -1 pas d'écran oled
        if (pEcran >= 0) {
            ClearOled(pEcran, false);
        }
    }
    return (*pFin);
}

/*
  //_______________________________________________________________________
  //    Initialisation jeux          ::::::::::::::::::::::::::::::::::::::
  //_______________________________________________________________________
  InitJeux(String pTopic, String pMes)
  ProgConsigne(int* pMatrSource, int* pMatrDest, String pNom)
  CodeOled(String jsCode, int pEcran)
  ProgJeux(String jsNom, String* pNom, String jsCode, String* pCode, int jsTime, int* pTime, int jsLevel, int* pLevel, int* pStart, int* pIndex, boolean* pOk, boolean* pFinJeux)
*/

//********************************************************************
// Initialisation jeux
//********************************************************************
void InitJeux(String pTopic, String pMes) {
    String CodeErr;
    int Consigne[] = {0, 0, 0, 0, 0, 0};

    //Trace(20, TAG_INIT, "topic : " + String(pTopic) + " (" + String(pTopic.length()) + ")");
    //Trace(20, TAG_INIT, "Data : " + String(pMes) + " (" + String(pMes.length()) + ")");

    CurrentTime = millis();
    // Deserialize the JSON document
    doc.clear();
    DeserializationError error = deserializeJson(doc, pMes);
    // Test if parsing succeeds.
    if (error) {
        CodeErr = error.c_str();
        Trace(20, TAG_JSON, "deserializeJson() init jeux failed : " + CodeErr);
    }

    const char *moduleName = doc[JSON_NOM];
    const char *answerCode = doc[JSON_CODE];
    for (int i = 0; i < 6; i++) {
        Consigne[i] = doc[JSON_REPONSE][i];
    }
    int TempsJeux = doc[JSON_TIME];
    int LevelJeux = doc[JSON_LEVEL];
    int IdAnswer = doc[JSON_ID_PLAYER_ANSWER];


    Trace(5, TAG_INIT, "Init jeux : " + String(moduleName));


    if (String(moduleName) == MODULE_ROTATIF) {
        Trace(3, TAG_ROTATIF, "start rotatif");
        ArretAnimation(ECRAN_ROTATIF);
        ProgJeux(String(moduleName), &NomModuleRotatif, String(answerCode), &CodeModuleRotatif, TempsJeux,
                 &TimeJeuxRotatif, LevelJeux, &LevelJeuxRotatif, IdAnswer, &IdAnswerRotatif, &StartJeuxRotatif,
                 &IndexRotatif, &RotatifOk, &FinJeuxRotatif);
        ProgConsigne(&Consigne[0], &ConsigneRotation[0], &RotationResult[0], String(moduleName));
        RazCompteurRotatif();
        //Trace(20, TAG_ROTATIF, "start rotatif : " + String(ConsigneRotation[0]) + ", " + String(ConsigneRotation[1]) + ", " + String(ConsigneRotation[2]) + ", " + String(ConsigneRotation[3]) + ", " + String(ConsigneRotation[4]));
        // affichage Oled
        CodeOled(String(answerCode), ECRAN_ROTATIF);
    }

    if (String(moduleName) == MODULE_POSITION) {
        Trace(5, TAG_POSITION, "start position");
        ArretAnimation(ECRAN_POSITION);
        ProgJeux(String(moduleName), &NomModulePosition, String(answerCode), &CodeModulePosition, TempsJeux,
                 &TimeJeuxPosition, LevelJeux, &LevelJeuxPosition, IdAnswer, &IdAnswerPosition, &StartJeuxPosition,
                 &IndexPosition, &PositionOk, &FinJeuxPosition);
        ProgConsigne(&Consigne[0], &ConsignePosition[0], &PositionResult[0], String(moduleName));
        CodeOled(String(answerCode), ECRAN_POSITION);
    }
    if (String(moduleName) == MODULE_SLIDER) {
        Trace(5, TAG_SLIDER, "start slider");
        ArretAnimation(ECRAN_SLIDER);
        InitBufferCirculaire();
        ProgJeux(String(moduleName), &NomModuleSlider, String(answerCode), &CodeModuleSlider, TempsJeux,
                 &TimeJeuxSlider, LevelJeux, &LevelJeuxSlider, IdAnswer, &IdAnswerSlider, &StartJeuxSlider,
                 &IndexSlider, &SliderOk, &FinJeuxSlider);
        ProgConsigne(&Consigne[0], &ConsigneSlider[0], &SliderResult[0], String(moduleName));
        DerniereLectureSlider = CurrentTime;

        CodeOled(String(answerCode), ECRAN_SLIDER);
    }

    if (String(moduleName) == MODULE_ORDRE) {
        Trace(5, TAG_ORDRE, "start ordre");
        ArretAnimation(ECRAN_ORDRE);
        ProgJeux(String(moduleName), &NomModuleOrdre, String(answerCode), &CodeModuleOrdre, TempsJeux, &TimeJeuxOrdre,
                 LevelJeux, &LevelJeuxOrdre, IdAnswer, &IdAnswerOrdre, &StartJeuxOrdre, &IndexOrdre, &OrdreOk,
                 &FinJeuxOrdre);
        ProgConsigne(&Consigne[0], &ConsigneOrdre[0], &OrdreResult[0], String(moduleName));
        CodeOled(String(answerCode), ECRAN_ORDRE);
    }

    if (String(moduleName) == MODULE_PUSH_BUTTON) {
        Trace(5, TAG_ORDRE, "start push button");
        ProgJeux(String(moduleName), &NomModulePushButton, String(answerCode), &CodeModulePushButton, TempsJeux,
                 &TimeJeuxPushButton, LevelJeux, &LevelJeuxPushButton, IdAnswer, &IdAnswerPushButton,
                 &StartJeuxPushButton, &IndexPushButton, &ButtonOk, &FinJeuxPushButton);
        MatrBoutonTraitement[BtnPushButton] = 0;
        //ProgConsigne( &Consigne[0], &ConsigneOrdre[0], String(moduleName));
        //CodeOled(String(answerCode), ECRAN_ORDRE);
    }
    //Trace(20, TAG_INIT, "Consigne rotation : " + String(ConsigneRotation[0]) + "," +String(ConsigneRotation[1]) + "," +String(ConsigneRotation[2]) + "," +String(ConsigneRotation[3]) + "," +String(ConsigneRotation[4]));
}

//********************************************************************
// ProgConsigne
//********************************************************************
void ProgConsigne(int *pMatrSource, int *pMatrDest, int *pMatrResult, String pNom) {

    if (pNom == MODULE_ROTATIF) {
        for (int i = 0; i < 6; i++) {
            //*pMatrDest = *pMatrSource / 18;
            *pMatrDest = *pMatrSource;
            *pMatrResult = 0;
            *pMatrDest++;
            *pMatrResult++;
            *pMatrSource++;
        }
    }

    if ((pNom == MODULE_POSITION) || (pNom == MODULE_SLIDER) || (pNom == MODULE_ORDRE)) {
        for (int i = 0; i < 6; i++) {
            *pMatrDest = *pMatrSource;
            *pMatrResult = 0;
            *pMatrDest++;
            *pMatrResult++;
            *pMatrSource++;
        }
    }
}

//********************************************************************
// CodeOled
//********************************************************************
void CodeOled(String jsCode, int pEcran) {
    //ClearOled(pEcran, false);
    OledTexte(20, 20, 4, jsCode, pEcran, true, true);
}

//********************************************************************
// ProgJeux
//********************************************************************
void
ProgJeux(String jsNom, String *pNom, String jsCode, String *pCode, int jsTime, int *pTime, int jsLevel, int *pLevel,
         int jsIdAnswer, int *pIdAnswer, int *pStart, int *pIndex, boolean *pOk, boolean *pFinJeux) {
    *pNom = jsNom;
    *pCode = jsCode;
    *pTime = jsTime;
    *pLevel = jsLevel;
    *pIdAnswer = jsIdAnswer;
    *pStart = CurrentTime;
    *pIndex = 0;
    *pOk = false;
    *pFinJeux = false;

}

/*
  //_______________________________________________________________________
  //    Consigne jeux                ::::::::::::::::::::::::::::::::::::::
  //_______________________________________________________________________
  EffacementConsigneJeux(String pTopic, String pMes)
  int ConsigneInverse(int pConsigne)
  AffichageConsigneJeux(String pTopic, String pMes)
*/

//********************************************************************
// EffacementConsigneJeux
//********************************************************************
void EffacementConsigneJeux(String pTopic, String pMes) {
    String CodeErr;

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, pMes);
    // Test if parsing succeeds.
    if (error) {
        CodeErr = error.c_str();
        Trace(20, TAG_JSON, "deserializeJson() effacement consigne failed: " + CodeErr);
    }
    const char *answerCode = doc[JSON_CODE];

    SupLcdTxt(String(answerCode));
    LcdChange = true;
}

//***********************************************************************
// ConsigneInverse inversion 0 1
//***********************************************************************
int ConsigneInverse(int pConsigne) {
    int Resultat;

    Resultat = 0;
    if (pConsigne == 0) {
        Resultat = 1;
    }
    if (pConsigne == 1) {
        Resultat = 0;
    }
    return (Resultat);

}

//********************************************************************
// AffichageConsigneJeux
//********************************************************************
void AffichageConsigneJeux(String pTopic, String pMes) {
    String CodeErr;
    String LigneLcd = "";
    int Consigne[] = {0, 0, 0, 0, 0, 0};
    //int ConsigneInverse;

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, pMes);
    // Test if parsing succeeds.
    if (error) {
        CodeErr = error.c_str();
        Trace(20, TAG_JSON, "deserializeJson() affichage consigne failed: " + CodeErr);
    }
    /*
      //const char* moduleName = doc["moduleName"];
      //const char* answerCode = doc["answerCode"];
      //for (int i = 0; i < 6; i++) {
      //  Consigne[i] = doc["answer"][i];
      //}
      //int TempsJeux = doc["diffuseTime"];
      //int LevelJeux = doc["level"];
    */


    const char *moduleName = doc[JSON_NOM];
    const char *answerCode = doc[JSON_CODE];
    for (int i = 0; i < 6; i++) {
        Consigne[i] = doc[JSON_REPONSE][i];
    }
    int TempsJeux = doc[JSON_TIME];
    int LevelJeux = doc[JSON_LEVEL];


    LigneLcd = String(answerCode) + ":";

    if ((String(moduleName) == MODULE_ROTATIF) || (String(moduleName) == MODULE_SLIDER)) {

        LigneLcd = LigneLcd + String(Consigne[0]);
        for (int i = 1; i < LevelJeux; i++) {
            LigneLcd = LigneLcd + "," + String(Consigne[i]);
        }
    }

    if (String(moduleName) == MODULE_POSITION) {
        LigneLcd = LigneLcd + String(Consigne[0]);
        //LigneLcd = LigneLcd + String(Consigne[0]);
        for (int i = 1; i < LevelJeux; i++) {
            LigneLcd = LigneLcd + "," + String(Consigne[i]);
            //LigneLcd = LigneLcd + "," + String(Consigne[i]);
        }
    }

    if (String(moduleName) == MODULE_ORDRE) {
        for (int i = 0; i < LevelJeux; i++) {
            LigneLcd = AjoutLcdSpecial(LigneLcd, Consigne[i]);
            LigneLcd = LigneLcd + " ";
        }
    }

    AddLcd(LigneLcd);
    LcdChange = true;
}

/*
  //_______________________________________________________________________
  //    MQTT                         ::::::::::::::::::::::::::::::::::::::
  //_______________________________________________________________________
  callback(char* topic, byte * message, unsigned int length)
  SendModuleResult(String pNom, String pCode, int pTemps, boolean pResult)
  SendMqtt(String pTopic, String pData, boolean pSecure)
*/

//******************************************************************
// callback
//******************************************************************
void callback(char *topic, byte *message, unsigned int length) {
    //Serial.print("Message arrived on topic: ");
    //Serial.print(topic);
    //Serial.print(". Message: ");
    String messageTemp;
    String Topic = "";
    int NumTrav = 0;
    String Prefixe = "";

    Topic = String(topic);
    for (int i = 0; i < length; i++) {
        //Serial.print((char)message[i]);
        messageTemp += (char) message[i];
    }

    Trace(20, TAG_INIT, "receive : " + Topic + " mes: " + messageTemp);

    Prefixe = String(IdBoite) + "/diffuse/" + String(Player) + "/" + MQTT_ACTIVATION_MODULE;
    if ((Topic.indexOf(Prefixe) >= 0) && (EcranLcd < 2)) {
        InitJeux(Topic, messageTemp);
    }

    Prefixe = "server/" + String(IdBoite) + "/" + MQTT_SYST + "/" + String(Player) + "/" + MQTT_LOG;
    if ((Topic.indexOf(Prefixe) >= 0) && (EcranLcd < 2)) {
        ProgTrace(Topic, messageTemp);
    }

    if ((Topic.indexOf(MQTT_CONFIG) >= 0) && (Topic.indexOf(AdresseMac) >= 0)) {
        ProgConfig(Topic, messageTemp);
    }

    if (Topic.indexOf(MQTT_STATUS) >= 0) {
        AffichageStatus(Topic, messageTemp);
        //TestHc165();
        //loopBouton();
    }

    Prefixe = String(IdBoite) + "/diffuse/" + String(Player) + "/" + MQTT_SEND_ANSWER;
    if ((Topic.indexOf(Prefixe) >= 0) && (EcranLcd < 2)) {
        AffichageConsigneJeux(Topic, messageTemp);
    }

    Prefixe = String(IdBoite) + "/diffuse/" + String(Player) + "/" + MQTT_DELETE_ANSWER;
    if ((Topic.indexOf(Prefixe) >= 0) && (EcranLcd < 2)) {
        EffacementConsigneJeux(Topic, messageTemp);
    }

    Prefixe = String(IdBoite) + "/diffuse/" + String(Player) + "/" + MQTT_GOOD_PUSH;
    if ((Topic.indexOf(Prefixe) >= 0) && (EcranLcd < 2)) {
        PushButtonOk();
    }

    Prefixe = "server/" + String(IdBoite) + "/syst/" + MQTT_TEST;
    if ((Topic.indexOf(Prefixe) >= 0) && (EcranLcd < 2)) {
        ProgTest(Topic, messageTemp);
    }

    // raz player
    Prefixe = String(IdBoite) + "/diffuse/" + String(Player) + "/" + MQTT_END;
    if ((Topic.indexOf(Prefixe) >= 0) && (EcranLcd < 2)) {
        RazGene();
    }
    // raz boite
    Prefixe = String(IdBoite) + "/diffuse/" + MQTT_END;
    if ((Topic.indexOf(Prefixe) >= 0) && (EcranLcd < 2)) {
        RazGene();
    }

    // Ecriture ecran
    Prefixe = String(IdBoite) + "/diffuse/" + String(Player) + "/" + MQTT_ECRAN;
    //if ((Topic.indexOf(MQTT_ECRAN) > 0) && (EcranLcd == 2)) {
    if ((Topic.indexOf(Prefixe) >= 0) && (EcranLcd == 2)) {
        EcritureLcd(Topic, messageTemp);
    }

}

//********************************************************************
// SendModuleResult réponse fin de module
//********************************************************************
void SendModuleResult(String pNom, String pCode, int pTemps, int pIdAnswer, boolean pResult) {
    String MesTopic = "";
    String MesData = "";

    MesTopic = "diffuse/" + String(Player) + "/";

    // delay affichage led
    CurrentTime = millis();
    DernierAffichageLed = CurrentTime;
    DernierRefreshAffichageLed = CurrentTime;

    /*if (pResult == true) {
      MesTopic = MesTopic + MQTT_GOOD_MODULE;
      } else {
      MesTopic = MesTopic + MQTT_BAD_MODULE;
      }*/

    if (pResult == true) {
        if (pNom == NomModulePushButton) {
            MesTopic = MesTopic + MQTT_PUSH_BUTTON;
        } else {
            MesTopic = MesTopic + MQTT_GOOD_MODULE;
        }
    } else {
        MesTopic = MesTopic + MQTT_BAD_MODULE;
    }

    doc.clear();
    doc[JSON_NOM] = pNom;
    doc[JSON_CODE] = pCode;
    doc[JSON_TIME] = String(pTemps);
    doc[JSON_ID_PLAYER_ANSWER] = String(pIdAnswer);

    serializeJson(doc, MesData);
    SendMqtt(MesTopic, MesData, false);

    // affichage résultat sur led
    /*
      #ifdef DEBUG
      if (pResult == true) {
        digitalWrite(ledPin1, HIGH);
      } else {
        digitalWrite(ledPin1, LOW);
      }
      #endif
    */

}

//********************************************************************
// SendMqtt envoi Mqtt
//********************************************************************
void SendMqtt(String pTopic, String pData, boolean pSecure) {
    char bufMqttTopic[50];
    char bufMqttData[200];

    String Topic = String(IdBoite);

    if (pSecure == true) {
        Topic = AdresseMac;
    }

    //Topic = Topic + "/server/" + pTopic;
    Topic = Topic + "/" + pTopic;
    Topic.toCharArray(bufMqttTopic, Topic.length() + 1);
    pData.toCharArray(bufMqttData, pData.length() + 1);
    client.publish(bufMqttTopic, bufMqttData);
}

/*
  //_______________________________________________________________________
  //    Outil Scan I2c               ::::::::::::::::::::::::::::::::::::::
  //_______________________________________________________________________
  ScanI2cTotal
  ScanI2c(int pCanal)
*/

//***********************************************************************
//    ScanI2cTotal Scan des periphériques I2C
//***********************************************************************
void ScanI2cTotal() {
    for (int i = 0; i < 8; i++) {
        TcaSelect(i);
        ScanI2c(i);
    }
}

//***********************************************************************
//    ScanI2c Scan des periphériques I2C
//***********************************************************************
void ScanI2c(int pCanal) {
    byte error, address;
    int nDevices;
    String ChaineTrav = "";
    String Addr = "";

    Trace(20, TAG_INIT, "Scanning...TCA voie: " + String(pCanal));
    //Trace(20, TAG_INIT, "Scanning...");


    nDevices = 0;
    for (address = 1; address < 127; address++) {
        //if (addr == TCAADDR) continue;
        Addr = "0x";
        if (address < 16) {
            Addr = Addr + "0";
        }
        Addr = Addr + String(address, HEX);

        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0) {
            ChaineTrav = "I2C device found at address " + Addr;
            Trace(20, TAG_INIT, ChaineTrav);
            nDevices++;
        } else if (error == 4) {
            ChaineTrav = "Unknow error at address " + Addr;
            Trace(20, TAG_INIT, ChaineTrav);
        }
    }

    if (nDevices == 0) {
        Trace(20, TAG_INIT, "No I2C devices found");
    } else {
        Trace(20, TAG_INIT, "done");
    }
}

/*
  //_______________________________________________________________________
  //    TCA9548                      ::::::::::::::::::::::::::::::::::::::
  //_______________________________________________________________________
  TcaOff
  byte TcaVoieActive
  TcaSelect(uint8_t i)
*/

//***********************************************************************
//    TcaOff deselection sortie
//***********************************************************************
void TcaOff() {
    Wire.beginTransmission(TCAADDR);
    Wire.write(0);  // no channel selected
    Wire.endTransmission();
}

//***********************************************************************
//    TcaVoieActive voie TCA9548A selectionnée
//***********************************************************************
byte TcaVoieActive() {
    byte voieSelect;
    Wire.beginTransmission(TCAADDR);
    Wire.requestFrom(TCAADDR, 1);
    voieSelect = Wire.read();
    return (voieSelect);
}

//***********************************************************************
//    TcaSelect Selection voie TCA9548A
//***********************************************************************
void TcaSelect(uint8_t i) {
    byte error;

    // pas de Tca sur écran déporté
    if (EcranLcd < 2) {
        if (i > 7) return;

        Wire.beginTransmission(TCAADDR);
        Wire.write(1 << i);
        error = Wire.endTransmission();
        if (error != 0) {
            Trace(20, TAG_INIT, "erreur tcaSelect : " + String(error));
        }
    }
}

/*
  //_______________________________________________________________________
  //  Progression OLED               ::::::::::::::::::::::::::::::::::::::
  //_______________________________________________________________________
  Progression(int pActuel, int pStart, int pTime, int* pPourCent, int pEcran)
  CalculPourcentage(int pActuel, int pStart, int pTime)
  OledProgress(int pPourCent, int pNumEcran)
*/

//***********************************************************************
// Progression Calcul et affichage de la progression
//***********************************************************************
boolean Progression(int pActuel, int pStart, int pTime, int *pPourCent, int pEcran) {
    int PourCent = 0;
    boolean ChangementProgress;

    ChangementProgress = false;

    PourCent = CalculPourcentage(pActuel, pStart, pTime);
    if (PourCent != *pPourCent) {
        ChangementProgress = true;
        *pPourCent = PourCent;
        if (pEcran >= 0) {
            OledProgress(PourCent, pEcran);
        }
    }
    return ChangementProgress;
}

//***********************************************************************
// CalculPourcentage calcule le poucentage du temps ecoule
//***********************************************************************
int CalculPourcentage(int pActuel, int pStart, int pTime) {
    int Resultat;

    if (((pActuel - pStart) >= (pTime * 1000)) || (pTime == 0)) {
        return (100);
    } else {
        return (round(float(pActuel - pStart) / float(pTime * 1000) * 100));
        //Resultat = (round(float(pActuel - pStart) / float(pTime * 1000) * 100));
        // Resultat = round(Resultat / 10) * 10;
        //return (Resultat);
    }
}

//***********************************************************************
// OledProgress   Barre de progression
//***********************************************************************
void OledProgress(int pPourCent, int pNumEcran) {
    byte VoieSelect;

    TcaSelect(pNumEcran);
    // test controle voie TCA9548 selectionnée
    //VoieSelect = TcaVoieActive();
    //Trace(20, TAG_INIT, "oled progress ecran : " + String(pNumEcran) + " voie: " + String(VoieSelect, HEX) + " valeur : " + String(pPourCent));


    if (pNumEcran == ECRAN_ORDRE) {
        display1.drawRect(10, 0, 100, 15, SSD1306_WHITE);
        display1.fillRect(10, 1, pPourCent, 13, SSD1306_WHITE);
        display1.display();
    }
    if (pNumEcran == ECRAN_POSITION) {
        display2.drawRect(10, 0, 100, 15, SSD1306_WHITE);
        display2.fillRect(10, 1, pPourCent, 13, SSD1306_WHITE);
        display2.display();
    }
    if (pNumEcran == ECRAN_ROTATIF) {
        display3.drawRect(10, 0, 100, 15, SSD1306_WHITE);
        display3.fillRect(10, 1, pPourCent, 13, SSD1306_WHITE);
        display3.display();
    }
    if (pNumEcran == ECRAN_SLIDER) {
        display4.drawRect(10, 0, 100, 15, SSD1306_WHITE);
        display4.fillRect(10, 1, pPourCent, 13, SSD1306_WHITE);
        display4.display();
    }

    TcaOff();
}
/*
  //_______________________________________________________________________
  //    Oled gestion                 ::::::::::::::::::::::::::::::::::::::
  //_______________________________________________________________________
  ClearOled(int pCanal, int pReset)
  OledTexte(int X, int Y, int Size, String pTexte, int pEcran, boolean pNew)
*/

//***********************************************************************
// ClearOled Clear afficheur
//***********************************************************************
void ClearOled(int pCanal, int pReset) {
    byte VoieSelect;

    TcaSelect(pCanal);
    // test vérification voie selectionnée
    //VoieSelect = TcaVoieActive();
    //Trace(20, TAG_INIT, "oled clear ecran : " + String(pCanal) + " voie: " + String(VoieSelect, HEX));
    if (pCanal == ECRAN_ORDRE) {
        display1.clearDisplay();
        display1.display();
        /*if (pReset == true) {
          Adafruit_SSD1306 display1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
          }*/
    }

    if (pCanal == ECRAN_POSITION) {
        display2.clearDisplay();
        display2.display();
        /*if (pReset == true) {
          Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
          }*/
    }
    if (pCanal == ECRAN_ROTATIF) {
        display3.clearDisplay();
        display3.display();
        /*if (pReset == true) {
          Adafruit_SSD1306 display3(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
          }*/
    }
    if (pCanal == ECRAN_SLIDER) {
        display4.clearDisplay();
        display4.display();
        /*if (pReset == true) {
          Adafruit_SSD1306 display4(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
          }*/
    }
    TcaOff();
}

//***********************************************************************
// OledTexte ecriture texte sur oled
//***********************************************************************
void OledTexte(int X, int Y, int Size, String pTexte, int pEcran, boolean pNew, boolean pDisplay) {
    byte VoieSelect;

    //String ChaineEspace = "            ";
    // pas de selection si NO_ECRAN
    if (pEcran >= 0) {
        TcaSelect(pEcran);
    }

    //VoieSelect = TcaVoieActive();
    //Trace(20, TAG_INIT, "oled texte : " + pTexte + "  ecran: " + String(pEcran) + " voie: " + String(VoieSelect, HEX));

    if (pEcran == ECRAN_ORDRE) {
        if (pNew == true) {
            display1.clearDisplay();
        }
        display1.setTextSize(Size);      // Normal 1:1 pixel scale
        display1.setTextColor(SSD1306_WHITE); // Draw white text
        display1.setCursor(X, Y);     // Start at top-left corner
        display1.cp437(true);         // Use full 256 char 'Code Page 437' font

        // Not all the characters will fit on the display. This is normal.
        // Library will draw what it can and the rest will be clipped.
        for (int i = 0; i < pTexte.length(); i++) {
            //display1.write(char(ChaineEspace[1]));
            display1.write(char(pTexte[i]));
        }
        //display1.print("    ");
        //display1.setCursor(X, Y);     // Start at top-left corner
        //display1.print(pTexte);
        if (pDisplay == true) {
            display1.display();
        }
    }

    if (pEcran == ECRAN_POSITION) {
        if (pNew == true) {
            display2.clearDisplay();
        }
        display2.setTextSize(Size);      // Normal 1:1 pixel scale
        display2.setTextColor(SSD1306_WHITE); // Draw white text
        display2.setCursor(X, Y);     // Start at top-left corner
        display2.cp437(true);         // Use full 256 char 'Code Page 437' font

        // Not all the characters will fit on the display. This is normal.
        // Library will draw what it can and the rest will be clipped.
        for (int i = 0; i < pTexte.length(); i++) {
            display2.write(char(pTexte[i]));
        }
        if (pDisplay == true) {
            display2.display();
        }
    }

    if (pEcran == ECRAN_ROTATIF) {
        if (pNew == true) {
            display3.clearDisplay();
        }
        display3.setTextSize(Size);      // Normal 1:1 pixel scale
        display3.setTextColor(SSD1306_WHITE); // Draw white text
        display3.setCursor(X, Y);     // Start at top-left corner
        display3.cp437(true);         // Use full 256 char 'Code Page 437' font

        // Not all the characters will fit on the display. This is normal.
        // Library will draw what it can and the rest will be clipped.
        for (int i = 0; i < pTexte.length(); i++) {
            display3.write(char(pTexte[i]));
        }
        if (pDisplay == true) {
            display3.display();
        }
    }

    if (pEcran == ECRAN_SLIDER) {
        if (pNew == true) {
            display4.clearDisplay();
        }
        display4.setTextSize(Size);      // Normal 1:1 pixel scale
        display4.setTextColor(SSD1306_WHITE); // Draw white text
        display4.setCursor(X, Y);     // Start at top-left corner
        display4.cp437(true);         // Use full 256 char 'Code Page 437' font

        // Not all the characters will fit on the display. This is normal.
        // Library will draw what it can and the rest will be clipped.
        for (int i = 0; i < pTexte.length(); i++) {
            display4.write(char(pTexte[i]));
        }
        if (pDisplay == true) {
            display4.display();
        }
    }


    TcaOff();
}

/*
  //_______________________________________________________________________
  //    Oled animation               ::::::::::::::::::::::::::::::::::::::
  //_______________________________________________________________________
  RazAnimation
  ArretAnimation(int pEcran)
  ProgAnimation(int pEcran, int pDuree, int pInterval, String pTexte, int pSens, boolean pJeuxOk)
  ControleAnimation
  TraitementAnimation(int* pDuree, int* pStart, int* pStartInterval, int* pInterval, int* pNiv, int* pScreen,  String* pTexte, int* pSens, boolean* pStop)
*/

//*********************************************************************
// RazAnimation
//***********************************************************************
void RazAnimation() {
    StopAnim1 = true;
    StopAnim2 = true;
    StopAnim3 = true;
    StopAnim4 = true;
    StopAnim5 = true;
}

//*********************************************************************
// ArretAnimation
//***********************************************************************
void ArretAnimation(int pEcran) {
    if (pEcran == ECRAN_ORDRE) {
        StopAnim1 = true;
    }
    if (pEcran == ECRAN_POSITION) {
        StopAnim2 = true;
    }
    if (pEcran == ECRAN_ROTATIF) {
        StopAnim3 = true;
    }
    if (pEcran == ECRAN_SLIDER) {
        StopAnim4 = true;
    }
    if (pEcran == NO_ECRAN) {
        StopAnim5 = true;
    }
}

//*********************************************************************
// ProgAnimation
//***********************************************************************
void ProgAnimation(int pEcran, int pDuree, int pInterval, String pTexte, int pSens, boolean pJeuxOk) {
    int NumAnim = 0;
    CurrentTime = millis();

    if (pEcran == ECRAN_ORDRE) {
        NumAnim = 1;
    }
    if (pEcran == ECRAN_POSITION) {
        NumAnim = 2;
    }
    if (pEcran == ECRAN_ROTATIF) {
        NumAnim = 3;
    }
    if (pEcran == ECRAN_SLIDER) {
        NumAnim = 4;
    }
    if (pEcran == NO_ECRAN) {
        NumAnim = 5;
    }
    //Trace(20, TAG_INIT, "num anim : " + String(NumAnim) + " ecran: " + String(pEcran));
    /*
      animation1 = new Animation();
      animation2 = new Animation();
      animation3 = new Animation();
      animations = [animation1, animation2, animation3];
      animations[NumAnim].DureeAnim = pDuree;
      animations[NumAnim].StartAnim = CurrentTime;
      animations[NumAnim].StartIntervalAnim = CurrentTime;
      animations[NumAnim].IntervalAnim = pInterval;
      animations[NumAnim].NivAnim = 0;
      animations[NumAnim].ScreenAnim = pEcran;
      animations[NumAnim].TexteAnim = pTexte;
      animations[NumAnim].SensAnim = pSens;
      animations[NumAnim].StopAnim = false;
    */

    switch (NumAnim) {
        case 1 :
            DureeAnim1 = pDuree;
            //Trace(20, TAG_INIT, "prog animation 1");
            StartAnim1 = CurrentTime;
            StartIntervalAnim1 = CurrentTime;
            IntervalAnim1 = pInterval;
            NivAnim1 = 0;
            ScreenAnim1 = pEcran;
            TexteAnim1 = pTexte;
            SensAnim1 = pSens;
            JeuxOkAnim1 = pJeuxOk;
            StopAnim1 = false;
            break;

        case 2 :
            DureeAnim2 = pDuree;
            StartAnim2 = CurrentTime;
            StartIntervalAnim2 = CurrentTime;
            IntervalAnim2 = pInterval;
            NivAnim2 = 0;
            ScreenAnim2 = pEcran;
            TexteAnim2 = pTexte;
            SensAnim2 = pSens;
            JeuxOkAnim2 = pJeuxOk;
            StopAnim2 = false;
            break;

        case 3 :
            DureeAnim3 = pDuree;
            StartAnim3 = CurrentTime;
            StartIntervalAnim3 = CurrentTime;
            IntervalAnim3 = pInterval;
            NivAnim3 = 0;
            ScreenAnim3 = pEcran;
            TexteAnim3 = pTexte;
            SensAnim3 = pSens;
            JeuxOkAnim3 = pJeuxOk;
            StopAnim3 = false;
            break;

        case 4 :
            DureeAnim4 = pDuree;
            StartAnim4 = CurrentTime;
            StartIntervalAnim4 = CurrentTime;
            IntervalAnim4 = pInterval;
            NivAnim4 = 0;
            ScreenAnim4 = pEcran;
            TexteAnim4 = pTexte;
            SensAnim4 = pSens;
            JeuxOkAnim4 = pJeuxOk;
            StopAnim4 = false;
            break;

        case 5 :
            DureeAnim5 = pDuree;
            StartAnim5 = CurrentTime;
            StartIntervalAnim5 = CurrentTime;
            IntervalAnim5 = pInterval;
            NivAnim5 = 0;
            ScreenAnim5 = pEcran;
            TexteAnim5 = pTexte;
            SensAnim5 = pSens;
            JeuxOkAnim5 = pJeuxOk;
            StopAnim5 = false;
            break;
    }
}

//*********************************************************************
// ControleAnimation
//***********************************************************************
void ControleAnimation() {
    if (StopAnim1 == false) {
        TraitementAnimation(&DureeAnim1, &StartAnim1, &StartIntervalAnim1, &IntervalAnim1, &NivAnim1, &ScreenAnim1,
                            &TexteAnim1, &SensAnim1, &CoulLedAnim1, &JeuxOkAnim1, &StopAnim1);
    }

    if (StopAnim2 == false) {
        TraitementAnimation(&DureeAnim2, &StartAnim2, &StartIntervalAnim2, &IntervalAnim2, &NivAnim2, &ScreenAnim2,
                            &TexteAnim2, &SensAnim2, &CoulLedAnim2, &JeuxOkAnim2, &StopAnim2);
    }

    if (StopAnim3 == false) {
        TraitementAnimation(&DureeAnim3, &StartAnim3, &StartIntervalAnim3, &IntervalAnim3, &NivAnim3, &ScreenAnim3,
                            &TexteAnim3, &SensAnim3, &CoulLedAnim3, &JeuxOkAnim3, &StopAnim3);
    }

    if (StopAnim4 == false) {
        TraitementAnimation(&DureeAnim4, &StartAnim4, &StartIntervalAnim4, &IntervalAnim4, &NivAnim4, &ScreenAnim4,
                            &TexteAnim4, &SensAnim4, &CoulLedAnim4, &JeuxOkAnim4, &StopAnim4);
    }

    if (StopAnim5 == false) {
        TraitementAnimation(&DureeAnim5, &StartAnim5, &StartIntervalAnim5, &IntervalAnim5, &NivAnim5, &ScreenAnim5,
                            &TexteAnim5, &SensAnim5, &CoulLedAnim5, &JeuxOkAnim5, &StopAnim5);
    }
}

//*********************************************************************
// TraitementAnimation
//***********************************************************************
boolean TraitementAnimation(int *pDuree, int *pStart, int *pStartInterval, int *pInterval, int *pNiv, int *pScreen,
                            String *pTexte, int *pSens, int *pCoulLed, boolean *pJeuxOk, boolean *pStop) {

    int PourCent;
    int CoulProgress;
    int Duree;

    CurrentTime = millis();

    // test fin animation
    if ((CurrentTime > (*pStart + *pDuree)) && (*pStop == false)) {
        //if ( CurrentTime > (StartAnim1 + DureeAnim1)) {
        *pStop = true;
        ClearOled(*pScreen, false);

        // extinction des leds de l'écran associe
        if (*pScreen = ECRAN_ROTATIF) {
            CouleurLedMatrice(NB_LED_ROTATIF, &MatrLedRotatif[0], clRien);
        }
        if (*pScreen = ECRAN_ORDRE) {
            CouleurLedMatrice(NB_LED_ORDRE, &MatrLedOrdre[0], clRien);
        }
        if (*pScreen = ECRAN_POSITION) {
            CouleurLedMatrice(NB_LED_POSITION, &MatrLedPosition[0], clRien);
        }
        if (*pScreen = ECRAN_SLIDER) {
            CouleurLedMatrice(NB_LED_SLIDER, &MatrLedSlider[0], clRien);
        }
        if (*pScreen = NO_ECRAN) {
            CouleurLedMatrice(NB_LED_PUSH_BUTTON, &MatrLedPushButton[0], clRien);
        }

    }

    // gestion animation
    if (*pStop == false) {
        if (CurrentTime > (*pStartInterval + *pInterval)) {
            *pStartInterval = CurrentTime;
            ClearOled(*pScreen, false);
            if (*pNiv > 0) {
                OledTexte(55 - (*pNiv * 6), 32 - (*pNiv * 3), *pNiv, *pTexte, *pScreen, false, true);
            }

            //Incrémentation niveau
            if (*pSens >= 0) {
                *pNiv = *pNiv + 1; // pas de *pNiv++ sur pointeur
                if (*pNiv > 4) {
                    *pNiv = 0;
                }
            }

            // Décrémentation niveau
            if (*pSens < 0) {
                *pNiv = *pNiv - 1; // pas de *pNiv-- sur pointeur
                if (*pNiv < 0) {
                    *pNiv = 4;
                }
            }

            // animation led
            //Trace(20, TAG_INIT, "progress : " + String(PourCent) + " couleur: " + String(CoulProgress) + " Ecran: " + String(*pScreen) + " mem :" + String(ScreenAnim3) + "Txt : " + *pTexte );
            Duree = round(float(*pDuree / 1000));
            PourCent = CalculPourcentage(CurrentTime, *pStart, Duree);
            CoulProgress = clRien;
            if (((PourCent > 20) && (PourCent < 55)) || (PourCent > 70)) {
                CoulProgress = clGreen;
                if (*pJeuxOk == false) {
                    CoulProgress = clRed;
                }
            }
            //Trace(20, TAG_INIT, "progress : " + String(pCanal) + " voie: " + String(VoieSelect, HEX));
            //Trace(20, TAG_INIT, "progress : " + String(PourCent) + " couleur: " + String(CoulProgress) + " Ecran: " + String(*pScreen) + " mem :" + String(ScreenAnim3) + "Txt : " + *pTexte );

            //affichage led associée à l'écran
            if (*pScreen == ECRAN_ROTATIF) {
                //Trace(20, TAG_INIT, "nb led : " + String(NB_LED_ROTATIF));
                CouleurLedMatrice(NB_LED_ROTATIF, &MatrLedRotatif[0], CoulProgress);
            }
            if (*pScreen == ECRAN_ORDRE) {
                CouleurLedMatrice(NB_LED_ORDRE, &MatrLedOrdre[0], CoulProgress);
            }
            if (*pScreen == ECRAN_POSITION) {
                CouleurLedMatrice(NB_LED_POSITION, &MatrLedPosition[0], CoulProgress);
            }
            if (*pScreen == ECRAN_SLIDER) {
                CouleurLedMatrice(NB_LED_SLIDER, &MatrLedSlider[0], CoulProgress);
            }
            if (*pScreen == NO_ECRAN) {
                CouleurLedMatrice(NB_LED_PUSH_BUTTON, &MatrLedPushButton[0], CoulProgress);
            }
        }
    }
}

/*
  //_______________________________________________________________________
  //  LCD gestion                    ::::::::::::::::::::::::::::::::::::::
  //_______________________________________________________________________
  InitLcd
  ClearLcd
  EcritureLcd
  AffichageLcd
  LcdToMqtt
  AjoutLcdSpecial(String pChaine, int pNum)
  AddLcd(String pChaine)
  SupLcdTxt(String pChaine)
  SupLcdLine(int pNum)
  CompleteChaine(String pChaine, int pNombre)
  ReclasseAfficheur
*/

//***********************************************************************
// InitLcd Initialisation LCD
//***********************************************************************
void InitLcd() {
    lcd.init(); // initialize the lcd
    lcd.backlight();
    lcd.createChar(0, bell);
    lcd.createChar(1, duck);
    lcd.createChar(2, check);
    lcd.createChar(3, bell);
    lcd.createChar(4, heart);
    lcd.createChar(5, clock1);
    lcd.createChar(6, note);
    lcd.createChar(7, retarrow);
    lcd.createChar(8, omega);
    lcd.home();
    lcd.clear();
}

//***********************************************************************
// ClearLcd raz afficheur
//***********************************************************************
void ClearLcd() {
    String ChaineTrav = "";

    for (int i = 0; i < LCD_NB_LIGNE; i++) {
        ChaineTrav = "";
        ChaineTrav = CompleteChaine(ChaineTrav, LCD_NB_COLONNE);
        Lcd[i] = ChaineTrav;
    }
    LcdChange = true;
}

//***********************************************************************
// EcritureLcd
//***********************************************************************
void EcritureLcd(String pTopic, String pMes) {
    String CodeErr;

    // Deserialize the JSON document
    doc.clear();
    DeserializationError error = deserializeJson(doc, pMes);
    // Test if parsing succeeds.
    if (error) {
        CodeErr = error.c_str();
        Trace(20, TAG_JSON, "deserializeJson() failed: " + CodeErr);
    }

    Trace(20, TAG_TEST, "Prog receive : " + String(pMes));

    const char *TexteLcd1 = doc[JSON_TXT_1];
    const char *TexteLcd2 = doc[JSON_TXT_2];
    const char *TexteLcd3 = doc[JSON_TXT_3];
    const char *TexteLcd4 = doc[JSON_TXT_4];

    Lcd[0] = CompleteChaine(String(TexteLcd1), LCD_NB_COLONNE);
    Lcd[1] = CompleteChaine(String(TexteLcd2), LCD_NB_COLONNE);
    Lcd[2] = CompleteChaine(String(TexteLcd3), LCD_NB_COLONNE);
    Lcd[3] = CompleteChaine(String(TexteLcd4), LCD_NB_COLONNE);

    LcdChange = true;
}

//***********************************************************************
// AffichageLcd envoi texte memoire sur Lcd
//***********************************************************************
void AffichageLcd() {
    // aiguillage TCA9548
    //SelectI2c(6);
    if (EcranLcd == 1) {
        LcdToMqtt();
    } else {
        for (int i = 0; i < LCD_NB_LIGNE; i++) {
            lcd.setCursor(0, i);
            lcd.print(Lcd[i]);
        }
        //LcdChange = false;
    }

    LcdChange = false;
}

//***********************************************************************
// LcdToMqtt envoi texte memoire sur mqtt
//***********************************************************************
void LcdToMqtt() {
    String MesTopic = "";
    String MesData = "";

    //MesTopic = MQTT_ECRAN;
    MesTopic = "diffuse/" + String(Player) + "/" + MQTT_ECRAN;

    doc.clear();
    doc[JSON_TXT_1] = Lcd[0];
    doc[JSON_TXT_2] = Lcd[1];
    doc[JSON_TXT_3] = Lcd[2];
    doc[JSON_TXT_4] = Lcd[3];

    serializeJson(doc, MesData);
    //SendMqtt(MQTT_ECRAN, MesData, false);
    SendMqtt(MesTopic, MesData, false);

}

//***********************************************************************
// AjoutLcdSpecial ajout d'un caractere special sur la chaine
//***********************************************************************
String AjoutLcdSpecial(String pChaine, int pNum) {
    if ((pNum >= 0) && (pNum < 16)) {
        pChaine = pChaine + char(pNum);
    } else {
        pChaine = pChaine;
    }

    return (pChaine);
}

//***********************************************************************
// AddLcd ajout d'un chaine sur l'afficheur
//***********************************************************************
void AddLcd(String pChaine) {
    int NumLibre = -1;
    String ChaineTrav = "";

    // recherche ligne libre
    for (int i = 0; i < LCD_NB_LIGNE; i++) {
        ChaineTrav = Lcd[i];
        ChaineTrav.trim();
        if (ChaineTrav == "") {
            NumLibre = i;
            break;
        }
    }

    // afficheur plein écriture derniére ligne
    if (NumLibre < 0) {
        ChaineTrav = "Ajout ligne impossible, afficheur plein";
        SendMqtt(MQTT_ERREUR, ChaineTrav, false);
        //NumLibre = LCD_NB_LIGNE - 1;
        //envoyer un message erreur MQTT "Ajout ligne impossible, lignes pleines"
    } else {
        Lcd[NumLibre] = CompleteChaine(pChaine, LCD_NB_COLONNE);
    }
}

//***********************************************************************
// SupLcdTxt suppression d'un chaine sur l'afficheur
//***********************************************************************
void SupLcdTxt(String pChaine) {

    String ChaineTrav = "";

    for (int i = 0; i < LCD_NB_LIGNE; i++) {
        ChaineTrav = Lcd[i];
        //if (Lcd[i].indexOf(pChaine) >= 0) {
        if (ChaineTrav.indexOf(pChaine) >= 0) {
            Lcd[i] = "";
        }
    }
    ReclasseAfficheur();
}

//***********************************************************************
// SupLcdLine suppression d'une ligne sur l'afficheur
//***********************************************************************
void SupLcdLine(int pNum) {
    if (pNum < LCD_NB_LIGNE) {
        Lcd[pNum] = "";
    }
    ReclasseAfficheur();
}

//***********************************************************************
// CompleteChaine avec espaces
//***********************************************************************
String CompleteChaine(String pChaine, int pNombre) {
    while (pChaine.length() < pNombre) {
        pChaine = pChaine + " ";
    }
    return (pChaine);
}

//***********************************************************************
// ReclasseAfficheur suppression des lignes vides sur afficheur
//***********************************************************************
void ReclasseAfficheur() {
    String LcdTrav[] = {"", "", "", ""};
    String ChaineTrav = "";
    int j = 0;

    // init LcdTrav
    for (int i = 0; i < LCD_NB_LIGNE; i++) {
        LcdTrav[i] = CompleteChaine(LcdTrav[i], LCD_NB_COLONNE);
    }

    // copie des chaines non vides dans LcdTrav
    for (int i = 0; i < LCD_NB_LIGNE; i++) {
        ChaineTrav = Lcd[i];
        ChaineTrav.trim();
        if (ChaineTrav != "") {
            LcdTrav[j] = CompleteChaine(Lcd[i], LCD_NB_COLONNE);
            j++;
        }
    }

    //LcdTrav vers Lcd
    for (int i = 0; i < LCD_NB_LIGNE; i++) {
        Lcd[i] = LcdTrav[i];
    }
}

/*
  //_______________________________________________________________________
  //  Configuration & EEPROM         ::::::::::::::::::::::::::::::::::::::
  //_______________________________________________________________________
  ProgConfig(String pTopic, String pMes)
  AffichageStatus(String pTopic, String pMes)
  EcritureEeprom (int pAdr, int pVal)
  LectureEeprom (int pAdr)
  EcritureEepromDouble (int pAdr, int pVal)
  LectureEepromDouble (int pAdr)
*/

//********************************************************************
// Programmation configuration
//********************************************************************
void ProgConfig(String pTopic, String pMes) {
    String CodeErr;

    CurrentTime = millis();
    // Deserialize the JSON document
    doc.clear();
    DeserializationError error = deserializeJson(doc, pMes);
    // Test if parsing succeeds.
    if (error) {
        CodeErr = error.c_str();
        Trace(20, TAG_JSON, "deserializeJson() failed: " + CodeErr);
    }

    Trace(20, TAG_INIT, "Prog config : " + String(pMes));

    IdBoite = doc["idBoite"];
    Player = doc["player"];
    Brightness = doc["brightness"];
    EcranLcd = doc["ecranLcd"];

    EcritureEeprom(EEP_ID_BOITE, IdBoite);
    EcritureEeprom(EEP_PLAYER, Player);
    EcritureEeprom(EEP_BRIGHTNESS, Brightness);
    EcritureEeprom(EEP_ECRAN_LCD, EcranLcd);

    // actualisation (changement ID)
    reconnect();
}

//********************************************************************
// Renvoie le status EEPROM
//********************************************************************
void AffichageStatus(String pTopic, String pMes) {
    String MesSend = "";

    doc.clear();
    doc["idBoite"] = String(LectureEeprom(EEP_ID_BOITE));
    doc["player"] = String(LectureEeprom(EEP_PLAYER));
    doc["brightness"] = String(LectureEeprom(EEP_BRIGHTNESS));
    doc["ecranLcd"] = String(LectureEeprom(EEP_ECRAN_LCD));

    // information version revision
    doc["version"] = String(VERSION);
    doc["revision"] = String(REVISION);

    serializeJson(doc, MesSend);
    SendMqtt(MQTT_STATUS, MesSend, true);
}

//*****************************************************************
// EcritureEeprom écriture d'une valeur EEPROM
//*****************************************************************
void EcritureEeprom(int pAdr, int pVal) {

    int LastVal;

    // lecture EEPROM avant ecriture pour prolonger la durée de vie
    LastVal = LectureEeprom(pAdr);
    if (LastVal != pVal) {
        EEPROM.write(pAdr, pVal);
        EEPROM.commit();
    }
}

//*****************************************************************
//  LectureEeprom  lecture d'une valeur EEPROM
//*****************************************************************
int LectureEeprom(int pAdr) {
    int ValEeprom;

    ValEeprom = EEPROM.read(pAdr);
    return (ValEeprom);
}

//*****************************************************************
// EcritureEepromDouble écriture d'une valeur EEPROM
//*****************************************************************
void EcritureEepromDouble(int pAdr, int pVal) {

    int LastVal;
    byte PoidsFort;
    byte PoidsFaible;
    int AdrEeprom;

    // lecture EEPROM avant ecriture pour prolonger la durée de vie
    AdrEeprom = pAdr;
    PoidsFort = highByte(pVal);
    PoidsFaible = lowByte(pVal);

    LastVal = LectureEeprom(AdrEeprom);
    if (LastVal != PoidsFort) {
        EEPROM.write(AdrEeprom, PoidsFort);
        EEPROM.commit();
    }

    AdrEeprom++;
    LastVal = LectureEeprom(AdrEeprom);
    if (LastVal != PoidsFaible) {
        EEPROM.write(AdrEeprom, PoidsFaible);
        EEPROM.commit();
    }
}

//*****************************************************************
//  LectureEepromDouble  lecture d'une valeur EEPROM
//*****************************************************************
int LectureEepromDouble(int pAdr) {
    int ValEeprom;
    byte PoidsFort;
    byte PoidsFaible;
    int AdrEeprom;

    AdrEeprom = pAdr;
    PoidsFort = EEPROM.read(AdrEeprom);
    AdrEeprom++;
    PoidsFaible = EEPROM.read(AdrEeprom);

    ValEeprom = word(PoidsFort, PoidsFaible);
    return (ValEeprom);
}

/*
  //_______________________________________________________________________
  //  Gestion Trace                  ::::::::::::::::::::::::::::::::::::::
  //_______________________________________________________________________
  ProgTrace(String pTopic, String pMes)
  TraceSelect (int pLevel)
  Trace (int pNiv, String pTag, String pInfo)
*/

//********************************************************************
// Programmation trace
//********************************************************************
void ProgTrace(String pTopic, String pMes) {
    String CodeErr;
    String ChaineTrav;

    CurrentTime = millis();
    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, pMes);
    // Test if parsing succeeds.
    if (error) {
        CodeErr = error.c_str();
        Trace(20, TAG_JSON, "deserializeJson() failed: " + CodeErr);
    }

    Trace(1, TAG_INIT, "Prog trace : " + String(pMes));

    const char *nomTag = doc["nomTag"];
    int nvMin = doc["nvMin"];
    int nvMax = doc["nvMax"];
    int afTime = doc["afTime"];
    int afTag = doc["afTag"];
    int afLev = doc["afLev"];
    int afSer = doc["afSer"];
    int afMq = doc["afMq"];

    TraceNiveauMini = nvMin;
    TraceNiveauMaxi = nvMax;
    TraceAffichageTemps = (afTime == 1);
    TraceAffichageTag = (afTag == 1);
    TraceAffichageLevel = (afLev == 1);
    TraceActiveSerie = (afSer == 1);
    TraceActiveMqtt = (afMq == 1);
    NomTagFiltre = String(nomTag);

}

//*****************************************************************
//  TraceSelect  la trace est active si le niveau est supérieur au mini inférieur au maxi et la trace autorisée
//*****************************************************************
boolean TraceSelect(int pLevel) {
    if ((pLevel > TraceNiveauMini) && (pLevel < TraceNiveauMaxi) &&
        ((TraceActiveSerie == true) || (TraceActiveMqtt == true)) && (EcranLcd < 2)) {
        return (true);
    } else {
        return (false);
    }
}

//*****************************************************************
// Trace envoi info sur port série / Mqtt
//*****************************************************************
void Trace(int pNiv, String pTag, String pInfo) {
    int TempsActuel;
    String InfoTrace;

    if (TraceSelect(pNiv) == true) {
        InfoTrace = "";
        if (TraceAffichageTag == true) {
            InfoTrace = pTag + " ";
            if (TraceAffichageLevel == true) {
                InfoTrace = InfoTrace + "(" + String(pNiv) + ") ";
            }
        }
        if (TraceAffichageTemps == true) {
            TempsActuel = millis();
            InfoTrace = InfoTrace + String(TempsActuel) + " ";
        }
        InfoTrace = InfoTrace + pInfo;

        if ((TraceActiveSerie == true) && ((NomTagFiltre == "") || (NomTagFiltre == pTag))) {
            Serial.println(InfoTrace);
            //Serial.println("Lng info trace : " + String(InfoTrace.length()));
        }

        if ((TraceActiveMqtt == true) && ((NomTagFiltre == "") || (NomTagFiltre == pTag))) {
            SendMqtt(MQTT_DEBUG, InfoTrace, false);
            //InfoTrace.toCharArray(bufMqtt, InfoTrace.length() + 1);
            //client.publish(MqttTraceDebug, bufMqtt);
            //a tester
            //client.publish_P(MqttTraceDebug, bufMqtt);
        }
    }
}

/*
  //_______________________________________________________________________
  //    Outil Test                   ::::::::::::::::::::::::::::::::::::::
  //_______________________________________________________________________
  ProgTest(String pTopic, String pMes)
  loopTest
  TestBoutonRotatif 1
  TestDac 2
  ScanI2cTotal 3
  VitesseCommutationPort 4
  ChangementEtatBouton 5
  TestLedBrigthness 6
  EtalonnageSlider 7
  ResetOled 8
  LectureEepromSlider 9
  LectureConversionSlider 10
*/

//********************************************************************
// Programmation test
//********************************************************************
void ProgTest(String pTopic, String pMes) {
    String CodeErr;

    if (EcranLcd < 2) {
        CurrentTime = millis();
        // Deserialize the JSON document
        doc.clear();
        DeserializationError error = deserializeJson(doc, pMes);
        // Test if parsing succeeds.
        if (error) {
            CodeErr = error.c_str();
            Trace(20, TAG_JSON, "deserializeJson() failed: " + CodeErr);
        }

        //Trace(20, TAG_TEST, "Prog test : " + String(pMes));

        TestActif = doc["actif"];
        NumTest = doc["num"];
        Aux1 = doc["aux1"];
        Aux2 = doc["aux2"];

        // Init début test
        PositionSlider = 0;
        DernierEtalonnageSlider = CurrentTime;

        DerniereLectureTest = CurrentTime;

        if (TestActif == true) {
            Trace(20, TAG_TEST, "Test actif : " + String(NumTest));
        }
        if (TestActif == false) {
            Brightness = LectureEeprom(EEP_BRIGHTNESS); // restitution luminosité programmée
            Trace(20, TAG_TEST, "Stop Test : " + String(NumTest));
        }
    }
}

//*********************************************************************
// loopTest
//***********************************************************************
void loopTest() {
    switch (NumTest) {
        // Test bouton rotatif
        case 1 :
            TestBoutonRotatif();
            break;

        case 2 :
            // test convertisseur analogique numérique
            TestDac();
            break;

        case 3 :
            // test I2C
            TestActif = false; // test unique
            ScanI2cTotal();
            break;

        case 4 :
            // mesure vitesse commutation port
            TestActif = false;
            VitesseCommutationPort();
            break;

        case 5 :
            // affichage changement état bouton
            ChangementEtatBouton();
            break;

        case 6 :
            // affichage led brigthness
            TestLedBrigthness();
            break;

        case 7 :
            // étalonnage des sliders
            EtalonnageSlider();
            break;

        case 8 :
            // reset des oled
            TestActif = false;
            ResetOled();
            break;

        case 9 :
            // Lecture eeprom slider
            TestActif = false;
            LectureEepromSlider();
            break;

        case 10 :
            // Lecture conversion slider
            TestActif = false;
            LectureConversionSlider();
            break;

    }

}

//*********************************************************************
// TestBoutonRotatif
//***********************************************************************
void TestBoutonRotatif() {

    if (MatrBoutonTraitement[BtnSw] == 1) {
        MatrBoutonTraitement[BtnSw] = 0;
        CompteurRotation = 0;
        OledTexte(20, 20, 3, String(CompteurRotation), ECRAN_ROTATIF, true, true);
        Trace(20, TAG_ROTATIF, "Raz compteur rotatif");
    }


#ifdef KY_040
    // changement Clk
    if (LastBtnClk != MatrBoutonEtat[BtnClk]) {
        LastBtnClk = MatrBoutonEtat[BtnClk];
        if (ChangeDt == true) {
            CompteurRotation--;
            Trace(20, TAG_ROTATIF, "Compteur rotatif decrement : " + String(CompteurRotation));
            OledTexte(20, 20, 3, String(CompteurRotation), ECRAN_ROTATIF, true, true);
        } else {
            ChangeClk = true;
        }
    }

    // changement dt
    if (LastBtnDt != MatrBoutonEtat[BtnDt]) {
        LastBtnDt = MatrBoutonEtat[BtnDt];
        if (ChangeClk == true) {
            CompteurRotation++;
            Trace(20, TAG_ROTATIF, "Compteur rotatif increment : " + String(CompteurRotation));
            OledTexte(20, 20, 3, String(CompteurRotation), ECRAN_ROTATIF, true, true);
        } else {
            ChangeDt = true;
        }
    }

    // Etat repos
    if (MatrBoutonEtat[BtnDt] == MatrBoutonEtat[BtnClk]) {
        ChangeClk = false;
        ChangeDt = false;
    }

#else
    if (MatrBoutonTraitement[BtnClk] == 1) {
        MatrBoutonTraitement[BtnClk] = 0;
        if (MatrBoutonEtat[BtnDt] == 0) {
            CompteurRotation--;
            Trace(20, TAG_TEST, "Dec rotatif : " + String(CompteurRotation));
        } else {
            CompteurRotation++;
            Trace(20, TAG_TEST, "Inc rotatif : " + String(CompteurRotation));
        }
        OledTexte(20, 20, 3, String(CompteurRotation), ECRAN_ROTATIF, true, true);
    }

    if (MatrBoutonTraitement[BtnDt] == 1) {
        MatrBoutonTraitement[BtnDt] = 0;
    }

#endif
}


//*********************************************************************
// Test DAC
//***********************************************************************
void TestDac() {

    int ValNumerique;

    if (CurrentTime > (DerniereLectureSlider + INTERVAL_LECTURE_SLIDER)) {
        DerniereLectureSlider = CurrentTime;

        switch (Aux1) {
            case 1 :
                ValNumerique = LectureSlider(Slider1, 1, 0);
                //ValAnalogique = analogRead(Slider1);
                break;

            case 2 :
                ValNumerique = LectureSlider(Slider2, 2, 0);
                //ValAnalogique = analogRead(Slider2);
                break;

            case 3 :
                ValNumerique = LectureSlider(Slider3, 3, 0);
                //ValAnalogique = analogRead(Slider3);
                break;

        }
        OledTexte(40, 2, 2, String(ValAnalogique), ECRAN_SLIDER, true, false);
        OledTexte(55, 30, 3, String(ValNumerique), ECRAN_SLIDER, false, true);
    }
}

//***********************************************************************
//    Vitesse commutation port
//***********************************************************************
void VitesseCommutationPort() {
    int Start;
    int Stop;
    int Duree;

    Start = millis();
    for (int i = 0; i < 1000000; i++) {
        digitalWrite(CLK_HC165, HIGH);
        digitalWrite(CLK_HC165, LOW);
    }
    Stop = millis();
    Duree = Stop - Start;

    Trace(20, TAG_TEST, "Duree : " + String(Duree) + " ms");
}

//*********************************************************************
// Test changement etat bouton
//***********************************************************************
void ChangementEtatBouton() {
    String ChaineTrav;

    for (int i = 0; i < TAILLE_BOUTON; i++) {

        // controle changement
        if (MatrBoutonEtat[i] != MatrBoutonChange[i]) {
            MatrBoutonChange[i] = MatrBoutonEtat[i];
            ChaineTrav = "Bouton " + String(i) + "  valeur : " + String(MatrBoutonChange[i]);
            Trace(20, TAG_TEST, ChaineTrav);
        }
    }
}

//*********************************************************************
// Test led brigthness
//***********************************************************************
void TestLedBrigthness() {
    String ChaineTrav;
    int CoulLed;

    CurrentTime = millis();

    if (CurrentTime > (DerniereLectureTest + INTERVAL_LECTURE_TEST)) {
        DerniereLectureTest = CurrentTime;

        // calcul brigthness
        Brightness = Brightness + 5;
        if (Brightness < 10) {
            Brightness = 10;
        }
        if (Brightness > 150) {
            Brightness = 10;
        }

        ClearOled(ECRAN_ROTATIF, false);
        OledTexte(20, 20, 3, "Test", ECRAN_ROTATIF, true, true);

        ClearOled(ECRAN_ORDRE, false);
        OledTexte(20, 20, 3, String(Brightness), ECRAN_ORDRE, true, true);

        ChaineTrav = "Brigthness : " + String(Brightness);
        Trace(20, TAG_TEST, ChaineTrav);

        // affichage des leds
        CoulLed = 0;
        for (int i = 0; i < PixelCount; i++) {
            CoulLed++;
            if (CoulLed > 10) {
                CoulLed = 1;
            }

            MatrLed[i] = CoulLed;

        }
        RefreshLed();
    }
}

//*********************************************************************
// Etalonnage des sliders
//***********************************************************************
void EtalonnageSlider() {
    String ChaineTrav;
    int NumSlider;
    int NumEeprom;
    int ValEeprom;

    CurrentTime = millis();
    if (CurrentTime > (DernierEtalonnageSlider + INTERVAL_ETALONNAGE_SLIDER)) {
        DernierEtalonnageSlider = CurrentTime;

        // Contrôle validité numéro
        NumSlider = Aux1;
        if ((NumSlider > 0) && (NumSlider < 4)) {

            // Contrôle de la position
            if (MatrBoutonTraitement[BtnPlus] == 1) {
                MatrBoutonTraitement[BtnPlus] = 0;
                PositionSlider = PositionSlider + 1;
            }

            if (MatrBoutonTraitement[BtnMoins] == 1) {
                MatrBoutonTraitement[BtnMoins] = 0;
                PositionSlider = PositionSlider - 1;
            }

            if (PositionSlider < 0) {
                PositionSlider = 9;
            }

            if (PositionSlider > 9) {
                PositionSlider = 0;
            }

            // Affichage des informations
            //NumEeprom = (NumSlider * 10) + PositionSlider;
            NumEeprom = EEP_SLIDER1_ETAL + ((NumSlider - 1) * 20) + (PositionSlider * 2);
            ValEeprom = LectureEepromDouble(NumEeprom);

            if (NumSlider == 1) {
                ValAnalogique = analogRead(Slider1);
            }
            if (NumSlider == 2) {
                ValAnalogique = analogRead(Slider2);
            }
            if (NumSlider == 3) {
                ValAnalogique = analogRead(Slider3);
            }

            ChaineTrav = "";
            ChaineTrav = "Slider : " + String(NumSlider);
            ChaineTrav = CompleteChaine(ChaineTrav, LCD_NB_COLONNE);
            Lcd[0] = ChaineTrav;

            ChaineTrav = "";
            ChaineTrav = "Position : " + String(PositionSlider);
            ChaineTrav = CompleteChaine(ChaineTrav, LCD_NB_COLONNE);
            Lcd[1] = ChaineTrav;

            ChaineTrav = "";
            ChaineTrav = "Eeprom : " + String(ValEeprom);
            ChaineTrav = CompleteChaine(ChaineTrav, LCD_NB_COLONNE);
            Lcd[2] = ChaineTrav;

            ChaineTrav = "";
            ChaineTrav = "Lue : " + String(ValAnalogique);
            ChaineTrav = CompleteChaine(ChaineTrav, LCD_NB_COLONNE);
            Lcd[3] = ChaineTrav;

            // controle refresh
            //if (CurrentTime > (DernierRefreshEtalonnageSlider + INTERVAL_REFRESH_ETALONNAGE_SLIDER)) {
            //DernierRefreshEtalonnageSlider = CurrentTime;
            LcdChange = true;
            //}
            // pas d'envoi pour ecran déporté refresh 100 ms
            // LcdChange = true;

            // Test de validation
            if (MatrBoutonTraitement[BtnValid] == 1) {
                MatrBoutonTraitement[BtnValid] = 0;
                // enregistrement EEPROM
                EcritureEepromDouble(NumEeprom, ValAnalogique);
                PositionSlider = PositionSlider + 1;

            }
            // test de fin
            if (MatrBoutonTraitement[BtnExit] == 1) {
                MatrBoutonTraitement[BtnExit] = 0;
                ClearLcd();
                //LcdChange = true;
            }

        }
    }
}

//*********************************************************************
// Reset Oled
//***********************************************************************
void ResetOled() {

    ClearOled(ECRAN_ORDRE, true);
    ClearOled(ECRAN_POSITION, true);
    ClearOled(ECRAN_ROTATIF, true);
    ClearOled(ECRAN_SLIDER, true);

}

//*********************************************************************
// Lecture eeprom slider
//***********************************************************************
void LectureEepromSlider() {
    String ChaineTrav;
    int NumSlider;
    int NumEeprom;
    int ValEeprom;

    NumSlider = Aux1;
    if ((NumSlider > 0) && (NumSlider < 4)) {
        for (int i = 0; i < 10; i++) {
            NumEeprom = EEP_SLIDER1_ETAL + ((NumSlider - 1) * 20) + (i * 2);
            ValEeprom = LectureEepromDouble(NumEeprom);
            ChaineTrav = "Slider : " + String(NumSlider) + " position " + String(i) + "  valeur : " + String(ValEeprom);
            Trace(20, TAG_TEST, ChaineTrav);
        }

    }

}

//*********************************************************************
// Lecture conversion slider
//***********************************************************************
void LectureConversionSlider() {
    String ChaineTrav;
    int NumSlider;
    int ValAnalog;
    int ValConvertie;

    NumSlider = Aux1;
    ValAnalog = Aux2;
    ValConvertie = LectureSlider(0, NumSlider, ValAnalog);

    ChaineTrav =
            "Slider : " + String(NumSlider) + "  valeur : " + String(ValAnalog) + " conv : " + String(ValConvertie);
    Trace(20, TAG_TEST, ChaineTrav);

}

/*
  void chenillardUp(int temps, int r, int v, int b)
  {
  for (int i = 0 ; i < 10 ; i++)
  {
    strip.setPixelColor(i, 0, 0, 0);
  }
  strip.show();
  for (int i = 0 ; i < 10 ; i++)
  {
    Serial.println(i);
    strip.setPixelColor(i, r, v, b);
    strip.show();
    delay(temps);
    //strip.setPixelColor(i, 0, 0, 0);
    // strip.show();

  }
  }*/