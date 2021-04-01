/*
*
*
*/

#ifndef __RGB_LED_DRIVER_H__
    #define __RGB_LED_DRIVER_H__
    
    #include "project.h"
    
    /**
    *   \brief Struct holding color data.
    */
    typedef struct{
        uint8_t red;    // Red value (0-255)
        uint8_t green;  // Green value (0-255)
        uint8_t blue;   // Blue value (0-255)
    }Color;
    
    /**
    *   \brief Start driver.
    */
    void RGBLed_Start(void);
    
    /**
    *   \brief Stop driver.
    */
    void RGBLed_Stop(void);
    
    /**
    *   \brief Write a new color to the RGB Led.
    *
    *   \param c the new color to be written.
    */
    void RGBLed_WriteColor(Color c);
    
#endif

/* [] END OF FILE */
