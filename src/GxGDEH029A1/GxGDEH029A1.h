// class GxGDEH029A1 : Display class for GDEH029A1 e-Paper from Dalian Good Display Co., Ltd.: www.good-display.com
//
// based on Demo Example from Good Display, available here: http://www.good-display.com/download_detail/downloadsId=515.html
// Controller: IL3820 : http://www.good-display.com/download_detail/downloadsId=540.html
//
// Author : J-M Zingg
//
// Version : see library.properties
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// Library: https://github.com/ZinggJM/GxEPD

#ifndef _GxGDEH029A1_H_
#define _GxGDEH029A1_H_

#include "../GxEPD.h"


// the physical number of pixels (for controller parameter)
#define GxGDEH029A1_X_PIXELS 128
#define GxGDEH029A1_Y_PIXELS 296

// the logical width and height of the display
#define GxGDEH029A1_WIDTH GxGDEH029A1_X_PIXELS
#define GxGDEH029A1_HEIGHT GxGDEH029A1_Y_PIXELS

#define GxGDEH029A1_BUFFER_SIZE (uint32_t(GxGDEH029A1_WIDTH) * uint32_t(GxGDEH029A1_HEIGHT) / 8)

// divisor for AVR, should be factor of GxGDEH029A1_HEIGHT
#define GxGDEH029A1_PAGES 4

#define GxGDEH029A1_PAGE_HEIGHT (GxGDEH029A1_HEIGHT / GxGDEH029A1_PAGES)
#define GxGDEH029A1_PAGE_SIZE (GxGDEH029A1_BUFFER_SIZE / GxGDEH029A1_PAGES)

class GxGDEH029A1 : public GxEPD
{
  public:
#if defined(ESP8266)
    //GxGDEH029A1(GxIO& io, int8_t rst = D4, int8_t busy = D2);
    // use pin numbers, other ESP8266 than Wemos may not use Dx names
    GxGDEH029A1(GxIO& io, int8_t rst = 2, int8_t busy = 4);
#else
    GxGDEH029A1(GxIO& io, int8_t rst = 9, int8_t busy = 7);
#endif

    /**
     * @brief Draw a pixel (in the buffer) at the coordinate X/Y with a color code
     * 
     * Only Black and white are available
     * 
     * @param x x coordinate
     * @param y y coordinate
     * @param color 
     */
    void drawPixel(int16_t x, int16_t y, uint16_t color);

    /**
     * @brief Init the screen hardware and fill the buffer in White
     * 
     * @param serial_diag_bitrate 
     */
    void init(uint32_t serial_diag_bitrate = 0); // = 0 : disabled
    
    /**
     * @brief Fill the buffer screen with a black or white color 
     * 
     * @param color 
     */
    void fillScreen(uint16_t color = GxEPD_WHITE); // 0x0 black, >0x0 white, to buffer
    
    /**
     * @brief Update the whole screen from the buffer
     * 
     */
    bool update(void);

    // to buffer, may be cropped, drawPixel() used, update needed
    void  drawBitmap(const uint8_t *bitmap, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, int16_t mode = bm_normal);
    // to full screen, filled with white if size is less, no update needed
    void drawBitmap(const uint8_t *bitmap, uint32_t size, int16_t mode = bm_normal); // only bm_normal, bm_invert, bm_partial_update modes implemented
    void eraseDisplay(bool using_partial_update = false);

    /**
     * @brief partial update of rectangle from buffer to screen, does not power off
     * 
     * @param x X coordonnate of the top left corner
     * @param y Y coordonnate of the top left corner
     * @param w width of the partial update
     * @param h height of the partial update
     * @param using_rotation (true by default) 
     * @return true 
     * @return false 
     */
    bool updateWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool using_rotation = true);
    
    
    // partial update of rectangle at (xs,ys) from buffer to screen at (xd,yd), does not power off
    void updateToWindow(uint16_t xs, uint16_t ys, uint16_t xd, uint16_t yd, uint16_t w, uint16_t h, bool using_rotation = true);
    
    // terminate cleanly updateWindow or updateToWindow before removing power or long delays
    void powerDown();
    
    // paged drawing, for limited RAM, drawCallback() is called GxGDEH029A1_PAGES times
    // each call of drawCallback() should draw the same
    void drawPaged(void (*drawCallback)(void));
    void drawPaged(void (*drawCallback)(uint32_t), uint32_t);
    void drawPaged(void (*drawCallback)(const void*), const void*);
    void drawPaged(void (*drawCallback)(const void*, const void*), const void*, const void*);
    
    // paged drawing to screen rectangle at (x,y) using partial update
    void drawPagedToWindow(void (*drawCallback)(void), uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void drawPagedToWindow(void (*drawCallback)(uint32_t), uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t);
    void drawPagedToWindow(void (*drawCallback)(const void*), uint16_t x, uint16_t y, uint16_t w, uint16_t h, const void*);
    void drawPagedToWindow(void (*drawCallback)(const void*, const void*), uint16_t x, uint16_t y, uint16_t w, uint16_t h, const void*, const void*);
    void drawCornerTest(uint8_t em = 0x01);
  private:
    template <typename T> static inline void
    swap(T& a, T& b)
    {
      T t = a;
      a = b;
      b = t;
    }
    void _writeToWindow(uint16_t xs, uint16_t ys, uint16_t xd, uint16_t yd, uint16_t w, uint16_t h);
    void _writeData(uint8_t data);
    void _writeCommand(uint8_t command);
    void _writeCommandData(const uint8_t* pCommandData, uint8_t datalen);
    void _SetRamPointer(uint8_t addrX, uint8_t addrY, uint8_t addrY1);
    void _SetRamArea(uint8_t Xstart, uint8_t Xend, uint8_t Ystart, uint8_t Ystart1, uint8_t Yend, uint8_t Yend1);
    void _PowerOn(void);
    void _PowerOff(void);
    void _waitWhileBusy(const char* comment = 0);
    void _setRamDataEntryMode(uint8_t em);
    void _InitDisplay(uint8_t em);
    void _Init_Full(uint8_t em);
    void _Init_Part(uint8_t em);
    void _Update_Full(void);
    void _Update_Part(void);
    void _rotate(uint16_t& x, uint16_t& y, uint16_t& w, uint16_t& h);
  protected:
#if defined(__AVR)
    uint8_t _buffer[GxGDEH029A1_PAGE_SIZE];
#else
    uint8_t _buffer[GxGDEH029A1_BUFFER_SIZE];
#endif
  private:
    GxIO& IO;
    int16_t _current_page;
    bool _using_partial_mode;
    bool _diag_enabled;
    int8_t _rst;
    int8_t _busy;
    static const uint8_t LUTDefault_full[];
    static const uint8_t LUTDefault_part[];
    static const uint8_t GDOControl[];
    static const uint8_t softstart[];
    static const uint8_t VCOMVol[];
    static const uint8_t DummyLine[];
    static const uint8_t Gatetime[];
#if defined(ESP8266) || defined(ESP32)
  public:
    // the compiler of these packages has a problem with signature matching to base classes
    void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color)
    {
      Adafruit_GFX::drawBitmap(x, y, bitmap, w, h, color);
    };
#endif
};

#ifndef GxEPD_Class
#define GxEPD_Class GxGDEH029A1
#define GxEPD_WIDTH GxGDEH029A1_WIDTH
#define GxEPD_HEIGHT GxGDEH029A1_HEIGHT
#define GxEPD_BitmapExamples <GxGDEH029A1/BitmapExamples.h>
#define GxEPD_BitmapExamplesQ "GxGDEH029A1/BitmapExamples.h"
#endif

#endif

