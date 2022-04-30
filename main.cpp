#include <stdio.h>
#include <stdlib.h>
#include <String.h>

char base[] = "[HKEY_CLASSES_ROOT\\Directory\\Background\\shell\\";
bool f = false; // 空格检查标志 
int baseSize = 50;

char* getAdress(){
	char* address = (char *)malloc(300 * sizeof(char));
	memset(address,'\0',300*sizeof(char));
	fflush(stdin);
	scanf("%[^\n]",address);
	return address;
}

char* getProgramName(char* path){
	char* name = (char*)malloc(20*sizeof(char));
	memset(name,'\0',20);
	int len = strlen(path);
	int index = len-1;
	// 空格检测 
	for(int i=0;i<index;i++){
		if(path[i] == ' '){
			f = true;
			break;
		}
	}
	
	while(index >= 0 && path[index] != '\\'){
		index--;
	}
	
	int idx = 0;
	int end = f ? len-5 : len - 4;
	for(int i=index+1;i<end;i++){
		name[idx++] = path[i];
	}
	name[idx] = '\0';
	return name;
}

char* myStrCat(char* c[]){
	int idx = 0;
	int len = 0;
	while(c[idx] != NULL){
		len += strlen(c[idx++]);
	}
	len+=10;
	idx = 0;
//	printf("len:%d\n",len);
	char* ret = (char *)malloc(len*sizeof(char));
	memset(ret,'\0',len);
	while(c[idx] != NULL){
		strcat(ret,c[idx++]);
	}
//	printf("%s\n",ret);
	return ret;
}

char* fillChar(char* chars,char c){
	int len = strlen(chars) + 20;
	char* newStr = (char*)malloc(sizeof(char)*len);
	memset(newStr,'\0',len);
	int idx = 0;
	for(int i=0;i<len-20;i++){
		if(chars[i] == '\\'){
			newStr[idx++] = c;
		}
		newStr[idx++] = chars[i];
	}
	newStr[idx] = '\0';
	return newStr;
} 

void process(){
	
	while(1){
		
		printf("\ninput file path:\n"); 
	
		f = false;
		char *address = getAdress();
		int len =  strlen(address);
		if(len == 0){
			free(address);
			continue;
		}
		char* name = getProgramName(address);
		
		// first line 
		char* f_l[baseSize] = {base,name,"]\n"};
		char* first_line = myStrCat(f_l);
		
		
		// sec line 
		char* sec_line;
		if(f){
			char* s_l[baseSize] = {"\"Icon\"=", fillChar(address,'\\') ,"\n"} ;
			sec_line = myStrCat(s_l); 
		}else{
			char* s_l[baseSize] = {"\"Icon\"=","\"", fillChar(address,'\\') ,"\"\n"};
			sec_line = myStrCat(s_l); 
		}
		
		// thrd line
		char* t_l[baseSize] = {base,name,"\\command]\n"};
		char* thrd_line = myStrCat(t_l);
		
		char* end;
		if(f){
			char* e_l[baseSize] =  {"@=",fillChar(address,'\\')};
			end = myStrCat(e_l);
			
		}else{
			char* e_l[baseSize] =  {"@=\"",fillChar(address,'\\'),"\""};
			end = myStrCat(e_l);
		
		}
		
		char* fileName[baseSize] = {name,".reg"};
		FILE* f = fopen(myStrCat(fileName),"w");
		fputs("Windows Registry Editor Version 5.00\n",f);
		fputs(first_line,f);
		fputs(sec_line,f);
		fputs(thrd_line,f);
		fputs(end,f);
		fclose(f);
		
		printf("SUCCESS...\n");
	}
}

int main(){

	printf("所有添加的项存在于(注册表)regedit的下列路径中，可自行修改:\n"); 
	printf("计算机\\HKEY_CLASSES_ROOT\\Directory\\Background\\shell\n\n");
	printf("将要添加到右键的exe文件(不是快捷方式！)拖入窗口回车，看到SUCCESS字样即表示成功\n"); 
	printf("双击生成的reg后缀文件添加即可。\n");
	process();
	return 0;
} 
