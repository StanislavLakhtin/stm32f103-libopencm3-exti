set target-async on
set confirm off
set history save
set mem inaccessible-by-default off
target extended-remote /dev/ttyACM0
mon swdp_scan
att 1
load ./build/stm32-f103-exti.elf
file ./build/stm32-f103-exti.elf
start
