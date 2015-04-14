main(){
	char line[80];
makeInterrupt21();
interrupt(0x21,1,line,0,0);
interrupt(0x21,0,line,0,0);


	while(1 == 1){	
	}
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
	while(size>0){
		interrupt(0x10, (0xE*256)+0x8, 0, 0, 0);
		size--;
	}
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
	else {
	printString("Error \0");
	}
}
