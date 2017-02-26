//
// Created by sl on 26.02.17.
//


#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/f1/nvic.h>
#include <stdio.h>
#include <errno.h>

// Реализуется простая схема с входом на каждую тактильную кнопку
// PB0 используется как общий индикатор прерывания
// PB8 и PB9 -- как входы для кнопок

static void clock_setup(void) {
  rcc_clock_setup_in_hse_8mhz_out_72mhz();

  rcc_periph_clock_enable(RCC_GPIOB); // btns
  rcc_periph_clock_enable(RCC_GPIOC); // led

  rcc_periph_clock_enable(RCC_AFIO);

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

void exti0_isr(void)
{
  gpio_toggle(GPIOC, GPIO13);
  exti_reset_request(EXTI0); // нам следует вручную очистить причину вызова прерывания
}

static void gpio_setup(void) {
  // BTNS
  gpio_set_mode(GPIOB, GPIO_MODE_INPUT,
                GPIO_CNF_INPUT_PULL_UPDOWN, GPIO0 | GPIO8 | GPIO9);

  // BTNS END

  // LED
  gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ,
                GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);

  AFIO_MAPR |= AFIO_MAPR_SWJ_CFG_FULL_SWJ_NO_JNTRST;

  gpio_set(GPIOC, GPIO13);
  // LED END
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

static void button_setup(void) {
  /* use NVIC_EXTI0_IRQ to control button */

  /* Enable NVIC */
  nvic_enable_irq(NVIC_EXTI0_IRQ);
  exti_enable_request(EXTI0);
  exti_set_trigger(EXTI0, EXTI_TRIGGER_RISING);
  exti_select_source(EXTI0,GPIOB);
}

int main(void) {
  clock_setup();
  gpio_setup();
  button_setup();
  usart_setup();

  wchar_t buffer[50];
  gpio_set(GPIOC, GPIO13);
  printf("\nUSART test");

  while(1) {

  }
}
