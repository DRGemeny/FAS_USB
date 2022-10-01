//FTDI 
#if !defined(AFX_USB__H__BB32D48F_96C3_11D9_BFD7_008048179F8F__INCLUDED_)
#define AFX_USB__H__BB32D48F_96C3_11D9_BFD7_008048179F8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include"ftd2xx.h"


class CFT_USB
{
public:
	FT_STATUS GetQStatus(FT_HANDLE m_FTHandle);



	CFT_USB();
	~CFT_USB();
	//__________________________________________________________________
	int Get_Number_Of_Devices(void);
	FT_STATUS Reset_Device();
	FT_STATUS Purge_Device(DWORD dwMask);
	FT_STATUS Set_Device_Timeouts(DWORD dwRTimeout, DWORD dwWTimeout);

	FT_STATUS Get_Device_List_By_SN(char BufferSN[64]);
	FT_STATUS Get_All_Devices_List_By_SN(DWORD devIndex, char* pBuf[5]);

	FT_STATUS Get_Device_List_By_Description(char BufDescr[64]);
	FT_STATUS Get_All_Devices_List_By_Description(DWORD devIndex, char* pBuf[5]);

	FT_STATUS Open_Device(int Num);
	FT_STATUS Open_Device_By_Description(char* m_cDscr);
	FT_STATUS Open_Device_by_SN(char* m_cSn);
	FT_STATUS Close_Device();

	DWORD Read_From_Device(char RxBuf[256]);
	DWORD Write_In_Device(LPBYTE lpWBuf, DWORD NumWDat);

private:
	FT_STATUS ft_status;
	FT_HANDLE ft_handle;
};

#endif // !defined(AFX_USB__H__BB32D48F_96C3_11D9_BFD7_008048179F8F__INCLUDED_)
