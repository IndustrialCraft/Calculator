#include "ESPIO.hpp"

#include <algorithm>
#include <stdexcept>

#include "HD44780.h"
#include "driver/adc.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

#define ROW_1_GPIO GPIO_NUM_13
#define ROW_2_GPIO GPIO_NUM_12
#define ROW_3_GPIO GPIO_NUM_14
#define ROW_4_GPIO GPIO_NUM_27
#define COL_1_GPIO GPIO_NUM_26
#define COL_2_GPIO GPIO_NUM_25
#define COL_3_GPIO GPIO_NUM_33
#define COL_4_GPIO GPIO_NUM_32

#define JOYSTICK_SW_GPIO GPIO_NUM_23
#define JOYSTICK_X_ADC ADC1_CHANNEL_6
#define JOYSTICK_Y_ADC ADC1_CHANNEL_7

#define LCD_ADDR 0x3F
//#define LCD_ADDR 0x27
#define SDA_PIN 21
#define SCL_PIN 22
#define LCD_COLS 20
#define LCD_ROWS 4

uint8_t keypad_rows[] = {ROW_1_GPIO, ROW_2_GPIO, ROW_3_GPIO, ROW_4_GPIO};
uint8_t keypad_cols[] = {COL_1_GPIO, COL_2_GPIO, COL_3_GPIO, COL_4_GPIO};

ESPIO::ESPIO() : m_inputMode(ESPIO::EInputMode::none), m_activateJoystickTimer(-1), m_lastInput(0), m_cursor(0, 0), m_isCursorVisible(false), m_input(EInput::none) {
    gpio_reset_pin(COL_1_GPIO);
    gpio_reset_pin(COL_2_GPIO);
    gpio_reset_pin(COL_3_GPIO);
    gpio_reset_pin(COL_4_GPIO);
    gpio_reset_pin(ROW_1_GPIO);
    gpio_reset_pin(ROW_2_GPIO);
    gpio_reset_pin(ROW_3_GPIO);
    gpio_reset_pin(ROW_4_GPIO);

    gpio_set_direction(COL_1_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_direction(COL_2_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_direction(COL_3_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_direction(COL_4_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_direction(ROW_1_GPIO, GPIO_MODE_INPUT);
    gpio_set_direction(ROW_2_GPIO, GPIO_MODE_INPUT);
    gpio_set_direction(ROW_3_GPIO, GPIO_MODE_INPUT);
    gpio_set_direction(ROW_4_GPIO, GPIO_MODE_INPUT);

    gpio_set_pull_mode(ROW_1_GPIO, GPIO_PULLDOWN_ONLY);
    gpio_set_pull_mode(ROW_2_GPIO, GPIO_PULLDOWN_ONLY);
    gpio_set_pull_mode(ROW_3_GPIO, GPIO_PULLDOWN_ONLY);
    gpio_set_pull_mode(ROW_4_GPIO, GPIO_PULLDOWN_ONLY);

    gpio_reset_pin(JOYSTICK_SW_GPIO);
    gpio_set_direction(JOYSTICK_SW_GPIO, GPIO_MODE_INPUT);
    gpio_set_pull_mode(JOYSTICK_SW_GPIO, GPIO_PULLUP_ONLY);
    adc1_config_width(ADC_WIDTH_BIT_10);
    LCD_init(LCD_ADDR, SDA_PIN, SCL_PIN, LCD_COLS, LCD_ROWS);

    uint8_t power0[] = {0b11100, 0b10100, 0b10100, 0b11100, 0b00000, 0b00000, 0b00000, 0b00000};
    uint8_t power1[] = {0b01000, 0b11000, 0b01000, 0b01000, 0b11100, 0b00000, 0b00000, 0b00000};
    uint8_t power4[] = {0x04, 0x0C, 0x14, 0x1E, 0x04, 0x00, 0x00, 0x00};
    uint8_t power5[] = {0x1C, 0x10, 0x1C, 0x04, 0x1C, 0x00, 0x00, 0x00};
    uint8_t power6[] = {0x1C, 0x10, 0x1C, 0x14, 0x1C, 0x00, 0x00, 0x00};
    uint8_t power7[] = {0x1C, 0x04, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00};
    uint8_t power8[] = {0x1C, 0x14, 0x1C, 0x14, 0x1C, 0x00, 0x00, 0x00};
    uint8_t power9[] = {0x1C, 0x14, 0x1C, 0x04, 0x1C, 0x00, 0x00, 0x00};

    LCD_uploadCharacter(0, power0);
    LCD_uploadCharacter(1, power1);
    LCD_uploadCharacter(2, power4);
    LCD_uploadCharacter(3, power5);
    LCD_uploadCharacter(4, power6);
    LCD_uploadCharacter(5, power7);
    LCD_uploadCharacter(6, power8);
    LCD_uploadCharacter(7, power9);
}

uint16_t read_keypad() {
    uint16_t output = 0;
    gpio_set_level(COL_2_GPIO, 0);
    gpio_set_level(COL_3_GPIO, 0);
    gpio_set_level(COL_4_GPIO, 0);
    for (int y = 0; y < 4; y++) {
        gpio_set_level((gpio_num_t)keypad_cols[y], 1);
        for (int x = 0; x < 4; x++) {
            if (gpio_get_level((gpio_num_t)keypad_rows[x]))
                output |= 1 << (x + y * 4);
        }
        gpio_set_level((gpio_num_t)keypad_cols[y], 0);
    }
    return output;
}
uint8_t read_joystick_x() {
    adc1_config_channel_atten(JOYSTICK_X_ADC, ADC_ATTEN_DB_11);
    return (uint8_t)(adc1_get_raw(JOYSTICK_X_ADC) >> 2);
}
uint8_t read_joystick_y() {
    adc1_config_channel_atten(JOYSTICK_Y_ADC, ADC_ATTEN_DB_11);
    return (uint8_t)(adc1_get_raw(JOYSTICK_Y_ADC) >> 2);
}

ESPIO::~ESPIO() {}

void ESPIO::setCursor(int x, int y) {
    this->m_cursor = std::pair<int, int>(x, y);
}
void ESPIO::setCursorVisibility(bool visible) {
    m_isCursorVisible = visible;
}
void ESPIO::tick() {
    uint32_t readval = read_keypad();
    // printf("read: %ld\n", readval);
    if (readval != m_lastInput) {
        if (m_inputMode != ESPIO::EInputMode::shift && (readval & 8)) {
            if (m_inputMode == ESPIO::EInputMode::alpha)
                m_inputMode = ESPIO::EInputMode::none;
            else
                m_inputMode = ESPIO::EInputMode::alpha;
            goto skip;
        }
        if ((readval & 2048)) {
            if (m_inputMode == ESPIO::EInputMode::shift)
                m_inputMode = ESPIO::EInputMode::none;
            else
                m_inputMode = ESPIO::EInputMode::shift;
            goto skip;
        }
        if (m_inputMode == ESPIO::EInputMode::none) {
            if (readval & 1)
                m_input = IO::EInput::n1;
            if (readval & 2)
                m_input = IO::EInput::n4;
            if (readval & 4)
                m_input = IO::EInput::n7;
            if (readval & 16)
                m_input = IO::EInput::n2;
            if (readval & 32)
                m_input = IO::EInput::n5;
            if (readval & 64)
                m_input = IO::EInput::n8;
            if (readval & 128)
                m_input = IO::EInput::n0;
            if (readval & 256)
                m_input = IO::EInput::n3;
            if (readval & 512)
                m_input = IO::EInput::n6;
            if (readval & 1024)
                m_input = IO::EInput::n9;
            if (readval & 4096)
                m_input = IO::EInput::plus;
            if (readval & 8192)
                m_input = IO::EInput::minus;
            if (readval & 16384)
                m_input = IO::EInput::del;
            if (readval & 32768)
                m_input = IO::EInput::exec;
        }
        if (m_inputMode == ESPIO::EInputMode::power) {
            if (readval & 1)
                m_input = IO::EInput::p1;
            else if (readval & 2)
                m_input = IO::EInput::p4;
            else if (readval & 4)
                m_input = IO::EInput::p7;
            else if (readval & 16)
                m_input = IO::EInput::p2;
            else if (readval & 32)
                m_input = IO::EInput::p5;
            else if (readval & 64)
                m_input = IO::EInput::p8;
            else if (readval & 128)
                m_input = IO::EInput::p0;
            else if (readval & 256)
                m_input = IO::EInput::p3;
            else if (readval & 512)
                m_input = IO::EInput::p6;
            else if (readval & 1024)
                m_input = IO::EInput::p9;
            else {
                goto skip;
            }
            m_inputMode = ESPIO::EInputMode::none;
        }
        if (m_inputMode == ESPIO::EInputMode::shift) {
            if (readval & 1)
                m_input = IO::EInput::left;
            else if (readval & 16)
                m_input = IO::EInput::equals;
            else if (readval & 256)
                m_input = IO::EInput::more;
            else if (readval & 4096)
                m_input = IO::EInput::times;
            else if (readval & 8192)
                m_input = IO::EInput::divide;
            else if (readval & 16384)
                m_input = IO::EInput::back;
            else if (readval & 2)
                m_input = IO::EInput::paranOpen;
            else if (readval & 32)
                m_input = IO::EInput::paranClose;
            else if (readval & 512)
                m_input = IO::EInput::comma;
            else if (readval & 32768)
                m_input = IO::EInput::history;
            else if (readval & 4)
                m_input = IO::EInput::bang;
            else if (readval & 8) {
                m_inputMode = ESPIO::EInputMode::power;
                goto skip;
            } else {
                goto skip;
            }
            m_inputMode = ESPIO::EInputMode::none;
        }
        if (m_inputMode == ESPIO::EInputMode::alpha) {
            if (readval & 1)
                m_input = IO::EInput::ka;
            else if (readval & 2)
                m_input = IO::EInput::kd;
            else if (readval & 4)
                m_input = IO::EInput::kx;
            else if (readval & 16)
                m_input = IO::EInput::kb;
            else if (readval & 32)
                m_input = IO::EInput::ke;
            else if (readval & 64)
                m_input = IO::EInput::ky;
            else if (readval & 256)
                m_input = IO::EInput::kc;
            else if (readval & 512)
                m_input = IO::EInput::kf;
            else if (readval & 1024)
                m_input = IO::EInput::kz;
            else {
                goto skip;
            }
            m_inputMode = ESPIO::EInputMode::none;
        }
    }
skip:
    m_lastInput = readval;
    // printf("joystick x:%d y:%d\n", read_joystick_x(), read_joystick_y());
    int joystickX = read_joystick_y();
    int joystickY = read_joystick_x();
    if (joystickX < 20 || joystickX > 200 || joystickY < 20 || joystickY > 200) {
        if (m_activateJoystickTimer < 0 || ((esp_timer_get_time() - m_activateJoystickTimer > 700000) && (esp_timer_get_time() - m_joystickLastRepeatActivation > 200000))) {
            if (m_activateJoystickTimer < 0)
                m_activateJoystickTimer = esp_timer_get_time();

            m_joystickLastRepeatActivation = esp_timer_get_time();
            if (joystickX < 20 || joystickX > 200)
                m_input = joystickX > 100 ? IO::EInput::right : IO::EInput::left;
            else
                m_input = joystickY > 100 ? IO::EInput::down : IO::EInput::up;
            return;
        }
    } else {
        m_activateJoystickTimer = -1;
    }
}
void ESPIO::setChar(int x, int y, char ch) {
    if (x < 0 || x >= 20 || y < 0 || y >= 4)
        throw std::range_error("character assign outside boundaries");
    m_display[x][y] = ch;
}
void ESPIO::flushDisplay() {
    auto toUpdate = compareAndCopyDisplayBuffers();
    for (int y = 0; y < 4; y++) {
        if ((toUpdate & (1 << y))) {
            LCD_setCursor(0, y);
            for (int x = 0; x < 20; x++) {
                LCD_writeChar(m_display[x][y]);
            }
        }
    }
    LCD_setCursor(18, 0);
    char mode = ' ';
    switch (m_inputMode) {
        case ESPIO::EInputMode::alpha:
            mode = 'A';
            break;
        case ESPIO::EInputMode::shift:
            mode = 'S';
            break;
        case ESPIO::EInputMode::power:
            mode = 'P';
            break;
        case ESPIO::EInputMode::none:
            break;
    }
    LCD_writeChar(mode);
    clearScreen();
    LCD_setCursor(m_cursor.first, m_cursor.second);
    LCD_setCursorVisible(m_isCursorVisible);
}
IO::EInput ESPIO::getInput() {
    EInput input = this->m_input;
    this->m_input = EInput::none;
    return input;
}
void ESPIO::clearScreen() {
    for (int x = 0; x < 20; x++) {
        for (int y = 0; y < 4; y++) {
            m_display[x][y] = ' ';
        }
    }
}

uint8_t ESPIO::compareAndCopyDisplayBuffers() {
    uint8_t toUpdate = 0;
    for (int x = 0; x < 20; x++) {
        for (int y = 0; y < 4; y++) {
            if (m_display[x][y] != m_lastDisplay[x][y])
                toUpdate |= 1 << y;
            m_lastDisplay[x][y] = m_display[x][y];
        }
    }
    return toUpdate;
}

std::string ESPIO::intToUpperNum(int input) {
    std::string stringified = std::to_string(input);
    std::replace(stringified.begin(), stringified.end(), '0', (char)0x08);
    std::replace(stringified.begin(), stringified.end(), '1', (char)0x09);
    std::replace(stringified.begin(), stringified.end(), '2', (char)0x1e);
    std::replace(stringified.begin(), stringified.end(), '3', (char)0x1f);
    std::replace(stringified.begin(), stringified.end(), '4', (char)0x0a);
    std::replace(stringified.begin(), stringified.end(), '5', (char)0x0b);
    std::replace(stringified.begin(), stringified.end(), '6', (char)0x0c);
    std::replace(stringified.begin(), stringified.end(), '7', (char)0x0d);
    std::replace(stringified.begin(), stringified.end(), '8', (char)0x0e);
    std::replace(stringified.begin(), stringified.end(), '9', (char)0x0f);
    return stringified;
}
int ESPIO::upperNumToInt(std::string input) {
    std::string inputCopy = input;
    std::replace(inputCopy.begin(), inputCopy.end(), (char)0x08, '0');
    std::replace(inputCopy.begin(), inputCopy.end(), (char)0x09, '1');
    std::replace(inputCopy.begin(), inputCopy.end(), (char)0x1e, '2');
    std::replace(inputCopy.begin(), inputCopy.end(), (char)0x1f, '3');
    std::replace(inputCopy.begin(), inputCopy.end(), (char)0x0a, '4');
    std::replace(inputCopy.begin(), inputCopy.end(), (char)0x0b, '5');
    std::replace(inputCopy.begin(), inputCopy.end(), (char)0x0c, '6');
    std::replace(inputCopy.begin(), inputCopy.end(), (char)0x0d, '7');
    std::replace(inputCopy.begin(), inputCopy.end(), (char)0x0e, '8');
    std::replace(inputCopy.begin(), inputCopy.end(), (char)0x0f, '9');
    return std::stoi(inputCopy);
}
bool ESPIO::isUpperNum(char ch) {
    return (ch >= 0x08 && ch <= 0x0f) || ch == 0x1e || ch == 0x1f;
}