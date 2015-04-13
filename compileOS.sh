cd //home/agent47/Codes/osproject/M1
echo 'went to the write drive'
echo 'compiling The c Kernel File kernel.c'
nasm bootload.asm
bcc -ansi -c -o kernel.o kernel.c

echo 'done compiling'
echo '-------------------------'

echo 'assembling kernel file'
as86 kernel.asm -o kernel_asm.o
ld86 -o kernel -d kernel.o kernel_asm.o
echo 'done assembling'
echo '-------------------------'
echo 'copying to floppya.img setor 3'
dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3
dd if=message.txt of=floppya.img bs=512 count=1 seek=30 conv=notrunc
echo 'done copying :D ur os is now ready to print hello world :D'

