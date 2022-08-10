#include "Watchy_Stevie.h"

const unsigned char *tetris_nums[10] = {tetris0, tetris1, tetris2, tetris3, tetris4, tetris5, tetris6, tetris7, tetris8, tetris9};

const uint8_t daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

RTC_DATA_ATTR int watchfaceMode = 3;
RTC_DATA_ATTR bool __showHands = false;
RTC_DATA_ATTR bool light = true;

void WatchyStevie::button1()
{
    RTC.read(currentTime);
    showWatchFace(false);
}
void WatchyStevie::button2()
{

    watchfaceMode = watchfaceMode + 1;
    if (watchfaceMode > 3)
    {
        watchfaceMode = 1;
    }

    __showHands = true;
    if (watchfaceMode == 1)
    {
    }
    else if (watchfaceMode == 2)
    {
        __showHands = false;
    }
    else if (watchfaceMode == 3)
    {
    }

    display.setFullWindow();
    display.fillScreen(GxEPD_BLACK);
    display.setTextColor(GxEPD_WHITE);
    // display.setFont(&DSEG7_Classic_Bold_53);
    display.setFont(&FreeMonoBold9pt7b);

    const char *menuItems[] = {
        "x",
        "Default",
        "Moon Phase",
        "Moon Hands",
    };

    display.setCursor(5, 53 + 60);
    display.println("Mode");
    display.println(watchfaceMode);
    display.println(menuItems[watchfaceMode]);
    display.display(true);

    // display.fillScreen(GxEPD_WHITE);
    // RTC.read(currentTime);
    // showWatchFace(true);
}

void WatchyStevie::drawWatchFace()
{
    if (watchfaceMode == 1)
    {
        drawWatchFace_default();
    }
    else
    {
        drawWatchFace_moonPhases();
    }
}

void WatchyStevie::drawWatchFace_moonPhases()
{

    display.fillScreen(GxEPD_BLACK); // GxEPD_WHITE
    // display.drawBitmap(0, 0, tetrisbg, DISPLAY_WIDTH, DISPLAY_HEIGHT, GxEPD_BLACK);
    display.drawBitmap(0, 0, epd_bitmap_moon_phases, DISPLAY_WIDTH, DISPLAY_HEIGHT, GxEPD_WHITE);

    uint8_t myHour;
    uint8_t myMinute;
    uint8_t radius;
    float angle_hourScale;
    float angle_minuteScale;
    float angle_hourHand;
    float angle_minuteHand;
    float pi = 3.1415926535897932384626433832795;

    myHour = currentTime.Hour > 12 ? currentTime.Hour - 12 : currentTime.Hour;
    myMinute = currentTime.Minute;

    angle_hourScale = 2.0 * pi / 12.0;
    angle_minuteScale = 2.0 * pi / 60.0;
    angle_hourHand = angle_hourScale * (myHour - 3.0) + 2.0 * pi / 720.0 * myMinute;
    angle_minuteHand = angle_minuteScale * (myMinute - 15.0);

    // // draw minute scale
    // radius = 98;
    // for (uint8_t i = 0; i < 60; i++)
    // {
    //     display.drawLine(100, 100, 100 + radius * cos(angle_minuteScale * i), 100 + radius * sin(angle_minuteScale * i), light ? GxEPD_BLACK : GxEPD_WHITE);
    // }
    // display.fillCircle(100, 100, 93, light ? GxEPD_WHITE : GxEPD_BLACK);

    // // draw hour scale
    // radius = 98;
    // for (uint8_t i = 0; i < 12; i++)
    // {
    //     drawHand(radius, angle_hourScale * i, light);
    // }
    // display.fillCircle(100, 100, 88, light ? GxEPD_WHITE : GxEPD_BLACK);

    if (__showHands)
    {
        // draw hour hand
        radius = 15;
        drawHand(radius, angle_hourHand, !light);

        // draw minute hand

        radius = 30;
        drawHand(radius, angle_minuteHand, !light);

        // draw center point
        display.fillCircle(100, 100, 3, light ? GxEPD_WHITE : GxEPD_BLACK);
    }

    float moonCycleDays = 29.53059;

    display.setFont(&DSEG7_Classic_Regular_15);
    display.setCursor(5, 120);

    float dayOfYear = 0.0;
    for (uint8_t i = 1; i < currentTime.Month; i++)
    {
        dayOfYear += daysInMonth[i - 1];
    }
    dayOfYear += currentTime.Day;

    float dayOfLastNewMoon = 91.0;
    float moonAgeOfsetDays = 0.23;
    float yearsSinceLastNewMoon = (currentTime.Year - (2022.0 - 1970.0)) * 365.25;
    float partialDay = (currentTime.Hour + currentTime.Minute / 60.0) / 24.0;
    float daysSinceLastNewMoon = dayOfYear + yearsSinceLastNewMoon - dayOfLastNewMoon + moonAgeOfsetDays + partialDay;

    // float phase = ((float)daysSinceLastNewMoon) / moonCycleDays;

    float angle_phaseScale = (2.0 * pi / moonCycleDays);
    float angle_phase = angle_phaseScale * daysSinceLastNewMoon + (1.0 * pi);

    radius = 90; // 100;
    drawHand2(radius, angle_phase, !light);

    // display.println(daysSinceLastNewMoon);
    // New moon April 1st, 2022

    // //Hour
    // display.drawBitmap(25, 20, tetris_nums[currentTime.Hour/10], 40, 60, GxEPD_BLACK); //first digit
    // display.drawBitmap(75, 20, tetris_nums[currentTime.Hour%10], 40, 60, GxEPD_BLACK); //second digit

    // //Minute
    // display.drawBitmap(25, 110, tetris_nums[currentTime.Minute/10], 40, 60, GxEPD_BLACK); //first digit
    // display.drawBitmap(75, 110, tetris_nums[currentTime.Minute%10], 40, 60, GxEPD_BLACK); //second digit

    // drawDate();
}

// helper function for hands drawing
void WatchyStevie::drawHand(uint8_t _radius_, float _angle_, bool _light_)
{
    float x = _radius_ * cos(_angle_);
    float y = _radius_ * sin(_angle_);
    display.drawLine(99, 99, 99 + x, 99 + y, _light_ ? GxEPD_BLACK : GxEPD_WHITE);
    display.drawLine(99, 100, 99 + x, 100 + y, _light_ ? GxEPD_BLACK : GxEPD_WHITE);
    display.drawLine(99, 101, 99 + x, 101 + y, _light_ ? GxEPD_BLACK : GxEPD_WHITE);
    display.drawLine(100, 99, 100 + x, 99 + y, _light_ ? GxEPD_BLACK : GxEPD_WHITE);
    display.drawLine(100, 100, 100 + x, 100 + y, _light_ ? GxEPD_BLACK : GxEPD_WHITE);
    display.drawLine(100, 101, 100 + x, 101 + y, _light_ ? GxEPD_BLACK : GxEPD_WHITE);
    display.drawLine(101, 99, 101 + x, 99 + y, _light_ ? GxEPD_BLACK : GxEPD_WHITE);
    display.drawLine(101, 100, 101 + x, 100 + y, _light_ ? GxEPD_BLACK : GxEPD_WHITE);
    display.drawLine(101, 101, 101 + x, 101 + y, _light_ ? GxEPD_BLACK : GxEPD_WHITE);
}

void WatchyStevie::drawHand2(uint8_t _radius_, float _angle_, bool _light_)
{
    float x = _radius_ * cos(_angle_);
    float y = _radius_ * sin(_angle_);
    display.fillCircle(100 + x, 100 + y, 10, _light_ ? GxEPD_WHITE : GxEPD_BLACK);
    display.fillCircle(100 + x, 100 + y, 8, _light_ ? GxEPD_BLACK : GxEPD_WHITE);
}

void WatchyStevie::drawDate()
{

    display.setFont(&DSEG7_Classic_Regular_15);
    // display.setFont(&Seven_Segment10pt7b);

    int16_t x1, y1;
    uint16_t w, h;

    String dayOfWeek = dayStr(currentTime.Wday);
    display.getTextBounds(dayOfWeek, 5, 85, &x1, &y1, &w, &h);
    if (currentTime.Wday == 4)
    {
        w = w - 5;
    }
    display.setCursor(85 - w, 85);
    display.println(dayOfWeek);

    String month = monthShortStr(currentTime.Month);
    display.getTextBounds(month, 60, 110, &x1, &y1, &w, &h);
    display.setCursor(85 - w, 110);
    display.println(month);

    // display.setFont(&DSEG7_Classic_Bold_25);
    display.setCursor(5, 120);
    if (currentTime.Day < 10)
    {
        display.print("0");
    }
    display.println(currentTime.Day);
    display.setCursor(5, 150);
    display.println(tmYearToCalendar(currentTime.Year)); // offset from 1970, since year is stored in uint8_t
}

void WatchyStevie::drawWatchFace_default()
{
    display.fillScreen(GxEPD_BLACK);
    display.setTextColor(GxEPD_WHITE);

    display.setFont(&DSEG7_Classic_Bold_53);
    display.setCursor(5, 53 + 60);
    if (currentTime.Hour < 10)
    {
        display.print("0");
    }
    display.print(currentTime.Hour);
    display.print(":");
    if (currentTime.Minute < 10)
    {
        display.print("0");
    }
    display.println(currentTime.Minute);
}