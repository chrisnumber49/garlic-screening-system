/* Compile with: gcc -o GPIO GPIO.c -l bcm2835   */

#include <bcm2835.h>
#include <stdio.h>


/* Blinks on RPi pin 11 */
#define PIN1 RPI_GPIO_P1_11
#define PIN2 RPI_GPIO_P1_22
#define PIN3 RPI_GPIO_P1_15
#define PIN4 RPI_GPIO_P1_12
#define PIN5 RPI_GPIO_P1_16
#define PIN6 RPI_GPIO_P1_18

int main(int argc, char **argv) {
	
	if(!bcm2835_init()){
		printf("error");
		return 1;
	}

        /* Set the pin to be an output */
        bcm2835_gpio_fsel(PIN1, BCM2835_GPIO_FSEL_OUTP);
        bcm2835_gpio_fsel(PIN2, BCM2835_GPIO_FSEL_OUTP);
        bcm2835_gpio_fsel(PIN3, BCM2835_GPIO_FSEL_OUTP);
        bcm2835_gpio_fsel(PIN4, BCM2835_GPIO_FSEL_OUTP);
        bcm2835_gpio_fsel(PIN5, BCM2835_GPIO_FSEL_OUTP);
        bcm2835_gpio_fsel(PIN6, BCM2835_GPIO_FSEL_OUTP);

        /* Blink LED */
        while (1) {
                bcm2835_gpio_write(PIN1, HIGH);
                delay(500);
                bcm2835_gpio_write(PIN2, HIGH);
                delay(500);
                bcm2835_gpio_write(PIN3, HIGH);
                delay(500);
                bcm2835_gpio_write(PIN4, HIGH);
                delay(500);
                bcm2835_gpio_write(PIN5, HIGH);
                delay(500);
                bcm2835_gpio_write(PIN6, HIGH);
                delay(500);
                
                bcm2835_gpio_write(PIN1, LOW);
                delay(500);
                bcm2835_gpio_write(PIN2, LOW);
                delay(500);
                bcm2835_gpio_write(PIN3, LOW);
                delay(500);
                bcm2835_gpio_write(PIN4, LOW);
                delay(500);
                bcm2835_gpio_write(PIN5, LOW);
                delay(500);
                bcm2835_gpio_write(PIN6, LOW);
                delay(500);
        }
	bcm2835_close();
        return 0;
}