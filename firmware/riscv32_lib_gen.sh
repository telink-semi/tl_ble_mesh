rm -f liblt_9518.a
find ./algorithm -name "*.o" -type f -print -exec riscv32-elf-ar -crs liblt_9518.a  {} \;
find ./drivers/B91/ext_driver -name "ext_gpio.o" -type f -print -exec riscv32-elf-ar -crs liblt_9518.a  {} \;
find ./drivers/B91/ext_driver -name "ext_misc.o" -type f -print -exec riscv32-elf-ar -crs liblt_9518.a  {} \;
find ./drivers/B91/ext_driver -name "ext_pm.o" -type f -print -exec riscv32-elf-ar -crs liblt_9518.a  {} \;
find ./drivers/B91/ext_driver -name "ext_rf.o" -type f -print -exec riscv32-elf-ar -crs liblt_9518.a  {} \;
find ./drivers/B91 -name "plic.o" -type f -print -exec riscv32-elf-ar -crs liblt_9518.a  {} \;
find ./drivers/B91 -name "pm.o" -type f -print -exec riscv32-elf-ar -crs liblt_9518.a  {} \;
find ./drivers/B91 -name "pm_32k_rc.o" -type f -print -exec riscv32-elf-ar -crs liblt_9518.a  {} \;
find ./drivers/B91 -name "pm_32k_xtal.o" -type f -print -exec riscv32-elf-ar -crs liblt_9518.a  {} \;
find ./drivers/B91 -name "rf.o" -type f -print -exec riscv32-elf-ar -crs liblt_9518.a  {} \;
find ./drivers/B91 -name "sys.o" -type f -print -exec riscv32-elf-ar -crs liblt_9518.a  {} \;
find ./drivers/B91 -name "trng.o" -type f -print -exec riscv32-elf-ar -crs liblt_9518.a  {} \;
find ./drivers/B91 -name "emi.o" -type f -print -exec riscv32-elf-ar -crs liblt_9518.a  {} \;
find ./stack -name "*.o" -type f -print -exec riscv32-elf-ar -crs liblt_9518.a  {} \;