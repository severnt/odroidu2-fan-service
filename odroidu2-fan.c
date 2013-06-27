/*
 * odroidu2-fan.c
 *
 *  Created on: May 9, 2013
 *  Original Author: mdrjr
 *  Modified by: zinc (https://github.com/severnt/)
 *
 * Public domain
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>

typedef struct
{
    const char * tmu;
    const char * pwm;
    int tmu_divisor;
} Mode;

#define MODES_COUNT 2

const Mode modes[MODES_COUNT] =
{
    {
        .tmu = "/sys/devices/platform/tmu/temperature",
        .pwm = "/sys/devices/platform/odroid-fan/pwm_duty",
        .tmu_divisor = 1
    },
    {
        .tmu = "/sys/devices/virtual/thermal/thermal_zone0/temp",
        .pwm = "/sys/devices/platform/odroidu2-fan/pwm_duty",
        .tmu_divisor = 1000
    }
};

int detect_mode ( void )
{
    int mode = -1;
    FILE * f1 = 0;
    FILE * f2 = 0;

    for ( int i = 0; i < MODES_COUNT; ++i )
    {
        f1 = fopen ( modes[i].tmu, "r" );
        f2 = fopen ( modes[i].pwm, "w" );

        if ( f1 != 0 )
        {
            fclose ( f1 );
        }

        if ( f2 != 0 )
        {
            fclose ( f2 );
        }

        if ( f1 != 0 && f2 != 0 )
        {
            mode = i + 1;
            break;
        }
    }

    return mode;
}

void u2_service ( const int mode, const int lower, const int upper, bool forceUpdates, bool log )
{
    FILE * fp_tmu = fopen ( modes[mode].tmu, "r" );
    FILE * fp_fan = fopen ( modes[mode].pwm, "w" );

    if ( !fp_tmu )
    {
        if ( log )
            fprintf ( stderr, "odroidu2-fan: cannot open tmu file: %s\n", modes[mode].tmu );
        return;
    };
    if ( !fp_fan )
    {
        if ( log )
            fprintf ( stderr, "odroidu2-fan: cannot open fan file: %s\n", modes[mode].pwm );
        return;
    }

    int temperature = 0;
    fscanf ( fp_tmu, "%d", &temperature );

    temperature /= modes[mode].tmu_divisor;

    /*
     * For U2 FAN PWM it starts on 0 and max value is 255
     * We'll also use kernel based parameters of the tmu to handle this.
     * Since we know that the first throttle happens at 85C, 85C is a temperature that we don't want.
     * we'll set our max speed temp as 80C
     */

    int target_pwm_value = 255 * ( temperature - lower ) / upper;

    if ( target_pwm_value <= 0 )
        target_pwm_value = 0;

    if ( target_pwm_value > 255 )
        target_pwm_value = 255;

    static int last_pwm_value = 0;

    if ( log )
    {
        fprintf ( stderr, "temperature: %d C, target PWM value: %d, last PWM value: %d\n", temperature, target_pwm_value, last_pwm_value );
    }

    if ( forceUpdates || target_pwm_value != last_pwm_value )
    {
        char to_write[3];
        sprintf ( to_write, "%d", target_pwm_value );
        fputs ( to_write, fp_fan );
        fflush ( fp_fan );

        last_pwm_value = target_pwm_value;
    }

    fclose ( fp_fan );
    fclose ( fp_tmu );
}

void showHelp ( const char * me )
{
    printf ( "%s [options]\n\n", me );
    printf ( "Varies the fan speed with PWM when temperature is between lower and upper bounds\n\n"
             "-f    Force update PWM each time temperature is checked (default: false)\n"
             "-d    Daemon mode\n"
             "-u n  At or above this temperature (degrees C), fan at 100%% (default: 80)\n"
             "-l n  Below this temperature (degrees C), fan off (default: 45)\n"
             "-m n  Operating mode (default: 0)\n"
             "      0 = auto-detect\n"
             "      1 = force kernel 3.0 mode\n"
             "      2 = force kernel 3.8+ mode\n"
             " -i n Interval (seconds) between probes (default: 1)\n"
             "\n" );

    exit ( 0 );
}

int main ( int argc, char *argv[] )
{
    int lower = 45;
    int upper = 80;
    int mode = 0;
    bool forceUpdates = false;
    bool daemon = false;
    int interval = 1;

    char * tmp = 0;

    if ( argc > 1 )
    {
        int opt = 0;

        while ( ( opt = getopt ( argc, argv, "fdu:l:m:i:" ) ) != -1 )
        {
            switch ( opt )
            {
                case 'f':
                    forceUpdates = true;
                    break;

                case 'd':
                    daemon = true;
                    break;

                case 'u':
                    upper = strtol ( optarg, &tmp, 0 );
                    if ( tmp == optarg )
                        showHelp ( argv[0] );
                    break;

                case 'l':
                    lower = strtol ( optarg, &tmp, 0 );
                    if ( tmp == optarg )
                        showHelp ( argv[0] );
                    break;

                case 'm':
                    mode = strtol ( optarg, &tmp, 10 );
                    if ( tmp == optarg || mode < 0 || mode > 2 )
                        showHelp ( argv[0] );
                    break;

                default:
                    showHelp ( argv[0] );
            }
        }
    }

    if ( mode == 0 )
    {
        mode = detect_mode();
    }

    if ( mode < 0 )
    {
        fprintf ( stderr, "Unable to detect kernel 3.0 style or 3.8 style TMU or PWM\n" );
        showHelp ( argv[0] );
    }

    if ( upper > 100 )
    {
        fprintf ( stderr, "Upper temperature cannot be above 100C\n" );
        showHelp ( argv[0] );
    }

    if ( upper < lower || lower > upper )
    {
        fprintf ( stderr, "Upper temperature must be greater than or equal to lower temperature\n" );
        showHelp ( argv[0] );
    }

    if ( ! daemon || fork() == 0 )
    {
        if ( daemon )
        {
            fclose ( stdout );
            fclose ( stderr );
        }

        while ( 1 )
        {
            sleep ( interval );
            u2_service ( mode - 1, lower, upper, forceUpdates, ! daemon );
        }
    }

    return 0;
}
