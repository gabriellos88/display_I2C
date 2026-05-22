# display_I2C

Progetto per comandare un display OLED SSD1306 128x64 via I2C con il
PIC18F45K22 

Il codice originale è stato preso da un post del forum
MikroE e poi ristrutturato con solo le funzioni necessarie.


## Struttura
...

## Collegamenti

| Display OLED | PIC18F45K22 |
| --- | --- |
| SCL/SCK | RC3, pin 18 |
| SDA | RC4, pin 23 |
| RST | RC0 |
| VCC | Alimentazione |
| GND | GND |


## Funzioni principali

```c
display_clear();
display_pixel(10, 5, DISPLAY_COLOR_ON);
display_pixel(10, 5, DISPLAY_COLOR_OFF);
display_pixel(10, 5, DISPLAY_COLOR_INVERT);
display_line(0, 0, 127, 31, DISPLAY_COLOR_ON);
display_rect(0, 0, 128, 32, DISPLAY_COLOR_ON);
display_update();
```

Attenzione che il display viene
aggiornato solo quando si chiama `display_update()`.

## HAL I2C
...

All'inizio il codice disegna un bordo, due diagonali e una griglia di pixel.
