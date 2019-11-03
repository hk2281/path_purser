#include <locale>
#include <codecvt>
#include <fstream>
#include <io.h>
#include <fcntl.h>
#include <windows.h>
#include <iostream>
#include <fstream>
void SearchPath (std::wstring path[], int count);
LPCWSTR star (std::wstring x);
int arrCount (std::wstring arr[], int x);
void defDisk();

const int sizeArr = 5000, allDrive =8;
std::wstring arr[sizeArr], getDrive[allDrive];
std::wstring arr1[sizeArr];
std::wstring nul ={L""};

void writeLogFile(std::wstring folderPath[],int elements, const char *a){
    const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
    std::wofstream f(a, std::ios_base::app);
    f.imbue(utf8_locale); f<<"\n";
    for (int i= 0;i<elements;++i){
        f<<folderPath[i]<<std::endl;
    }
    f<<"===================\n";
    f.close();
}

void writeLogFile(std::wstring folderPath , const char *a){
    const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
    std::wofstream f(a, std::ios_base::app);
    f.imbue(utf8_locale); f<<"\n";
    f<<folderPath<<std::endl;
    f.close();
}
int arrCount (std::wstring arr[], int x){
    int ret = 0;
    
    for(int i=0; i<x;++i){
        if(arr[i]!=nul){
            ++ret; 
        }
    }
    return ret;
}
int main(){
    setlocale(LC_ALL, "Russian");
    defDisk();  
    printf("done!");
    return 0; 
}
void SearchPath (std::wstring path[], int count){
std::wstring sub,iff = {L"."},pp = {L".."}, nul ={L""}; 
int r=1, breakk=0;
_WIN32_FIND_DATAW FindFileData;
HANDLE hFindFile;  
    for(int i=0;i<count;++i){
        if (breakk ==2) break;  //3 отзначает колво уровней углублений
        hFindFile = FindFirstFileW(star(path[i]), &FindFileData);
        if (hFindFile!=INVALID_HANDLE_VALUE){ 
            do{ 
                sub=FindFileData.cFileName;       
                if(sub!=iff && sub!=pp && path[0]!=nul){// заполняем первый массив 
                    // printf("%ls\n",FindFileData.cFileName);
                    arr1[r-1]=path[0]+L"/"+sub;
                    std::wstring vie = arr1[r-1];
                    writeLogFile(vie,"logArr.txt");
                    if(arr[r]!=nul){
                        int x;
                        for(x=r;x<=sizeArr;x++){
                            arr1[x]=nul;
                            if(arr1[x+1]==nul)break;}
                    }
                    ++r;
                }
            }
            while (FindNextFileW(hFindFile,&FindFileData)!=0);
            {
                if (path[0]==nul){   //следит за колвом углублений 
                    for(int x = 0 ;x<r;++x){
                    path[x] = arr1[x];   
                    }
                    ++breakk;
                    r = 1;i=-1;   
                }
                else if (i==0 && arrCount(path,sizeArr)==1){ //перезапись пройденного буфера
                    writeLogFile(arr1,arrCount(arr1,sizeArr),"log.txt");
                    for(int x = 0 ;x<r;++x){
                    path[x] = arr1[x];   
                    }
                    ++breakk; 
                    r = 1;i=-1;   
                    continue;
                } 
                else {
                    path[0] = path[i+1];
                    // path[i+1] = nul; 
                    for(int scale=1;scale<arrCount(path,sizeArr);++scale){//сдвиг очереди в первый индекс
                        path[scale]=path[scale+1];
                    }
                    i=-1;
                    continue;
                }
            }
        }
         
    }
    FindClose(hFindFile);
    return;
}
// ???????? подставление определенных дисков в рабочий массив 
void defDisk(){
    LPCWSTR pthe[8] = { L"a:",L"d:",L"c:",L"b:",L"e:",L"f:",L"l:",L"g:"}; 
    for(int i=0;i<allDrive;++i){
        if (GetDriveTypeW(pthe[i])==DRIVE_FIXED){
            arr[0]=pthe[i];
            SearchPath(arr,allDrive);
            for(int i=0 ;i<sizeArr;++i){
                arr[i]=arr[i]=nul;
            }
        }    
    }
}
LPCWSTR star (std::wstring x){
    x+=L"\\*";
    return x.c_str();
}
