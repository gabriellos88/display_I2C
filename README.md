# display I2C OLED 128x64

Progetto per comandare un display OLED SSD1306 128x64 via I2C con il PIC18F45K22 

Il codice originale è stato preso da un post del [Forum MikroE](https://forum.mikroe.com/viewtopic.php?p=310697)
che è stato ristrutturato con solo le funzioni necessarie.

Inoltre in [Hadrware](Hardware/OLED_mikrobus_adapter) ho aggiunto un prototipo di adattatore tra il 
mikro bus e il display con TP e selezione VDD (5V o 3.3V).

## Struttura
...

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
Attenzione: il display viene aggiornato SOLO quando si chiama `display_update()`.

All'inizio il codice disegna un bordo, due diagonali e una griglia di pixel.
