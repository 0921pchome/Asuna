#ifndef _ZWEI2PLUS_H_
#define _ZWEI2PLUS_H_

#include <Windows.h>
#include "my_headers.h"

typedef struct
{
    WChar Orig;
    WChar Relpace;
} TCharMap;

Void  Init();
DWORD CDECL  Mystrncpy(LPSTR lpDest, LPSTR lpSource, DWORD dwLength);
Void  WINAPI ProcessCharName(LPSTR lpString1, LPCSTR lpString2);
Void  WINAPI ProcessNiikki(LPSTR lpString1, LPCSTR lpString2);
LPSTR CDECL  ProcessBuildingName(LPSTR pbBuffer);
int   CDECL  Mywvsprintf(LPSTR *lpOutput, LPSTR lpFmt, va_list arglist);
BOOL  WINAPI MySetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags);
BOOL  WINAPI MyPeekMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);
DWORD WINAPI GetCharSurface(PBYTE pbSurface, LONG lHeight, int iOffset, UINT uChar, DWORD dwColor, DWORD dwColor2);
HANDLE  WINAPI MyCreateFileA(LPSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);

#define STR(s) (DWORD)&(s)

SPatch p[] = 
{
    { 0xEB,                  1, 0x02509D },     // ·�Ʊ߽���
    { 0x6,                   1, 0xD43FF  },     // dinput ����win
#if defined(USE_ENCRYPT_TEXT)
    { 0x04,                  1, 0x26BA7  },     // �ַ����ض� mov byte ptr [esp+ eax +38h], bl
    { 0x04,                  1, 0x262E9  },     // �ַ����ض� mov byte ptr [esp+ eax +48h], bl
    { 0x96,                  1, 0x40FB3  },     // xchg eax, esi for ProcessBuildingName
    { 0x56,                  1, 0x40FAD  },     // push esi
    { 0x10148D50,            4, 0x01E20  },     // push eax; lea edx, dword ptr [edx+eax];
    { 0x52,                  1, 0x01E24  },     // push edx
    { 0x10148D50,            4, 0x03A10  },     // nikki
    { 0x52,                  1, 0x03A14  }, 
    { 0x10148D50,            4, 0x03B20  },     // mail
    { 0x52,                  1, 0x03B24  }, 
#endif

    // text offset patch
    { STR("�ң��롡%s"),                                                4, 0x0857A },
    { STR("�ң��롡%s"),                                                4, 0x0859F },
    { STR("�ң��롡%s"),                                                4, 0x085BF },
    { STR("�ң��롡%s"),                                                4, 0x085DC },
    { STR("�ԣ��塡%02d��%02d"),                                        4, 0x086CA },
    { STR("�̣֣�s070��\\n�ң��룺s070%s\\n�ԣ��壺s070��"),               4, 0x30BC4 },
    { STR("�̣֣�s070%s\\n�ң��룺s070%s\\n�ԣ��壺s070����������"),       4, 0x309E9 },
    { STR("�̣֣�s070%d\\n�ң��룺s070%s\\n�ԣ��壺s070%d��%02d"),        4, 0x30A34 },
    { STR("�̣֣�s070����\\n�ң��룺s070��������\\n�ԣ��壺s070����������"), 4, 0x30CC2 },
    { STR("�ң�����"),                                                         4, 0x29ED3 },
    { STR("�ӣ������������ǣ���Ŀ��װ"),                                     4, 0x2A7B0 },
    { STR("�ӣ���������櫣�С��"),                                           4, 0x2A83D },
    { STR("ʱװ�̵ꡡ������������塷"),                                       4, 0x3CC6D },
    { STR("���������ǣ����塤�ǣ������"),                              4, 0x3CC8D },
    { STR("%s�ĵȼ��ﵽ%d\nѧ���ˡ�����˼����赸�����ܣ���"),                   4, 0x0AE19 },

    // IAT Hook
    { (DWORD)MySetWindowPos, 4, 0x1732CC }, 
    { (DWORD)MyPeekMessageA, 4, 0x173278 }, 
    { (DWORD)MyCreateFileA,  4, 0x1731C0 }, 
};

SFuncPatch f[] = 
{
    { JUMP, 0xD9C40,  GetCharSurface,        3 }, 
    { JUMP, 0xD9BC0,  GetCharSurface,        1 },
#if defined(USE_ENCRYPT_TEXT)
    { CALL, 0x13DCB3, Mywvsprintf,           0 },
    { CALL, 0x40FAE,  ProcessBuildingName,   0 },
    { CALL, 0x01E25,  ProcessCharName,       0 },
    { CALL, 0x03A15,  ProcessCharName,       0 },    // nikki
    { CALL, 0x03B25,  ProcessCharName,       0 },    // mail
    { CALL, 0x262DE,  Mystrncpy,             0 },
    { CALL, 0x26B9E,  Mystrncpy,             0 },
#endif
};

#endif /* _ZWEI2PLUS_H_ */