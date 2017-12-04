//============================================================================
// Name        : stemming.cpp
// Author      : Jigulin
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#ifdef _WIN
  #include <windows.h>
#else
  #include <stdio.h>
  #include <string.h>
  #define V_STEMM_API extern "C"
  typedef  unsigned int DWORD;

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Wwrite-strings"

#endif

#include <stemming/stemming/english_stem.h>
#include <stemming/stemming/french_stem.h>
#include <stemming/stemming/german_stem.h>
#include <stemming/stemming/finnish_stem.h>
#include <stemming/stemming/swedish_stem.h>
#include <stemming/stemming/spanish_stem.h>
#include <stemming/stemming/dutch_stem.h>
#include <stemming/stemming/danish_stem.h>
#include <stemming/stemming/italian_stem.h>
#include <stemming/stemming/norwegian_stem.h>
#include <stemming/stemming/portuguese_stem.h>
#include <stemming/stemming/russian_stem.h>

#include <string>
#include <locale>

/**
 *  Elastic declare
 */
typedef struct Binary_Methods_STRUC {
	char                  *Name;
    void                  (*Method)(int **Adr, int NumOfOpr, DWORD *DataSeg);
}Binary_Methods_STRUC;

typedef struct ELAST_FUNC {
	char     *Name;
    void    (*Func)();
}ELAST_FUNC;

typedef int (*GetFuncOffFT)(DWORD *Body, char *FuncName);
typedef int (*CallFuncFT)(DWORD *Body, DWORD  FileInd, DWORD Off, int **Parms, int ParmCount);
typedef int (*GetFileIndFT)();
typedef char *(*ralloc_chFT)(char **Str, int Size);
typedef void (*rrealloc_chFT)(char **Str, int Size);
typedef void (*rfree_chFT)(char **Str);

typedef DWORD *(*ConstructProc)();
GetFuncOffFT  GetFuncOff    = NULL;
CallFuncFT    CallFuncOff   = NULL;
GetFileIndFT  GetFileIndOff = NULL;
ralloc_chFT   ralloc_ch     = NULL;
rrealloc_chFT rrealloc_ch   = NULL;
rfree_chFT    rfree_ch      = NULL;
int           GetInitialNumberOff;

#define LOCALE_OFFSET 100
#define RETURN_SUCCESS 0
#define RETURN_FAIL -1

typedef enum StemmLocale {
	LOCALE_ENGLISH = LOCALE_OFFSET+1,
	LOCALE_FRENCH = LOCALE_OFFSET+2,
	LOCALE_GERMAN = LOCALE_OFFSET+3,
	LOCALE_FINNISH = LOCALE_OFFSET+4,
	LOCALE_SWEDISH = LOCALE_OFFSET+5,
	LOCALE_SPANISH = LOCALE_OFFSET+6,
	LOCALE_DUTCH = LOCALE_OFFSET+7,
	LOCALE_DANISH = LOCALE_OFFSET+8,
	LOCALE_ITALIAN = LOCALE_OFFSET+9,
	LOCALE_NORWEGIAN = LOCALE_OFFSET+10,
	LOCALE_PORTUGUESE = LOCALE_OFFSET+11,
	LOCALE_RUSSIAN = LOCALE_OFFSET+12
}StemmLocale;

class V_Stemm {
public:
	V_Stemm();
};

void V_Stemm_Stemming(int **Adr, int NumOfOpr, DWORD *DataSeg) {
	/**
	 * @string - src
	 * @string - locale
	 * @string - dst
	 */
	V_Stemm *Class  = (V_Stemm*)Adr[2][2];
	DWORD **Src, **Locale, **Dst;
	char *src, *dst;
	int locale;
	Src = (DWORD **)Adr[4+0];
	Dst = (DWORD **)Adr[4+1];
	Locale = (DWORD **)Adr[4+2];
	src = (char *)Src[1];
	dst = (char *)Dst[1];
	locale = (int)Locale[0];
	std::string stringSrc = std::string(src);

	//printf("src = %s\ndst = %s\nlocale = %d\nlength = %d\n", src, dst, locale, stringSrc.length());

	std::wstring word(stringSrc.begin(), stringSrc.end());

	stemming::english_stem<> StemEnglish;
	stemming::french_stem<> StemFrench;
	stemming::german_stem<> StemGerman;
	stemming::finnish_stem<> StemFinnish;
	stemming::swedish_stem<> StemSwedish;
	stemming::spanish_stem<> StemSpanish;
	stemming::dutch_stem<> StemDutch;
	stemming::danish_stem<> StemDanish;
	stemming::italian_stem<> StemItalian;
	stemming::norwegian_stem<> StemNorwegian;
	stemming::portuguese_stem<> StemPortuguese;
	stemming::russian_stem<> StemRussian;

	switch(locale) {
	case LOCALE_ENGLISH:
		StemEnglish(word);
		break;
	case LOCALE_FRENCH:
		StemFrench(word);
		break;
	case LOCALE_GERMAN:
		StemGerman(word);
		break;
	case LOCALE_FINNISH:
		StemFinnish(word);
		break;
	case LOCALE_SWEDISH:
		StemSwedish(word);
		break;
	case LOCALE_SPANISH:
		StemSpanish(word);
		break;
	case LOCALE_DUTCH:
		StemDutch(word);
		break;
	case LOCALE_DANISH:
		StemDanish(word);
		break;
	case LOCALE_ITALIAN:
		StemItalian(word);
		break;
	case LOCALE_NORWEGIAN:
		StemNorwegian(word);
		break;
	case LOCALE_PORTUGUESE:
		StemPortuguese(word);
		break;
	case LOCALE_RUSSIAN:
		StemRussian(word);
		break;
	default:
		char *output;
		asm("pushad");
		ralloc_ch(&output, 1);
		asm("popad");
		output[0]  = '\0';
		Dst[0] = 0;
		Dst[1] = (DWORD *)output;
		DataSeg[3] = RETURN_FAIL;
		break;
	}
	std::string result(word.begin(), word.end());
	char *output;
	asm("pushad");
	ralloc_ch(&output, result.length());
	asm("popad");
	strcpy(output, result.c_str());
	Dst[0] = (DWORD *)result.length();
	Dst[1] = (DWORD *)output;
	Dst[3] = RETURN_SUCCESS;
}

// Вызывается из elastic при создании его класса
DWORD* V_Stemm_Construct() {
	V_Stemm *p;
	p = new V_Stemm();
	return (DWORD*) p;
}

const char *Tbl[] = {"V_Stemm"};

/**
 * Служебное
 * Описываем параметры вызова
 */
Binary_Methods_STRUC V_Stemm_STRUC_Methods[] = {
	"int Stem string ptr string ptr int ptr", &V_Stemm_Stemming,
	NULL
};

Binary_Methods_STRUC *MethodStrucTable[] = {
    V_Stemm_STRUC_Methods,
    NULL
};

V_Stemm::V_Stemm() {

}

V_STEMM_API ConstructProc GetConstructor(int i) {
    printf("GetConstructor:%d\n",i);
    return V_Stemm_Construct;
}

// Вызывается по загрузке .so
V_STEMM_API Binary_Methods_STRUC  **Init(const char ***Names, ELAST_FUNC *FuncTable) {
	Names[0] = Tbl;
    for(int i=0; ; i++){
        if(FuncTable[i].Name == NULL)break;
        if(strcmp(FuncTable[i].Name, "GetFuncOff") == 0){
           GetFuncOff = (GetFuncOffFT)FuncTable[i].Func;
        }else if(strcmp(FuncTable[i].Name, "CallFunc") == 0){
           CallFuncOff = (CallFuncFT)FuncTable[i].Func;
        }else if(strcmp(FuncTable[i].Name, "GetFileInd") == 0){
           GetFileIndOff = (GetFileIndFT)FuncTable[i].Func;
        }else if(strcmp(FuncTable[i].Name, "alloc_ch") == 0){
           ralloc_ch         = (ralloc_chFT)FuncTable[i].Func;
        }else if(strcmp(FuncTable[i].Name, "free_ch") == 0){
           rfree_ch          = (rfree_chFT)FuncTable[i].Func;
        }else if(strcmp(FuncTable[i].Name, "realloc_ch") == 0){
           rrealloc_ch          = (rrealloc_chFT)FuncTable[i].Func;
        }
    }
    return MethodStrucTable;
}
