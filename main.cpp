#include <stdio.h>
#include <stdlib.h>
#include <String.h>

char base[] = "[HKEY_CLASSES_ROOT\\Directory\\Background\\shell\\";
bool f = false; // 空格检查标志 

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

char* myStrCat(char* str1,char* str2){
	
	int len1 = strlen(str1);
	int len2 = strlen(str2);
	int len = len1+len2;

	char* newStr = (char*)malloc(sizeof(char)*(len+1));
	memset(newStr,'\0',len);
	int idx = 0;
	
	for(int i=0;i<len1;i++){
		newStr[idx++] = str1[i];
	}
	
	for(int i=0;i<len2 && idx < len;i++){
		newStr[idx++] = str2[i];
	}
	newStr[idx] = '\0';
	return newStr;
}

char* fillChar(char* chars){
	int len = strlen(chars) + 20;
	char* newStr = (char*)malloc(sizeof(char)*len);
	memset(newStr,'\0',len);
	int idx = 0;
	for(int i=0;i<len-20;i++){
		if(chars[i] == '\\'){
			newStr[idx++] = '\\';
		}
		newStr[idx++] = chars[i];
	}
	newStr[idx] = '\0';
	return newStr;
} 

void myFree(char* str){
	free(str);
	str = NULL;
}

void process(){
	
	while(1){
		
		printf("\ninput file path:\n"); 
	
		f = false;
		char *address = getAdress();
		int len =  strlen(address);
		if(len == 0){
			myFree(address);
			continue;
		}
		char* name = getProgramName(address);
		char* a =  myStrCat(base,name);
		char* fir = myStrCat(a,"]");
		char* b = myStrCat(base,name);
		char* sec = myStrCat(b,"\\command]");
		char* fileName = myStrCat(name,".reg");
		char* thr = NULL; 
		
		// 路径中存在空格的话，不用添加引号 
		char* tmp = NULL;
		if(f){
			tmp = myStrCat("@=",address);
		}else{
			char* k =  myStrCat("@=\"",address);
			tmp = myStrCat(k,"\"");
			myFree(k);	
		}
		
		thr = fillChar(tmp);
		
		FILE* f = fopen(fileName,"w");
		
		fputs("Windows Registry Editor Version 5.00\n",f);
		
		fputs(fir,f);
		fputc('\n',f);
		
		fputs("\"Icon\"=\"",f);
		fputs(fillChar(address),f);
		fputs("\"",f);
		fputc('\n',f);	
		
		fputs(sec,f);
		fputc('\n',f);
		
		fputs(thr,f);
		fputc('\n',f);
		
		fclose(f);
		
		myFree(address);
		myFree(name);
		myFree(a);
		myFree(b);
		myFree(tmp);
		myFree(fir);
		myFree(sec);
		myFree(thr);
		myFree(fileName);
		
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
