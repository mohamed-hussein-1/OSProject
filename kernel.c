main(){
	 // char buffer[13312];
	// makeInterrupt21();
	// interrupt(0x21, 7, "messag\0", 0, 0); //delete messag
	// interrupt(0x21, 3, "messag\0", buffer, 0); // try to read messag
	// interrupt(0x21, 0, buffer, 0, 0); //print out the contents of buffer
	// runShell();
	// dir();
	// char testim[512];
	// writeFile("testim\0" , "the hardest\0" , 1);
	// readFile("testim" , testim);
	// printString(testim);
	// int i=0;
	// char buffer1[13312];
	// char buffer2[13312];
	// buffer2[0]='h'; buffer2[1]='e'; buffer2[2]='l'; buffer2[3]='l';
	// buffer2[4]='o';
	// for(i=5; i<13312; i++) buffer2[i]=0x0;
	// makeInterrupt21();
	// interrupt(0x21,8, "testW\0", buffer2, 1); //write file testW
	// interrupt(0x21,3, "testW\0", buffer1, 0); //read file testW
	// interrupt(0x21,0, buffer1, 0, 0); // print out contents of testW
	dir();
	 // runShell();
	// readFile("messag\0" , buffer);
	// printString(buffer);
	// deleteFile("messag\0");
	// readFile("messag\0" , buffer);
	// printString(buffer);

	while (1){

	}	
	
	// while(1){

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
	else if(ax == 6){
		writeSector(bx,cx);
	}
	else if(ax == 7){
		deleteFile(bx);
	}
	else if(ax == 8){
		writeFile(bx , cx , dx);
	}
	else if(ax == 9){
		dir();
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

// readFile(char* fileName,char* fileToBeRead){
// 	char dirSector[512];
// 	int isEqual = 3;
// 	int 
// 	int i = 0;
// 	int j = 0;

// 	readSector(dirSector , 2);
// 	while(i<512){
// 		while(j<6){
// 			if(dirSector[i+j] != fileName[j]){

// 			}
// 		}
// 	}


charEqual2(char* x , char* y , int l){
	while(l>0){
		if(x[l]!= y[l]){
			return 3;
		}
	}
	return 1;
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

writeSector(char* arr , int sector){
	int rel_sec;
	int head;
	int track;
	rel_sec = mod(sector,18) +1;
	head = mod(divide(sector,18),2);
	track = divide(sector,36);
	interrupt(0x13, (3*256)+1,arr, (track*256)+rel_sec,(head*256)+0);
}

deleteFile(char* name){
	char map[512];
	char directory[512];
	int positionInDirectory;
	int i = 0;
	int j = 0;
	int found = 0;
	int flag = 0;

	readSector(map , 1);
	readSector(directory , 2);

	while(i<512){
		while(j<6){
			if(directory[i+j] != name[j]){
				i += 32;
				found = 0;
				break;
			}
			j++;
			found++;
		}
		j = 0;
		if(found == 6){
			positionInDirectory = i;
			directory[i] = 0x00;
			flag = 1;
		}
	}
	if(flag == 1){
		writeSector(directory , 2);
		positionInDirectory += 6 ;

		while(directory[positionInDirectory] != 0x00){
			map[positionInDirectory+1] = 0x00;
			positionInDirectory++;
		}

		 writeSector(map ,1);
	}
	else{
		printString("name of file not found .. better luck next time :D \0");
	}

}

dir(){
	char directory[512];
	char name[50];
	int j = 0;
	int i = 0;
	int flag = 0;
	int num = 0;
	int found = 0;
	int x = 11;

	name[6] = ' ';
	name[7] = ' ';
	name[8] = ' ';
	name[9] = ' ';

	while(x<50){
		name[x] = ' ';
		x++;
	}
	readSector(directory ,2);
	while(i<512){
		if (directory[i] != 0x00){
			while(j<6){
			name[j] = directory[i+j];
			j++;
			}
			while(directory[i+j] != 0x00){
				num ++;
				j++;
				flag = 1;
			}
			found = 1;
		}
		i += 32;
		j = 0;
		if (flag == 1){
			name[11] = mod(num , 10)+48;
			name[10] = divide(num , 10)+48;
		}
		if(found == 1){
		printString(name);
		interrupt(0x10,(0xE*256)+0xd,0,0,0);
		}
		num = 0;
		flag = 0;
		found = 0;
	}
}

writeFile(char* name, char* buffer,int secNum){
	char map[512];
	char directory[512];
	int i = 0;
	int ij = 0;
	int j = 0;
	readSector(map,1);
	readSector(directory,2);
	while(i<16){
		if(directory[ij] == 0x00){
			break;
		}
		i++;
		ij+=32;
	}
	if (i == 16){
		printString("maximumnumber of files \0");
		return;
	}
	i = 0;
	while(i<6){
		if (name[i] == '\0'){
			break;
		}
		directory[ij] = name[i];
		++ij;
		++i;
	}
	while(i<6){
		directory[ij] = 0x00;
		++ij;
		++i;
	}
	i = 0;
	while(i<secNum){
		j = 0;
		while(j < 256){
			if (map[j] == 0x00){
				map[j] = 0xff;
				writeSector(buffer,j);
				directory[ij+i] = j;
				j = 900;
				buffer += 512;
			}
			j++;
		}
		i++;
	}
	while(i<26){
		map[ij+i] = 0x00;
		++i;
	}
	writeSector(map,1);
	writeSector(directory,2);
}