main(){
	runShell();
}
printString(char* arr){
	while(*arr != '\0'){
		interrupt(0x10, (0xE*256)+(*arr), 0, 0, 0);
		++arr;
	}
}
readString(char* arr){
	int size = 0;
	while(*arr != 0xd){
		*arr = (char) interrupt(0x16,0,0,0,0);
		if (*arr == 0x8){
			if (size > 0){
				interrupt(0x10, (0xE*256)+0x8, 0, 0, 0);
				interrupt(0x10, (0xE*256), 0, 0, 0);
				interrupt(0x10, (0xE*256)+0x8, 0, 0, 0);
				--arr;
				--size;
				*arr = '\0';
			}
		}
		else if (*arr != 0xd ){
		interrupt(0x10, (0xE*256)+(*arr), 0, 0, 0);
		++arr;
		++size;
		}
	}
	interrupt(0x10,(0xE*256)+0xd,0,0,0);
	interrupt(0x10, (0xE*256)+0xa, 0, 0, 0);
	*arr = 0xa;
	++arr;
	*arr = 0x0;
}
int mult(int x , int y){
	int result = 0;
	while(y > 0){
		result += x;
		y--;
	}
	return result ;
}

int divide(int num, int denom)
{
  int q = 0;
  while(num >= denom)
  {
      num -= denom;
      q++;
  }
  
  return q;
}

int mod(int x , int y){
	int div = divide(x,y);
	return x - (mult(div,y));
}

readSector(char* arr , int sector){
	int rel_sec;
	int head;
	int track;
	rel_sec = mod(sector,18) +1;
	head = mod(divide(sector,18),2);
	track = divide(sector,36);
	interrupt(0x13, (2*256)+1,arr, (track*256)+rel_sec,(head*256)+0);
}

void handleInterrupt21(int ax, int bx, int cx, int dx){
	if(ax == 0){
		printString(bx);
	}
	else if(ax == 1){
		readString(bx);
	}
	else if(ax == 2){
		readSector(bx , cx);
	}
	else if(ax == 3){
		readFile(bx,cx);
	}
	else if(ax == 4){
		executeProgram(bx , cx);
		terminate();
	}
	else if(ax == 5){
		terminate();
	}
	else {
	printString("Error \0");
	}
}
readFile(char* fileName,char* fileToBeRead){
	char dirSector[512];
	char* dirSectorP = dirSector;
	int isEqual = 33;
	int i = 0;

	readSector(dirSector,2);
	while(i < 16){
		isEqual = charEqual(dirSectorP,fileName,6);
		if (isEqual == 1){
			dirSectorP = dirSectorP+6;
			readThisFile(dirSectorP,fileToBeRead);
			break;
		}
		dirSectorP = dirSectorP+32;
		++i;
	}
	return;
}
int charEqual(char* x,char* y,int size){
	int i = 0;
	int eq = 1;
	while (i < size){
		if(*x==*y){
			if(*x == '\0'){
				break;
			}
		}
		else {
			eq = 0;
			break;
		}
		++x;
		++y;
		++i;
	}
	return eq;
}
readThisFile(char* secNum,char* fileToBeRead){
	int i = 0;
	int secAsInt;
	while(i<26){
		secAsInt = *secNum;	
		if (secAsInt == 0){
			break;
		}
		readSector(fileToBeRead,secAsInt);
		fileToBeRead += 512;
		secNum = secNum+1;
		++i;
	}
}

executeProgram(char* name, int segment){
	int i ;
	char buffer[512];
	readFile(name , buffer);
	i = 0;
	while(i<512){
		putInMemory(segment , i , buffer[i]);
		i++;
	}
	launchProgram(segment);
}

terminate(){
	char shell[6];
	shell[0] = 's';
	shell[1] = 'h';
	shell[2] = 'e';
	shell[3] = 'l';
	shell[4] = 'l';
	shell[5] = 0x0;
	interrupt(0x21, 4, shell, 0x2000, 0);
}
mile3test1(){
	char buffer[13312]; /*this is the maximum size of a file*/
	makeInterrupt21();
	interrupt(0x21, 3, "messag\0", buffer, 0); /*read the file into buffer*/
	interrupt(0x21, 0, buffer, 0, 0); /*print out the file*/
	while(1){
	}
	// after compiling run: ./loadFile message.txt

}
mile3test2(){
	makeInterrupt21();
	interrupt(0x21, 4, "tstprg\0", 0x2000, 0);
	while(1){

	}
	//after compiling run: ./loadfile tstprg

}
mile3test3(){
	makeInterrupt21();
	interrupt(0x21, 5, 0, 0x2000, 0);
	//after compiling run: ./loadfile tstpr2
}
mile3test4(){
	makeInterrupt21();
	interrupt(0x21,4,"shell\0",0x2000,0);
}
runShell(){
	mile3test4();
	return ;
}
