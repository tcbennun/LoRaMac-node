/*!
 * \file      systime.c
 *
 * \brief     System time functions implementation.
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2018 Semtech - STMicroelectronics
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 *
 * \author    MCD Application Team ( STMicroelectronics International )
 */
#include <stdio.h>
#include "rtc-board.h"
#include "systime.h"

#include <esp_timer.h>

SysTime_t SysTimeAdd( SysTime_t a, SysTime_t b )
{
    SysTime_t c =  { .Seconds = 0, .SubSeconds = 0 };

    c.Seconds = a.Seconds + b.Seconds;
    c.SubSeconds = a.SubSeconds + b.SubSeconds;
    if( c.SubSeconds >= 1000 )
    {
        c.Seconds++;
        c.SubSeconds -= 1000;
    }
    return c;
}

SysTime_t SysTimeSub( SysTime_t a, SysTime_t b )
{
    SysTime_t c = { .Seconds = 0, .SubSeconds = 0 };

    c.Seconds = a.Seconds - b.Seconds;
    c.SubSeconds = a.SubSeconds - b.SubSeconds;
    if( c.SubSeconds < 0 )
    {
        c.Seconds--;
        c.SubSeconds += 1000;
    }
    return c;
}

void SysTimeSet( SysTime_t sysTime )
{
    RtcBkupWrite( sysTime.Seconds, ( uint32_t )sysTime.SubSeconds );
}

SysTime_t SysTimeGet( void )
{
    uint32_t seconds;
    uint32_t subSeconds;

    RtcBkupRead( &seconds, &subSeconds );

    SysTime_t sysTime = { .Seconds = seconds, .SubSeconds = ( int16_t )subSeconds };

    return sysTime;
}

SysTime_t SysTimeGetMcuTime( void )
{
    int64_t t = esp_timer_get_time();

    SysTime_t calendarTime = {
        .Seconds = t / 1000000,
        .SubSeconds = (t % 1000000) / 1000,
    };

    return calendarTime;
}

TimerTime_t SysTimeToMs( SysTime_t sysTime )
{
    return ( TimerTime_t )( sysTime.Seconds * 1000 + sysTime.SubSeconds );
}

SysTime_t SysTimeFromMs( TimerTime_t timeMs )
{
    uint32_t seconds = timeMs / 1000;
    uint32_t subSeconds = timeMs - seconds * 1000;
    SysTime_t sysTime = { .Seconds = seconds, .SubSeconds = ( int16_t )subSeconds };

    return sysTime;
}
