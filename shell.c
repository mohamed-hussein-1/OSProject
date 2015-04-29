main(){
	char command[512];
	char buffer[13312];
	while(1){
		// makeInterrupt21();
		buffer[0] = '\0';
		interrupt(0x21, 0, "A: \0", 0, 0); /*print A:*/
		
		interrupt(0x21,1,command,0,0); /*takes input*/

		if (charEqual("view", command,4) == 1){
			interrupt(0x21,3,command+5,buffer,0);
			interrupt(0x21,0,buffer,0,0);
		}
		else if (charEqual("excute", command,6) == 1){
			interrupt(0x21,4,command+7,0x2000,0);
		}
		else if (charEqual("delete", command,6) == 1){
			interrupt(0x21,7,command+7,0,0);
		}
		else if (charEqual("dir", command,3) == 1){
			interrupt(0x21,9,0,0,0);
		}
		else {
			interrupt(0x21,0,"bad command\n\r\0",0,0);
		}
	}
}
//add this to run ./loadfile shell
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
