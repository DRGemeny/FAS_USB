#if !defined(AFX_USB__H__BB32D48F_96C3_11D9_BFD7_008048179F8F__INCLUDED_)
#define AFX_USB__H__BB32D48F_96C3_11D9_BFD7_008048179F8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "FT_USB.h"

CFT_USB::CFT_USB()
{
	ft_handle = NULL;
	ft_status = FT_IO_ERROR;
}

CFT_USB::~CFT_USB()
{

}

int CFT_USB::Get_Number_Of_Devices()
{
	DWORD numDevs;
	ft_status = FT_ListDevices(&numDevs, NULL, FT_LIST_NUMBER_ONLY);

	if (ft_status == FT_OK)
	{
		return numDevs;
	}
	else
	{
		return -111;
	}
}
FT_STATUS CFT_USB::Get_Device_List_By_SN(char BufferSN[64])
{

	ft_status = FT_ListDevices(0, BufferSN, FT_LIST_BY_INDEX | FT_OPEN_BY_SERIAL_NUMBER);

	return ft_status;
}


FT_STATUS CFT_USB::Get_All_Devices_List_By_SN(DWORD devIndex, char* pBuf[5])
{

	ft_status = FT_ListDevices(pBuf, &devIndex, FT_LIST_ALL | FT_OPEN_BY_SERIAL_NUMBER);

	return ft_status;
}

FT_STATUS CFT_USB::Get_Device_List_By_Description(char BufDescr[64])
{
	ft_status = FT_ListDevices(0, BufDescr, FT_LIST_BY_INDEX | FT_OPEN_BY_DESCRIPTION);

	return ft_status;
}

FT_STATUS CFT_USB::Get_All_Devices_List_By_Description(DWORD devIndex, char* pBuf[5])
{
	ft_status = FT_ListDevices(pBuf, &devIndex, FT_LIST_ALL | FT_OPEN_BY_DESCRIPTION);

	return ft_status;
}



FT_STATUS CFT_USB::Open_Device(int Num)
{
	ft_status = FT_Open(Num, &ft_handle);//открыть устройство
	return ft_status;

}

FT_STATUS CFT_USB::Close_Device()
{
	ft_status = FT_Close(ft_handle);//«акрыть устройство
	return ft_status;
}

FT_STATUS CFT_USB::Open_Device_by_SN(char* m_cSn)
{
	ft_status = FT_OpenEx(m_cSn, FT_OPEN_BY_SERIAL_NUMBER, &ft_handle);
	return ft_status;

}

FT_STATUS CFT_USB::Open_Device_By_Description(char* m_cDscr)
{
	ft_status = FT_OpenEx(m_cDscr, FT_OPEN_BY_DESCRIPTION, &ft_handle);
	return ft_status;

}

DWORD CFT_USB::Read_From_Device(char RxBuf[256])
{
	DWORD ReturnRBytes;
	DWORD QueueBytes;
	int attempt = 0;
	while (-1)
	{
		ft_status = FT_GetQueueStatus(ft_handle, &QueueBytes);//¬з€ть статус очереди приема

		if (QueueBytes > 0)
		{
			/*!
			CString s;
			s.Format("%d",QueueBytes);
			AfxMessageBox(s);
			*/
			break;
		}
		else
		{
			attempt++;
		}
		if (attempt == 100000)
		{
			//AfxMessageBox("число попыток истекло");
			break;
		}
	}
	if (ft_status == FT_OK)
	{
		ft_status = FT_Read(ft_handle, RxBuf, QueueBytes, &ReturnRBytes);//„тение из устройства
	}
	else
	{
		AfxMessageBox("Error FT_GetQueueStatus");
	}
	if (ft_status == FT_OK)
	{

		if (ReturnRBytes == QueueBytes)
		{
			// FT_Read OK
		//AfxMessageBox("FT_Read OK");
			return ReturnRBytes;
		}
		else {
			// FT_Read Timeout
			AfxMessageBox("Error FT_Read_Timeout");
		}
	}
	else {
		// FT_Read Failed
		CString s1, s2, s3, sF;
		s1.Format("%d ", QueueBytes);
		s2.Format("%d ", ReturnRBytes);
		s3.Format("%d", ft_status);
		sF = "Error FT_READ " + s1 + s2 + " tf_status - " + s3;
		AfxMessageBox(sF);
		return -1;
	}
	/*	if(ft_status!=FT_OK)
		{
			AfxMessageBox("Error FT_READ");
			return -1;
		}
		else
		{
			return ReturnRBytes;
		}*/
}

FT_STATUS CFT_USB::Reset_Device()
{
	ft_status = FT_ResetDevice(ft_handle);//сброс устройства

	return ft_status;
}

DWORD CFT_USB::Write_In_Device(LPBYTE lpWBuf, DWORD NumWDat)
{
	DWORD ReturnWBytes;

	ft_status = FT_Write(ft_handle, lpWBuf, NumWDat, &ReturnWBytes);//запись в устройство

	return ReturnWBytes;
}

FT_STATUS CFT_USB::Purge_Device(DWORD dwMask)
{
	ft_status = FT_Purge(ft_handle, dwMask);//очистка приемо-передающих буферов

	return ft_status;
}

FT_STATUS CFT_USB::Set_Device_Timeouts(DWORD dwRTimeout, DWORD dwWTimeout)
{
	ft_status = FT_SetTimeouts(ft_handle, dwRTimeout, dwWTimeout);//врем€ на обмен с устройством

	return ft_status;
}

FT_STATUS CFT_USB::GetQStatus(FT_HANDLE m_FTHandle)
{
	FT_STATUS ft_st;
	DWORD QBytes = 0;
	do {
		ft_st = FT_GetQueueStatus(m_FTHandle, &QBytes);

	} while (QBytes != 0);

	return ft_st;
}
