// Tonokip RepRap firmware rewrite based off of Hydra-mmm firmware.
// License: GPL

#ifndef MACKEREL_H
#define MACKEREL_H

#define  FORCE_INLINE __attribute__((always_inline)) inline

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>


#include "fastio.h"
#include "Configuration.h"
#include "pins.h"

#ifndef AT90USB
#define  HardwareSerial_h // trick to disable the standard HWserial
#endif

#if (ARDUINO >= 100)
# include "Arduino.h"
#else
# include "WProgram.h"
  //Arduino < 1.0.0 does not define this, so we need to do it ourselves
# define analogInputToDigitalPin(p) ((p) + A0)
#endif

#ifdef AT90USB
#include "HardwareSerial.h"
#endif

#include "MarlinSerial.h"

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#include "WString.h"

#ifdef AT90USB
   #ifdef BTENABLED
         #define MYSERIAL bt
   #else
         #define MYSERIAL Serial
   #endif // BTENABLED
#else
  #define MYSERIAL MSerial
#endif

#define SERIAL_PROTOCOL(x) (MYSERIAL.print(x))
#define SERIAL_PROTOCOL_F(x,y) (MYSERIAL.print(x,y))
#define SERIAL_PROTOCOLPGM(x) (serialprintPGM(PSTR(x)))
#define SERIAL_PROTOCOLLN(x) (MYSERIAL.print(x),MYSERIAL.write('\n'))
#define SERIAL_PROTOCOLLNPGM(x) (serialprintPGM(PSTR(x)),MYSERIAL.write('\n'))


const char errormagic[] PROGMEM ="Error:";
const char echomagic[] PROGMEM ="echo:";
#define SERIAL_ERROR_START (serialprintPGM(errormagic))
#define SERIAL_ERROR(x) SERIAL_PROTOCOL(x)
#define SERIAL_ERRORPGM(x) SERIAL_PROTOCOLPGM(x)
#define SERIAL_ERRORLN(x) SERIAL_PROTOCOLLN(x)
#define SERIAL_ERRORLNPGM(x) SERIAL_PROTOCOLLNPGM(x)

#define SERIAL_ECHO_START (serialprintPGM(echomagic))
#define SERIAL_ECHO(x) SERIAL_PROTOCOL(x)
#define SERIAL_ECHOPGM(x) SERIAL_PROTOCOLPGM(x)
#define SERIAL_ECHOLN(x) SERIAL_PROTOCOLLN(x)
#define SERIAL_ECHOLNPGM(x) SERIAL_PROTOCOLLNPGM(x)

#define SERIAL_ECHOPAIR(name,value) (serial_echopair_P(PSTR(name),(value)))

void serial_echopair_P(const char *s_P, float v);
void serial_echopair_P(const char *s_P, double v);
void serial_echopair_P(const char *s_P, unsigned long v);


//Things to write to serial from Program memory. Saves 400 to 2k of RAM.
FORCE_INLINE void serialprintPGM(const char *str)
{
  char ch=pgm_read_byte(str);
  while(ch)
  {
    MYSERIAL.write(ch);
    ch=pgm_read_byte(++str);
  }
}


void get_command();
void process_commands();

void manage_inactivity();

#if defined(DUAL_X_CARRIAGE) && defined(X_ENABLE_PIN) && X_ENABLE_PIN > -1 \
    && defined(X2_ENABLE_PIN) && X2_ENABLE_PIN > -1
  #define  enable_x() do { WRITE(X_ENABLE_PIN, X_ENABLE_ON); WRITE(X2_ENABLE_PIN, X_ENABLE_ON); } while (0)
  #define disable_x() do { WRITE(X_ENABLE_PIN,!X_ENABLE_ON); WRITE(X2_ENABLE_PIN,!X_ENABLE_ON); axis_known_position[X_AXIS] = false; } while (0)
#elif defined(X_ENABLE_PIN) && X_ENABLE_PIN > -1
  #define  enable_x() WRITE(X_ENABLE_PIN, X_ENABLE_ON)
  #define disable_x() { WRITE(X_ENABLE_PIN,!X_ENABLE_ON); axis_known_position[X_AXIS] = false; }
#else
  #define enable_x() ;
  #define disable_x() ;
#endif

#if defined(Y_ENABLE_PIN) && Y_ENABLE_PIN > -1
  #ifdef Y_DUAL_STEPPER_DRIVERS
    #define  enable_y() { WRITE(Y_ENABLE_PIN, Y_ENABLE_ON); WRITE(Y2_ENABLE_PIN,  Y_ENABLE_ON); }
    #define disable_y() { WRITE(Y_ENABLE_PIN,!Y_ENABLE_ON); WRITE(Y2_ENABLE_PIN, !Y_ENABLE_ON); axis_known_position[Y_AXIS] = false; }
  #else
    #define  enable_y() WRITE(Y_ENABLE_PIN, Y_ENABLE_ON)
    #define disable_y() { WRITE(Y_ENABLE_PIN,!Y_ENABLE_ON); axis_known_position[Y_AXIS] = false; }
  #endif
#else
  #define enable_y() ;
  #define disable_y() ;
#endif

#if defined(Z_ENABLE_PIN) && Z_ENABLE_PIN > -1
  #ifdef Z_DUAL_STEPPER_DRIVERS
    #define  enable_z() { WRITE(Z_ENABLE_PIN, Z_ENABLE_ON); WRITE(Z2_ENABLE_PIN, Z_ENABLE_ON); }
    #define disable_z() { WRITE(Z_ENABLE_PIN,!Z_ENABLE_ON); WRITE(Z2_ENABLE_PIN,!Z_ENABLE_ON); axis_known_position[Z_AXIS] = false; }
  #else
    #define  enable_z() WRITE(Z_ENABLE_PIN, Z_ENABLE_ON)
    #define disable_z() { WRITE(Z_ENABLE_PIN,!Z_ENABLE_ON); axis_known_position[Z_AXIS] = false; }
  #endif
#else
  #define enable_z() ;
  #define disable_z() ;
#endif

#if defined(E0_ENABLE_PIN) && (E0_ENABLE_PIN > -1)
  #define enable_e0() WRITE(E0_ENABLE_PIN, E_ENABLE_ON)
  #define disable_e0() WRITE(E0_ENABLE_PIN,!E_ENABLE_ON)  
#else
  #define enable_e0()  /* nothing */
  #define disable_e0() /* nothing */
#endif

#if defined(P_ENABLE_PIN) && (P_ENABLE_PIN > -1)
  #define enable_p() WRITE(P_ENABLE_PIN, P_ENABLE_ON)
  #define disable_p() WRITE(P_ENABLE_PIN,!P_ENABLE_ON)
#else
  #define enable_e1()  /* nothing */
  #define disable_e1() /* nothing */
#endif

#if (EXTRUDERS > 2) && defined(E2_ENABLE_PIN) && (E2_ENABLE_PIN > -1)
  #define enable_e2() WRITE(E2_ENABLE_PIN, E_ENABLE_ON)
  #define disable_e2() WRITE(E2_ENABLE_PIN,!E_ENABLE_ON)
#else
  #define enable_e2()  /* nothing */
  #define disable_e2() /* nothing */
#endif


enum AxisEnum {X_AXIS=0, Y_AXIS=1, Z_AXIS=2, E_AXIS=3, P_AXIS=4};  //FMM added P_AXIS to list for puller motor


void FlushSerialRequestResend();
void ClearToSend();

void get_coordinates();
#ifdef DELTA
void calculate_delta(float cartesian[3]);
extern float delta[3];
#endif
void prepare_move();
void kill();
void Stop();

bool IsStopped();

void enquecommand(const char *cmd); //put an ASCII command at the end of the current buffer.
void enquecommand_P(const char *cmd); //put an ASCII command at the end of the current buffer, read from flash
void prepare_arc_move(char isclockwise);
void clamp_to_software_endstops(float target[3]);

void refresh_cmd_timeout(void);

#ifdef FAST_PWM_FAN
void setPwmFrequency(uint8_t pin, int val);
#endif

#ifndef CRITICAL_SECTION_START
  #define CRITICAL_SECTION_START  unsigned char _sreg = SREG; cli();
  #define CRITICAL_SECTION_END    SREG = _sreg;
#endif //CRITICAL_SECTION_START

extern float extruder_feedrate;
extern float extruder_rpm; 
extern float extruder_rpm_set;
extern float puller_feedrate;
extern float homing_feedrate[];
extern bool axis_relative_modes[];
extern unsigned char extrude_status;
#define ES_ENABLE_SET 1
#define ES_SWITCH_SET 2
#define ES_AUTO_SET 4
#define ES_HOT_SET 8
#define ES_TEMP_SET 16
#define ES_STATS_SET 128
#define ES_ENABLE_CLEAR 254
#define ES_SWITCH_CLEAR 253
#define ES_AUTO_CLEAR 251
#define ES_HOT_CLEAR 247
#define ES_TEMP_CLEAR 239
#define ES_STATS_CLEAR 127


extern float filament_width_desired; //holds the desired filament width (2.8mm)
extern float sum_measured_filament_width; 
extern float n_measured_filament_width;  
extern float avg_measured_filament_width; 
extern float max_measured_filament_width;
extern float min_measured_filament_width;
extern float extrude_length;
extern float fil_length_cutoff;
extern float filament_control; //output of the PID controller
extern volatile float filament_width_meas; //holds the filament width as measured by the sensor
extern float puller_feedrate_default;
extern int feedmultiply;
extern unsigned long duration;
extern unsigned long timeremaining;
extern int default_winder_speed;
extern int winder_rpm_factor;

extern int extrudemultiply; // Sets extrude multiply factor (in percent) for all extruders
extern int extruder_multiply[EXTRUDERS]; // sets extrude multiply factor (in percent) for each extruder individually
extern float volumetric_multiplier[EXTRUDERS]; // reciprocal of cross-sectional area of filament (in square millimeters), stored this way to reduce computational burden in planner
extern float current_position[NUM_AXIS] ;
extern float add_homeing[3];
#ifdef DELTA
extern float endstop_adj[3];
extern float delta_radius;
extern float delta_diagonal_rod;
extern float delta_segments_per_second;
void recalc_delta_settings(float radius, float diagonal_rod);
#endif
extern float min_pos[3];
extern float max_pos[3];
extern bool axis_known_position[3];
extern float zprobe_zoffset;
extern int winderSpeed;
#ifdef BARICUDA
extern int ValvePressure;
extern int EtoPPressure;
#endif

#ifdef FAN_SOFT_PWM
extern unsigned char wSpeedSoftPwm;
#endif

#ifdef FWRETRACT
extern bool autoretract_enabled;
extern bool retracted;
extern float retract_length, retract_feedrate, retract_zlift;
extern float retract_recover_length, retract_recover_feedrate;
#endif

extern unsigned long starttime;
extern unsigned long stoptime;

// Handling multiple extruders pins
extern uint8_t active_extruder;

#ifdef DIGIPOT_I2C
extern void digipot_i2c_set_current( int channel, float current );
extern void digipot_i2c_init();
#endif

#endif
