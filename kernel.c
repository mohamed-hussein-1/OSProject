main(){
	char buffer[13312]; /*this is the maximum size of a file*/
	makeInterrupt21();
	interrupt(0x21, 3, "messag\0", buffer, 0); /*read the file into buffer*/
	interrupt(0x21, 0, buffer, 0, 0); /*print out the file*/
	while(1){
	}
// 	char line[80];
// makeInterrupt21();
// interrupt(0x21,1,line,0,0);
// interrupt(0x21,0,line,0,0);
	// int m ;
	// char x1[2];
	// char x2[2];
	// char yes[2];
	// char no[2];
	// x1[0] = 'a';
	// x1[1] = 'b';
	// x2[0] = 'a';
	// x2[1] = 'b';
	// yes[0] = '1';
	// yes[1] = '\0';
	// no[0] = '0';
	// no[1] = '\0';
	// m = charEqual(x1,x2,2);
	// if (m == 1){
	// 	printString(yes);
	// }else if (m == 0){
	// 	printString(no);
	// }
	// else {
	// 	printString(no);	
	// }
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
	else if(ax == 3){
		readFile(bx,cx);
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