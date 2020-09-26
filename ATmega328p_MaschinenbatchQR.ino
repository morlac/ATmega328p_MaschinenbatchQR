/**
 * Author: Christian Adams <morlac@morlac.de>
 *
 * A simple demo to output a slightly variable QR-Code ("https://maschinendeck.org/?" + <some random string generated on every loop>)
 * on PE12864 displays salvaged from old Hypercom POS terminals
 */


#define QR_VERSION 8

#define LOCK_VERSION 8

#include <qrcode.h>

#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#define DISPLAY_RST   8
#define DISPLAY_CS    10
#define DISPLAY_DC    9
// initialize U8g2 library with hardware SPI module
// SCK (CLK) ----> Arduino pin D13
// MOSI(DIN) ----> Arduino pin D12

U8G2_ST7565_PE12864_004_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ DISPLAY_CS, /* dc=*/ DISPLAY_DC, /* reset=*/ DISPLAY_RST);

QRCode maschinenQR;

#define QR_BUFFER_SIZE_1 (4 * QR_VERSION + 17)
#define QR_BUFFER_SIZE ((QR_BUFFER_SIZE_1 * QR_BUFFER_SIZE_1) + 7) / 8

static uint8_t qrcodeData[QR_BUFFER_SIZE];

/**
 *
 */
void setup() {
  u8g2.begin();

  u8g2.enableUTF8Print();
}

/**
 *
 */
void loop() {
  uint32_t m = millis();

  qrcode_initText(&maschinenQR, qrcodeData, QR_VERSION, ECC_HIGH, String(String(F("https://maschinendeck.org/?")) + String(m)).c_str());

  u8g2.firstPage();

  do {
    for (uint8_t y = 0; y < maschinenQR.size; y++) {
      // Each horizontal module
      for (uint8_t x = 0; x < maschinenQR.size; x++) {
        if (qrcode_getModule(&maschinenQR, x, y)) {
          u8g2.drawPixel(1 + x, 1 + y);
        }
      }
    }
  } while (u8g2.nextPage());

  delay(1000);
}

