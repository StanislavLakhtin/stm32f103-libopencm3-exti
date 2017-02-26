//
// Created by sl on 26.02.17.
//

#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/f1/nvic.h>
#include <stdio.h>
#include <errno.h>

static void clock_setup(void) {
  rcc_clock_setup_in_hse_8mhz_out_72mhz();

  rcc_periph_clock_enable(RCC_GPIOB); // btns
  rcc_periph_clock_enable(RCC_GPIOC); // led

  rcc_periph_clock_enable(RCC_USART2); //debug
}

int _write(int file, char *ptr, int len) {
  int i;

  if (file == 1) {
    for (i = 0; i < len; i++)
      usart_send_blocking(USART2, ptr[i]);
    return i;
  }
  errno = EIO;
  return -1;
}

static void gpio_setup(void) {
  gpio_set_mode(GPIOB, GPIO_MODE_INPUT,
                GPIO_CNF_INPUT_PULL_UPDOWN, GPIO8 | GPIO9);

  gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
                GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
}

static void usart_setup(void) {
  gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
                GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART2_TX);
  gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
                GPIO_CNF_INPUT_PULL_UPDOWN, GPIO_USART2_RX);

  usart_set_baudrate(USART2, 9600);
  usart_set_databits(USART2, 8);
  usart_set_stopbits(USART2, USART_STOPBITS_1);
  usart_set_mode(USART2, USART_MODE_TX_RX);
  usart_set_parity(USART2, USART_PARITY_NONE);
  usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);
  usart_enable(USART2);
}

int main(void) {
  clock_setup();
  usart_setup();
  gpio_setup();

  wchar_t buffer[50];
  printf("\nUSART test");

  while(1) {
    gpio_toggle(GPIOC, GPIO13);
  }
}
